#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum { t_int, t_float32, t_bool, t_string, t_void, t_undef } t_type;

typedef struct _f_params {
    t_type type;
    struct _f_params* next;
} f_params;

typedef struct _symtab {
    char* id;
    int line, col;
    t_type type;
    char is_func;
    f_params* params;
    char is_param;
    char is_return;
    char used;
    struct _symtab* next;
} symtab;


/* ------ SYMBOL TABLE CREATION ------ */
symtab* insert_el(symtab** tab, char* id, int line, int col, t_type type, char is_func, f_params* params, char is_param, char is_return);
symtab* search_el(symtab* tab, char* id);
void show_table(symtab* tab);

/* ------ SYMBOL TABLE DESTRUCTION ------ */
void destroy_table(symtab* tab);
void destroy_f_params(f_params* param);

#endif