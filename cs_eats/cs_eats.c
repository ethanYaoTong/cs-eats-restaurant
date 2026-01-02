// CS Eats
// cs_eats.c
// Written by Ethan Tong, z5691989
// on 7/08/2025

////////////////////////////////////////////////////////////////////////////////
// Provided Libraries
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs_eats.h"

////////////////////////////////////////////////////////////////////////////////
// Your Libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Function Definitions
////////////////////////////////////////////////////////////////////////////////

// Stage 1.1
// Function to create a restaurant
// Params:
//      name - the restaurant's name
// Returns: a pointer to the new restaurant
struct restaurant *create_restaurant(char name[MAX_SIZE]) {
    // TODO: Replace this with your code
    struct restaurant *new_restaurant = malloc(sizeof(struct restaurant));
    strcpy(new_restaurant->name, name);
    new_restaurant->profit = 0;
    new_restaurant->customers = NULL;
    new_restaurant->orders = NULL;
    return new_restaurant;
}

// Stage 1.1
// Function to create a customer
// Params:
//      name     - the customer's name
//      balance  - the amount of money the customer starts with ($)
//      distance - the distance of the customer from the restaurant (km)
// Returns: a pointer to the new customer
struct customer *create_customer(
    char name[MAX_SIZE],
    int balance,
    int distance
) {
    // TODO: Replace this with your code
    struct customer *new_customer = malloc(sizeof(struct customer));
    strcpy(new_customer->name, name);
    new_customer->balance = balance;
    new_customer->distance = distance;
    new_customer->orders = NULL;
    new_customer->next = NULL;
    return new_customer;
}

// Stage 1.1
// Function to create an order
// Params:
//      customer_name - the name of the customer who placed the order
//      flavour       - the order's selected flavour
//      protein       - the order's selected protein
//      base          - the order's selected base menu item
//      id            - the order's unique identification number
//      status        - the order's status
// Returns: a pointer to the new order
struct order *create_order(
    char customer_name[MAX_SIZE],
    enum order_flavour flavour,
    enum order_protein protein,
    enum order_base base,
    int id,
    enum order_status status
) {
    // TODO: Replace this with your code
    struct order *new_order = malloc(sizeof(struct order));
    strcpy(new_order->customer_name, customer_name);
    new_order->flavour = flavour;
    new_order->protein = protein;
    new_order->base = base;
    new_order->id = id;
    new_order->status = status;
    new_order->next = NULL;
    return new_order;
}

// Stage 1.2
// Function to run the command loop
// Params:
//     restaurant - a pointer to the restaurant
// Returns: None
void command_loop(struct restaurant *restaurant) {
    // TODO: Replace this with your code
    char command;
    char command2;
    struct driver driver;
    driver.enabled = 0;
    printf("Enter command: ");
    while (scanf(" %c", &command) == 1 && command != 'q') {
        if (command == '?') {
            print_help();
        } else if (command == 'a') {
            scanf(" %c ", &command2);
            if (command2 == 'c') {
                add_customer(restaurant);
            } else if (command2 == 'o') {
                add_order(restaurant);
            } else if (command2 == 'd') {
                driver = add_driver(restaurant, driver);
            }
        } else if (command == 'p') {
            print_restaurant(restaurant, driver);
        } else if (command == 'i') {
            scanf(" %c ", &command2);
            if (command2 == 'c') {
                insert_customer(restaurant);
            }
        } else if (command == 'c') {
            cook_orders(restaurant);
        } else if (command == 'd') {
            deliver_orders(restaurant);
        } else if (command == 'x') {
            scanf(" %c ", &command2);
            if (command2 == 'o') {
                cancel_order(restaurant);
            } else if (command2 == 'c') {
                remove_customer(restaurant);
            }
        } else if (command == 'e') {
            cust_eat(restaurant);
        } else if (command == 'm') {
            merge_customer(restaurant);
        } else if (command == 's') {
            sort(restaurant);
        } else {
            printf("ERROR: Invalid command\n");
        }
        printf("Enter command: ");
    }
    free_memory(restaurant);
}

// Stage 1.3
// Function to add a customer to the restaurant
// Params:
//      restaurant - a pointer to the restaurant
// Returns: None
void add_customer(struct restaurant *restaurant) {
    // TODO: Replace this with your code
    char name[MAX_SIZE];
    int balance, distance;
    scan_name(name);
    scanf(" %d %d", &balance, &distance);
    if (customer_errors(restaurant, name, balance, distance)) {
        struct customer *new_cust = create_customer(name, balance, distance);
        if (restaurant->customers != NULL) {
            new_cust->next = restaurant->customers;
            restaurant->customers = new_cust;
        } else {
            restaurant->customers = new_cust;
        }
        printf("Customer '");
        fputs(name, stdout);
        printf("' added\n");
    }
    
}

// Stage 1.4
// Function to print the restaurant
// Params:
//      restaurant - a pointer to the restaurant
// Returns: None
void print_restaurant(struct restaurant *restaurant, struct driver driver) {
    // TODO: Replace this with your code
    print_header(restaurant->name);
    printf("Profit: $%d\n", restaurant->profit);
    printf("[CUSTOMERS]\n");
    struct customer *current = restaurant->customers;
    if (current == NULL) {
        printf("  (no customers)\n");
    }
    while (current != NULL) {
        print_customer(current);
        struct order *cust_order = current->orders;
        while (cust_order != NULL) {
            print_order(cust_order);
            cust_order = cust_order->next;
        }
        current = current->next;
    }
    printf("[ORDERS]\n");
    struct order *curr_order = restaurant->orders;
    if (curr_order == NULL) {
        printf("  (no orders)\n");
    }
    while (curr_order != NULL) {
        if (curr_order->status != CANCELLED) {
            print_order(curr_order);
        }
        curr_order = curr_order->next;
    }
    if (driver.enabled == 1) {
        printf("[DRIVER]\n");
        print_driver_details(driver.name, driver.action);
    }
    printf("\n");
}


// Add your function definitions below this line

// This function returns FALSE if error is found, returns TRUE if no errors.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - name: The name of the customer.
// - balance: The amount of money the customer has.
// - distance: The distance the customer is from the restaurant.
//
// returns: Either TRUE or FALSE
int customer_errors(struct restaurant *restaurant, char name[MAX_SIZE], 
                    int balance, int distance) {
    struct customer *current = restaurant->customers;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0) {
            printf("ERROR: Customer '%s' already exists\n", name);
            return FALSE;
        }
        current = current->next;
    }
    if (balance <= 0) {
        printf("ERROR: Balance must be positive\n");
        return FALSE;
    } else if (distance <= 0) {
        printf("ERROR: Distance must be positive\n");
        return FALSE;
    } else {
        return TRUE;
    }
}

// This function scans input and inserts a new customer into the restaurant's 
// customer list at a specified position. It first checks for input errors.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void insert_customer(struct restaurant *restaurant) {
    char name[MAX_SIZE];
    int balance, distance, position, counter = 0;

    // Scanning information
    scan_name(name);
    scanf(" %d %d %d", &balance, &distance, &position);

    // Proceed only if no input errors
    if (customer_errors(restaurant, name, balance, distance)) {
        struct customer *new_customer = create_customer(name, 
                                                        balance, distance);
        // inserting at the head
        if (position <= 1 || restaurant->customers == NULL) {
            struct customer *current = restaurant->customers;
            restaurant->customers = new_customer;
            new_customer->next = current;
        } else {
            // inserting at nth
            struct customer *current = restaurant->customers;
            while (current->next != NULL && counter < position - 2) {
                current = current->next;
                counter++;
            }
            new_customer->next = current->next;
            current->next = new_customer;
        }
        printf("Customer '");
        fputs(name, stdout);
        printf("' added\n");
    }
}


// This function adds a new order for a customer and updates the restaurant's 
// records.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void add_order(struct restaurant *restaurant) {
    char name[MAX_SIZE];
    // Scanning information
    scan_name(name);
    enum order_flavour flavour = scan_flavour();
    enum order_protein protein = scan_protein();
    enum order_base base = scan_base();
    struct customer *cust = restaurant->customers;
    // Return if errors are found
    if (order_errors(restaurant, name, flavour, protein, base)) {
        return;
    }
    // goes to the customer that is making the order
    while (cust->next != NULL && strcmp(name, cust->name) != 0) {
        cust = cust->next;
    }
    int prior_orders = count_cust_orders(restaurant, cust->name);
    int price = get_item_price(flavour, protein, base, prior_orders);
    int delivery_fee = get_delivery_fee(cust->distance);
    int new_id = 1;
    struct order *current = restaurant->orders;
    while (current != NULL) {
        if (current->id >= new_id) {
            new_id = current->id + 1;
        }
        current = current->next;
    }
    struct order *new_order = create_order(
        name, flavour, protein, base, new_id, PENDING 
    );
    // Add to restaurant order list
    if (restaurant->orders == NULL) {
        restaurant->orders = new_order;
    } else {
        current = restaurant->orders;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_order;
    }
    restaurant->profit += price;
    cust->balance -= (price + delivery_fee);
    if ((prior_orders + 1) % 4 == 0) {
        printf("Order '#%d' added: $%d item + $%d delivery - $5 discount\n",
           new_id, price + 5, delivery_fee);
    } else {
        printf("Order '#%d' added: $%d item + $%d delivery\n",
           new_id, price, delivery_fee);
    }   
}


// This function calculates the total item price based on selected options
// and applies a discount every fourth order.
//
// parameters:
// - flavour: The selected flavour of the order.
// - protein: The selected protein of the order.
// - base: The selected base of the order.
// - count: The number of previous orders by the customer.
//
// returns: The total price of the item
int get_item_price(enum order_flavour flavour, enum order_protein protein, 
                   enum order_base base, int count) {
    int flavour_price = get_flavour_price(flavour); 
    int protein_price = get_protein_price(protein); 
    int base_price = get_base_price(base);
    
    int total = flavour_price + protein_price + base_price;

    // Apply 5 dollar discount on every fourth order
    if ((count + 1) % 4 == 0) {
        total -= 5;
    }
    return total;
}

// This function returns the price of a given flavour.
//
// parameters:
// - flavour: The selected flavour.
//
// returns: The price of the flavour
int get_flavour_price(enum order_flavour flavour) {
    if (flavour == NONE) {
        return 0;
    } else {
        return 1;
    }
}

// This function returns the price of a given protein.
//
// parameters:
// - protein: The selected protein.
//
// returns: The price of the protein
int get_protein_price(enum order_protein protein) {
    if (protein == CHICKEN) {
        return 2;
    } else if (protein == BEEF) {
        return 4;
    } else if (protein == LAMB) {
        return 6;
    } else if (protein == TOFU) {
        return 2;
    } else {
        return 0;
    }
}

// This function returns the price of a given base.
//
// parameters:
// - base: The selected base.
//
// returns: The price of the base
int get_base_price(enum order_base base) {
    if (base == CURRY) {
        return 16;
    } else if (base == NOODLES) {
        return 14;
    } else if (base == SALAD) {
        return 12;
    } else if (base == SOUP) {
        return 10;
    } else {
        return 14;
    }
}

// This function calculates the delivery fee based on the customer's distance.
//
// parameters:
// - distance: The distance from the restaurant to the customer.
//
// returns: The calculated delivery fee
int get_delivery_fee(int distance) {
    if (distance <= 10) {
        return 5;
    } else {
        int total = distance - 5;
        return total;
    }
}

// This function counts the number of orders made by a specific customer.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - name: The name of the customer.
//
// returns: The number of matching orders
int count_cust_orders(struct restaurant *restaurant, char name[MAX_SIZE]) {
    int count = 0;
    struct order *current = restaurant->orders;
    while (current != NULL) {
        if ((strcmp(current->customer_name, name) == 0)) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// This function checks for errors before placing an order, including invalid 
// input, missing customers, and insufficient balance.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - name: The name of the customer placing the order.
// - flavour: The selected flavour.
// - protein: The selected protein.
// - base: The selected base.
//
// returns: TRUE if an error is found, otherwise FALSE
int order_errors(struct restaurant *restaurant, char *name,
                 enum order_flavour flavour, enum order_protein protein, 
                 enum order_base base) {
    // Check whether order components are valid
    if (flavour == INVALID_FLAVOUR) {
        printf("ERROR: Invalid flavour\n");
        return TRUE;
    }
    if (protein == INVALID_PROTEIN) {
        printf("ERROR: Invalid protein\n");
        return TRUE;
    }
    if (base == INVALID_BASE) {
        printf("ERROR: Invalid base\n");
        return TRUE;
    }
    // Find the customer by name
    struct customer *cust = restaurant->customers;
    while (cust != NULL && strcmp(cust->name, name) != 0) {
        cust = cust->next;
    }
    if (cust == NULL) {
        printf("ERROR: Customer '%s' not found\n", name);
        return TRUE;
    }
    // Check if the customer has enough balance
    int prior_orders = count_cust_orders(restaurant, name);
    int price = get_item_price(flavour, protein, base, prior_orders);
    int delivery_fee = get_delivery_fee(cust->distance);
    int remaining_balance = cust->balance - (price + delivery_fee);
    if (remaining_balance < 0) {
        printf("ERROR: Customer '%s' has insufficient balance\n", name);
        return TRUE;
    }
    return FALSE;
}

// This function updates the status of pending orders to ready and deducts
// cooking costs from the restaurant's profit.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void cook_orders(struct restaurant *restaurant) {
    // Cost to cook an order:
    // Fixed $4 labour cost + half the order's base price
    int cook_num, counter = 0;
    scanf(" %d", &cook_num);
    struct order *current = restaurant->orders;
    // skip non-pending orders
    while (current != NULL && current->status != PENDING) {
        current = current->next;
    }
    // cook up to 'cook_num' pending orders
    while (current != NULL && counter < cook_num) {
        current->status = READY;
        restaurant->profit -= (4 + get_base_price(current->base)/2);
        current = current->next;
        counter++;
    }
    printf("Cooked %d order(s)\n", counter);
}

// This function delivers a specified order by updating its status,
// removing it from the restaurant's order list, and adding it to the customer's 
// order list.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void deliver_orders(struct restaurant *restaurant) {
    int order_id;
    scanf(" %d", &order_id);
    // Check if order has already been delivered
    if (already_delivered(restaurant, order_id)) {
        return;
    }
    struct order *curr_order = restaurant->orders;
    struct order *temp = NULL;
    // Find the order by ID
    while (curr_order != NULL && curr_order->id != order_id) {
        temp = curr_order;
        curr_order = curr_order->next;
    }
    if (curr_order == NULL) {
        printf("ERROR: Order '#%d' not found\n", order_id);
        return;
    }
    if (curr_order->status != READY) {
        printf("ERROR: Order '#%d' not READY\n", order_id);
        return;
    }
    // Find the customer who placed the order
    struct customer *curr_customer = restaurant->customers;
    while (curr_customer != NULL && 
          strcmp(curr_customer->name, curr_order->customer_name) != 0) {
        curr_customer = curr_customer->next;
    }
    // Remove order from restaurant->orders list
    if (temp == NULL) {
        restaurant->orders = curr_order->next;
    } else {  
        temp->next = curr_order->next;
    }
    curr_order->status = DELIVERED;
    curr_order->next = NULL;
    if (curr_customer->orders == NULL) {
        curr_customer->orders = curr_order;
    } else {
        struct order *cust_order = curr_customer->orders;
        while (cust_order->next != NULL) {
            cust_order = cust_order->next;
        }
        cust_order->next = curr_order;
    }
    printf("Order '#%d' delivered to '%s'\n", order_id, curr_customer->name);
}

// This function checks if an order has already been delivered.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - id: The ID of the order to check.
//
// returns: TRUE if the order is already delivered, otherwise FALSE
int already_delivered(struct restaurant *restaurant, int id) {
    struct customer *current = restaurant->customers;
    // Traverse all customers
    while (current != NULL) {
        struct order *delivered = current->orders;
        // check each delivered order for a matching ID
        while (delivered != NULL) {
            if (delivered->id == id) {
                printf("ERROR: Order '#%d' already delivered\n", id);
                return TRUE;
            }
            delivered = delivered->next;
        }
        current = current->next;
    }
    return FALSE;
}

// This function frees all dynamically allocated memory associated with
// the restaurant, including orders and customers.
//
// parameters:
// - restaurant: A pointer to the restaurant struct to be freed.
//
// returns: Nothing
void free_memory(struct restaurant *restaurant) {
    struct order *curr_ord = restaurant->orders;
    // Free all orders in the restaurant's main order list
    while (curr_ord != NULL) {
        struct order *delete_ord = curr_ord;
        curr_ord = curr_ord->next;
        free(delete_ord);
    }
    struct customer *curr_cust = restaurant->customers;

    // Free each customer and their delivered orders
    while (curr_cust != NULL) {
        struct customer *delete_cust = curr_cust;

        struct order *cust_order = curr_cust->orders;
        while (cust_order != NULL) {
            struct order *delete_ord = cust_order;
            cust_order = cust_order->next;
            free(delete_ord);
        }
        curr_cust = curr_cust->next;
        free(delete_cust);
    }
    // free the restaurant struct
    free(restaurant);
}


// This function cancels a pending order, updates the restaurant's profit
// and refunds the customer accordingly.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void cancel_order(struct restaurant *restaurant) {
    int id;
    scanf(" %d", &id);
    // Find the order by ID
    struct order *curr_ord = restaurant->orders;
    while (curr_ord != NULL && curr_ord->id != id) { 
        curr_ord = curr_ord->next;
    }
    // error if order not found
    if (curr_ord == NULL) {
        printf("ERROR: Order '#%d' not at restaurant\n", id);
        return;
    }
    // Only pending orders can be cancelled
    if (curr_ord->status != PENDING) {
        printf("ERROR: Order '#%d' not in PENDING state\n", id);
        return;
    }
    // Find the customer who placed the order
    struct customer *curr_customer = restaurant->customers;
    while (curr_customer != NULL && 
          strcmp(curr_customer->name, curr_ord->customer_name) != 0) {
        curr_customer = curr_customer->next;
    }
    // calculate refund amount
    int money_back = get_item_price(curr_ord->flavour, curr_ord->protein, 
                            curr_ord->base, id - 1);
    restaurant->profit -= money_back;
    curr_customer->balance += money_back + 
                            get_delivery_fee(curr_customer->distance);
    curr_ord->status = CANCELLED;
    printf("Order '#%d' removed\n", id);
}

// This function removes a customer from the restaurant if they have no 
// undelivered orders, freeing their memory and updating the customer list.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void remove_customer(struct restaurant *restaurant) {
    char name[MAX_SIZE];
    scan_name(name);
    struct customer *cust = restaurant->customers;
    struct order *order = restaurant->orders;
    struct customer *temp = NULL;
    // find the customer to remove
    while (cust != NULL && strcmp(name, cust->name) != 0) {
        temp = cust;
        cust = cust->next;
    }
    // Error if customer not found
    if (cust == NULL) {
        printf("ERROR: Customer '%s' not found \n", name);
        return;
    }
    // Check for undelivered orders by this customer
    while (order != NULL) {
        if (strcmp(order->customer_name, name) == 0) {
            printf("ERROR: Customer '%s' has undelivered orders\n", name);
            return;
        }
        order = order->next;
    }
    // Remove customer from linked list
    if (temp == NULL) {
        restaurant->customers = cust->next;
    } else {  
        temp->next = cust->next;
    }
    // free customer's delivered orders
    struct customer *delete_cust = cust;
    struct order *cust_order = cust->orders;
    while (cust_order != NULL) {
        struct order *delete_ord = cust_order;
        cust_order = cust_order->next;
        free(delete_ord);
    }
    free(delete_cust);
    printf("Customer '%s' removed\n", name);
}

// This function allows a customer to consume (delete) a number of their 
// delivered orders.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void cust_eat(struct restaurant *restaurant) {
    char name[MAX_SIZE];
    int position, amount;
    // Read customer name and order details
    scan_name(name);
    scanf(" %d %d", &position, &amount);
    if (amount <= 0) {
        printf("ERROR: Amount must be positive\n");
        return;
    }
    // Find the customer
    struct customer *cust = restaurant->customers;
    while (cust != NULL && strcmp(cust->name, name) != 0) {
        cust = cust->next;
    }
    if (cust == NULL) {
        printf("ERROR: Customer '%s' not found\n", name);
        return;
    }

    delete_cust_orders(cust, position, amount);
}

// Deletes a specified number of delivered orders from a customer's order list.
//
// parameters:
// - cust: Pointer to the customer whose orders are to be deleted.
// - position: The starting position (1-indexed) of the order to begin deletion.
// - amount: The number of consecutive orders to delete.
//
// returns: Nothing
void delete_cust_orders(struct customer *cust, int position, int amount) {
    struct order *curr = cust->orders;
    struct order *prev = NULL;
    // go to the given position
    int i = 1;
    while (curr != NULL && i < position) {
        prev = curr;
        curr = curr->next;
        i++;
    }
    // prints error if no such position
    if (curr == NULL || position <= 0) {
        printf("ERROR: Customer '%s' has no order at position '%d'\n",
        cust->name, position);
        return;
    }
    // Check if enough orders remain to delete 'amount'
    struct order *temp = curr;
    for (i = 0; i < amount; i++) {
        if (temp == NULL) {
            printf("ERROR: Customer '%s' wants to eat too many orders\n",
            cust->name);
            return;
        }
        temp = temp->next;
    }
    // remove and free 'amount' orders starting from 'curr'
    for (i = 0; i < amount; i++) {
        struct order *to_delete = curr;
        curr = curr->next;
        free(to_delete);
    }
    if (prev == NULL) {
        // we're deleting from the head of the list
        cust->orders = curr;
    } else {
        prev->next = curr;
    }
    printf("'%s' ate %d order(s)\n", cust->name, amount);
}

// Merges all customers within a certain distance of the target customer.
//
// parameters:
// - restaurant: Pointer to the restaurant structure.
//
// returns: Nothing
void merge_customer(struct restaurant *restaurant) {
    char name[MAX_SIZE];
    int distance;
    scan_name(name);
    scanf(" %d", &distance);
    // ERROR CHECKING
    if (distance <= 0) {
        printf("ERROR: Distance must be positive\n");
        return;
    }
    struct customer *dest = restaurant->customers;
    while (dest != NULL && strcmp(dest->name, name) != 0) {
        dest = dest->next;
    }

    if (dest == NULL) {
        printf("ERROR: Customer '%s' not found\n", name);
        return;
    }
    // MAIN LOGIC
    int dest_distance = dest->distance;
    struct customer *prev = NULL;
    struct customer *curr = restaurant->customers;
    while (curr != NULL) {
        struct customer *next = curr->next;
        // checking if the current customer is close enough to be merged
        if (curr != dest) {
            int diff;
            if (curr->distance > dest_distance) {
                diff = curr->distance - dest_distance;
            } else {
                diff = dest_distance - curr->distance;
            }
            if (diff <= distance) {
                merge_helper(restaurant, dest, curr, prev);
                curr = prev;
            }
        }
        prev = curr;
        curr = next;
    }
}

// This function adds the curr's orders to the destination customer, updates
// customer names in the order list and then frees it
//
// parameters:
// - restaurant: Pointer to the restaurant structure.
// - dest: Destination customer who will receive orders and balance.
// - curr: Current customer to be merged and removed.
// - prev: Customer before curr in the linked list (NULL if curr is head).
//
// returns: Nothing
void merge_helper(struct restaurant *restaurant, struct customer *dest,
                  struct customer *curr, struct customer *prev) {
    struct order *tail = dest->orders;
    if (tail == NULL) {
        dest->orders = curr->orders;
    } else {
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = curr->orders;
    }
    struct order *update = curr->orders;
    while (update != NULL) {
        strcpy(update->customer_name, dest->name);
        update = update->next;
    }
    struct order *curr_order = restaurant->orders;
    while (curr_order != NULL) {
        if (strcmp(curr_order->customer_name, curr->name) == 0) {
            strcpy(curr_order->customer_name, dest->name);
        }
        curr_order = curr_order->next;
    }
    dest->balance += curr->balance;
    if (prev == NULL) {
        restaurant->customers = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
}

// Insert a customer into a list sorted in ascending order by distance,
// then by name. Used for customers with even distances.
//
// parameters:
// - head: head of the list to insert into
// - node: customer to insert
//
// returns: New head of the list
struct customer *insert_sorted_even(struct customer *head, 
                                    struct customer *node) {
    if (head == NULL || 
        node->distance < head->distance || 
        (node->distance == head->distance && 
            strcmp(node->name, head->name) < 0)) {
        node->next = head;
        return node;
    }
    struct customer *curr = head;
    while (curr->next != NULL &&
          (curr->next->distance < node->distance || 
          (curr->next->distance == node->distance && 
            strcmp(curr->next->name, node->name) < 0))) {
        curr = curr->next;
    }
    node->next = curr->next;
    curr->next = node;
    return head;
}

// Insert a customer into a list sorted in descending order by distance,
// then by name. Used for customers with odd distances.
//
// parameters:
// - head: head of the list to insert into
// - node: customer to insert
//
// returns: New head of the list
struct customer *insert_sorted_odd(struct customer *head, 
                                   struct customer *node) {
    if (head == NULL || 
        node->distance > head->distance || 
        (node->distance == head->distance && 
            strcmp(node->name, head->name) < 0)) {
        node->next = head;
        return node;
    }
    struct customer *curr = head;
    while (curr->next != NULL &&
          (curr->next->distance > node->distance || 
          (curr->next->distance == node->distance && 
            strcmp(curr->next->name, node->name) < 0))) {
        curr = curr->next;
    }
    node->next = curr->next;
    curr->next = node;
    return head;
}

// Sort the restaurant's customer list by splitting into even and odd distances.
// Even distance customers are sorted ascending by distance, then name.
// Odd distance customers are sorted descending by distance, then name.
//
// parameters:
// - restaurant: pointer to the restaurant whose list is sorted
//
// returns: Nothing
void sort(struct restaurant *restaurant) {
    struct customer *even_head = NULL;
    struct customer *odd_head = NULL;
    struct customer *curr = restaurant->customers;
    while (curr != NULL) {
        struct customer *next = curr->next;
        curr->next = NULL;
        if (curr->distance % 2 == 0) {
            even_head = insert_sorted_even(even_head, curr);
        } else {
            odd_head = insert_sorted_odd(odd_head, curr);
        }
        curr = next;
    }
    if (even_head == NULL) {
        restaurant->customers = odd_head;
    } else {
        restaurant->customers = even_head;
        struct customer *tail = even_head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = odd_head;
    }
}

// Add a driver to the restaurant system
//
// parameters:
// - restaurant: pointer to the restaurant structure
// - driver: driver structure to add
//
// returns: Updated driver structure
struct driver add_driver(struct restaurant *restaurant, struct driver driver) {
    char action[MAX_SIZE] = "1 - Waiting";
    char delete_name[MAX_SIZE];
    if (driver.enabled == 0) {
        scan_name(driver.name);
        driver.enabled = 1;
    } else {
        scan_name(delete_name);
        printf("ERROR: Driver already exists\n");
        return driver;
    }
    strcpy(driver.action, action);
    printf("Driver '%s' added\n", driver.name);
    return driver;
}

////////////////////////////////////////////////////////////////////////////////
// Provided Function Definitions
////////////////////////////////////////////////////////////////////////////////

// Function to print a welcome banner
// Params: None
// Returns: None
// Usage:
// ```
//      print_welcome_banner();
// ```
void print_welcome_banner(void) {
    printf(
        "Welcome to CS Eats!\n"
        "    _____           )   ________        ___       \n"
        "   (     )      )  (___/___     \\  __  |\" \"|  __  \n"
        "    |   |     _(__/_)_     \\S   | |''| |'\" | |\"\"| \n"
        "    |___|    /        \\S   |TS  | | \"| |''\"| |'\"| \n"
        "    :^_^:    |   CS   |TS  |____| |\"'| |\" \"| |\" | \n"
        "   _`~^~'_   |  EATS  |____| __   ---------------\n"
        " /    ^    \\ |________|   .-'--`--. - - - - - - -\n"
        " ~[$|$|$|$]~              '-O---O-' -------------\n\n"
    );
}

// Function to print help information
// '?' command
// Params: None
// Returns: None
// Usage:
// ```
//      print_help();
// ```
void print_help(void) {
    printf(
        "*********************[     Usage Info     ]*********************\n"
        "  a c [name] [balance] [distance]                               \n"
        "    Add a customer to the head of the customer list             \n"
        "  a o [customer] [flavour] [protein] [base]                     \n"
        "    Append an order for a customer                              \n"
        "  a d [driver_name]                                             \n"
        "    Add a driver                                                \n"
        "  i c [name] [balance] [distance] [position]                    \n"
        "    Insert a customer at [position]                             \n"
        "  p                                                             \n"
        "    Print the restaurant, including customers and orders        \n"
        "  c [n]                                                         \n"
        "    Cook up to [n] PENDING orders                               \n"
        "  d [order_id]                                                  \n"
        "    Deliver a READY order                                       \n"
        "  x o [order_id]                                                \n"
        "    Cancel a PENDING order and refund the customer              \n"
        "  x c [name]                                                    \n"
        "    Remove customer [name]                                      \n"
        "  e [name] [position] [amount]                                  \n"
        "    Customer eats [amount] delivered orders from [position]     \n"
        "  m [name] [distance]                                           \n"
        "    Merge customers within [distance] from [name] into [name]   \n"
        "  s                                                             \n"
        "    Sort customers by distance                                  \n"
        "  r [name] [n] [id_1] [id_2] ... [id_n]                         \n"
        "    Reorder past orders                                         \n"
        "  t                                                             \n"
        "    Simulate one driver turn                                    \n"
        "  f [n]                                                         \n"
        "    Fast forward simulation by 'n' driver turns                 \n"
        "  q                                                             \n"
        "    Quit the program, freeing all memory                        \n"
        "  ?                                                             \n"
        "    Show this help information                                  \n"
        "****************************************************************\n"
        "\n"
    );
}

// Scans a name string into the provided buffer, placing '\0' at the end.
//
// Params:
//      name - a char array of length MAX_SIZE, where the name should be stored
// Returns: None
// Usage:
// ```
//      char name[MAX_SIZE];
//      scan_name(name);
// ```
void scan_name(char name[MAX_SIZE]) {
    scan_token(name, MAX_SIZE);
}

// Function to print a header
// Params:
//      name - the name of the restaurant
// Returns: None
// Usage:
// ```
//      print_header(name);
// ```
void print_header(char name[MAX_SIZE]) {
    printf("******************[ %s ]******************\n", name);
}

// Function to print a customer's details (name, balance, distance)
// Params:
//      customer - a pointer to the customer to print
// Returns: None
// Usage:
// ```
//      print_customer(customer);
// ```
void print_customer(struct customer *customer) {
    printf(
        "  %s ($%d, %d km)\n", 
        customer->name, 
        customer->balance, 
        customer->distance
    );
}

// Scans a string and converts it to an order_flavour.
//
// Params: None
// Returns:
//      The corresponding order_flavour enum, if the string was valid,
//      Otherwise, returns INVALID_FLAVOUR.
//
// Usage:
// ```
//      enum order_flavour flavour = scan_flavour();
// ```
enum order_flavour scan_flavour(void) {
    char type[MAX_SIZE];
    scan_token(type, MAX_SIZE);
    return string_to_order_flavour(type);
}

// Scans a string and converts it to an order_protein.
//
// Params: None
// Returns:
//      The corresponding order_protein enum, if the string was valid,
//      Otherwise, returns INVALID_PROTEIN.
//
// Usage:
// ```
//      enum order_protein protein = scan_protein();
// ```
enum order_protein scan_protein(void) {
    char type[MAX_SIZE];
    scan_token(type, MAX_SIZE);
    return string_to_order_protein(type);
}

// Scans a string and converts it to an order_base.
//
// Params: None
// Returns:
//      The corresponding order_base enum, if the string was valid,
//      Otherwise, returns INVALID_BASE.
//
// Usage:
// ```
//      enum order_base base = scan_base();
// ```
enum order_base scan_base(void) {
    char type[MAX_SIZE];
    scan_token(type, MAX_SIZE);
    return string_to_order_base(type);
}

// Function to print a single order
// Params:
//      order - a pointer to the order to print
// Returns: None
// Usage:
// ```
//      print_order(order);
// ```
void print_order(struct order *order) {
    printf("    %s :: ", order_status_to_string(order->status));
    printf("#%d ", order->id);
    if (order->flavour != NONE) {
        printf("%s ", order_flavour_to_string(order->flavour));
    }
    printf(
        "%s %s",
        order_protein_to_string(order->protein),
        order_base_to_string(order->base)
    );
    printf(" :: %s", order->customer_name);
    printf("\n");
}

// Function to print a driver's details (name, action)
// Params:
//      name   - the driver's name
//      action - the driver's most recent action
// Returns: None
// Usage:
// ```
//      print_driver_details(name, action);
// ```
void print_driver_details(char name[MAX_SIZE], char *action) {
    printf(
        "       __\n"
        "    .-'--`--. Driver: %s\n"
        "    '-O---O-' Action: %s\n",
        name,
        action
    );
}

////////////////////////////////////////////////////////////////////////////////
// Additional Provided Functions
////////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    scanf(" ");

    while (i < buffer_size - 1 && (num_scanned = scanf("%c", &c)) == 1 &&
           !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}

enum order_flavour string_to_order_flavour(char *flavour_str) {
    int len = strlen(flavour_str);

    if (strncasecmp(flavour_str, "CHILLI", len) == 0) {
        return CHILLI;
    }
    if (strncasecmp(flavour_str, "GARLIC", len) == 0) {
        return GARLIC;
    }
    if (strncasecmp(flavour_str, "LEMON", len) == 0) {
        return LEMON;
    }
    if (strncasecmp(flavour_str, "SESAME", len) == 0) {
        return SESAME;
    }
    if (strncasecmp(flavour_str, "NONE", len) == 0) {
        return NONE;
    }

    return INVALID_FLAVOUR;
}

enum order_protein string_to_order_protein(char *protein_str) {
    int len = strlen(protein_str);

    if (strncasecmp(protein_str, "CHICKEN", len) == 0) {
        return CHICKEN;
    }
    if (strncasecmp(protein_str, "BEEF", len) == 0) {
        return BEEF;
    }
    if (strncasecmp(protein_str, "LAMB", len) == 0) {
        return LAMB;
    }
    if (strncasecmp(protein_str, "TOFU", len) == 0) {
        return TOFU;
    }
    if (strncasecmp(protein_str, "VEGGIE", len) == 0) {
        return VEGGIE;
    }

    return INVALID_PROTEIN;
}

enum order_base string_to_order_base(char *base_str) {
    int len = strlen(base_str);

    if (strncasecmp(base_str, "CURRY", len) == 0) {
        return CURRY;
    }
    if (strncasecmp(base_str, "NOODLES", len) == 0) {
        return NOODLES;
    }
    if (strncasecmp(base_str, "SALAD", len) == 0) {
        return SALAD;
    }
    if (strncasecmp(base_str, "SOUP", len) == 0) {
        return SOUP;
    }
    if (strncasecmp(base_str, "WRAP", len) == 0) {
        return WRAP;
    }

    return INVALID_BASE;
}

char *order_flavour_to_string(enum order_flavour flavour) {
    if (flavour == CHILLI) {
        return "Chilli";
    }
    if (flavour == GARLIC) {
        return "Garlic";
    }
    if (flavour == LEMON) {
        return "Lemon";
    }
    if (flavour == SESAME) {
        return "Sesame";
    }
    if (flavour == NONE) {
        return "";
    }

    return "FLAVOUR_INVALID";
}

char *order_protein_to_string(enum order_protein protein) {
    if (protein == CHICKEN) {
        return "Chicken";
    }
    if (protein == BEEF) {
        return "Beef";
    }
    if (protein == LAMB) {
        return "Lamb";
    }
    if (protein == TOFU) {
        return "Tofu";
    }
    if (protein == VEGGIE) {
        return "Veggie";
    }

    return "PROTEIN_INVALID";
}

char *order_base_to_string(enum order_base base) {
    if (base == CURRY) {
        return "Curry";
    }
    if (base == NOODLES) {
        return "Noodles";
    }
    if (base == SALAD) {
        return "Salad";
    }
    if (base == SOUP) {
        return "Soup";
    }
    if (base == WRAP) {
        return "Wrap";
    }

    return "BASE_INVALID";
}

char *order_status_to_string(enum order_status status) {
    if (status == PENDING) {
        return "PENDING";
    }
    if (status == READY) {
        return "READY";
    }
    if (status == EN_ROUTE) {
        return "EN_ROUTE";
    }
    if (status == DELIVERED) {
        return "DELIVERED";
    }

    return "STATUS_INVALID";
}
