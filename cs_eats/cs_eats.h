// CS Eats
// cs_eats.h
// Written by Ethan Tong, z5691989
// on 01/08/25

////////////////////////////////////////////////////////////////////////////////
// Provided Constants
////////////////////////////////////////////////////////////////////////////////

#define MAX_SIZE 100

////////////////////////////////////////////////////////////////////////////////
// Your Constants
////////////////////////////////////////////////////////////////////////////////

#define TRUE 1
#define FALSE 0

////////////////////////////////////////////////////////////////////////////////
// Provided Enums
////////////////////////////////////////////////////////////////////////////////

enum order_status { PENDING, READY, EN_ROUTE, DELIVERED, CANCELLED };

enum order_flavour {
    CHILLI,
    GARLIC,
    LEMON,
    SESAME,
    NONE,
    INVALID_FLAVOUR
};

enum order_protein {
    CHICKEN,
    BEEF,
    LAMB,
    TOFU,
    VEGGIE,
    INVALID_PROTEIN
};

enum order_base {
    CURRY,
    NOODLES,
    SALAD,
    SOUP,
    WRAP,
    INVALID_BASE
};

////////////////////////////////////////////////////////////////////////////////
// Provided Structs
////////////////////////////////////////////////////////////////////////////////

struct restaurant {
    // The name of the restaurant
    char name[MAX_SIZE];
    // The profit of the restaurant
    int profit;
    // The list of the restaurant's customers
    struct customer *customers;
    // The list of orders placed by customers
    struct order *orders;
};

struct customer {
    // The name of the customer
    char name[MAX_SIZE];
    // The amount of money the customer has remaining
    int balance;
    // The distance (km) of the customer from the restaurant
    int distance;
    // The list of orders delivered to the customer
    struct order *orders;
    // The next customer in the list
    struct customer *next;
};

struct order {
    // The name of the customer who placed the order
    char customer_name[MAX_SIZE];
    // The flavour added to the order
    enum order_flavour flavour;
    // The protein used in the order
    enum order_protein protein;
    // The base menu item of the order
    enum order_base base;
    // The unique identification number of the order
    int id;
    // The status of the order
    enum order_status status;
    // The next order in the list
    struct order *next;
};

struct driver {
    char name[MAX_SIZE];
    char action[MAX_SIZE];
    int enabled;
};

////////////////////////////////////////////////////////////////////////////////
// Provided Function Prototypes
////////////////////////////////////////////////////////////////////////////////

void print_welcome_banner(void);
void print_help(void);
void scan_name(char name[MAX_SIZE]);
void print_header(char name[MAX_SIZE]);
void print_customer(struct customer *customer);
enum order_flavour scan_flavour(void);
enum order_protein scan_protein(void);
enum order_base scan_base(void);
void print_order(struct order *order);
void print_driver_details(char name[MAX_SIZE], char *action);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
enum order_base string_to_order_base(char *base_str);
enum order_protein string_to_order_protein(char *protein_str);
enum order_flavour string_to_order_flavour(char *flavour_str);
char *order_base_to_string(enum order_base base_item);
char *order_protein_to_string(enum order_protein protein_item);
char *order_flavour_to_string(enum order_flavour flavour_item);
char *order_status_to_string(enum order_status status);

////////////////////////////////////////////////////////////////////////////////
// Your Function Prototypes
////////////////////////////////////////////////////////////////////////////////

// Stage 1
struct restaurant *create_restaurant(char name[MAX_SIZE]);
struct customer *create_customer(
    char name[MAX_SIZE],
    int balance,
    int distance
);
struct order *create_order(
    char customer_name[MAX_SIZE],
    enum order_flavour flavour,
    enum order_protein protein,
    enum order_base base,
    int id,
    enum order_status status
);
void command_loop(struct restaurant *restaurant);
void add_customer(struct restaurant *restaurant);
void print_restaurant(struct restaurant *restaurant, struct driver driver);

// Add your function prototypes below


// This function returns FALSE if error is found, returns TRUE if no errors.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - name: The name of the customer.
// - balance: The amount of money the customer has.
// - distance: The distance the customer is from the restaurant.
//
// returns: Either TRUE or FALSE
int customer_errors(
    struct restaurant *restaurant, 
    char name[MAX_SIZE], 
    int balance,
    int distance
);

// This function scans input and inserts a new customer into the restaurant's 
// customer list at a specified position. It first checks for input errors.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void insert_customer(struct restaurant *restaurant);

// This function adds a new order for a customer and updates the restaurant's 
// records.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void add_order(struct restaurant *restaurant);

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
                   enum order_base base, int count);

// This function calculates the delivery fee based on the customer's distance.
//
// parameters:
// - distance: The distance from the restaurant to the customer.
//
// returns: The calculated delivery fee
int get_delivery_fee(int distance);

// This function returns the price of a given flavour.
//
// parameters:
// - flavour: The selected flavour.
//
// returns: The price of the flavour
int get_flavour_price(enum order_flavour flavour);

// This function returns the price of a given protein.
//
// parameters:
// - protein: The selected protein.
//
// returns: The price of the protein
int get_protein_price(enum order_protein protein);

// This function returns the price of a given base.
//
// parameters:
// - base: The selected base.
//
// returns: The price of the base
int get_base_price(enum order_base base);

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
                 enum order_base base);

// This function counts the number of orders made by a specific customer.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - name: The name of the customer.
//
// returns: The number of matching orders
int count_cust_orders(struct restaurant *restaurant, char name[MAX_SIZE]);

// This function updates the status of pending orders to ready and deducts
// cooking costs from the restaurant's profit.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void cook_orders(struct restaurant *restaurant);

// This function delivers a specified order by updating its status,
// removing it from the restaurant's order list, and adding it to the customer's 
// order list.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void deliver_orders(struct restaurant *restaurant);

// This function checks if an order has already been delivered.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
// - id: The ID of the order to check.
//
// returns: TRUE if the order is already delivered, otherwise FALSE
int already_delivered(struct restaurant *restaurant, int id);

// This function frees all dynamically allocated memory associated with
// the restaurant, including orders and customers.
//
// parameters:
// - restaurant: A pointer to the restaurant struct to be freed.
//
// returns: Nothing
void free_memory(struct restaurant *restaurant);

// This function cancels a pending order, updates the restaurant's profit
// and refunds the customer accordingly.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void cancel_order(struct restaurant *restaurant);

// This function removes a customer from the restaurant if they have no 
// undelivered orders, freeing their memory and updating the customer list.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void remove_customer(struct restaurant *restaurant);

// This function allows a customer to consume (delete) a number of their 
// delivered orders.
//
// parameters:
// - restaurant: A struct that contains details about the customer and orders.
//
// returns: Nothing
void cust_eat(struct restaurant *restaurant);

// Deletes a specified number of delivered orders from a customer's order list.
//
// parameters:
// - cust: Pointer to the customer whose orders are to be deleted.
// - position: The starting position (1-indexed) of the order to begin deletion.
// - amount: The number of consecutive orders to delete.
//
// returns: Nothing
void delete_cust_orders(struct customer *cust, int position, int amount);

// Merges all customers within a certain distance of the target customer.
//
// parameters:
// - restaurant: Pointer to the restaurant structure.
//
// returns: Nothing
void merge_customer(struct restaurant *restaurant);

// The function appends curr's orders to dest, updates all relevant customer
// names in the order list, and frees the curr customer.
//
// parameters:
// - restaurant: Pointer to the restaurant structure.
// - dest: Destination customer who will receive orders and balance.
// - curr: Current customer to be merged and removed.
// - prev: Customer before curr in the linked list (NULL if curr is head).
//
// returns: Nothing
void merge_helper(struct restaurant *restaurant, struct customer *dest,
                  struct customer *curr, struct customer *prev);

// Sort the restaurant's customer list by splitting into even and odd distances.
// Even-distance customers are sorted ascending by distance, then name.
// Odd-distance customers are sorted descending by distance, then name.
//
// parameters:
// - restaurant: pointer to the restaurant whose list is sorted
//
// returns: Nothing
void sort(struct restaurant *restaurant);

// Insert a customer into a list sorted in ascending order by distance,
// then by name. Used for customers with even distances.
//
// parameters:
// - head: head of the list to insert into
// - node: customer to insert
//
// returns: New head of the list
struct customer *insert_sorted_even(struct customer *head, 
                                    struct customer *node);

// Insert a customer into a list sorted in descending order by distance,
// then by name. Used for customers with odd distances.
//
// parameters:
// - head: head of the list to insert into
// - node: customer to insert
//
// returns: New head of the list
struct customer *insert_sorted_odd(struct customer *head, 
                                   struct customer *node);

// Add a driver to the restaurant system
//
// parameters:
// - restaurant: pointer to the restaurant structure
// - driver: driver structure to add
//
// returns: Updated driver structure
struct driver add_driver(struct restaurant *restaurant, struct driver driver);

