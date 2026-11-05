# Glossary

Amazon fulfillment terms used in this codebase. Real Amazon operations are far more complex.
These definitions describe only this exercise.

| Term | Meaning here |
|---|---|
| **ASIN / SKU** | Identifier for a product, e.g. `B0KINDLE01`. Real ASINs are 10 characters starting with `B0`, and the fixtures imitate that. |
| **Fulfillment Center (FC)** | A warehouse that stores inventory and ships customer orders. Identified by an integer `id` and an airport-style `code` (e.g. `SEA1`, `PDX9`, `BFI4`). |
| **FC code** | Amazon names FCs after the nearest major airport code plus a number. Codes are for humans; the planner always works with `id`s. |
| **Inventory** | Units of each SKU physically available at one FC. |
| **Reserve** | Removing units from available inventory because an order has been assigned to that FC. |
| **Order line** | One `{sku, quantity}` entry on an order. |
| **Prime** | Membership with faster delivery and free shipping. Prime orders are processed before Standard orders. |
| **Standard** | Any non-Prime order. Slower delivery, and shipping is paid. |
| **Delivery promise** | The number of days shown to the customer at checkout (§2). |
| **Box size** | `Small`, `Medium`, `Large`, or `Freight` (palletized / oversized). |
| **Freight** | Shipments too large for parcel networks. They carry a surcharge regardless of Prime status. |
| **Backorder** | An order that cannot currently be fulfilled from any single FC. It waits for restock. |
| **Batch** | A group of orders planned together against shared inventory (e.g. one planning wave). |
| **Grid miles** | The abstract distance unit of the delivery grid (Manhattan distance). |
