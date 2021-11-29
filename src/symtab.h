#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum { t_int, t_float32, t_bool, t_string } type;

typedef struct _symtab {
    char* id;
    type type;
    struct _symtab* next;
} symtab;

symtab* insert_el(symtab* tab, char* id, type type);
symtab* search_el(symtab* tab, char* id);
void show_table(symtab* tab);

#endif