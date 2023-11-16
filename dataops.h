#ifndef ADDDATA_H
#define ADDDATA_H

#include "employee.h"
#include "readfile.h"

void clearInputBuffer();
Employee *addData(Employee ***employees, int *count);
void deleteById(Employee **employee, int *target_id, int *count, int index);
Employee *updateById(Employee **employees, int *count, int *target_id, int index);

#endif /* ADDDATA_H */
