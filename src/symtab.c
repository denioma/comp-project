#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// insert a new symbol in symbol table if it is not there yet
symtab* insert_el(symtab** tab, char* id, t_type type, char is_func, f_params* params, char is_param, char is_return) {
    // alocate space for new symbol
    symtab* newSym = (symtab*)malloc(sizeof(symtab));
    symtab* aux;
    symtab* prev;
    // populate symbol fields
    if (id) newSym->id = strdup(id);
    else newSym->id = NULL;
    newSym->type = type;
    newSym->is_func = is_func;
    newSym->params = params;
    newSym->is_param = is_param;
    newSym->is_return = is_return;
    newSym->next = NULL;
    
    if (*tab) {
        for (aux = *tab; aux; aux = aux->next) {
            if (!aux->is_return && strcmp(aux->id, id) == 0) {
                // if the symbol was already declared in tab, free space
                free(newSym->id);
                if (is_func) {} //invoque destroy f_params function;
                free(newSym);
                newSym = 0;
                return NULL;
            }
            // save reference to previous symbol
            prev = aux;
        }
        // if the symbol wasn't yet declared, added to the tail of tab
        prev->next = newSym;
    } else {
        // if the table doesn't exist, the new symbol will be the table head
        *tab = newSym;
    }
    return newSym;
}

// search for symbol in the given symbol table
symtab* search_el(symtab* tab, char* id) {
    symtab* aux;
    for (aux = tab; aux; aux = aux->next) {
        if (strcmp(aux->id, id) == 0) {
            return aux;
        }
    }
    return NULL;
}

void print_sym_type(t_type type) {
    switch (type) {
    case t_int:
        printf("int");
        break;
    case t_float32:
        printf("float32");
        break;
    case t_bool:
        printf("bool");
        break;
    case t_string:
        printf("string");
        break;
    case t_void:
        printf("none");
        break;
    }
}

void print_params(f_params* params) {
    printf("(");
    if (params) {
        for (f_params* aux = params; aux; aux = aux->next) {
            if (aux != params) printf(",");
            print_sym_type(aux->type);
        }
    }
    printf(")");
}

void show_table(symtab* tab) {
    if (!tab) return;

    for (symtab* aux = tab; aux; aux = aux->next) {
        if (aux->is_return) {
            printf("return\t\t");
            print_sym_type(aux->type);
            printf("\n");
            continue;
        }
        printf("%s\t", aux->id);
        if (aux->is_func) {
            print_params(aux->params);
        }
        printf("\t");
        print_sym_type(aux->type);
        if (aux->is_param) printf("\tparam");
        printf("\n");
    }
}