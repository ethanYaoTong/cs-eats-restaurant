// CS Eats
// main.c
// Written by Ethan Tong, z5691989
// on 01/08/2025
//
// This program is designed to be an ordering app similar to Uber Eats, and
// allows the customer to input various commands such as ordering food.

#include <stdio.h>

#include "cs_eats.h"

int main(void) {
    // Welcome banner
    print_welcome_banner();

    // Restaurant setup
    printf("Enter a restaurant name: ");
    char name[MAX_SIZE];
    scan_name(name);
    struct restaurant *restaurant = create_restaurant(name); 

    printf("\n");

    // Command loop
    command_loop(restaurant);

    // End message
    printf("Goodbye!\n");

    return 0;
}
