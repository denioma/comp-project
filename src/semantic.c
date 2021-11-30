#include <stdio.h>
#include <stdlib.h>
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
    if (insert_el(tab, var->tkn->value, t_bool, 0, 0, 0, 0))
        return 0;

    printf("Line %d, col %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_float_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_float32, 0, 0, 0, 0))
        return 0;

    printf("Line %d, col: %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_int_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_int, 0, 0, 0, 0))
        return 0;

    printf("Line %d, col %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_string_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_string, 0, 0, 0, 0))
        return 0;

    printf("Line %d, col %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}


int check_var(symtab** tab, var_dec* var) {
    switch (var->typespec) {
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

int check_func_body(symtab** functab, func_body* body) {
    if (!body) return 0;
    int error = 0;
    func_body* aux = body;
    for (; aux; aux = aux->next) {
        switch(aux->type) {
        case b_var:
            error = check_var(functab, aux->dec.var);
            break;
        case b_stmt:
            break;
        }
    }
    return error;
}

f_params* check_params(symtab** funtab, param_dec* params) {
    if (!params) return NULL;
    f_params* fparams = (f_params*)malloc(sizeof(f_params));
    fparams->type = convert_v_type(params->typespec);
    fparams->next = NULL;
    insert_el(funtab, params->tkn->value, fparams->type, 0, 0, 1, 0);

    f_params* faux = fparams;
    param_dec* aux = params->next;
    for (; aux; aux = aux->next) {
        faux->next = (f_params*)malloc(sizeof(f_params));
        faux = faux->next;
        faux->type = convert_v_type(aux->typespec);
        faux->next = NULL;
        insert_el(funtab, aux->tkn->value, faux->type, 0, 0, 1, 0);
    }

    return fparams;
}

int check_func(symtab** tab, func_dec* func) {
    int error = 0;
    symtab** localtab = &func->localsym;
    char* id = func->f_header->tkn->value;
    t_type type = convert_v_type(func->f_header->typespec);
    insert_el(localtab, 0, type, 0, 0, 0, 1);
    f_params* params = check_params(localtab, func->f_header->param);
    error = check_func_body(localtab, func->f_body);
    if (insert_el(tab, id, type, 1, params, 0, 0))
        return error;
    else error += 1;
    token* tkn = func->f_header->tkn;
    printf("Line %d, col %d: Symbol %s already defined\n",
        tkn->line, tkn->col, tkn->value);
    return error;
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

extern void print_sym_type(t_type);

void print_v_type(v_type type) { print_sym_type(convert_v_type(type)); }

void print_func_table(symtab* global, func_dec* fun) {
    func_header* header = fun->f_header;
    printf("\n===== Function %s(", header->tkn->value);
    // Print return types - parse params typespec
    param_dec* aux = header->param;
    for (; aux; aux = aux->next) {
        if (aux != header->param) printf(",");
        print_v_type(aux->typespec);
    }
    printf(") Symbol Table =====\n");
    show_table(fun->localsym);
}

void show_tables(symtab* global, prog_node* program) {
    printf("===== Global Symbol Table =====\n");
    show_table(global);
    if (!program->dlist) return;
    dec_node* aux = program->dlist;
    for (; aux; aux = aux->next) {
        if (aux->type == d_func)
            print_func_table(global, aux->dec.func);
    }
}