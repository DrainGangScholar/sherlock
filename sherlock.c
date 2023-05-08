#include <sherlock.h>

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

// IMPLEMENTIRAJ :DDD
void print_structure_rec(struct_db_rec_t *struct_rec);
void print_structure_db(struct_db_t *struct_db);