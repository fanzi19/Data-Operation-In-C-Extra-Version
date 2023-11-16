#include "dataops.h"
#include "readfile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// Function to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to get the maximum ID from the file
int getMaxID() {
    FILE *fp;
    int maxID = 0;
    char buffer[100];

    fp = fopen("data.txt", "r");
    skip_header(fp);
    if (fp == NULL) {
        return 970017;  // If the file doesn't exist, return default threshold
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int id;
        if (sscanf(buffer, "%d", &id) == 1) {
            if (id > maxID) {
                maxID = id;
            }
        } else {
            // Handle invalid data in the file (non-integer)
            printf("Invalid data in the file: %s\n", buffer);
        }

        // Clear the buffer to ensure it's empty before each read
        memset(buffer, 0, sizeof(buffer));
    }

    fclose(fp);
    return maxID;
}

//check if a string contains only alphabetic characters
int isAlphaString(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return 0;  // Not an alphabetical character
        }
    }
    return 1;  // All characters are alphabetical
}

//check if a string contains only digits
int isNumericString(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;  // Not a digit
        }
        str++;
    }
    return 1;  // All characters are digits
}

//unique check for updating
int isIdUnique(Employee *employees, int count, int new_id) {
    for (int i = 0; i < count; i++) {
        if (employees[i].id == new_id) {
            return 0; // ID is not unique
        }
    }
    return 1; // ID is unique
}

// Function to add data set to the original struct data and write it to a file
void addDataToFile(const Employee *newEmployee) {
    FILE *originalFile = fopen("data.txt", "r");
    if (originalFile == NULL) {
        perror("Error opening original file");
        return;
    }

    FILE *tempFile = fopen("temp_file.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(originalFile);
        return;
    }

    Employee tempEmployee;

    // Copy the existing data to the temporary file with updates
    fprintf(tempFile, "ID First_Name Last_Name Salary\n");
    skip_header(originalFile);

    while (fscanf(originalFile, "%d %63s %63s %d",
                 &(tempEmployee.id), tempEmployee.first_name, tempEmployee.last_name, &(tempEmployee.salary)) == 4) {
        fprintf(tempFile, "%d %s %s %d\n",
                tempEmployee.id, tempEmployee.first_name, tempEmployee.last_name, tempEmployee.salary);
    }

    // Add the new data
    fprintf(tempFile, "%d %s %s %d\n",
            newEmployee->id, newEmployee->first_name, newEmployee->last_name, newEmployee->salary);

    fclose(originalFile);
    fclose(tempFile);

    remove("data.txt");
    rename("temp_file.txt", "data.txt");
}

// Function to add data to memory
void addDataToMemory(Employee **employees, int *count, const Employee *newEmployee) {
    // Allocate memory for the first entry if it hasn't been allocated yet
    if (*employees == NULL) {
        *employees = malloc(sizeof(Employee));
        if (*employees == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
    } else {
        // Reallocate memory for the new entry
        Employee *temp = realloc(*employees, (*count + 1) * sizeof(Employee));
        if (temp == NULL) {
            perror("Error reallocating memory");
            exit(EXIT_FAILURE);
        }
        *employees = temp;
    }

    // Copy data into the new entry
    Employee *newEntry = &(*employees)[*count];

    newEntry->id = newEmployee->id;

    strncpy(newEntry->first_name, newEmployee->first_name, sizeof(newEntry->first_name) - 1);
    newEntry->first_name[sizeof(newEntry->first_name) - 1] = '\0';

    strncpy(newEntry->last_name, newEmployee->last_name, sizeof(newEntry->last_name) - 1);
    newEntry->last_name[sizeof(newEntry->last_name) - 1] = '\0';

    newEntry->salary = newEmployee->salary;

    // Increment the count for the next entry
    (*count)++;

    // Print debug information
    printf("----------!!\n");
    printf("newEmployee: %d, %s, %s, %d\n",
           newEmployee->id, newEmployee->first_name,
           newEmployee->last_name, newEmployee->salary);
    printf("*count = %d\n", *count);
    printf("(*employees)[%d].id = %d\n", *count - 1, (*employees)[*count - 1].id);
}


// Function to add data set to the original struct data and write it to a file
Employee *addData(Employee ***employees, int *count) {
    clearInputBuffer();
    Employee tempEmployee;
    char buffer[100];

    int threshold = getMaxID();  // You may want to set a suitable default threshold value

    // Input employee ID
    while (1) {
        printf("Enter employee ID (should be larger than %d): ", threshold);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &(tempEmployee.id)) == 1 && tempEmployee.id > threshold) {
            break;
        }
        printf("Invalid input. Please enter a valid ID.\n");
    }

    // Input first name
    while (1) {
        printf("Enter first name (not exceeding 64 characters): ");
        fgets(tempEmployee.first_name, sizeof(tempEmployee.first_name), stdin);
        strtok(tempEmployee.first_name, "\n");
        if (strlen(tempEmployee.first_name) <= 64 && isAlphaString(tempEmployee.first_name)) {
            break;
        }
        printf("Invalid input. Please enter a valid first name.\n");
    }

    // Input last name
    while (1) {
        printf("Enter last name (not exceeding 64 characters): ");
        fgets(tempEmployee.last_name, sizeof(tempEmployee.last_name), stdin);
        strtok(tempEmployee.last_name, "\n");
        if (strlen(tempEmployee.last_name) <= 64 && isAlphaString(tempEmployee.last_name)) {
            break;
        }
        printf("Invalid input. Please enter a valid last name.\n");
    }

    // Input salary
    while (1) {
        printf("Enter salary (should be a non-decimal digit): ");
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n"); 
        if (sscanf(buffer, "%d", &(tempEmployee.salary)) == 1 && isNumericString(buffer)) {
            break;
        }
        printf("Invalid input. Please enter a valid salary.\n");
    }

    printf("\nEntered data:\n");
    printf("ID: %d\n", tempEmployee.id);
    printf("First name: %s\n", tempEmployee.first_name);
    printf("Last name: %s\n", tempEmployee.last_name);
    printf("Salary: %d\n", tempEmployee.salary);

    // Confirm data entry
    while (1) {
        char confirmation[2];
        printf("Confirm data entry (Y/N): ");
        fgets(confirmation, sizeof(confirmation), stdin);
        if (confirmation[0] == 'Y' || confirmation[0] == 'y') {
            // Create a copy of tempEmployee and pass it to functions
            Employee tempCopy = tempEmployee;

            // Add data to the file
            addDataToFile(&tempCopy);
            printf("Data has been inserted into the file.\n");

            // Update employees and count
            *employees = realloc(*employees, (*count + 1) * sizeof(Employee *));
            (*employees)[*count] = malloc(sizeof(Employee));
            (*employees)[*count]->id = tempCopy.id;
            strcpy((*employees)[*count]->first_name, tempCopy.first_name);
            strcpy((*employees)[*count]->last_name, tempCopy.last_name);
            (*employees)[*count]->salary = tempCopy.salary;

            (*count)++;

            // Print debug information
            printf("----------!!\n");
            printf("newEmployee: %d, %s, %s, %d\n",
                   tempCopy.id, tempCopy.first_name,
                   tempCopy.last_name, tempCopy.salary);
            printf("*count = %d\n", *count);
            printf("(*employees)[%d].id = %d\n", *count - 1, (*employees)[*count - 1]->id);

            return (*employees)[*count - 1];  // Return the newly added Employee
        } else if (confirmation[0] == 'N' || confirmation[0] == 'n') {
            printf("Data entry cancelled. Please try again.\n");
            break;
        } else {
            printf("Invalid input. Please enter Y or N.\n");
        }
    }

    return NULL;  // Return NULL if data entry is cancelled
}

//delete data
void deleteById(Employee **employee, int *target_id, int *count, int index) {

	FILE *inputFile, *outputFile;
    inputFile = fopen("data.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    skip_header(inputFile);

    outputFile = fopen("temp_file.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }
    //add a header to the output file
    fprintf(outputFile, "ID First_Name Last_Name Salary\n");


    //prompt to confirm
    while (1) {
    	clearInputBuffer();
        char confirmation[2];
        printf("Confirm data delete (Y/N):");
        fgets(confirmation, sizeof(confirmation), stdin);
        if (confirmation[0] == 'Y' || confirmation[0] == 'y') {

        	//deal with the struct array
    		//clear the target element
        	memset(employee[index], 0, sizeof(Employee));

    		for (int i=index; i< *count-1; i++) {
    				memcpy(employee[i], employee[i + 1], sizeof(Employee));
    		}	
    				
    		//clear the last element
            memset(employee[*count - 1], 0, sizeof(Employee));

    		(*count)--;
    		printf("*count= %d\n", *count);
    		
    		//deal with the data file, avoid read write conflict, define a new array
        	Employee tempEmployee;

            while (fscanf(inputFile, "%d %63s %63s %d", 
            			&(tempEmployee.id), tempEmployee.first_name, tempEmployee.last_name, &(tempEmployee.salary)) == 4) {
        		if (tempEmployee.id != *target_id) {
            		// Write the record to the output file
            		fprintf(outputFile, "%d %s %s %d\n",
                    		tempEmployee.id, tempEmployee.first_name, tempEmployee.last_name, tempEmployee.salary);
        		} else {
            		printf("Deleted record with ID %d\n", *target_id);
        		}
        	}	

            break;
        } else if (confirmation[0] == 'N' || confirmation[0] == 'n') {
            printf("Delete Operation Cancelled.\n");
            break;
        } else {
            printf("Invalid input. Please enter Y or N.\n");
        }
    }

    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);

    // Remove the original file
    remove("data.txt");

    // Rename the temporary file to the original file
    if (rename("temp_file.txt", "data.txt") != 0) {
        perror("Error renaming file");
        exit(EXIT_FAILURE);
    }
}

//update data
Employee *updateById(Employee **employees, int *count, int *target_id, int index) {
    int update_choice;
    int new_id;
    char temp_first_name[64];
    char temp_last_name[64];
    int temp_salary;
    Employee *updatedEmployee;

    // Validate index to avoid out-of-bounds access
    if (index < 0 || index >= *count) {
        printf("Invalid index.\n");
        return NULL;
    }

    // Open the original file in read mode
    FILE *originalFile = fopen("data.txt", "r");
    if (originalFile == NULL) {
        perror("Error opening original file");
        return NULL;
    }

    skip_header(originalFile);

    // Open a temporary file in write mode
    FILE *tempFile = fopen("temp_file.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(originalFile);
        return NULL;
    }

    // Allocate memory for updatedEmployee
    updatedEmployee = malloc(sizeof(Employee));
    if (updatedEmployee == NULL) {
        perror("Error allocating memory for updatedEmployee");
        fclose(originalFile);
        fclose(tempFile);
        return NULL;
    }

    // Copy the original data to updatedEmployee
    memcpy(updatedEmployee, employees[index], sizeof(Employee));

    do {
        printf("Select parts to update for record with ID %d:\n", *target_id);
        printf("1. ID\n");
        printf("2. First Name\n");
        printf("3. Last Name\n");
        printf("4. Salary\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &update_choice);

        switch (update_choice) {
            case 1:
                do {
                    printf("Enter new id: ");
                    scanf("%d", &new_id);

                    // Validate the input new id
                    if (new_id < 100000 || new_id > 10000000) {
                        printf("Enter a 6-digit ID.\n");
                        continue;  // Prompt for a new ID
                    }

                    while (!isIdUnique((*employees), *count, new_id)) {
                        printf("ID %d already exists. Enter a different ID: ", new_id);
                        scanf("%d", &new_id);
                    }
                } while (new_id < 100000 || new_id > 10000000);
                updatedEmployee->id = new_id;
                break;
            case 2:
                printf("Enter new first name: ");
                scanf("%63s", temp_first_name);

                // Validate the first name (alphabetic characters only)
                if (!isAlphaString(temp_first_name)) {
                    printf("Invalid first name. Please enter alphabetic characters only.\n");
                } else {
                    strcpy(updatedEmployee->first_name, temp_first_name);
                }
                break;
            case 3:
                printf("Enter new last name: ");
                scanf("%63s", temp_last_name);

                // Validate the last name (alphabetic characters only)
                if (!isAlphaString(temp_last_name)) {
                    printf("Invalid last name. Please enter alphabetic characters only.\n");
                } else {
                    strcpy(updatedEmployee->last_name, temp_last_name);
                }
                break;
            case 4:
                printf("Enter new salary: ");
                scanf("%d", &temp_salary);

                // Convert the integer to a string
                char temp_salary_str[20];
                snprintf(temp_salary_str, sizeof(temp_salary_str), "%d", temp_salary);

                // Validate the salary (numeric characters only)
                while (getchar() != '\n');  // Clear input buffer
                if (!isNumericString(temp_salary_str)) {
                    printf("Invalid salary. Please enter numeric characters only.\n");
                } else {
                    updatedEmployee->salary = temp_salary;
                }
                break;
            case 5:
                printf("Bye!\n");
                break;
            default:
                printf("Invalid choice. No fields updated.\n");
        }

    } while (update_choice != 5);

    // Copy the existing data to the temporary file with updates
    fprintf(tempFile, "ID First_Name Last_Name Salary\n");

    Employee tempEmployee;

    while (fscanf(originalFile, "%d %63s %63s %d",
                  &(tempEmployee.id), tempEmployee.first_name, tempEmployee.last_name, &(tempEmployee.salary)) == 4) {
        if (tempEmployee.id == *target_id) {
            // Update the data if it matches the target ID
            fprintf(tempFile, "%d %s %s %d\n",
                    updatedEmployee->id, updatedEmployee->first_name, updatedEmployee->last_name, updatedEmployee->salary);
        } else {
            // Copy the existing data as is
            fprintf(tempFile, "%d %s %s %d\n",
                    tempEmployee.id, tempEmployee.first_name, tempEmployee.last_name, tempEmployee.salary);
        }
    }

    // Close the files
    fclose(originalFile);
    fclose(tempFile);

    // Remove the original file
    remove("data.txt");

    // Replace the original file with the temporary file
    if (rename("temp_file.txt", "data.txt") != 0) {
        perror("Error renaming file");
        free(updatedEmployee);  // Free memory in case of an error
        return NULL;
    }

    printf("Record with ID %d updated successfully.\n", *target_id);
    return updatedEmployee;
}