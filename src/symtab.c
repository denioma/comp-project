#include "symtab.h"
#include "string.h"

// insert a new symbol in symbol table if it is not there yet
symtab* insert_el(symtab** tab, char* id, t_type type, char is_func, f_params* params, char is_param){
    // alocate space for new symbol
    symtab* newSym = (symtab*)malloc(sizeof(symtab*));
    symtab* aux;
    symtab* prev;
    // populate symbol fields
    newSym->id = strdup(id);
    newSym->type = type;
    newSym->next = NULL;
    newSym->is_func = is_func;
    if (is_func){
        newSym->params = params;
    }
    newSym->is_param = is_param;

    if (*tab){
        for (aux = *tab; aux; aux = aux->next){
            if (strcmp(aux->id, id) == 0){
                // if the symbol was already declared in tab, free space
                free(newSym->id);
                if (is_func) //invoque destroy f_params function;
                free(newSym);
                return NULL;
            }
            // save reference to previous symbol
            prev = aux;
        }
        // if the symbol wasn't yet declared, added to the tail of tab
        prev->next = newSym;
    }
    else{
        // if the table doesn't exist, the new symbol will be the table head
        *tab = newSym;
    }

    return newSym;
}

// search for symbol in the given symbol table
symtab* search_el(symtab* tab, char* id){
    symtab* aux;
    for(aux = tab; aux; aux = aux->next){
        if (strcmp(aux->id, id) == 0){
            return aux;
        }
    }
    return NULL;
}