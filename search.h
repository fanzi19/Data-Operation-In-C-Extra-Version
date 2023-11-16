#ifndef SEARCH_H
#define SEARCH_H

#include "employee.h"

void print_sorted_data(Employee *employees, int count);

//sorting
int compareEmployees(const Employee *emp1, const Employee *emp2, const char *column);
Employee **quicksort(Employee **arr, int low, int high, int (*compareEmployees) (const Employee *, const Employee *, const char *), char *column);


//search methods
int linear_search(Employee **employees, int count, int targe_id);
int binary_search(Employee **employees, int count, const char *target_lastname);
int fibonacci_search(Employee **employees, int count, const char *target_lastname);
void findAllOccurrences(Employee **employees, int count, const char *target_lastname);


// Tree node struct
struct TreeNode {
    Employee *employees;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Function to insert an employee into the tree
struct TreeNode *insert(struct TreeNode *root, Employee *employees);

// Function to free the memory occupied by the tree
void freeTree(struct TreeNode *root);

// Function to perform an inorder traversal of the tree
void inorder(struct TreeNode *root);

// Function to search for an employee by last name
struct TreeNode *search(struct TreeNode *root, const char *target_lastname);


#endif /* SEARCH_H */
