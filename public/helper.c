/**
 * helper.h
 *
 * Computer Science 50
 * Final Project
 * Elena Kwan
 *
 * Implement's a helper file's functionality.
 */

#define INT_MAX 2147483647
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "helper.h"

/**
 * Creates a NxN 2D array and store the 2D array at the pointer provided.
 */
void create_array(int num_cities, int*** input_array)
{
    // create a temporary array
    int** temp_arr;
    
    // create the row array
    if (( temp_arr = malloc(num_cities * sizeof( int* ))) == NULL )
    { 
        printf("Could not create array for inputs\n");
    }
    
    // create columns for each row
    for (int i = 0; i < num_cities; i++ )
    {
        // malloc space for a given row
        if (( temp_arr[i] = malloc( num_cities * sizeof(int) )) == NULL )
        { 
            printf("Could not create array for inputs\n");
        }
    
    }
    
    *input_array = temp_arr;
    
}

/**
 * Stores file inputs into a 2D array. Stores the 2D array at the pointer provided
 */
void input_to_array(FILE* rawfile, int*** input_array, int num_cities)
{

    // create the array
    create_array(num_cities, input_array);
    int** temp_arr = *input_array;

    
    // initiate the array
    char line[100];


    while (fgets(line, 100, rawfile) != NULL) 
    {
        // find the ith, jith index and the element's value from the line
        int i_index= -1;
        int j_index= -1;
        unsigned int value = 0;
        sscanf(line, "%d %d %d", &i_index, &j_index, &value);
        
        // store the value in the array
        temp_arr[i_index-1][j_index-1] = value;

    }
    
}


/**
 * Takes a pointer to a node and finds the Total Reduction and stores the (1) total 
 * reduction and the (2) reduced matrix in the appropriate fields of the node. 
 */
void matrix_reduc(node* node, int num_cities)
{

    int** temp_input_arr = node->cost_matrix;
    int** temp_reduct_arr = NULL;
    

    // Step 1: copy the inputed array and store in new_reduct_array
   
    // Step 1a: Malloc space for new_reduct_array
    create_array(num_cities, &temp_reduct_arr);
    
    if(temp_reduct_arr == NULL)
    {
        printf("array was not successfully created\n");
    }
    
    
    // Step 1b: copy the inputed array into new_reduct_array

    for(int i = 0; i < num_cities; i++)
    {
        for(int j=0; j< num_cities; j++)
        {
            temp_reduct_arr[i][j] = temp_input_arr[i][j];
        }
    }

    
    int total_row_min = 0;
    int total_col_min = 0;
    
    // Step 2: Perform row reduction. Iterate through all the rows.
    for(int i = 0; i < num_cities; i++)
    {
        // create a variable to store the row's min. Initialize with first row element.
        int row_min = temp_reduct_arr[i][0];
        
        // iterate through all elements within the row to find the min
        for(int j = 0; j < num_cities; j++)
        {
            // if determine if a min is found. If so, store it.
            if (temp_reduct_arr[i][j] < row_min)
            {
                row_min = temp_reduct_arr[i][j];
            }
        }
        
        // if the entire row has been negated, then the row's min is zero
        if (row_min == INT_MAX)
        {
            row_min = 0;
        }
        
        // Once the row's minimum is found, add it to the row reduction total
        total_row_min += row_min;
           
        // Once row_min is found, reduce the each element in the row by the min (only if the element is not Int_max)
        for(int j = 0; j < num_cities; j++)
        {
            if (temp_reduct_arr[i][j] != INT_MAX)
            {
                temp_reduct_arr[i][j] -=  row_min;
            }
        }
        
    }
    
    // Step 3: Perform Column reduction. Iterate through all the columns.
    for(int j = 0; j < num_cities; j++)
    {
        // create a variable to store the row's min. Initialize with first row element.
        int col_min = temp_reduct_arr[0][j];
        
        // iterate through all elements within the column to find the min
        for(int i = 0; i < num_cities; i++)
        {
            // if determine if a min is found. If so, store it.
            if (temp_reduct_arr[i][j] < col_min)
            {
                col_min = temp_reduct_arr[i][j];
            }
        }
        
        // if the entire row has been negated, then the row's min is zero
        if (col_min == INT_MAX)
        {
            col_min = 0;
        }
        
        // Once the column's minimum is found, increase column reduction total
        total_col_min += col_min;
        
        // Once col_min is found, reduce the each element in the column by the min (only if the element is not equal to int max) //COME BACK HERE!
        for(int i = 0; i < num_cities; i++)
        {
            if (temp_reduct_arr[i][j] != INT_MAX)
            {
                temp_reduct_arr[i][j] -=  col_min;
            }
        }
        
    }
    
    // Step 4: Save the reduced matrix in the node
    node->reduced_cost_matrix = temp_reduct_arr;
    
    // Step 5: Calculate total reduction and save it into the node;
    node->total_reduction = total_row_min + total_col_min;
    
}



/**
 * Takes a pointer to a node and evaluates it's children (i.e. creates the child nodes,
 * stores nodes in a sorted doubly linked list based on the child's lower bound value)
 * reduction and the (2) reduced matrix in the appropriate fields of the node. 
 */
void evaluate_children(node** head_ptr, int num_cities)
{
    int next_city;
    int last_city;
    node* head = *head_ptr;

    
    // STEP 0: Save the reference to the parent node and delete it from the linked list (but does not free it)
    node* parent_node = head;
    remove_node_from_list(parent_node, &head);
    

    // STEP 1: Determine if parent_node is gone through all the cities and about to become a leaf node
    if(parent_node->path_size == num_cities)
    {
        next_city = parent_node->path[0];
        last_city = parent_node->path[(parent_node->path_size)-1];
        
        // if parent_node has gone through all the cities, then the next city in the path is back to the first cit
        // No need to create child nodes. Existing parent_node node just needs to be updated.
        if(parent_node->path == NULL)
        {
            printf("Error: could not allocate memory for the path");
            return;
        }
        parent_node->path[num_cities] = next_city;
        parent_node->path_size++;

        parent_node->lower_bound = parent_node->lower_bound + parent_node->reduced_cost_matrix[last_city][next_city];
        parent_node->total_reduction = 0;
        parent_node->cost_matrix[last_city][next_city] = INT_MAX;
        parent_node->reduced_cost_matrix[last_city][next_city] = INT_MAX;
        
        // reinsert the node back into the linked list
        insert_node(parent_node, &head);
    }
    else
    {
        // STEP 2: For each child, Create child nodes, initiate, the nodes, and store it in a linked list
        for(int i = 0; i < (num_cities)-(parent_node->path_size); i++)
        {
            next_city = parent_node->possible_children[i];
            last_city = parent_node->path[parent_node->path_size-1];
            int is_child_leaf = ((parent_node->path_size + 1) == num_cities)? 1: 0;
    
            
            // Step 2a: create the child node;
            node* child = malloc(sizeof(node));
            if(child == NULL)
            {
                printf("Error: child node could not be created\n");
            }
            
            // Step 2b: initialize the child's path
            child->path_size = parent_node->path_size + 1;
            child->path = malloc(sizeof(int*) * (num_cities + 1));
            //child->path = malloc(sizeof(int*) * child->path_size);
            if(child->path == NULL)
            {
                printf("Error: child path could not be created\n");
            }
            
            for(int j = 0; j < parent_node->path_size; j++)
            {
                child->path[j] = parent_node->path[j];
            }
            child->path[parent_node->path_size] = next_city;
            
            // Step 2c: initialize the child's possible_children array
            child->possible_children = malloc(sizeof(int*) * (num_cities-(child->path_size)));
            if(child->possible_children == NULL)
            {
                printf("Error: child possible_children array could no be created\n");
            }
            
            int n = 0;
            for(int k = 0; k < (num_cities) - (parent_node->path_size); k++)
            {
    
                if( parent_node->possible_children[k] != next_city)
                {
                    child->possible_children[n] = parent_node->possible_children[k];
                    n++;
                }
            }
            
            // Step 2d: initialize the cost matrix
            int** temp_child_cost_matrix = NULL;
            create_array(num_cities, &temp_child_cost_matrix);
            
            // Step 2d.1: copy the the parent's cost matrix
            for(int l = 0; l < num_cities; l++)
            {
                for(int m = 0; m < num_cities; m++)
                {
                    temp_child_cost_matrix[l][m] = parent_node->reduced_cost_matrix[l][m];
                }
            }
            
            // step 2d.2: modify the child's cost matrix
            // step 2d.2.1: negate the row corresponding to the last_city
            for(int j = 0; j < num_cities; j++)
            {
                temp_child_cost_matrix[last_city][j] = INT_MAX;
            }
            
            // step 2d.2.2: negate the column corresponding to the next_city
            for(int j = 0; j < num_cities; j++)
            {
                temp_child_cost_matrix[j][next_city] = INT_MAX;
            }
            
            // step 2d.2.3: if child is NOT a leaf, also negate Cost_matrix[next_city, last_city] so that the travel loop doesn't short circuit
            if(is_child_leaf == 0)
            {
                temp_child_cost_matrix[next_city][last_city] = INT_MAX;
            }
            
            // step 2d.2.3: save the modified cost matrix to the child node's cost_matrix field
            child->cost_matrix = temp_child_cost_matrix;
            
            // step 2e: find the reduced_cost_matrix and total reduction
            matrix_reduc(child, num_cities);

            
            // Step 2f: calculate the lower_bound
            child->lower_bound = (parent_node->lower_bound) + parent_node->reduced_cost_matrix[last_city][next_city] + child->total_reduction;
            
            // store it in the linked list
            child->prev = NULL;
            child->next = NULL;
            insert_node(child, &head);
        }
        
        // STEP 3: Delete the parent node
        free_node(parent_node, num_cities);
    }
    
    *head_ptr = head;
}

/**
 * Takes a pointer to a node and evaluates it's children (i.e. creates the child nodes,
 * stores nodes in a sorted doubly linked list based on the child's lower bound value)
 * reduction and the (2) reduced matrix in the appropriate fields of the node. 
 */
void insert_node(node* child, node** head_ptr)
{
    node* head = *head_ptr;
    // check to see if list exists. If not, add the child to the front
    if(head == NULL)
    {
        head = child;
    }
	// else check if child belongs at list's head
	else if (child->lower_bound < head->lower_bound)
	{
		 child->next = head;
		 head->prev = child;
		 head = child;
	}

	// else try to insert number in middle or tail
	else
	{
		 node* ptr = head;
		 while (true)
		 {
			 // check for insertion at tail
			 if (ptr->next == NULL)
			 {
				 ptr->next = child;
				 child->prev = ptr;
				 return;
			 }

			 // check for insertion in middle
			 else if (ptr->next->lower_bound >= child->lower_bound)
			 {
				 child->next = ptr->next;
				 ptr->next->prev = child;
				 child->prev = ptr;
				 ptr->next = child;
				 return;
			 }

			 // advance pointer to next node
			 ptr = ptr->next;
		 }
	 }
	 *head_ptr = head;
}


/**
 * Prints the linked list. Used for debugging purposes.
 */
void print_linked_list(node* head)
{

    printf("the doubly linked list contains: \n");
	node* ptr = head;
	int i = 0;
	if(ptr == NULL)
	{
	    printf("linked list is empty\n");
	}
	while (ptr != NULL)
	{
		 printf("node %i, lowerbound %i, level: %i\n", i, ptr->lower_bound, ptr->path_size);
		 // advance pointer to next node
		 i++;
		 ptr = ptr->next;
	}
 }
 
 /**
 * Takes a pointer to a node and removes it from the linked list, but does not free the node
 */
void remove_node_from_list(node* ptr, node** head_ptr)
{
    node* head = *head_ptr;
    //ensure ptr is pointing to a node
	if (ptr == NULL)
	{
	    printf("Error: pointer is empty");
	}
	// if node is at the head of the list
	else if(ptr->prev == NULL)
	{
	    head = ptr-> next;
		if (head != NULL)
		{
		    head->prev = NULL;		    
		}
	    
	    *head_ptr = head;
		return;
	}
	// if node is in the middle of the list or at the end
	else
	{
	    ptr->prev->next = ptr->next;
		if (ptr->next != NULL)
		{
			ptr->next->prev = ptr->prev;
		}
	}
	
}
	
/**
 * Takes a pointer to a doubly linked list and frees the nodes. 
 */
void free_list(node** head_ptr, int num_cities)	
{
    node* ptr = *head_ptr;
    if(ptr != NULL)
    {
        //traverse to the second last node of the linked list
        while(ptr->next != NULL)
        {
            ptr = ptr->next;
            free_node(ptr->prev, num_cities);
        }
        free_node(ptr, num_cities);
    }
}

/**
 * Takes a pointer to a node and frees pointers within the node and the node itself 
 */
void free_node(node* ptr, int num_cities)
{
        // free the int arrays stored in the node
        free(ptr->path);
        free(ptr->possible_children);
        
        // free the 2D arrays stored in the node
        for(int i = 0; i < num_cities; i++)
        {
            free(ptr->cost_matrix[i]);
            free(ptr->reduced_cost_matrix[i]);
        }
        free(ptr->cost_matrix);
        free(ptr->reduced_cost_matrix);
        
        // free the node itself
        free(ptr);
}

 
 
 
 


