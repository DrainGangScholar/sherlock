#ifndef SHERLOCK_H
#define SHERLOCK_H

#include <assert.h>
#include <string.h>

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef enum
{
    UINT8,
    UINT32,
    INT32,
    CHAR,
    FLOAT,
    DOUBLE,
    OBJ_PTR,
    OBJ_STRUCT
} data_type_t;

// kad se cast-uje nullptr u struct_name*, onda pokazuje na pocetak
// tog struct-a u memoriji.

#define OFFSETOF(struct_name, fld_name) \
    (unsigned long)&(((struct_name *)NULL)->fld_name)
// pristupim fld_name, reference-ujem ga sa & i onda ga cast-ujem u unsigned long
// tako dobijem offset fld_name-a unutar te strukture
#define FIELD_SIZE(struct_name, fld_name) \
    sizeof(((struct_name *)NULL)->fld_name)
// velicina fld_name-a u bajtovima :DDD

// Informacije vezane za jedno polje u strukturi
typedef struct _field_info_
{
    char fld_name[MAX_FIELD_NAME_SIZE]; // Ime polja
    data_type_t data_type;              // Tip podatka
    unsigned int size;                  // Velicina polja
    unsigned int offset;                // Pomeraj u odnosu na pocetak strukture
    // Sluzi samo za OBJ_PTR i OBJ_STRUCT, ime ugnjezdene strukture
    char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

// Informacije vezane za jednu strukturu
typedef struct _struct_db_rec_ struct_db_rec_t;
struct _struct_db_rec_
{
    struct_db_rec_t *next;                     // Pokazivac na sledecu strukturu u LL
    char struct_name[MAX_STRUCTURE_NAME_SIZE]; // Ime strukture
    unsigned int ds_size;                      // Velicina strukture
    unsigned int n_fields;                     // Broj polja u strukturi
    field_info_t *fields;                      // Pokazivac na niz polja
};

// Klasicna lancana lista, nema sta da se prica... :D
typedef struct _struct_db_
{
    struct_db_rec_t *head;
    unsigned int count;
} struct_db_t;

// Jedan unos u "bazi"
void print_structure_rec(struct_db_rec_t *struct_rec);

// Cela "baza"
void print_structure_db(struct_db_t *struct_db);

// 0 je dobro, -1 je zapitaj se
int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec);

// Mapiranje polja za unos u
#define FIELD_INFO(struct_name, fld_name, dtype, nested_struct_name) \
    {                                                                \
        #fld_name, dtype, FIELD_SIZE(struct_name, fld_name),         \
            OFFSETOF(struct_name, fld_name), #nested_struct_name     \
    }

#define REG_STRUCT(struct_db, st_name, fields_arr)                    \
    do                                                                \
    {                                                                 \
        struct_db_rec_t *rec = calloc(1, sizeof(struct_db_rec_t));    \
        strncpy(rec->struct_name, #st_name, MAX_STRUCTURE_NAME_SIZE); \
        rec->ds_size = sizeof(st_name);                               \
        rec->n_fields = sizeof(fields_arr) / sizeof(field_info_t);    \
        rec->fields = fields_arr;                                     \
        if (add_structure_to_struct_db(struct_db, rec))               \
        {                                                             \
            assert(0);                                                \
        }                                                             \
    } while (0);

#endif // SHERLOCK_H
