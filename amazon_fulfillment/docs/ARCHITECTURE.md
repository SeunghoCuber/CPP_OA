# Architecture

All code lives in the `fulfillment` namespace. Headers are in `include/`, implementations in `src/`.

## Class overview

```
                    ┌──────────────────────────┐
                    │    FulfillmentPlanner    │   src/FulfillmentPlanner.cpp  <- you implement
                    │──────────────────────────│
                    │ Catalog catalog_         │
                    │ vector<FulfillmentCenter>│
                    └─────────┬────────────────┘
               owns           │            owns
        ┌─────────────────────┴───────────────────┐
        ▼                                          ▼
 ┌─────────────┐                        ┌────────────────────┐
 │   Catalog   │  SKU -> Item           │ FulfillmentCenter  │  id, code ("SEA1"), Location
 └──────┬──────┘                        └─────────┬──────────┘
        │ holds                                   │ owns
        ▼                                         ▼
 ┌─────────────┐                        ┌────────────────────┐
 │    Item     │  sku, title,           │     Inventory      │  SKU -> units available
 └─────────────┘  weight g, volume cm³  └────────────────────┘

 ┌─────────────┐   ┌─────────────┐      ┌────────────────────┐
 │    Order    │──▶│  OrderLine  │      │ Shipment / BatchPlan│  output value types
 └─────────────┘   └─────────────┘      └────────────────────┘
   id, destination, isPrime, placedAt
```

## Components

### `Location` (`include/Location.h`, header-only)
A point `{x, y}` on the delivery grid, plus `gridDistance(a, b)`, which returns the
Manhattan distance between two points.

### `Item` (`include/Item.h`, `src/Item.cpp`)
Immutable product data for **one unit**: SKU, title, weight in grams, volume in cm³.
The constructor validates that the SKU is non-empty and that weight and volume are positive.

### `Catalog` (`include/Catalog.h`, `src/Catalog.cpp`)
Lookup table from SKU to `Item`.
- `addItem` throws `std::invalid_argument` on a duplicate SKU.
- `get` throws `std::out_of_range` on an unknown SKU.

### `Inventory` (`include/Inventory.h`, `src/Inventory.cpp`)
Stock levels for one FC.
- `available(sku)` returns 0 for SKUs that were never stocked.
- `hasAtLeast(sku, n)` checks a single SKU.
- `reserve(sku, n)` decrements stock. It throws `std::runtime_error` if stock is
  insufficient, in which case stock is left unchanged.

### `Order` / `OrderLine` (`include/Order.h`, `src/Order.cpp`)
A customer order: id, destination `Location`, Prime flag, `placedAt` timestamp, and lines
of `{sku, quantity}`. The constructor rejects empty orders and non-positive quantities.
**The same SKU may appear on more than one line.**

### `FulfillmentCenter` (`include/FulfillmentCenter.h`, `src/FulfillmentCenter.cpp`)
An FC with an integer id, a human-readable code such as `"SEA1"`, a location, and its own
`Inventory` (mutable and const accessors).

### `Shipment`, `BatchPlan`, `BoxSize` (`include/Shipment.h`, header-only)
Plain output types. `BoxSize` is an ordered `enum class`: `Small < Medium < Large < Freight`.
It has `operator<<` so it prints nicely in test failures.

### `FulfillmentPlanner` (`include/FulfillmentPlanner.h`, `src/FulfillmentPlanner.cpp`)
The component under construction. It owns a **copy** of the catalog and the FC list.

| Method | Status |
|---|---|
| constructor (rejects duplicate FC ids) | provided |
| `center(id)` | provided |
| `catalog()`, `centers()` | provided (inline in header) |
| `selectFulfillmentCenter` | **TODO** (§1) |
| `estimateDeliveryDays` | **TODO** (§2) |
| `chooseBox` | **TODO** (§3) |
| `shippingCostCents` | **TODO** (§4) |
| `planBatch` | **TODO** (§5) |

Because the header is read-only, any helpers you need must be **free functions** or
constants in `FulfillmentPlanner.cpp` (for example in the anonymous namespace), not new
member functions.

## Error-handling conventions

| Situation | Exception |
|---|---|
| Bad argument from a caller (negative distance, duplicate ids, invalid quantities) | `std::invalid_argument` |
| Lookup of something that does not exist | `std::out_of_range` |
| An operation that cannot be performed in the current state (e.g. reserving more than is available) | `std::runtime_error` |

## Tests

```
tests/
├── TestHarness.h        # TEST_CASE / EXPECT_EQ / EXPECT_TRUE / EXPECT_THROWS
├── TestMain.cpp         # runner: sorts tests by name, supports a name filter
├── TestFixtures.h       # sample Amazon catalog + helpers to build FCs and orders
├── test_selection.cpp   # T01-T04  -> §1
├── test_shipping.cpp    # T05-T07  -> §2, §3, §4
└── test_batch.cpp       # T08-T10  -> §5
```
