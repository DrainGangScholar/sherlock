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

static object_db_rec_t *object_db_look_up(object_db_t *obj_db, void *ptr){

    object_db_rec_t *head = obj_db->head;
    if(!head) 
        return NULL;
    while(head!=NULL)
    {
        if(head->ptr==ptr)
            return head;
        head=head->next;
    }
    return NULL;
}


void add_object_to_object_db(object_db_t* obj_db ,void* ptr,int units,struct_db_rec_t* struct_rec,sherlock_boolean_t flag)
{
    object_db_rec_t* obj_rec = object_db_look_up(obj_db, ptr);

    assert(!obj_rec);

    obj_rec = calloc(1, sizeof(object_db_rec_t));

    obj_rec->next = NULL;
    obj_rec->ptr = ptr;
    obj_rec->units = units;
    obj_rec->struct_rec = struct_rec;
    obj_rec->is_visited=SHERLOCK_FALSE;
    obj_rec->is_root=flag;

    if (!obj_db->head)
    {
        obj_db->head = obj_rec;
    }
    else
    {
        obj_rec->next = obj_db->head;
        obj_db->head = obj_rec;
    }

    obj_db->count++;
}

void print_object_rec(object_db_rec_t *obj_rec, int i)
{
    if(!obj_rec) return;
    printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s | is_root = %s |\n"ANSI_COLOR_RESET, 
            i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name, obj_rec->is_root ? "TRUE " : "FALSE"); 
    printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
}

void print_object_db(object_db_t *object_db)
{
    object_db_rec_t *head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN "Printing OBJECT DATABASE\n");
    for(; head; head = head->next){
        print_object_rec(head, i++);
    }
}

void* memlock(object_db_t* obj_db, char* struct_name, int units)
{
    struct_db_rec_t* struct_rec = struct_db_look_up(obj_db->struct_db,struct_name);
    void* ptr=calloc(units,struct_rec->ds_size);
    add_object_to_object_db(obj_db,ptr,units,struct_rec,SHERLOCK_FALSE);
    return ptr;
}

void register_root_object(object_db_t *obj_db,void* obj_ptr,char* struct_name,unsigned int units)
{
    struct_db_rec_t *struct_rec=struct_db_look_up(obj_db->struct_db,struct_name);
    assert(struct_rec);

    add_object_to_object_db(obj_db,obj_ptr,units,struct_rec,SHERLOCK_TRUE);
}

void set_object_as_global_root(object_db_t* obj_db,void* obj_ptr)
{
    object_db_rec_t *obj_rec =object_db_look_up(obj_db,obj_ptr);
    assert(obj_rec);

    obj_rec->is_root=SHERLOCK_TRUE;
}
