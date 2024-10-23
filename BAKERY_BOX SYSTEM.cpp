#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Define maximum sizes for user input
#define MAX_NAME_LEN 50
#define MAX_ADDRESS_LEN 50
#define MAX_PHONE_NUM_LEN 50
#define MAX_TYPE_LEN 10
#define MAX_CUSTOMERS 1000

// STRUCTURE
// Structure to hold order information
struct Order {
    char type[MAX_TYPE_LEN];
    int chocFilling;
    int strawbFilling;
    int wholeCake;
    int slicesCake;
    int cakeFlav;
    int quantityEdit;
};

// Structure to hold customer information
struct Customer {
    char name[MAX_NAME_LEN];
    int services; // 1 for DELIVERY, 2 for PICKUP
    char address[MAX_ADDRESS_LEN];
    char phoneNum[MAX_PHONE_NUM_LEN];
    int paymentMethod; // 1 for CASH, 2 for QRSCAN, 3 for TRANSFER
    struct Order order;
    int membership; // 1 for Members, 2 for Non-Members
};

// FUNCTION DECLARATIONS
void display(struct Customer c[], int count);
double totalPrice(struct Customer c[], int index);
void generateReport(struct Customer c[], int count);
void searchCustomer(struct Customer c[], int count, char name[]);
void displayReceipt(struct Customer c);
void editOrder(struct Customer c[], int count);
double calculateAverageTotalPrice(struct Customer c[], int count);

// Function to display customer information
void display(struct Customer c[], int count) {
    printf("\nDisplaying information\n");
    for (int i = 0; i < count; i++) {
        printf("Customer %d:\n", i + 1);
        printf("Name: %s\n", c[i].name);
        printf("Services: %s\n", c[i].services == 1 ? "DELIVERY" : "PICKUP");
        printf("Address: %s\n", c[i].address);
        printf("Phone Number: %s\n", c[i].phoneNum);
        printf("Payment Method: ");
        if (c[i].paymentMethod == 1) printf("CASH\n");
        else if (c[i].paymentMethod == 2) printf("QRSCAN\n");
        else printf("TRANSFER\n");
        printf("Order Type: %s\n", c[i].order.type);
        printf("Order Details:\n");
        if (strcmp(c[i].order.type, "BUN") == 0) 
		{
            printf("Chocolate Buns: %d\n", c[i].order.chocFilling);
            printf("Strawberry Buns: %d\n", c[i].order.strawbFilling);
        } 
		else if (strcmp(c[i].order.type, "CAKE") == 0) 
		{
            printf("Whole Cakes: %d\n", c[i].order.wholeCake);
            printf("Slices Cakes: %d\n", c[i].order.slicesCake);
        }
        printf("\n");
    }
}

// Function to calculate total payment not including tax and delivery charge
double totalPrice(struct Customer c[], int index) {
    double totalPrice = 0.00;
    if (strcmp(c[index].order.type, "BUN") == 0) 
	{
        totalPrice = (c[index].order.chocFilling * 1.50) + (c[index].order.strawbFilling * 1.00);
    } 
	else if (strcmp(c[index].order.type, "CAKE") == 0) {
        double priceWholeCake = 0.00;
        if (c[index].order.cakeFlav == 1 || c[index].order.cakeFlav == 3) 
		{
            priceWholeCake = 20.00 * c[index].order.wholeCake;
        } 
		else if (c[index].order.cakeFlav == 2) 
		{
            priceWholeCake = 25.00 * c[index].order.wholeCake;
        }
        double priceSlices = 5.00 * c[index].order.slicesCake;
        totalPrice = priceWholeCake + priceSlices;
    }
    return totalPrice;
}

// Function to generate report with highest and lowest order by pastry type

void generateReport(struct Customer c[], int count) {
    int highestCount = 0, lowestCount = 9999999;
    char highestType[MAX_TYPE_LEN], lowestType[MAX_TYPE_LEN];
    struct Customer highestCustomer, lowestCustomer;
    int totalCount = 0;
    int totalCus = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(c[i].order.type, "BUN") == 0) {
            totalCount = c[i].order.chocFilling + c[i].order.strawbFilling;
        } else if (strcmp(c[i].order.type, "CAKE") == 0) {
            totalCount = c[i].order.wholeCake + c[i].order.slicesCake;
        }

        if (totalCount > highestCount) {
            highestCount = totalCount;
            strcpy(highestType, c[i].order.type);
            highestCustomer = c[i];
        }
        if (totalCount < lowestCount) {
            lowestCount = totalCount;
            strcpy(lowestType, c[i].order.type);
            lowestCustomer = c[i];
        }
        totalCus++;
    }

    printf("Pastry type with highest order: %s (%d items)\n", highestType, highestCount);
    printf("Customer with highest order: %s\n", highestCustomer.name);
    printf("Address: %s\n", highestCustomer.address);
    printf("Phone: %s\n\n", highestCustomer.phoneNum);

    printf("Pastry type with lowest order: %s (%d items)\n", lowestType, lowestCount);
    printf("Customer with lowest order: %s\n", lowestCustomer.name);
    printf("Address: %s\n", lowestCustomer.address);
    printf("Phone: %s\n\n", lowestCustomer.phoneNum);
    
    double averageTotalPrice = calculateAverageTotalPrice(c, totalCus);
    printf("\nAverage total price per customer: RM %.2f\n", averageTotalPrice);

}

double calculateAverageTotalPrice(struct Customer c[], int count) {
    double sumTotalPrice = 0.0;
    for (int i = 0; i < count; i++) {
        double discount = (c[i].membership == 1) ? 0.06 : 0.00;
        double totalPastry = totalPrice(c, i);
        double totalDisc = totalPastry * discount;
        double tax = (totalPastry - totalDisc) * 0.06;
        double deliveryCharge = (c[i].services == 1) ? 8.00 : 0.00;
        double totalOrder = totalPastry - totalDisc + tax + deliveryCharge;
        sumTotalPrice += totalOrder;
    }
    double averageTotalPrice = sumTotalPrice / count;
    return averageTotalPrice;
}

// Function to search orders based on customer name
void searchCustomer(struct Customer c[], int count, char name[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(c[i].name, name) == 0) 
		{
            printf("Order found for customer %s:\n", name);
            display(&c[i], 1); // Display the found order
            return;
        }
    }
    printf("No order found for customer %s.\n", name);
}

// Function to search customer name and edit the details
void editCustomer(struct Customer c[], int count, char name[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(c[i].name, name) == 0) {
            int choice;
            printf("Customer found: %s\n", c[i].name);
            printf("1. Edit Name\n");
            printf("2. Edit Address\n");
            printf("3. Edit Phone Number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); // To consume newline character left in buffer

            switch (choice) {
                case 1:
                    printf("Enter new name: ");
                    fgets(c[i].name, MAX_NAME_LEN, stdin);
                    c[i].name[strcspn(c[i].name, "\n")] = '\0'; // Remove newline character
                    printf("Name updated successfully.\n");
                    break;
                case 2:
                    printf("Enter new address: ");
                    fgets(c[i].address, MAX_ADDRESS_LEN, stdin);
                    c[i].address[strcspn(c[i].address, "\n")] = '\0'; // Remove newline character
                    printf("Address updated successfully.\n");
                    break;
                case 3:
                    printf("Enter new phone number: ");
                    fgets(c[i].phoneNum, MAX_PHONE_NUM_LEN, stdin);
                    c[i].phoneNum[strcspn(c[i].phoneNum, "\n")] = '\0'; // Remove newline character
                    printf("Phone number updated successfully.\n");
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
            }
            printf("\nNEW UPDATED DETAILS\n");
            printf("Name: %s\n", c[i].name);
            printf("Address: %s\n", c[i].address);
            printf("Phone Number: %s\n", c[i].phoneNum);
            return;
        }
    }
    printf("No order found for customer %s.\n", name);
}


// Function to display receipt
void displayReceipt(struct Customer c) {
    double discount = (c.membership == 1) ? 0.06 : 0.00;
    double totalPastry = totalPrice(&c, 0);
    double totalDisc = totalPastry * discount;
    double tax = (totalPastry - totalDisc) * 0.06;
    double deliveryCharge = (c.services == 1) ? 8.00 : 0.00;
    double totalOrder = totalPastry - totalDisc + tax + deliveryCharge;

    printf("\n\n\t\t\t##### WELCOME TO BAKERYBOX SYSTEM #####");
    printf("\n\t\t\t\tCUSTOMER RECEIPT");
    printf("\n==================================================================================================");
    printf("\n\t Customer Name     : %s", c.name);
    printf("\n\t Services          : %s", c.services == 1 ? "DELIVERY" : "PICKUP");
    printf("\n\t Address           : %s", c.address);
    printf("\n\t Phone Number      : %s", c.phoneNum);
    printf("\n\t Payment Method    : ");
    if (c.paymentMethod == 1) printf("CASH\n");
    else if (c.paymentMethod == 2) printf("QRSCAN\n");
    else printf("TRANSFER\n");
    printf("\n--------------------------------------------------------------------------------------------------");
    printf("\n\t Order Type        : %s", c.order.type);
    printf("\n\t Order Details     :");
    if (strcmp(c.order.type, "BUN") == 0) {
        printf("\n\t Chocolate Buns    : %d", c.order.chocFilling);
        printf("\n\t Strawberry Buns   : %d", c.order.strawbFilling);
    } else if (strcmp(c.order.type, "CAKE") == 0) {
        printf("\n\t Whole Cakes       : %d", c.order.wholeCake);
        printf("\n\t Slices Cakes      : %d", c.order.slicesCake);
    }
    printf("\n--------------------------------------------------------------------------------------------------");
    printf("\n\t TOTAL ORDER                                                     : RM %.2f", totalPastry);
    printf("\n\t TOTAL DISCOUNT                                                  : RM %.2f", totalDisc);
    printf("\n\t TOTAL TAX                                                       : RM %.2f", tax);
    printf("\n\t DELIVERY CHARGE                                                 : RM %.2f", deliveryCharge);
    printf("\n--------------------------------------------------------------------------------------------------");
    printf("\n\t TOTAL PRICES                                                    : RM %.2f", totalOrder);
    printf("\n--------------------------------------------------------------------------------------------------");
    printf("\n\t\t\t\tTHANK YOU FOR BUYING!\n\n");
}

// Function to edit order
void editOrder(struct Customer c[], int count) {
    for (int i = 0; i < count; i++) {
        char editConfirmation[MAX_TYPE_LEN];
        //printf("Edit order for customer %s? (Yes/No): ", c[i].name);
        //fgets(editConfirmation, MAX_TYPE_LEN, stdin);
        editConfirmation[strcspn(editConfirmation, "\n")] = '\0';

        while (strcmp(editConfirmation, "Y") == 0) {
            int editPastry;
            printf("Enter the type of pastry to edit (1-chocFilling, 2-strawberryFilling, 3-wholeCake, 4-slices): ");
            scanf("%d", &editPastry);
            getchar();

            if (strcmp(c[i].order.type, "BUN") == 0) {
                printf("Enter quantity to delete: ");
                scanf("%d", &c[i].order.quantityEdit);
                getchar();

                if (editPastry == 1) {
                    c[i].order.chocFilling -= c[i].order.quantityEdit;
                } else if (editPastry == 2) {
                    c[i].order.strawbFilling -= c[i].order.quantityEdit;
                }
            } else if (strcmp(c[i].order.type, "CAKE") == 0) {
                printf("Enter quantity to delete: ");
                scanf("%d", &c[i].order.quantityEdit);
                getchar();

                if (editPastry == 3) {
                    c[i].order.wholeCake -= c[i].order.quantityEdit;
                } else if (editPastry == 4) {
                    c[i].order.slicesCake -= c[i].order.quantityEdit;
                }
            }
            printf("Edit more order? (Y-Yes/N-No): ");
            fgets(editConfirmation, MAX_TYPE_LEN, stdin);
            editConfirmation[strcspn(editConfirmation, "\n")] = '\0';
        }
    }
}

int main() {
    struct Customer customers[MAX_CUSTOMERS];
    int customerCount = 0;
    char typeUser[MAX_TYPE_LEN];
    bool orderFlag = false;
    bool exitFlag = false; // Added exit flag for the loop

    do {
        printf("\n##### WELCOME TO BAKERYBOX SYSTEM #####\n");
        printf("[C] CUSTOMER [S] STAFF\n");
        fgets(typeUser, MAX_TYPE_LEN, stdin);
        typeUser[strcspn(typeUser, "\n")] = '\0';

        if (strcmp(typeUser, "C") == 0) {
            do {
                printf("\nCUSTOMER NAME: ");
                fgets(customers[customerCount].name, MAX_NAME_LEN, stdin);
                customers[customerCount].name[strcspn(customers[customerCount].name, "\n")] = '\0';  // Remove the trailing newline character

                printf("SERVICES [1-DELIVERY | 2-PICKUP]: ");
                scanf("%d", &customers[customerCount].services);
                getchar(); // Consume the newline character left by scanf

                printf("ADDRESS [STREET, CITY, STATE]: ");
                fgets(customers[customerCount].address, MAX_ADDRESS_LEN, stdin);
                customers[customerCount].address[strcspn(customers[customerCount].address, "\n")] = '\0';

                printf("PHONE NUMBER [XXX-XXX XXXX]: ");
                fgets(customers[customerCount].phoneNum, MAX_PHONE_NUM_LEN, stdin);
                customers[customerCount].phoneNum[strcspn(customers[customerCount].phoneNum, "\n")] = '\0';

                printf("PAYMENT METHOD [1-CASH | 2-QRSCAN | 3-TRANSFER]: ");
                scanf("%d", &customers[customerCount].paymentMethod);
                getchar(); // Consume the newline character left by scanf

                printf("MEMBERSHIP [1-Members | 2-Non-Members]: ");
                scanf("%d", &customers[customerCount].membership);
                getchar(); // Consume the newline character left by scanf

                // GET CUSTOMER ORDER FOR PASTRY TYPE
                // Customer can only buy one type of pastry at one time
                do {
                    printf("PASTRY TYPE [1-BUN | 2-CAKE]: ");
                    int pastryType;
                    scanf("%d", &pastryType);
                    getchar();  // Consume the newline character left by scanf

                    if (pastryType == 1) 
					{
                        strcpy(customers[customerCount].order.type, "BUN");
                        printf("\nFill in the amount of buns you want for each filling\n");
                        printf("CHOCOLATE BUN: ");
                        scanf("%d", &customers[customerCount].order.chocFilling);
                        printf("STRAWBERRY BUN: ");
                        scanf("%d", &customers[customerCount].order.strawbFilling);
                        getchar();  // Consume the newline character left by scanf
                        orderFlag = true;
                    } 
					else if (pastryType == 2) 
					{
                        strcpy(customers[customerCount].order.type, "CAKE");
                        printf("\nFill in the amount of cake in each\n");
                        printf("WHOLE CAKE: ");
                        scanf("%d", &customers[customerCount].order.wholeCake);
                        printf("SLICES CAKE: ");
                        scanf("%d", &customers[customerCount].order.slicesCake);
                        getchar();  // Consume the newline character left by scanf
                        orderFlag = true;

                        printf("\nCAKE FLAVOR: ");
                        printf("\n1. CHOCOLATE");
                        printf("\n2. STRAWBERRY");
                        printf("\n3. VANILLA\n");
                        scanf("%d", &customers[customerCount].order.cakeFlav);
                        getchar();  // Consume the newline character left by scanf
                    } 
					else 
					{
                        printf("Invalid option. Please try again.\n");
                        orderFlag = false;
                    }
                } while (!orderFlag);

                // Ask if the customer wants to edit the order
                char editConfirmation[MAX_TYPE_LEN];
                printf("Do you want to edit your order? (Yes/No): ");
                fgets(editConfirmation, MAX_TYPE_LEN, stdin);
                editConfirmation[strcspn(editConfirmation, "\n")] = '\0';

                while (strcmp(editConfirmation, "Yes") == 0 || strcmp(editConfirmation, "yes") == 0 || strcmp(editConfirmation, "Y") == 0 || strcmp(editConfirmation, "y") == 0) {
                    int editChoice;
                    printf("Enter the item to edit (1-CHOCOLATE BUN, 2-STRAWBERRY BUN, 3-WHOLE CAKE, 4-SLICES CAKE): ");
                    scanf("%d", &editChoice);
                    getchar();  // Consume the newline character left by scanf

                    int newQuantity;
                    printf("Enter the new quantity (or 0 to delete the item): ");
                    scanf("%d", &newQuantity);
                    getchar();  // Consume the newline character left by scanf

                    if (editChoice == 1) 
					{
                        customers[customerCount].order.chocFilling = newQuantity;
                    } 
					else if (editChoice == 2) 
					{
                        customers[customerCount].order.strawbFilling = newQuantity;
                    } 
					else if (editChoice == 3) 
					{
                        customers[customerCount].order.wholeCake = newQuantity;
                    } 
					else if (editChoice == 4) 
					{
                        customers[customerCount].order.slicesCake = newQuantity;
                    } 
					else 
					{
                        printf("Invalid option. Please try again.\n");
                    }

                    printf("Do you want to edit your order? (Yes/No): ");
                    fgets(editConfirmation, MAX_TYPE_LEN, stdin);
                    editConfirmation[strcspn(editConfirmation, "\n")] = '\0';
                }

                // Generate receipt
                displayReceipt(customers[customerCount]);
                customerCount++;

                printf("Do you want to make another order? [y/n]: ");
                char repeat;
                scanf(" %c", &repeat);
                getchar(); // Consume the newline character left by scanf
                if (repeat == 'n' || repeat == 'N') {
                    break;
                }
            } while (true);
        } 
		else if (strcmp(typeUser, "S") == 0) {
            int choice;
            printf("\n##### STAFF MENU #####\n");
            printf("[1] DISPLAY ALL ORDERS\n[2] GENERATE REPORT\n[3] SEARCH CUSTOMER ORDER\n[4] EDIT CUSTOMER DETAILS\n[5] EXIT\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); // Consume the newline character left by scanf

            if (choice == 1) 
			{
                display(customers, customerCount);
            } 
			else if (choice == 2) 
			{
                generateReport(customers, customerCount);
            } 
			else if (choice == 3) 
			{
                char name[MAX_NAME_LEN];
                printf("Enter customer name to search: ");
                fgets(name, MAX_NAME_LEN, stdin);
                name[strcspn(name, "\n")] = '\0';
                searchCustomer(customers, customerCount, name);
            }  
            else if (choice == 4) 
			{
                char name[MAX_NAME_LEN];
                printf("Enter customer name to search: ");
                fgets(name, MAX_NAME_LEN, stdin);
                name[strcspn(name, "\n")] = '\0';
                editCustomer(customers, customerCount, name);
            } 
			else if (choice == 5) 
			{
                exitFlag = true;
            } 
			else 
			{
                printf("Invalid choice. Please try again.\n");
            }
        } 
		else 
		{
            printf("Invalid option. Please enter 'C' for customer or 'S' for staff.\n");
        }
    } while (!exitFlag);

    return 0;
}

