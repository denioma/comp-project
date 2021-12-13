#include "codegen.h"
#include "semantic.h"
#include <stdio.h>

char declare_puts = 0;

typedef struct {
    symtab *global, *local;
} symtables;

const char t_types[6][7] = {"i32", "double", "i1", "", "void", ""};

void cgen_type(t_type type) {
    switch (type) {
    case t_bool:
        printf("i1");
        break;
    case t_int:
        printf("i32");
        break;
    case t_float32:
        printf("double");
        break;
    case t_void:
        printf("void");
        break;
    default:
        break;
    }
}

void cgen_alloca_params(symtab* local) {
    symtab* aux = local->next;
    for (;aux && aux->is_param; aux = aux->next) {
        printf("\t%%%s = alloca %s", aux->id, t_types[aux->type]);
        puts("");
        printf("\tstore %s %%a.%s, %s* %%%s\n", 
            t_types[aux->type], aux->id, 
            t_types[aux->type], aux->id);
    }
    
}

void cgen_stmt(stmt_dec* stmt, int* tmp) {
    switch (stmt->type) {
        case s_assign:
            // cgen_assign();
            break;
        case s_block:
            // cgen_block();
            break;
        case s_call:
            // cgen_call();
            break;
        case s_for:
            // cgen_for();
            break;
        case s_if:
            // cgen_if();
            break;
        case s_parse:
            // cgen_parse();
            break;
        case s_print:
            // cgen_print();
            break;
        case s_return:
            // cgen_return();
            break;
        }
}

void cgen_func_body(func_body* body, symtables tables) {
    cgen_alloca_params(tables.local);
    int tmp = 1;
    func_body *aux = body;
    stmt_dec* stmt;
    for (;aux;aux = aux->next) {
        if (aux->type == b_var);
        else {
            stmt = aux->dec.stmt;
        };
    }
}

void cgen_func_params(param_dec* params) {
    param_dec* aux = params;
    for (;aux;aux = aux->next) {
        if (convert_v_type(aux->typespec) == t_string) continue; // TODO figure out what to do with strings in params
        if (aux != params) printf(", ");
        cgen_type(convert_v_type(aux->typespec));
        printf(" %%a.%s", aux->tkn->value);
    }
}

void cgen_func(func_dec* func, symtables tables) {
    tables.local = func->localsym;
    printf("define ");
    cgen_type(func->localsym->type);
    printf(" @%s(", func->f_header->tkn->value);
    cgen_func_params(func->f_header->param);
    puts(") {");
    cgen_func_body(func->f_body, tables);
    puts("}\n");
}

void cgen_var(var_dec* var) {
    if (var->typespec == v_string)
        /* TODO figure out what to do with strings */;
    else {
        printf("@%s = global ", var->tkn->value);
        cgen_type(var->type);
        puts(" 0");
    }
}

void codegen(prog_node* program, symtab* global) {
    symtables tables;
    tables.global = global;
    if (!program) return;
    dec_node* dec = program->dlist;
    for (;dec;dec = dec->next) {
        if (dec->type == d_var)
            cgen_var(dec->dec.var);
        else if (dec->type == d_func)
            cgen_func(dec->dec.func, tables);
    }
}