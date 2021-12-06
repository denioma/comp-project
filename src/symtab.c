#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "structs.h"

// insert a new symbol in symbol table if it is not there yet
symtab* insert_el(symtab** tab, char* id, int line, int col, t_type type, char is_func, f_params* params, char is_param, char is_return) {
    // alocate space for new symbol
    symtab* newSym = (symtab*)malloc(sizeof(symtab));
    symtab* aux;
    symtab* prev;
    // populate symbol fields
    newSym->id = id;
    newSym->line = line;
    newSym->col = col;
    newSym->type = type;
    newSym->is_func = is_func;
    newSym->params = params;
    newSym->is_param = is_param;
    newSym->is_return = is_return;
    if (is_func) newSym->used = 1;
    else newSym->used = 0;
    newSym->next = NULL;
    
    if (*tab) {
        for (aux = *tab; aux; aux = aux->next) {
            if (!aux->is_return && strcmp(aux->id, id) == 0) {
                // if the symbol was already declared in tab, free space
                // free(newSym->tkn);
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


/*
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
    if (is_func) newSym->used = 1;
    else newSym->used = 0;
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
*/
// search for symbol in the given symbol table
symtab* search_el(symtab* tab, char* id) {
    symtab* aux;
    for (aux = tab; aux; aux = aux->next) {
        if (!aux->is_return && strcmp(aux->id, id) == 0) {
            aux->used = 1;
            return aux;
        }
    }
    return NULL;
}

const char types[6][8] = { "int", "float32", "bool", "string", "none", "undef" };
void print_sym_type(t_type type) {
    printf("%s", types[type]);
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

/* ------ SYMBOL TABLE DESTRUCTION ------ */

// destroy symbol table
void destroy_table(symtab* tab) {
    if (!tab) return;
    if (tab->next) destroy_table(tab->next);
    destroy_f_params(tab->params);
    free(tab);
}

// destroy function parameters type list
void destroy_f_params(f_params* param) {
    if (!param) return;
    if (param->next) destroy_f_params(param->next);
    free(param);
}