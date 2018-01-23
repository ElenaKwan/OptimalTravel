/**
 * helper.h
 *
 * Computer Science 50
 * Final Project
 * Elena Kwan
 *
 * Declares a helper file's functionality.
 */
 
#define UNSIGNED_INT_MAX 4294967295;
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/**
 * NODE
 *
 * Each node represents a particular path (complete or incomplete) in the 
 * travelling salesman problem. If the node is a leaf, then the path is complete.
 *
 * While conceptually, all belong in a solution tree, they will be  stored in a 
 * sorted linked list (the node with the lowest lower bound) is stored first in the list).
 * 
 * This node structure contains information about the path that has currently 
 * the path size, the node's children. It also contains information about the 
 * current lowerbound and cost matrix.
 *
 */
 
typedef struct node
{
    // pointers to store the node in a sorted doubly linked list
    struct node* prev;
    struct node* next;
    
    // node's path information
    int* path;
    int path_size;
    int* possible_children;
    
    // node's cost information
    int lower_bound;
    int total_reduction;
    int** cost_matrix;
    int** reduced_cost_matrix;
    
}node;
 
 
/**
 * Creates a NxN 2D array and store the 2D array at the pointer provided.
 */
void create_array(int num_cities, int*** input_array);

 /**
 * Stores file inputs into a 2D array. Stores the 2D array at the pointer provided
 */
 
void input_to_array(FILE* rawfile, int*** input_array, int num_cities);


/**
 * Takes a pointer to a cost matrix, and finds the Total Reduction and stores the reduced
 * matrix in another pointer provided. 
 */
void matrix_reduc(node* node, int num_cities);

/**
 * Takes a pointer to a node and evaluates it's children (i.e. creates the child nodes,
 * stores nodes in a sorted doubly linked list based on the child's lower bound value)
 * reduction and the (2) reduced matrix in the appropriate fields of the node. 
 */
void evaluate_children(node** head_ptr, int num_cities);


/**
 * Takes a pointer to a node and evaluates it's children (i.e. creates the child nodes,
 * stores nodes in a sorted doubly linked list based on the child's lower bound value)
 * reduction and the (2) reduced matrix in the appropriate fields of the node. 
 */
void insert_node(node* child, node** head);

/**
 * Prints the linked list
 */
void print_linked_list(node* head);


 /**
 * Takes a pointer to a node and removes it from the linked list
 */
void remove_node_from_list(node* ptr, node** head_ptr);

/**
 * Takes a pointer to a doubly linked list and frees the nodes. 
 */
void free_list(node** head_ptr, int num_cities);

/**
 * Takes a pointer to a node and frees pointers within the node and the node itself 
 */
void free_node(node* ptr, int num_cities);