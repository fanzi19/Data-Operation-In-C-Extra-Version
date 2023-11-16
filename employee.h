#ifndef EMPLOYEE_H
#define EMPLOYEE_H

typedef struct {
    int id;
    char first_name[64];
    char last_name[64];
    int salary;
} Employee;

#endif