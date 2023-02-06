/*************************************
 * Lab 1 Exercise 2
 * Name:
 * Student No:
 * Lab Group:
 *************************************/

#include "node.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Add in your implementation below to the respective functions
// Feel free to add any headers you deem fit (although you do not need to)

// Traverses list and returns the sum of the data values
// of every node in the list.
long sum_list(list *lst)
{
  node *curr = lst->head;
  int sum = 0;
  while (curr) {
    sum += curr->data;       
    curr = curr->next; 
  }
  return sum;
}

// Traverses list and returns the number of data values in the list.
int list_len(list *lst)
{
  node *curr = lst->head;
  int size = 0;
  while (curr) {
    size ++;       
    curr = curr->next; 
  }
  return size;
}

// Inserts a new node with data value at index (counting from head
// starting at 0).
// Note: index is guaranteed to be valid.
void insert_node_at(list *lst, int index, int data)
{
  // create new node in heap
  node *newNode = malloc(sizeof(node));
  newNode->data = data;

  node *slow = malloc(sizeof(node));
  node *to_clean_up = slow;
  slow->next = lst->head;
  node *fast = lst->head;
  int i;
  for (i = 0; i < index; i++)
  {
    slow = slow->next;
    fast = fast->next;
  }

  slow->next = newNode;
  newNode->next = fast;
  if (index == 0)
  {
    lst->head = newNode;
  }

  free(to_clean_up);
}

// Deletes node at index (counting from head starting from 0).
// Note: index is guarenteed to be valid.

void delete_node_at(list *lst, int index)
{
  if (index == 0)
  {
    node *temp = lst->head;
    lst->head = lst->head->next;
    free(temp);
    return;
  }
  // using slow pointer approacher this time as opposed to considering edge cases
  node *slow = malloc(sizeof(node));
  node *to_clean_up = slow;
  slow->next = lst->head;
  node *fast = lst->head;
  int i;
  for (i = 0; i < index; i++)
  {
    slow = slow->next;
    fast = fast->next;
  }

  node *temp = slow->next;
  slow->next = fast->next;
  free(temp);
  free(to_clean_up);
}

// Search list by the given element.
// If element not present, return -1 else return the index. If lst is empty return -2.

int search_list(list *lst, int element)
{
  if (lst->head == NULL)
  {
    return -2;
  }

  node *curr = lst->head;
  int index = 0;
  while (curr != NULL)
  {
    if (curr->data == element)
    {
      return index;
    }
    index++;
    curr = curr->next;
  }
  return -1;
}

node *reverse(node *curr)
{
  if (curr == NULL || curr->next == NULL)
  {
    return curr;
  }

  node *next = curr->next;
  node *res = reverse(curr->next);
  next->next = curr;
  curr->next = NULL;
  return res;
}

// Reverses the list with the last node becoming the first node.
void reverse_list(list *lst)
{
  lst->head = reverse(lst->head);
}

// Resets list to an empty state (no nodes) and frees
// any allocated memory in the process
void reset_list(list *lst)
{
  node *curr = lst->head;
  node *next;

  while (curr != NULL)
  {
    next = curr->next;
    free(curr);
    curr = next;
  }
  lst->head = NULL;
}

// Traverses list and applies func on data values of
// all elements in the list.
void map(list *lst, int (*func)(int))
{
  node *curr = lst->head;
  while (curr) {
    curr->data = func(curr->data);
    curr = curr->next;
  }
}
