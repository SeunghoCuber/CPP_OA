# Build and Test

## Prerequisites

- A C++17 compiler: Apple clang, clang++ ≥ 7, or g++ ≥ 8
- GNU or BSD `make`

There are no external dependencies. The test harness is a single header in `tests/`.

## Commands

All commands are run from the `amazon_fulfillment/` directory.

| Command | Effect |
|---|---|
| `make` | Compile everything into `build/run_tests` |
| `make test` | Compile (if needed) and run every test |
| `make test FILTER=T08` | Run only tests whose name contains `T08` |
| `make test FILTER=selects` | Filters are substring matches, so this works too |
| `make clean` | Delete `build/` |
| `./build/run_tests T03` | Run the binary directly (same filter syntax) |

To use a specific compiler: `make CXX=g++-13 test`.

The build uses `-Wall -Wextra -Wpedantic`. Your submission is expected to compile **warning-free**.

## Reading test output

```
[ RUN  ] T05_estimates_delivery_days_for_prime_and_standard
    tests/test_shipping.cpp:21: EXPECT_EQ(planner.estimateDeliveryDays(prime, 0), 1)
        actual:   0
        expected: 1
[ FAIL ] T05_estimates_delivery_days_for_prime_and_standard
...
2/10 tests passed
```

- Each test **stops at its first failing expectation**, so fixing one line of a test can
  reveal the next failure further down in the same test.
- The `file:line` points at the expectation in the test file. Open it to see the input data.
- `actual` is what your code returned. `expected` is what the spec requires.
- `unexpected exception: ...` means your code threw something the test was not expecting.
- `did not throw` means the spec requires an exception that your code did not raise.

`make test` exits non-zero while any test fails, so seeing `make: *** [test] Error 1` at the
end is normal until everything passes.

## Test catalog

| Test | Spec | Starts as |
|---|---|---|
| T01_selects_nearest_center_with_stock | §1 | FAIL |
| T02_breaks_distance_ties_by_lowest_center_id | §1 | FAIL |
| T03_requires_full_stock_for_every_sku_in_the_order | §1 | FAIL |
| T04_returns_nullopt_when_no_center_can_fulfill | §1 | PASS |
| T05_estimates_delivery_days_for_prime_and_standard | §2 | FAIL |
| T06_chooses_box_by_volume_and_weight | §3 | FAIL |
| T07_computes_shipping_cost_in_cents | §4 | FAIL |
| T08_plans_batch_by_priority_and_reserves_inventory | §5 | FAIL |
| T09_empty_batch_produces_empty_plan | §5 | PASS |
| T10_rejects_duplicate_order_ids_without_touching_inventory | §5 | FAIL |

The two tests that pass at the start pass only because the stubs happen to return
"empty" values. **Keep them passing** as you implement.

## Sample data

`tests/TestFixtures.h` defines the catalog used by every test:

| Constant | Product | Weight (g) | Volume (cm³) |
|---|---|---|---|
| `KINDLE` | Kindle Paperwhite | 205 | 700 |
| `ECHO_DOT` | Echo Dot (5th Gen) | 304 | 1,000 |
| `FIRE_TV` | Fire TV Stick 4K | 150 | 300 |
| `SKILLET` | Amazon Basics Cast Iron Skillet | 3,000 | 4,000 |
| `WATER_24` | Amazon Basics Spring Water, 24-pack | 9,000 | 12,000 |
| `DUMBBELL` | Amazon Basics 20 lb Dumbbell | 9,000 | 1,500 |
| `DESKCHAIR` | Amazon Basics Office Desk Chair | 16,000 | 90,000 |
