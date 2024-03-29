/*************************************
* Lab 1 Exercise 3
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>

#include "function_pointers.h"
#include "node.h"

// The runner is empty now! Modify it to fulfill the requirements of the
// exercise. You can use ex2.c as a template

// DO NOT initialize the func_list array in this file. All initialization
// logic for func_list should go into function_pointers.c.

// Macros
#define SUM_LIST 0
#define INSERT_AT 1
#define DELETE_AT 2
#define SEARCH_LIST 3
#define REVERSE_LIST 4
#define RESET_LIST 5
#define LIST_LEN 6
#define MAP 7
#define PRINT_LIST 8

void run_instruction(list *lst, int instr, FILE *fp);
void print_list(list *lst);
void print_index(int index);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: expecting 1 argument, %d found\n", argc - 1);
        exit(1);
    }

    // We read in the file name provided as argument
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    

    // Update the array of function pointers
    // DO NOT REMOVE THIS CALL
    // (You may leave the function empty if you do not need it)
    update_functions();

    // Rest of code logic here
    list *lst = (list *)malloc(sizeof(list));
    lst->head = NULL;

    int num;

    while (fscanf(fp, "%d", &num) == 1) {  // Read integers from the file
        // printf("%d\n", num);
        run_instruction(lst, num, fp);
    }
    
    
    fclose(fp);
    reset_list(lst);
    free(lst);
}


void run_instruction(list *lst, int instr, FILE *fp)
{
    int index, data, element;
    switch (instr)
    {
    case SUM_LIST:
        printf("%ld\n", sum_list(lst));
        break;
    case LIST_LEN:
        printf("%d\n", list_len(lst));
        break;
    case MAP:
        fscanf(fp, "%d", &index);
        map(lst, func_list[index]);
        break;
    case PRINT_LIST:
        print_list(lst);
        break;
    case INSERT_AT:
        fscanf(fp, "%d %d", &index, &data);
        insert_node_at(lst, index, data);
        break;
    case DELETE_AT:
        fscanf(fp, "%d", &index);
        delete_node_at(lst, index);
        break;
    case SEARCH_LIST:
        fscanf(fp, "%d", &element);
        int ind = search_list(lst, element);
        print_index(ind);
        break;
    case REVERSE_LIST:
        reverse_list(lst);
        break;
    case RESET_LIST:
        reset_list(lst);
    }
}

// Prints out the whole list in a single line
void print_list(list *lst)
{
    if (lst->head == NULL)
    {
        printf("[ ]\n");
        return;
    }

    printf("[ ");
    node *curr = lst->head;
    do
    {
        printf("%d ", curr->data);
        curr = curr->next;
    } while (curr != NULL);
    printf("]\n");
}

// Print index
void print_index(int index)
{
    if (index == -2)
        printf("{}\n");
    else
        printf("{%d}\n", index);
}
