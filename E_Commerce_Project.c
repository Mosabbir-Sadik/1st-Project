/**
 * =============================================================================
 * E-Commerce Order Management System
 * =============================================================================
 * 
 * Description: A comprehensive order management system for e-commerce operations.
 *              Supports adding, viewing, searching, updating, and persisting orders.
 * 
 * Author:      [Your Name]
 * Version:     1.0.0
 * Created:     January 2026
 * 
 * =============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* =============================================================================
 * CONFIGURATION CONSTANTS
 * ============================================================================= */

#define MAX_ORDERS          100
#define MAX_NAME_LENGTH     50
#define MAX_PRODUCT_LENGTH  50
#define MAX_STATUS_LENGTH   20
#define DATA_FILE           "orders.txt"
#define TEMP_BUFFER_LENGTH  128

static const char *STATUS_OPTIONS[] = {
    "Pending",
    "Processing",
    "Shipped",
    "Delivered",
    "Cancelled"
};

static const size_t STATUS_OPTION_COUNT = sizeof(STATUS_OPTIONS) / sizeof(STATUS_OPTIONS[0]);

/* =============================================================================
 * DATA STRUCTURES
 * ============================================================================= */

/**
 * Order structure representing a single order in the system.
 */
typedef struct {
    int  orderID;
    char customerName[MAX_NAME_LENGTH];
    char productName[MAX_PRODUCT_LENGTH];
    int  quantity;
    float price;
    char orderStatus[MAX_STATUS_LENGTH];
} Order;

/* =============================================================================
 * GLOBAL VARIABLES
 * ============================================================================= */

static Order orders[MAX_ORDERS];
static int   orderCount = 0;
static bool  hasUnsavedChanges = false;

/* =============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================= */

/* Core Operations */
void addOrder(void);
void displayOrders(void);
void searchOrder(void);
void updateOrder(void);
void deleteOrder(void);
void displayAnalytics(void);

/* File Operations */
int  saveToFile(void);
int  loadFromFile(void);

/* User Interface */
void displayMenu(void);
void displayWelcomeBanner(void);
void displayOrderDetails(const Order *order);
void promptOrderStatus(char *buffer, int maxLength);

/* Utility Functions */
void clearInputBuffer(void);
int  readInteger(const char *prompt, int *value);
int  readFloat(const char *prompt, float *value);
void readString(const char *prompt, char *buffer, int maxLength);
int  findOrderByID(int orderID);
int  isValidOrderID(int orderID);
void trimNewline(char *str);
void pressEnterToContinue(void);
int  equalsIgnoreCase(const char *a, const char *b);
int  containsIgnoreCase(const char *haystack, const char *needle);
void toLowerCopy(const char *src, char *dest, size_t destSize);
int  mapStatusToIndex(const char *status);
int  promptYesNo(const char *message);

/* =============================================================================
 * MAIN FUNCTION
 * ============================================================================= */

int main(void) {
    displayWelcomeBanner();
    
    printf("\n[INFO] Loading existing orders from database...\n");
    int loadedCount = loadFromFile();
    
    if (loadedCount > 0) {
        printf("[INFO] Successfully loaded %d order(s).\n", loadedCount);
    } else {
        printf("[INFO] No existing orders found. Starting fresh.\n");
    }
    
    displayMenu();
    
    return EXIT_SUCCESS;
}

/* =============================================================================
 * USER INTERFACE FUNCTIONS
 * ============================================================================= */

/**
 * Displays the welcome banner at application startup.
 */
void displayWelcomeBanner(void) {
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|                                                  |\n");
    printf("|     E-COMMERCE ORDER MANAGEMENT SYSTEM v2.0      |\n");
    printf("|                                                  |\n");
    printf("|          Professional Edition - 2026             |\n");
    printf("|                                                  |\n");
    printf("+--------------------------------------------------+\n");
}

/**
 * Displays the main menu and handles user navigation.
 */
void displayMenu(void) {
    int choice;
    int running = 1;
    
    while (running) {
        printf("\n");
        printf("+--------------------------------------------------+\n");
        printf("|              MAIN MENU                           |\n");
        printf("+--------------------------------------------------+\n");
        printf("|  [1]  Add New Order                              |\n");
        printf("|  [2]  Display All Orders                         |\n");
        printf("|  [3]  Search Order                               |\n");
        printf("|  [4]  Update Order                               |\n");
        printf("|  [5]  Delete Order                               |\n");
        printf("|  [6]  Save Orders to File                        |\n");
        printf("|  [7]  View Analytics Dashboard                   |\n");
        printf("|  [8]  Exit Application                           |\n");
        printf("+--------------------------------------------------+\n");
        printf("   Total Orders in System: %d / %d\n", orderCount, MAX_ORDERS);
        printf("   Pending Changes      : %s\n", hasUnsavedChanges ? "YES" : "NO");
        printf("+--------------------------------------------------+\n");
        
        if (!readInteger("Enter your choice (1-8): ", &choice)) {
            printf("\n[ERROR] Invalid input. Please enter a number between 1 and 8.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                addOrder();
                break;
            case 2:
                displayOrders();
                break;
            case 3:
                searchOrder();
                break;
            case 4:
                updateOrder();
                break;
            case 5:
                deleteOrder();
                break;
            case 6:
                if (saveToFile()) {
                    printf("\n[SUCCESS] Orders saved successfully!\n");
                }
                break;
            case 7:
                displayAnalytics();
                break;
            case 8:
                if (hasUnsavedChanges) {
                    if (promptYesNo("\n[WARN] Unsaved changes detected. Save before exit? (yes/no): ")) {
                        saveToFile();
                    }
                }
                printf("\n+--------------------------------------------------+\n");
                printf("|  Thank you for using our system. Goodbye!        |\n");
                printf("+--------------------------------------------------+\n\n");
                running = 0;
                break;
            default:
                printf("\n[ERROR] Invalid choice. Please select an option between 1 and 8.\n");
        }
    }
}

/**
 * Displays formatted details of a single order.
 * 
 * @param order Pointer to the order to display
 */
void displayOrderDetails(const Order *order) {
    if (order == NULL) return;
    
    printf("+--------------------------------------------------+\n");
    printf("| Order ID      : %-32d |\n", order->orderID);
    printf("| Customer      : %-32s |\n", order->customerName);
    printf("| Product       : %-32s |\n", order->productName);
    printf("| Quantity      : %-32d |\n", order->quantity);
    printf("| Unit Price    : $%-31.2f |\n", order->price);
    printf("| Total Amount  : $%-31.2f |\n", order->price * order->quantity);
    printf("| Status        : %-32s |\n", order->orderStatus);
    printf("+--------------------------------------------------+\n");
}

/**
 * Prompts the user to select a valid order status.
 */
void promptOrderStatus(char *buffer, int maxLength) {
    if (buffer == NULL || maxLength <= 0) {
        return;
    }

    printf("\nAvailable Order Status Options:\n");
    for (size_t i = 0; i < STATUS_OPTION_COUNT; i++) {
        printf("  [%zu] %s\n", i + 1, STATUS_OPTIONS[i]);
    }
    printf("  [0] Enter custom status\n");

    int selection;
    if (readInteger("Select status (0 for custom): ", &selection)) {
        if (selection == 0) {
            readString("Enter Order Status: ", buffer, maxLength);
        } else if (selection >= 1 && (size_t)selection <= STATUS_OPTION_COUNT) {
            strncpy(buffer, STATUS_OPTIONS[selection - 1], maxLength - 1);
            buffer[maxLength - 1] = '\0';
            return;
        } else {
            printf("[WARN] Invalid selection. Please enter a custom status.\n");
            readString("Enter Order Status: ", buffer, maxLength);
        }
    } else {
        printf("[WARN] Invalid input detected. Please enter a custom status.\n");
        readString("Enter Order Status: ", buffer, maxLength);
    }

    if (strlen(buffer) == 0) {
        strcpy(buffer, STATUS_OPTIONS[0]);
    }
}

/* =============================================================================
 * CORE OPERATION FUNCTIONS
 * ============================================================================= */

/**
 * Adds a new order to the system.
 */
void addOrder(void) {
    printf("\n+--------------------------------------------------+\n");
    printf("|              ADD NEW ORDER                       |\n");
    printf("+--------------------------------------------------+\n");
    
    /* Check capacity */
    if (orderCount >= MAX_ORDERS) {
        printf("\n[ERROR] Order limit reached (%d). Cannot add more orders.\n", MAX_ORDERS);
        printf("[TIP] Delete some orders or increase MAX_ORDERS limit.\n");
        return;
    }
    
    Order newOrder;
    int orderID;
    
    /* Get and validate Order ID */
    if (!readInteger("Enter Order ID: ", &orderID)) {
        printf("\n[ERROR] Invalid Order ID. Please enter a valid number.\n");
        return;
    }
    
    if (orderID <= 0) {
        printf("\n[ERROR] Order ID must be a positive number.\n");
        return;
    }
    
    /* Check for duplicate Order ID */
    if (findOrderByID(orderID) != -1) {
        printf("\n[ERROR] Order ID %d already exists. Please use a unique ID.\n", orderID);
        return;
    }
    
    newOrder.orderID = orderID;
    
    /* Get customer details */
    readString("Enter Customer Name: ", newOrder.customerName, MAX_NAME_LENGTH);
    if (strlen(newOrder.customerName) == 0) {
        printf("\n[ERROR] Customer name cannot be empty.\n");
        return;
    }
    
    readString("Enter Product Name: ", newOrder.productName, MAX_PRODUCT_LENGTH);
    if (strlen(newOrder.productName) == 0) {
        printf("\n[ERROR] Product name cannot be empty.\n");
        return;
    }
    
    /* Get quantity */
    if (!readInteger("Enter Quantity: ", &newOrder.quantity) || newOrder.quantity <= 0) {
        printf("\n[ERROR] Invalid quantity. Please enter a positive number.\n");
        return;
    }
    
    /* Get price */
    if (!readFloat("Enter Unit Price: $", &newOrder.price) || newOrder.price < 0) {
        printf("\n[ERROR] Invalid price. Please enter a non-negative number.\n");
        return;
    }
    
    promptOrderStatus(newOrder.orderStatus, MAX_STATUS_LENGTH);
    
    /* Add order to array */
    orders[orderCount] = newOrder;
    orderCount++;
    hasUnsavedChanges = true;
    
    printf("\n[SUCCESS] Order #%d added successfully!\n", newOrder.orderID);
    printf("[INFO] Total Amount: $%.2f\n", newOrder.price * newOrder.quantity);
}

/**
 * Displays all orders in the system.
 */
void displayOrders(void) {
    printf("\n+--------------------------------------------------+\n");
    printf("|              ORDER LIST                          |\n");
    printf("+--------------------------------------------------+\n");
    
    if (orderCount == 0) {
        printf("\n[INFO] No orders available in the system.\n");
        printf("[TIP] Use option [1] to add new orders.\n");
        return;
    }
    
    printf("\n[INFO] Displaying %d order(s):\n", orderCount);
    
    float totalRevenue = 0.0f;
    
    for (int i = 0; i < orderCount; i++) {
        printf("\n--- Order %d of %d ---\n", i + 1, orderCount);
        displayOrderDetails(&orders[i]);
        totalRevenue += orders[i].price * orders[i].quantity;
    }
    
    printf("\n+--------------------------------------------------+\n");
    printf("|  SUMMARY                                         |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Total Orders  : %-32d |\n", orderCount);
    printf("| Total Revenue : $%-31.2f |\n", totalRevenue);
    printf("+--------------------------------------------------+\n");
}

/**
 * Provides high-level metrics to help operators understand performance.
 */
void displayAnalytics(void) {
    printf("\n+--------------------------------------------------+\n");
    printf("|              ANALYTICS DASHBOARD                |\n");
    printf("+--------------------------------------------------+\n");

    if (orderCount == 0) {
        printf("\n[INFO] No orders available. Submit orders to unlock analytics.\n");
        return;
    }

    float totalRevenue = 0.0f;
    float highestOrderValue = -1.0f;
    int highestIndex = -1;
    int statusCounts[STATUS_OPTION_COUNT + 1];
    memset(statusCounts, 0, sizeof(statusCounts));

    for (int i = 0; i < orderCount; i++) {
        float currentValue = orders[i].price * orders[i].quantity;
        totalRevenue += currentValue;

        if (currentValue > highestOrderValue) {
            highestOrderValue = currentValue;
            highestIndex = i;
        }

        int statusIndex = mapStatusToIndex(orders[i].orderStatus);
        if (statusIndex < 0 || statusIndex > (int)STATUS_OPTION_COUNT) {
            statusIndex = (int)STATUS_OPTION_COUNT;
        }
        statusCounts[statusIndex]++;
    }

    float averageOrderValue = totalRevenue / orderCount;

    printf("\nKey Metrics:\n");
    printf("  Total Orders        : %d\n", orderCount);
    printf("  Total Revenue       : $%.2f\n", totalRevenue);
    printf("  Average Order Value : $%.2f\n", averageOrderValue);
    printf("  Highest Order Value : $%.2f\n", highestOrderValue);

    printf("\nStatus Breakdown:\n");
    for (size_t i = 0; i < STATUS_OPTION_COUNT; i++) {
        printf("  %-12s : %d\n", STATUS_OPTIONS[i], statusCounts[i]);
    }
    if (statusCounts[STATUS_OPTION_COUNT] > 0) {
        printf("  Custom/Other : %d\n", statusCounts[STATUS_OPTION_COUNT]);
    }

    if (highestIndex != -1) {
        printf("\nTop Performing Order:\n");
        displayOrderDetails(&orders[highestIndex]);
    }
}

/**
 * Searches for orders by ID or customer name.
 */
void searchOrder(void) {
    printf("\n+--------------------------------------------------+\n");
    printf("|              SEARCH ORDER                        |\n");
    printf("+--------------------------------------------------+\n");
    
    if (orderCount == 0) {
        printf("\n[INFO] No orders available to search.\n");
        return;
    }
    
    printf("\nSearch Options:\n");
    printf("  [1] Search by Order ID\n");
    printf("  [2] Search by Customer Name\n");
    
    int choice;
    if (!readInteger("Enter your choice: ", &choice)) {
        printf("\n[ERROR] Invalid input.\n");
        return;
    }
    
    if (choice == 1) {
        /* Search by Order ID */
        int searchID;
        if (!readInteger("Enter Order ID to search: ", &searchID)) {
            printf("\n[ERROR] Invalid Order ID.\n");
            return;
        }
        
        int index = findOrderByID(searchID);
        if (index != -1) {
            printf("\n[SUCCESS] Order found!\n");
            displayOrderDetails(&orders[index]);
        } else {
            printf("\n[INFO] No order found with ID: %d\n", searchID);
        }
        
    } else if (choice == 2) {
        /* Search by Customer Name */
        char searchName[MAX_NAME_LENGTH];
        readString("Enter Customer Name to search: ", searchName, MAX_NAME_LENGTH);

        if (strlen(searchName) == 0) {
            printf("\n[ERROR] Customer name cannot be empty.\n");
            return;
        }
        
        int foundCount = 0;
        printf("\n[INFO] Search results for \"%s\":\n", searchName);
        
        for (int i = 0; i < orderCount; i++) {
            if (containsIgnoreCase(orders[i].customerName, searchName)) {
                displayOrderDetails(&orders[i]);
                foundCount++;
            }
        }
        
        if (foundCount == 0) {
            printf("\n[INFO] No orders found for customer: %s\n", searchName);
        } else {
            printf("\n[INFO] Found %d matching order(s).\n", foundCount);
        }
        
    } else {
        printf("\n[ERROR] Invalid choice. Please select 1 or 2.\n");
    }
}

/**
 * Updates an existing order.
 */
void updateOrder(void) {
    printf("\n+--------------------------------------------------+\n");
    printf("|              UPDATE ORDER                        |\n");
    printf("+--------------------------------------------------+\n");
    
    if (orderCount == 0) {
        printf("\n[INFO] No orders available to update.\n");
        return;
    }
    
    int updateID;
    if (!readInteger("Enter Order ID to update: ", &updateID)) {
        printf("\n[ERROR] Invalid Order ID.\n");
        return;
    }
    
    int index = findOrderByID(updateID);
    if (index == -1) {
        printf("\n[ERROR] Order ID %d not found.\n", updateID);
        return;
    }
    
    printf("\n[INFO] Current order details:\n");
    displayOrderDetails(&orders[index]);
    
    printf("\n[INFO] Enter new details (press Enter to keep current value):\n");
    
    /* Update Customer Name */
    char tempStr[MAX_NAME_LENGTH];
    printf("Current Customer Name: %s\n", orders[index].customerName);
    readString("New Customer Name: ", tempStr, MAX_NAME_LENGTH);
    if (strlen(tempStr) > 0) {
        strcpy(orders[index].customerName, tempStr);
    }
    
    /* Update Product Name */
    printf("Current Product Name: %s\n", orders[index].productName);
    readString("New Product Name: ", tempStr, MAX_PRODUCT_LENGTH);
    if (strlen(tempStr) > 0) {
        strcpy(orders[index].productName, tempStr);
    }
    
    /* Update Quantity */
    int tempInt;
    printf("Current Quantity: %d\n", orders[index].quantity);
    if (readInteger("New Quantity (0 to skip): ", &tempInt) && tempInt > 0) {
        orders[index].quantity = tempInt;
    }
    
    /* Update Price */
    float tempFloat;
    printf("Current Price: $%.2f\n", orders[index].price);
    if (readFloat("New Price (-1 to skip): $", &tempFloat) && tempFloat >= 0) {
        orders[index].price = tempFloat;
    }
    
    /* Update Status */
    printf("Current Status: %s\n", orders[index].orderStatus);
    if (promptYesNo("Update status? (yes/no): ")) {
        promptOrderStatus(orders[index].orderStatus, MAX_STATUS_LENGTH);
    }
    
    printf("\n[SUCCESS] Order #%d updated successfully!\n", updateID);
    printf("\n[INFO] Updated order details:\n");
    displayOrderDetails(&orders[index]);
    hasUnsavedChanges = true;
}

/**
 * Deletes an order from the system.
 */
void deleteOrder(void) {
    printf("\n+--------------------------------------------------+\n");
    printf("|              DELETE ORDER                        |\n");
    printf("+--------------------------------------------------+\n");
    
    if (orderCount == 0) {
        printf("\n[INFO] No orders available to delete.\n");
        return;
    }
    
    int deleteID;
    if (!readInteger("Enter Order ID to delete: ", &deleteID)) {
        printf("\n[ERROR] Invalid Order ID.\n");
        return;
    }
    
    int index = findOrderByID(deleteID);
    if (index == -1) {
        printf("\n[ERROR] Order ID %d not found.\n", deleteID);
        return;
    }
    
    printf("\n[WARNING] You are about to delete the following order:\n");
    displayOrderDetails(&orders[index]);
    
    if (promptYesNo("Are you sure you want to delete this order? (yes/no): ")) {
        /* Shift remaining orders */
        for (int i = index; i < orderCount - 1; i++) {
            orders[i] = orders[i + 1];
        }
        orderCount--;
        hasUnsavedChanges = true;
        
        printf("\n[SUCCESS] Order #%d deleted successfully.\n", deleteID);
    } else {
        printf("\n[INFO] Delete operation cancelled.\n");
    }
}

/* =============================================================================
 * FILE OPERATION FUNCTIONS
 * ============================================================================= */

/**
 * Saves all orders to file.
 * 
 * @return 1 on success, 0 on failure
 */
int saveToFile(void) {
    FILE *file = fopen(DATA_FILE, "w");
    
    if (file == NULL) {
        printf("\n[ERROR] Unable to open file for writing: %s\n", DATA_FILE);
        printf("[TIP] Check file permissions and disk space.\n");
        return 0;
    }
    
    /* Write header comment */
    fprintf(file, "# E-Commerce Orders Database\n");
    fprintf(file, "# Format: OrderID,CustomerName,ProductName,Quantity,Price,Status\n");
    fprintf(file, "# Total Orders: %d\n", orderCount);
    
    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%d,%s,%s,%d,%.2f,%s\n",
                orders[i].orderID,
                orders[i].customerName,
                orders[i].productName,
                orders[i].quantity,
                orders[i].price,
                orders[i].orderStatus);
    }
    
    fclose(file);
    hasUnsavedChanges = false;
    printf("[INFO] %d order(s) saved to %s\n", orderCount, DATA_FILE);
    return 1;
}

/**
 * Loads orders from file.
 * 
 * @return Number of orders loaded
 */
int loadFromFile(void) {
    FILE *file = fopen(DATA_FILE, "r");
    
    if (file == NULL) {
        return 0;
    }
    
    char line[256];
    int loadedCount = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && orderCount < MAX_ORDERS) {
        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        
        Order tempOrder;
        int parsed = sscanf(line, "%d,%49[^,],%49[^,],%d,%f,%19[^\n]",
                           &tempOrder.orderID,
                           tempOrder.customerName,
                           tempOrder.productName,
                           &tempOrder.quantity,
                           &tempOrder.price,
                           tempOrder.orderStatus);
        
        if (parsed == 6) {
            orders[orderCount] = tempOrder;
            orderCount++;
            loadedCount++;
        }
    }
    
    fclose(file);
    hasUnsavedChanges = false;
    return loadedCount;
}

/* =============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================= */

/**
 * Clears the input buffer to prevent input issues.
 */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Reads an integer from user input with validation.
 * 
 * @param prompt The prompt to display
 * @param value  Pointer to store the result
 * @return 1 on success, 0 on failure
 */
int readInteger(const char *prompt, int *value) {
    printf("%s", prompt);
    
    if (scanf("%d", value) != 1) {
        clearInputBuffer();
        return 0;
    }
    
    clearInputBuffer();
    return 1;
}

/**
 * Reads a float from user input with validation.
 * 
 * @param prompt The prompt to display
 * @param value  Pointer to store the result
 * @return 1 on success, 0 on failure
 */
int readFloat(const char *prompt, float *value) {
    printf("%s", prompt);
    
    if (scanf("%f", value) != 1) {
        clearInputBuffer();
        return 0;
    }
    
    clearInputBuffer();
    return 1;
}

/**
 * Reads a string from user input safely.
 * 
 * @param prompt    The prompt to display
 * @param buffer    Buffer to store the result
 * @param maxLength Maximum length of input
 */
void readString(const char *prompt, char *buffer, int maxLength) {
    printf("%s", prompt);
    
    if (fgets(buffer, maxLength, stdin) != NULL) {
        trimNewline(buffer);
    } else {
        buffer[0] = '\0';
    }
}

/**
 * Finds an order by its ID.
 * 
 * @param orderID The order ID to search for
 * @return Index of the order, or -1 if not found
 */
int findOrderByID(int orderID) {
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderID == orderID) {
            return i;
        }
    }
    return -1;
}

/**
 * Removes trailing newline character from string.
 * 
 * @param str The string to trim
 */
void trimNewline(char *str) {
    if (str == NULL) return;
    
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/**
 * Pauses execution until user presses Enter.
 */
void pressEnterToContinue(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

/**
 * Case-insensitive string comparison.
 */
int equalsIgnoreCase(const char *a, const char *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

/**
 * Converts a string to lowercase safely.
 */
void toLowerCopy(const char *src, char *dest, size_t destSize) {
    if (src == NULL || dest == NULL || destSize == 0) {
        return;
    }
    size_t i = 0;
    for (; i < destSize - 1 && src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

/**
 * Checks if haystack contains needle, case-insensitive.
 */
int containsIgnoreCase(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) {
        return 0;
    }

    char haystackLower[TEMP_BUFFER_LENGTH];
    char needleLower[TEMP_BUFFER_LENGTH];
    toLowerCopy(haystack, haystackLower, sizeof(haystackLower));
    toLowerCopy(needle, needleLower, sizeof(needleLower));

    if (strlen(needleLower) == 0) {
        return 0;
    }

    return strstr(haystackLower, needleLower) != NULL;
}

/**
 * Maps a status string to the index of the predefined status array.
 */
int mapStatusToIndex(const char *status) {
    if (status == NULL) {
        return (int)STATUS_OPTION_COUNT;
    }
    for (size_t i = 0; i < STATUS_OPTION_COUNT; i++) {
        if (equalsIgnoreCase(status, STATUS_OPTIONS[i])) {
            return (int)i;
        }
    }
    return (int)STATUS_OPTION_COUNT;
}

/**
 * Prompts the user for a yes/no response.
 */
int promptYesNo(const char *message) {
    char response[16];
    readString(message, response, (int)sizeof(response));
    char normalized[16];
    toLowerCopy(response, normalized, sizeof(normalized));
    if (normalized[0] == 'y') {
        return 1;
    }
    return 0;
}
