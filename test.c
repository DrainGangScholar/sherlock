#include "sherlock.h"
#include <stdint.h>
#include <stdlib.h>

// testiranje :DD
typedef struct _teacher_
{
    char name[50];
    uint8_t age;
    uint32_t id_number;
    float salary;
} teacher_t;
typedef struct _student_
{
    char name[50];
    uint8_t age;
    uint32_t id_number;
    float gpa;
} student_t;

field_info_t teacher_fields[] = {
    FIELD_INFO(teacher_t, name, CHAR, NULL),
    FIELD_INFO(teacher_t, age, UINT8, NULL),
    FIELD_INFO(teacher_t, id_number, UINT32, NULL),
    FIELD_INFO(teacher_t, salary, FLOAT, NULL)};

field_info_t student_fields[] = {
    FIELD_INFO(student_t, name, CHAR, NULL),
    FIELD_INFO(student_t, age, UINT8, NULL),
    FIELD_INFO(student_t, id_number, UINT32, NULL),
    FIELD_INFO(student_t, gpa, FLOAT, NULL)};
int main()
{
    // generise se baza :DDD
    struct_db_t struct_db;
    struct_db.head = NULL;
    struct_db.count = 0;

    // registruju se strukture :DDD
    REG_STRUCT(&struct_db, teacher_t, teacher_fields);
    REG_STRUCT(&struct_db, student_t, student_fields);

    // print_structure_db(&struct_db);

    struct_db_rec_t *check = struct_db_look_up(&struct_db, "student_t");
    print_structure_rec(check);

    return 0;
}
