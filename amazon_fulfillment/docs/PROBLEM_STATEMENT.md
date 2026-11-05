# Problem Statement: Fulfillment Planner

Amazon's regional network has several **fulfillment centers (FCs)**, each holding its own
inventory. Customer orders arrive throughout the day. For each order, the
`FulfillmentPlanner` must decide:

1. **Which FC** ships it
2. **How many days** delivery will take
3. **Which box** it ships in
4. **How much** the customer pays for shipping
5. Finally, how to process a **whole batch** of orders against limited inventory

You implement all five in `src/FulfillmentPlanner.cpp`. Each section below is the
authoritative spec for one method. If a test and this document seem to disagree, the
document describes intent. Re-read it carefully before assuming the test is wrong.

---

## Shared definitions

| Term | Definition |
|---|---|
| **Distance** | Grid (Manhattan) distance in "grid miles": `gridDistance(a, b)` in `include/Location.h`. |
| **SKU requirement** | For a given order and SKU, the **sum of the quantities on all of the order's lines** with that SKU. |
| **Total weight** | Sum over all order lines of `unit weight (g) × quantity`, using the `Catalog`. |
| **Total volume** | Sum over all order lines of `unit volume (cm³) × quantity`, using the `Catalog`. |
| **Prime order** | `order.isPrime() == true`. Otherwise the order is **Standard**. |

All SKUs on an order can be assumed to exist in the catalog. If one does not, letting the
`std::out_of_range` from `Catalog::get` propagate is acceptable.

---

## §1 `selectFulfillmentCenter(const Order&) const → std::optional<int>`

Returns the **id** of the FC that should ship the order.

- An FC is **eligible** if, for **every** SKU on the order, the FC's available stock is
  at least the order's SKU requirement for that SKU. Orders are never split across FCs.
- Among eligible FCs, choose the one **closest** to the order's destination.
- If several eligible FCs are equally close, choose the one with the **lowest id**.
  (The order of centers in the planner's list has no significance.)
- If no FC is eligible (including when there are no FCs at all), return `std::nullopt`.
- This method **must not change** any inventory.

**Example.** FCs `SEA1 (id 1) @ (0,0)` and `BFI4 (id 4) @ (30,40)` both stock the item.
An order to `(25,35)` is 60 from SEA1 and 10 from BFI4, so the result is `4`.

---

## §2 `estimateDeliveryDays(const Order&, int distance) const → int`

The delivery promise shown to the customer, in whole days.

**Prime**

| Distance | Days |
|---|---|
| 0 – 50 | 1 |
| 51 – 200 | 2 |
| over 200 | 3 |

**Standard**

- `3 + (distance / 100)` using **integer division**,
- but never more than **10** days.

**Errors.** If `distance < 0`, throw `std::invalid_argument`.

**Examples.** Prime @ 51 → 2. Standard @ 250 → 5. Standard @ 5000 → 10.

---

## §3 `chooseBox(const Order&) const → BoxSize`

Pick the smallest box that satisfies **both** rules below.

**Volume rule.** Based on total volume:

| Total volume (cm³) | Box |
|---|---|
| ≤ 2,000 | `Small` |
| ≤ 10,000 | `Medium` |
| ≤ 40,000 | `Large` |
| > 40,000 | `Freight` |

**Weight rule.** If total weight is **strictly greater than 15,000 g**, the box must be
**at least `Large`**.

**Examples.**

- 3 Echo Dots (3,000 cm³, 912 g) → `Medium`
- 1 dumbbell (1,500 cm³, 9,000 g) → `Small`
- 2 dumbbells (3,000 cm³, 18,000 g) → `Large`

---

## §4 `shippingCostCents(const Order&, int distance) const → long long`

What the customer pays, in **US cents**.

**Standard orders:**

```
499                                    base fee
+ 99 × (total weight in kg, rounded UP to a whole kg)
+ 2 × distance
```

"Rounded up" means every started kilogram counts: 1 g → 1 kg, 1,000 g → 1 kg, 1,001 g → 2 kg.

**Prime orders:** shipping is free (`0`), except as below.

**Freight surcharge.** If the order's box (per §3) is `Freight`, add **2,500** cents.
This applies to **both** Prime and Standard orders.

**Errors.** If `distance < 0`, throw `std::invalid_argument`.

**Example.** Standard, one skillet (3,000 g), distance 40:
`499 + 99×3 + 2×40 = 876`.

---

## §5 `planBatch(const std::vector<Order>&) → BatchPlan`

Plans a batch of orders against the planner's (shared, limited) inventory.

**Validation.** If two orders in the batch share the same `id`, throw
`std::invalid_argument`. When this happens, **no inventory may have been changed**.

**Processing order.** Orders are processed one at a time in this priority:

1. Prime orders before Standard orders
2. Then earlier `placedAt` first
3. Then lower order `id` first

**For each order, in processing order:**

- Select an FC exactly as in §1, using inventory **as it stands at that moment**
  (i.e. after earlier orders in the batch have taken their stock).
- If an FC is found:
  - reserve the order's full SKU requirements from that FC's inventory, and
  - append a `Shipment` to `shipments` with:
    `orderId`, `fulfillmentCenterId`, `distance` (FC → destination),
    `deliveryDays` (§2), `box` (§3), `shippingCostCents` (§4).
- If no FC is found, append the order's id to `backorderedOrderIds`.

Both output vectors are in **processing order**. An empty batch produces an empty plan.

Inventory changes made by `planBatch` are permanent. They are visible through
`planner.center(id).inventory()` afterwards and to later calls.

---

## Out of scope

- Splitting one order across multiple FCs
- Partial fulfillment of a line
- Multiple boxes per order
- Thread safety
