#include "../include/sherlock.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
                     "CHAR", "FLOAT", "OBJ_PTR",
                     "DOUBLE", "OBJ_STRUCT"};

int add_structure_to_struct_db(struct_db_t *struct_db,
                               struct_db_rec_t *struct_rec)
{
    struct_db_rec_t *head = struct_db->head;

    if (!head)
    {
        struct_db->head = struct_rec;
        struct_rec->next = NULL;
        struct_db->count++;
        return 0;
    }

    struct_rec->next = head;
    struct_db->head = struct_rec;
    struct_db->count++;
    return 0;
}

void print_structure_rec(struct_db_rec_t *struct_rec)
{
    if (!struct_rec)
    {
        return;
    }

    field_info_t *field = NULL;
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);

    for (unsigned int i = 0; i < struct_rec->n_fields; i++)
    {
        field = &struct_rec->fields[i];
        printf("  %-20s |", "");
        printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n",
               i, field->fld_name, DATA_TYPE[field->data_type], field->size, field->offset, field->nested_str_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_CYAN "--------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}

void print_structure_db(struct_db_t *struct_db)
{
    struct_db_rec_t *current = struct_db->head;

    while (current != NULL)
    {
        printf("----------\n");
        print_structure_rec(current);
        current = current->next;
    }
}

struct_db_rec_t *struct_db_look_up(struct_db_t *struct_db, char *struct_name)
{
    if (!struct_db)
        return NULL;
    struct_db_rec_t *current = struct_db->head;

    while (current != NULL && strcmp(current->struct_name, struct_name))
    {
        current = current->next;
    }
    return current; // ko pozove nek proveri da li je null ili ne :D
}
