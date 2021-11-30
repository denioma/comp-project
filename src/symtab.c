#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// insert a new symbol in symbol table if it is not there yet
symtab* insert_el(symtab** tab, char* id, t_type type, char is_func, f_params* params, char is_param) {
    // alocate space for new symbol
    symtab* newSym = (symtab*)malloc(sizeof(symtab));
    symtab* aux;
    symtab* prev;
    // populate symbol fields
    newSym->next = 0;
    newSym->id = strdup(id);
    newSym->type = type;
    newSym->is_func = is_func;
    if (is_func) {
        newSym->params = params;
    }
    newSym->is_param = is_param;
    if (*tab) {
        for (aux = *tab; aux; aux = aux->next) {
            if (strcmp(aux->id, id) == 0) {
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
        printf("void");
        break;
    }
}

void print_params(f_params* params) {
    printf("(");
    if (params) {
        for (f_params* aux = params; aux; aux = aux->next) {
            if (aux != params) printf(", ");
            print_sym_type(aux->type);
        }
    }
    printf(")\t");
}

void show_table(symtab* tab) {
    if (!tab) return;

    for (symtab* aux = tab; aux; aux = aux->next) {
        if (aux->is_func) {
            printf("func\t%s\t", aux->id);
            print_params(aux->params);
        }
        else printf("symbol\t%s\t", aux->id);
        print_sym_type(aux->type);
        printf("\n");
    }
}