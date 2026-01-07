# E-Commerce Order Management System

An interactive, console-based application that helps operations teams manage the full lifecycle of e-commerce orders. The system supports order capture, review, analytics, and secure persistence in a portable text database.

## Key Features
- Guided order creation with validated IDs, structured customer/product inputs, and curated status presets.
- Search, update, and delete operations with confirmation prompts to prevent accidental data loss.
- Real-time analytics dashboard summarizing revenue, order volumes, and status breakdowns.
- Persistent storage (`orders.txt`) for seamless reloads between sessions.
- Unsaved-change tracking with graceful shutdown prompts to safeguard work-in-progress.

## Getting Started
### Prerequisites
- C compiler (e.g., `gcc`, `clang`, or MSVC on Windows)

### Build
```bash
gcc E_Commerce_Project.c -o ecommerce
```

### Run
```bash
./ecommerce
```

## Usage Guide
- **Add New Order**: Enter a unique numeric ID, customer/product details, quantity, price, and choose a status from the curated list (or define a custom label).
- **Display All Orders**: Review every order with totals and revenue summary.
- **Search Order**: Locate orders by ID or via case-insensitive customer-name matches.
- **Update/Delete Order**: Modify existing records or remove them, with confirmations and status validation.
- **Save Orders**: Persist all in-memory changes to `orders.txt`. You are reminded automatically if changes are pending during exit.
- **Analytics Dashboard**: Inspect KPIs such as total revenue, average order value, status distribution, and the highest-value order.

## Data Persistence
Orders are stored in a CSV-like format under `orders.txt`. You can back up or version-control this file to maintain historical records.

## Author
- Md. Mosabbir Sadik
