# Amazon Prime Fulfillment Planner: C++ Coding Assessment

Welcome! In this exercise you finish the **Fulfillment Planner**, the component that decides
which Amazon fulfillment center (FC) ships a customer's order, which box it goes in,
how fast it arrives, and what the customer pays for shipping.

Most of the codebase is already written and working. Your job is to implement the
five methods marked `TODO` in **one file**:

```
src/FulfillmentPlanner.cpp
```

## Quick start

```bash
cd amazon_fulfillment
make test                 # build and run all tests
make test FILTER=T05      # run a single test (substring match on test name)
make clean                # remove build artifacts
```

Requirements: a C++17 compiler (clang++ or g++) and `make`. No third-party libraries are needed.

When you start, **2 of 10 tests pass**. Your goal is **10 / 10**.

## Rules

1. **Edit only `src/FulfillmentPlanner.cpp`.** Do not modify headers, tests, the Makefile,
   or any other source file. Graders build your file against the original versions of everything else.
2. You may add `#include`s, constants, and helper functions inside `FulfillmentPlanner.cpp`
   (an anonymous namespace is provided for this).
3. Do not change the behavior of the already-implemented constructor or `center()`.
4. Your code must compile with no warnings under the flags in the `Makefile`.
5. An AI assistant is available to you during the session. It can help you **understand**
   the codebase, the tooling, C++ itself, and error messages. It will **not** write code for
   you or tell you how to solve the problem. See [AI_AGENT_INSTRUCTIONS.md](AI_AGENT_INSTRUCTIONS.md).
6. Suggested time box: **90 minutes**.

## Documentation

| Document | What's in it |
|---|---|
| [docs/PROBLEM_STATEMENT.md](docs/PROBLEM_STATEMENT.md) | **The spec.** Exact rules for each of the five parts. Read this first. |
| [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) | Classes, how they relate, and which files they live in. |
| [docs/BUILD_AND_TEST.md](docs/BUILD_AND_TEST.md) | Building, running and filtering tests, and reading test output. |
| [docs/GLOSSARY.md](docs/GLOSSARY.md) | Amazon fulfillment terminology used in the code. |
| [AI_AGENT_INSTRUCTIONS.md](AI_AGENT_INSTRUCTIONS.md) | The operating rules for the AI assistant. |

## Project layout

```
amazon_fulfillment/
├── include/                 # Public headers (read-only)
│   ├── Location.h
│   ├── Item.h
│   ├── Catalog.h
│   ├── Inventory.h
│   ├── Order.h
│   ├── FulfillmentCenter.h
│   ├── Shipment.h
│   └── FulfillmentPlanner.h
├── src/
│   ├── Item.cpp
│   ├── Catalog.cpp
│   ├── Inventory.cpp
│   ├── Order.cpp
│   ├── FulfillmentCenter.cpp
│   └── FulfillmentPlanner.cpp   <-- YOU EDIT THIS FILE ONLY
├── tests/                   # Test harness, fixtures, and test cases (read-only)
├── docs/
├── Makefile
├── AI_AGENT_INSTRUCTIONS.md     # rules for the AI assistant
├── CLAUDE.md / AGENTS.md        # auto-loaded pointers to those rules
└── .claude/settings.json        # blocks the assistant's file-editing tools
```

## Grading

| Area | Weight |
|---|---|
| Tests passing | 60% |
| Code clarity and structure | 25% |
| Handling of edge cases and error conditions per spec | 15% |

Graders may run additional tests built from the same spec with different data. Code
written to pass only the visible test data will not score well.
