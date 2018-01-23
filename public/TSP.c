/**
 * TSP.c
 *
 * Computer Science 50
 * Final Project
 * Elena Kwan
 *
 * Given (1) a number of cities and (2) the cost of travelling between each city,
 * this program solves for the travelling salesman problem using a branch-and-bound
 * approach. It prints out the (1) lowest total cost of travelling to all cities 
 * exactly once and returning to the original destination and (2) the route through
 * all the cities that will provide that lowest cost.
 */

#define INT_MAX 2147483647;
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "helper.h"


int main(int argc, char* argv[])
{
    // STEP 0: ensure correct arguments provided
    if(argc != 3)
    {
        printf("Usage: ./TSP number_of_cities filename.txt\n");
    }
        
    int num_cities = atoi(argv[1]);
    
    // STEP 1: open input file and ensure it is not null
    FILE* rawfile = fopen(argv[2], "r");
    if (rawfile == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }
    
    // STEP 2:  store file inputs in a 2D-array
    int** input_array = NULL;

    input_to_array(rawfile, &input_array, num_cities);
    if(input_array == NULL)
    {
        printf("Could not initialize the input array\n");
    }
    
    fclose(rawfile);

    // STEP 3: Create start of a sorted doubly-linked list to hold on the nodes
    // (sorted in ascending order by lowerbound), and initialize the first node
    
    // STEP 3.a. create the start of a linked list
    node* head = malloc(sizeof(node));
    if (head == NULL)
    {
        printf("Error: unable to allocate memory for the linked list");
        return 1;
    }
    
    
    // STEP 3.b: initialize the first node
    
    // STEP 3.b.1: initialize pointers to store the node in a sorted doubly linked list
    head->prev = NULL;
    head->next = NULL;
    
    // STEP 3.b.2: initialize the node's path information
    head->path = malloc(sizeof(int*) * (num_cities + 1));
    if (head->path == NULL)
    {
        printf("Error: unable to allocate memory for the path");
        return 1;
    }
    head->path[0] = 0;
    head->path_size = 1;
    head->possible_children = malloc(sizeof(int*) * (num_cities-1));
    if (head->possible_children == NULL)
    {
        printf("Error: unable to allocate memory for children");
        return 1;
    }
    for(int i = 0; i < num_cities -1 ; i++)
    {
        head->possible_children[i] = i+1;

    }

    // STEP 3.b.3: Initialize the node's cost information
    head->cost_matrix = input_array;

    // STEP 3.b.3 (continued): calculate the reduced matrix and total reduction and store it in the 
    // appropriate node fields
    matrix_reduc(head, num_cities);
    
    // STEP 3.b.3 (continued): for the first node, the lower bound is equal to the total reduction
    head->lower_bound = head->total_reduction;
    
    
    // STEP 4: Evaulate the children 
    while(head->path_size < num_cities + 1)
    {   
         evaluate_children(&head, num_cities);
    }
    
    
    // STEP 5: print optimal path
    printf("the lower bound is : %i\n", head->lower_bound);
    printf("optimal path is : ");
    for(int i = 0; i < head->path_size; i++)
    {
        printf("%i ", head->path[i] + 1);
    }
    printf("\n");
    
    // STEP 6: Free the linked list
    free_list(&head, num_cities);

}

