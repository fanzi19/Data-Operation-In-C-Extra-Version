#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readfile.h"
#include "search.h"
#include "dataops.h"
#include "employee.h"

#define MAX_EMPLOYEES 1024

int main() {
    FILE *file;
    if (open_file(&file, "data.txt") != 0) {
        printf("Failed to open file.\n");
        return 1;
    }

    skip_header(file); //skip first line

    Employee *employees[MAX_EMPLOYEES]; 
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
    employees[i] = malloc(sizeof(Employee));
    }
    int count = 0;

    while (1) {
        int id;
        char first_name[64];
        char last_name[64];
        int salary;

        if (read_int(file, &id) != 1) {
            break;
        }

        if (read_string(file, first_name, 64) != 1) {
            break;
        }

        if (read_string(file, last_name, 64) != 1) {
            break;
        }

        if (read_int(file, &salary) != 1) {
            break;
        }

        employees[count]->id = id;
        strcpy(employees[count]->first_name, first_name);
        strcpy(employees[count]->last_name, last_name);
        employees[count]->salary = salary;

        count++;

        if (count >= MAX_EMPLOYEES) {
            printf("Maximum employee count reached.\n");
            break;
        }
    }

    //start up with sorted employee id
    char column[20];
    strcpy(column, "id");
    quicksort(employees, 0, count - 1, compareEmployees, column);

    int choice; //for main menu options
    int target_id; //for ID search
    char target_lastname[64]; //for last name search
    int search_choice; //for search method options
    int top_m; //for the top M employees with the highest salaries
    struct TreeNode *root = NULL; 

    Employee *newEmployee;
    Employee *updatedEmployee;

    //enter the menu repeatedly until exited
    do {
        printf("\nEmployee DB Menu:\n");
        printf("---------------------\n");
        printf("  (1) Print the Database\n");
        printf("  (2) Lookup by ID\n");
        printf("  (3) Lookup by Last Name\n");
        printf("  (4) Add an Employee\n");
        printf("  (5) Quit\n");
        printf("  (6) Remove an Employee\n");
        printf("  (7) Update an Employee's Info\n");
        printf("  (8) Print the M Employees with the Highest Salaries\n");
        printf("  (9) Find all Employees with matching Last Name\n");
        printf("---------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nsix_digit_ID  first_name  last_name  salary\n");
                printf("--------------------------\n");
                printData(&count);
                printf("--------------------------\n");
                printf("Number of Employees (%d)\n", count);
                break;
            case 2:
                printf("Enter a 6 digit employee id: ");
                scanf("%d", &target_id);
                linear_search(employees, count, target_id);
                break;
            case 3:
                printf("Enter Employee's last name (no extra spaces): ");
                scanf("%s", target_lastname);

                // Consume the newline character
                getchar();

                printf("--------------------------\n");
                printf("  (1) Binary Search\n");
                printf("  (2) Fibonacci search\n");
                printf("  (3) Binary Tree Search\n");
                printf("--------------------------\n");
                printf("Enter your search choice: ");
                scanf("%d", &search_choice);

                switch (search_choice) {
                    case 1:
                        //binary search
                        strcpy(column, "last_name");
                        quicksort(employees, 0, count - 1, compareEmployees, column);
                        int mid = binary_search(employees, count, target_lastname);
                        if (mid != -1) {
                            printf("\nsix_digit_ID  first_name  last_name  salary\n");
                            printf("--------------------------\n");
                            printf("%d %s %s %d\n", employees[mid]->id, employees[mid]->first_name, employees[mid]->last_name, employees[mid]->salary);
                            printf("--------------------------\n");
                        }
                        // Add an else block to handle the case when the result is not found
                        else {
                            printf("Employee with last name %s not found.\n", target_lastname);
                        }
                        break;
                    case 2:
                        //fibonacci search
                        strcpy(column, "last_name");
                        quicksort(employees, 0, count - 1, compareEmployees, column);
                        fibonacci_search(employees, count, target_lastname);
                        break;
                    case 3:
                        //binary tree search
                        // Inserting employees into the tree
                        for (int i = 0; i < count; i++) {
                            root = insert(root, employees[i]);
                        }

                        // Printing employees using an inorder traversal
                        printf("\nInorder Traversal of the Tree:\n");
                        printf("--------------------------------\n");
                        inorder(root);
                        printf("--------------------------------\n");
                        // Searching for an employee by last name
                        struct TreeNode *result = search(root, target_lastname);

                        if (result != NULL) {
                            printf("\nEmployee found by last name %s:\n", target_lastname);
                            printf("--------------------------------\n");
                            printf("%d %s %s %d\n", result->employees->id, result->employees->first_name, result->employees->last_name, result->employees->salary);
                            printf("--------------------------------\n");
                        } else {
                            printf("Employee with last name %s not found in the database.\n", target_lastname);
                        }
                        freeTree(root);
                        break;
                    default:
                        printf("Invalid choice for search method\n");
                        break;
                }
                break;
            case 4:
                //add an employee
                newEmployee = addData(&employees, &count);
                printf("Check employee data after add operation:\n");
                printf("\nsix_digit_ID  first_name  last_name  salary\n");
                printf("--------------------------------\n");
                if (newEmployee != NULL) {
                    employees[count - 1] = newEmployee;
                    for (int i = 0; i < count; i++) {
                        printf("%d, %s, %s, %d\n",
                               employees[i]->id, employees[i]->first_name,
                               employees[i]->last_name, employees[i]->salary);
                    }
                } else {
                    printf("Error adding employee.\n");
                }
                printf("--------------------------------\n");
                break;
            case 5:
                //exit the program
                printf("Bye!\n");
                break;
            case 6:
                //remove an employee
                printf("Enter a 6 digit employee id: ");
                scanf("%d", &target_id);
                //search for the target id
                int index6 = linear_search(employees, count, target_id);
                if (index6 == -1) {
                    break;
                } else {
                    deleteById(employees, &target_id, &count, index6);
                    //back to the array sorted by id
                    strcpy(column, "id");
                    quicksort(employees, 0, count - 1, compareEmployees, column);
                    printf("check employee data after delete operation: \n");
                    printf("\nsix_digit_ID  first_name  last_name  salary\n");
                    printf("--------------------------------\n");
                    for (int i = 0; i < count; i++) {
                        printf("%d, %s, %s, %d\n",
                            employees[i]->id, employees[i]->first_name,
                            employees[i]->last_name, employees[i]->salary);
                    }
                    printf("--------------------------------\n");
                }
                break;
            case 7:
            // Update info
            printf("Enter a 6 digit employee id: ");
            scanf("%d", &target_id);

            // Search for the target ID
            int index7 = linear_search(employees, count, target_id);
            if (index7 == -1) {
                printf("Employee with ID %d not found.\n", target_id);
                break;
            } else {
                // Update employee data by ID
                updatedEmployee = updateById(employees, &count, &target_id, index7);

                // Check if the update was successful
                if (updatedEmployee != NULL) {
                    // Free the memory of the old employee
                    free(employees[index7]);

                    // Update the array with the updated employee
                    employees[index7] = updatedEmployee;

                    // Back to the array sorted by ID
                    strcpy(column, "id");
                    quicksort(employees, 0, count - 1, compareEmployees, column);

                    // Print updated employee data
                    printf("\nCheck employee data after the update operation:\n");
                    printf("\nSix_digit_ID  First_name  Last_name  Salary\n");
                    printf("------------------------------------------\n");
                    for (int i = 0; i < count; i++) {
                        printf("%d, %s, %s, %d\n",
                            employees[i]->id, employees[i]->first_name,
                            employees[i]->last_name, employees[i]->salary);
                    }
                    printf("------------------------------------------\n");

                    printf("Record with ID %d updated successfully.\n", target_id);
                } else {
                    printf("Failed to update record with ID %d.\n", target_id);
                }
            }
            break;
            case 8:
                //print the M highest salary
                strcpy(column, "salary");
                quicksort(employees, 0, count - 1, compareEmployees, column);

                printf("Enter a number to return the M Highest Salaries: ");
                scanf("%d", &top_m);
                printf("\nsix_digit_ID  first_name  last_name  salary\n");
                printf("--------------------------------\n");
                for (int i = count-1; i > count - 1 - top_m; i--) {
                        printf("%d, %s, %s, %d\n",
                            employees[i]->id, employees[i]->first_name,
                            employees[i]->last_name, employees[i]->salary);
                    }
                printf("--------------------------------\n");
                //back to the array sorted by id
                strcpy(column, "id");
                quicksort(employees, 0, count - 1, compareEmployees, column);
                break;
            case 9:
                //find all employees with matching last name
                strcpy(column, "last_name");
                quicksort(employees, 0, count - 1, compareEmployees, column);
                printf("Enter Employee's last name (no extra spaces): ");
                scanf("%s", target_lastname);
                findAllOccurrences(employees, count, target_lastname);
                break;
            default:
                printf("Invalid choice. Please enter a valid choice.\n");
                clearInputBuffer();
        }
    } while (choice != 5);

    if (close_file(file) != 0) {
        printf("Failed to close file.\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
    free(employees[i]);
    }



    return 0;
}
