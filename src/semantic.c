#include <stdio.h>
#include "structs.h"
#include "semantic.h"

t_type convert_v_type(v_type type) {
    switch (type) {
    case v_int:
        return t_int;
        break;
    case v_float:
        return t_float32;
        break;
    case v_bool:
        return t_bool;
        break;
    case v_string:
        return t_string;
        break;
    case v_void:
        return t_void; 
        break;
    }
}


int check_bool_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_bool, 0, 0, 0))
        return 0;

    printf("Line %d, col %d: Symbol %s already defined\n", 
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_float_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_float32, 0, 0, 0))
        return 0;

    printf("Line %d, col: %d: Symbol %s already defined\n", 
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_int_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_int, 0, 0, 0))
        return 0;

    printf("Line %d, col %d: Symbol %s already defined\n", 
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_string_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_string, 0, 0, 0))
        return 0;

    printf("Line %d, col %d: Symbol %s already defined\n", 
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}


int check_var(symtab** tab, var_dec* var) {
    switch(var->typespec) {
        case v_bool:
            return check_bool_var(tab, var);
        case v_float:
            return check_float_var(tab, var);
        case v_int:
            return check_int_var(tab, var);
        case v_string:
            return check_string_var(tab, var);
        case v_void:
        default:
            return 0;
    }
}

int check_func(symtab** tab, func_dec* func) {
    char* id = func->f_header->tkn->value;
    t_type type = convert_v_type(func->f_header->typespec);
    insert_el(tab, id, type, 1, 0, 0);
    return 0;
}

int semantic_check(symtab** tab, prog_node* program) {
    if (!program) return 0;
    int errors = 0;

    dec_node* dec = program->dlist;
    for (; dec; dec = dec->next) {
        switch (dec->type) {
        case d_var:
            errors += check_var(tab, dec->dec.var);
            break;
        case d_func:
            errors += check_func(tab, dec->dec.func);
            break;
        }
    }

    return errors;
}