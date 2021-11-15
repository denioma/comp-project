#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

prog_node* new_prog(dec_node* declarations) {
    prog_node* pn = (prog_node*) malloc(sizeof(prog_node));
    pn->dlist = declarations;
    return pn;
}

dec_node* insert_dec(dec_node* head, dec_node* node) {
    if (head == NULL) return node;
    
    dec_node* tmp = head;
    for (; tmp->next; tmp = tmp->next);
    tmp->next = node;
    return head;
}

dec_node* insert_var(char* id, char* token) {
    dec_node* node = (dec_node*) malloc(sizeof(dec_node));

    node->type = d_var;
    node->dec.var->id = id;

    return node;
}

void print_var(const var_dec* node) {
    printf("VarDecl\n");
    switch (node->typespec) {
        case v_int: 
            printf("Int\n");
            break;
        case v_float:
            printf("Float32\n");
            break;
        case v_string:
            printf("String\n");
            break;
        case v_bool:
            printf("Bool\n");
            break;
    }
    printf("Id(%s)\n", node->id);
}

void print_ast(const prog_node* head) {
    printf("Program\n");
    dec_node* d_list = head->dlist;
    while (d_list) {
        switch(d_list->type) {
            case d_var:
                print_var(&d_list->dec.var);
                break;
            case d_func:
                break;
        }
    }
}
