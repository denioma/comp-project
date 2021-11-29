#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum { t_int, t_float32, t_bool, t_string } type;

typedef struct _symtab {
    char* id;
    type type;
} symtab;

#endif