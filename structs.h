#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    char* id;
} func_dec;

typedef enum {v_int, v_float, v_bool, v_string} v_type;

typedef struct {
    v_type typespec;
    char* id;
} var_dec;

typedef enum { d_var, d_func } d_type;

typedef struct _dec_node {
    d_type type;
    union {
        func_dec func;
        var_dec var;
    } dec;
    struct _dec_node* next;
} dec_node;

typedef struct {
    dec_node* dlist;
} prog_node;

#endif