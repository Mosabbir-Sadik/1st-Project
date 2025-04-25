#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int orderID;
    char customerName[50];
    char productName[50];
    int quantity;
    float price;
    char orderStatus[20];
} Order;

#define MAX_ORDERS 100

Order orders[MAX_ORDERS];
int orderCount = 0;

void addOrder();
void displayOrders();
void searchOrder();
void updateOrder();
void saveToFile();
void loadFromFile();
void menu();

int main() {
    printf("**********************************************\n");
    printf("|                                            |\n");
    printf("|      Welcome to the E-commerce System      |\n");
    printf("|                                            |\n");
    printf("**********************************************\n");
    printf("\nLoading data...\n");

    loadFromFile();
    menu();
    return 0;
}

void menu() {
    int choice;
    do {
        printf("\n");
        printf("**********************************************\n");
        printf("|                                            |\n");
        printf("|  >> E-commerce Order Management System <<  |\n");
        printf("|                                            |\n");
        printf("**********************************************\n");
        printf("============================================\n");
        printf("||           1. Add New Order             ||\n");
        printf("||           2. Display All Orders        ||\n");
        printf("||           3. Search Order              ||\n");
        printf("||           4. Update Order              ||\n");
        printf("||           5. Save Orders to File       ||\n");
        printf("||           6. Exit                      ||\n");
        printf("============================================\n");
        printf("Enter your choice: >> ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        getchar(); // Consume newline character

        switch (choice) {
            case 1: addOrder(); break;
            case 2: displayOrders(); break;
            case 3: searchOrder(); break;
            case 4: updateOrder(); break;
            case 5: saveToFile(); break;
            case 6: 
                saveToFile(); 
                printf("Thank you for using the system. Goodbye!\n"); 
                break;
            default: 
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

void addOrder() {
    if (orderCount >= MAX_ORDERS) {
        printf("Error: Order limit reached. Cannot add more orders.\n");
        return;
    }
    printf("\nEnter Order ID: ");
    scanf("%d", &orders[orderCount].orderID);
    getchar();
    printf("Enter Customer Name: ");
    fgets(orders[orderCount].customerName, 50, stdin);
    orders[orderCount].customerName[strcspn(orders[orderCount].customerName, "\n")] = 0;
    printf("Enter Product Name: ");
    fgets(orders[orderCount].productName, 50, stdin);
    orders[orderCount].productName[strcspn(orders[orderCount].productName, "\n")] = 0;
    printf("Enter Quantity: ");
    scanf("%d", &orders[orderCount].quantity);
    printf("Enter Price: ");
    scanf("%f", &orders[orderCount].price);
    getchar();
    printf("Enter Order Status: ");
    fgets(orders[orderCount].orderStatus, 20, stdin);
    orders[orderCount].orderStatus[strcspn(orders[orderCount].orderStatus, "\n")] = 0;
    orderCount++;
    printf("\nOrder added successfully!\n");
}

void displayOrders() {
    if (orderCount == 0) {
        printf("\nNo orders available to display.\n");
        return;
    }
    printf("\n============================================\n");
    printf("               Order List\n");
    printf("============================================\n");
    for (int i = 0; i < orderCount; i++) {
        printf("Order ID: %d\n", orders[i].orderID);
        printf("Customer Name: %s\n", orders[i].customerName);
        printf("Product Name: %s\n", orders[i].productName);
        printf("Quantity: %d\n", orders[i].quantity);
        printf("Price: %.2f\n", orders[i].price);
        printf("Order Status: %s\n", orders[i].orderStatus);
        printf("--------------------------------------------\n");
    }
}

void searchOrder() {
    int searchID;
    char searchName[50];
    printf("\nSearch by:\n1. Order ID\n2. Customer Name\nEnter choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
    getchar();

    if (choice == 1) {
        printf("Enter Order ID to search: ");
        scanf("%d", &searchID);
        for (int i = 0; i < orderCount; i++) {
            if (orders[i].orderID == searchID) {
                printf("\nOrder Found:\n");
                printf("Order ID: %d\n", orders[i].orderID);
                printf("Customer Name: %s\n", orders[i].customerName);
                printf("Product Name: %s\n", orders[i].productName);
                printf("Quantity: %d\n", orders[i].quantity);
                printf("Price: %.2f\n", orders[i].price);
                printf("Order Status: %s\n", orders[i].orderStatus);
                return;
            }
        }
        printf("\nOrder not found.\n");
    } else if (choice == 2) {
        printf("Enter Customer Name to search: ");
        fgets(searchName, 50, stdin);
        searchName[strcspn(searchName, "\n")] = 0;
        for (int i = 0; i < orderCount; i++) {
            if (strcmp(orders[i].customerName, searchName) == 0) {
                printf("\nOrder Found:\n");
                printf("Order ID: %d\n", orders[i].orderID);
                printf("Customer Name: %s\n", orders[i].customerName);
                printf("Product Name: %s\n", orders[i].productName);
                printf("Quantity: %d\n", orders[i].quantity);
                printf("Price: %.2f\n", orders[i].price);
                printf("Order Status: %s\n", orders[i].orderStatus);
                return;
            }
        }
        printf("\nOrder not found.\n");
    } else {
        printf("\nInvalid choice.\n");
    }
}

void updateOrder() {
    int updateID;
    printf("\nEnter Order ID to update: ");
    scanf("%d", &updateID);
    getchar();
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderID == updateID) {
            printf("Enter new Customer Name: ");
            fgets(orders[i].customerName, 50, stdin);
            orders[i].customerName[strcspn(orders[i].customerName, "\n")] = 0;
            printf("Enter new Product Name: ");
            fgets(orders[i].productName, 50, stdin);
            orders[i].productName[strcspn(orders[i].productName, "\n")] = 0;
            printf("Enter new Quantity: ");
            scanf("%d", &orders[i].quantity);
            printf("Enter new Price: ");
            scanf("%f", &orders[i].price);
            getchar();
            printf("Enter new Order Status: ");
            fgets(orders[i].orderStatus, 20, stdin);
            orders[i].orderStatus[strcspn(orders[i].orderStatus, "\n")] = 0;
            printf("\nOrder updated successfully!\n");
            return;
        }
    }
    printf("\nOrder ID not found.\n");
}

void saveToFile() {
    FILE *file = fopen("orders.txt", "w");
    if (file == NULL) {
        printf("\nError saving to file.\n");
        return;
    }
    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%d,%s,%s,%d,%.2f,%s\n", orders[i].orderID, orders[i].customerName, orders[i].productName, orders[i].quantity, orders[i].price, orders[i].orderStatus);
    }
    fclose(file);
    printf("\nOrders saved successfully to orders.txt!\n");
}

void loadFromFile() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%f,%19[^\n]\n", &orders[orderCount].orderID, orders[orderCount].customerName, orders[orderCount].productName, &orders[orderCount].quantity, &orders[orderCount].price, orders[orderCount].orderStatus) == 6) {
        orderCount++;
    }
    fclose(file);
}
