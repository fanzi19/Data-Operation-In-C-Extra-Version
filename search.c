#include "search.h"
#include "dataops.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_sorted_data(Employee *employees, int count) {
    printf("Sorted Data:\n");
    for (int i = 0; i < count; i++) {
        printf("%d %s %s %d\n", employees[i].id, employees[i].first_name, employees[i].last_name, employees[i].salary);
    }
}

// Function to compare two employees based on the specified column
int compareEmployees(const Employee *emp1, const Employee *emp2, const char *column) {
    if (strcmp(column, "last_name") == 0) {
        return strcmp(emp1->last_name, emp2->last_name);
    } else if (strcmp(column, "id") == 0) {
        return emp1->id - emp2->id;
    } else if (strcmp(column, "salary") == 0) {
        return emp1->salary - emp2->salary;
    }
    return 0; // Default to no change
}

void swapEmployees(Employee **a, Employee **b) {
    Employee *temp = *a;
    *a = *b;
    *b = temp;
}

//sorting before search
Employee **quicksort(Employee **arr, int low, int high, int (*compareEmployees)(const Employee *, const Employee *, const char *), char *column) {
    if (low < high) {
        int pivot_index = (low + high) / 2;
        Employee *pivot = arr[pivot_index];
        int i = low;
        int j = high;

        while (i <= j) {
            while (compareEmployees(arr[i], pivot, column) < 0 && i <= j) {
                i++;
            }
            while (compareEmployees(arr[j],pivot,column) > 0 && i <= j) {
                j--;
            }
            if (i <= j) {
                swapEmployees(&arr[i], &arr[j]);
                i++;
                j--;
            }
        }
        if (low < j) {
            arr = quicksort(arr, low, j, compareEmployees, column);
        }
        if (i < high) {
            arr = quicksort(arr, i, high, compareEmployees, column);
        }
    }

    return arr;
}


//search methods
int linear_search(Employee **employees, int count, int target_id) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (employees[i]->id == target_id) {
            printf("\nsix_digit_ID  first_name  last_name  salary\n");
            printf("--------------------------\n");
            printf("%d %s %s %d\n", employees[i]->id, employees[i]->first_name, employees[i]->last_name, employees[i]->salary);
            printf("--------------------------\n");
            found = 1;
            return i;
        }
    }
    if (!found) {
        printf("Employee with id %d not found in DB\n", target_id);
        clearInputBuffer();
    }
    return -1;
}


int binary_search(Employee **employees, int count, const char *target_lastname) {
    int left = 0;
    int right = count - 1;
    int found = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(employees[mid]->last_name, target_lastname);
        if (cmp == 0) {
            found = 1;
            return mid;
        } else if (cmp < 0) {
            left = mid + 1; // Continue searching in the right half
        } else {
            right = mid - 1; // Continue searching in the left half
        }
    }
    if (!found) {
        printf("Employee %s not found in DB\n", target_lastname);
    }
    return -1; // Element not found
}


// Function to find all occurrences of the target value in a sorted array
void findAllOccurrences(Employee **employees, int count, const char *target_lastname) {
    int firstIndex = binary_search(employees, count, target_lastname);

    if (firstIndex == -1) {
        return;
    }

    printf("All occurrences of %s are at indices: \n", target_lastname);
    printf("\nsix_digit_ID  first_name  last_name  salary\n");
    printf("--------------------------------\n");
    // Print occurrences backward
    for (int i = firstIndex; i >= 0 && strcmp(employees[i]->last_name, target_lastname) == 0; --i) {
        printf("%d, %s, %s, %d\n",
            employees[i]->id, employees[i]->first_name,
            employees[i]->last_name, employees[i]->salary);
    }

    // Print occurrences forward
    for (int i = firstIndex + 1; i < count && strcmp(employees[i]->last_name, target_lastname) == 0; ++i) {
        printf("%d, %s, %s, %d\n",
            employees[i]->id, employees[i]->first_name,
            employees[i]->last_name, employees[i]->salary);
    }
    printf("--------------------------------\n");
    printf("\n");
}



int fibonacci_search(Employee **employees, int count, const char *target_lastname) {
    // Initialize Fibonacci numbers
    int fibMMm2 = 0; // (m-2)'th Fibonacci No.
    int fibMMm1 = 1; // (m-1)'th Fibonacci No.
    int fibM = fibMMm2 + fibMMm1; // m'th Fibonacci

    // Calculate the smallest Fibonacci number greater than or equal to count
    while (fibM < count) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1; // Initialize the offset

    // Search for the target_lastname in the array
    while (fibM > 1) {
        int i = (offset + fibMMm2) < (count - 1) ? (offset + fibMMm2) : (count - 1); // Calculate the next offset

        // Compare the target_lastname with the current element in the array
        if (strcmp(employees[i]->last_name, target_lastname) < 0) { // If the current element is less than the target
            fibM = fibMMm1; // Update Fibonacci series
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i; // Update the offset
        } else if (strcmp(employees[i]->last_name, target_lastname) > 0) { // If the current element is greater than the target
            fibM = fibMMm2; // Update Fibonacci series
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else { // If the current element is the target
            // Print the details of the employee
            printf("%d %s %s %d\n", employees[i]->id, employees[i]->first_name, employees[i]->last_name, employees[i]->salary);
            return i; // Return the index of the found element
        }
    }

    // Check if the last element matches the target
    if (fibMMm1 && strcmp(employees[offset + 1]->last_name, target_lastname) == 0) {
        // Print the details of the employee
        printf("\nsix_digit_ID  first_name  last_name  salary\n");
        printf("--------------------------\n");
        printf("%d %s %s %d\n", employees[offset + 1]->id, employees[offset + 1]->first_name, employees[offset + 1]->last_name, employees[offset + 1]->salary);
        printf("--------------------------\n");
        return offset + 1; // Return the index of the found element
    }

    // If the element is not found, print a message
    printf("Employee %s not found in DB\n", target_lastname);
    return -1; // Element not found
}

// Function to insert an employee into the tree
struct TreeNode *insert(struct TreeNode *root, Employee *employees) {
    if (root == NULL) {
        struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
        node->employees = employees;
        node->left = NULL;
        node->right = NULL;
        return node;
    }

    int checklastname = strcmp(employees->last_name, root->employees->last_name); 

    if (checklastname < 0 || (checklastname == 0 && strcmp(employees->first_name, root->employees->first_name) < 0)) {
        root->left = insert(root->left, employees);
    } else if (checklastname > 0 || (checklastname == 0 && strcmp(employees->first_name, root->employees->first_name) > 0)){
        root->right = insert(root->right, employees);
    }

    return root;
}

// Function to free the memory occupied by the tree
void freeTree(struct TreeNode *root) {
    if (root == NULL) {
        return;
    }

    // Recursively free left and right subtrees
    freeTree(root->left);
    freeTree(root->right);

    // Free the current node
    free(root);
}


// Function to perform an inorder traversal of the tree
void inorder(struct TreeNode *root) {
    if (root != NULL) {
        inorder(root->left);
        // Check for NULL before accessing members
        if (root->employees != NULL) {
            printf("%d %s %s %d\n", root->employees->id, root->employees->first_name, root->employees->last_name, root->employees->salary);
        } else {
            printf("Warning: NULL employee data in tree node.\n");
        }
        inorder(root->right);
    }
}

// Function to search for an employee by last name
struct TreeNode *search(struct TreeNode *root, const char *target_lastname) {
    if (root == NULL || strcmp(root->employees->last_name, target_lastname) == 0) {
        return root;
    }

    if (strcmp(root->employees->last_name, target_lastname) < 0) {
        return search(root->right, target_lastname);
    }

    return search(root->left, target_lastname);
}