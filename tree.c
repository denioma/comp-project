#include "structs.h"
#include <stdlib.h>

// Only useful for debug printing, remove later!
#include <stdio.h> 

prog_node* new_prog(dec_node* declarations) {
    prog_node* pn = (prog_node*)malloc(sizeof(prog_node));
    pn->dlist = declarations;
    return pn;
}

dec_node* alloc_node() {
    dec_node* node = (dec_node*)malloc(sizeof(dec_node));
    node->next = NULL;
    return node;
}

dec_node* insert_var_dec(dec_node* head, var_dec* var) {
    if (!head) {
        head = alloc_node();
        head->type = d_var;
        head->dec.var = var;
        head->next = NULL;
    } else {
        dec_node* tmp = head;
        for (; tmp->next; tmp = tmp->next);
        tmp->next = alloc_node();
        tmp->next->type = d_var;
        tmp->next->dec.var = var;
    }
    return head;
}

var_dec* create_var(char* id, v_type typespec) {
    var_dec* var = (var_dec*)malloc(sizeof(var_dec));
    var->typespec = typespec;
    var->id = id;

    return var;
}

dec_node* insert_func_dec(dec_node* head, func_dec* func) {
    if (!head) {
        head = alloc_node();
        head->type = d_func;
        head->dec.func = func;
        head->next = NULL;
    } else {
        dec_node* tmp = head;
        for (; tmp->next; tmp = tmp->next);
        tmp->next = alloc_node();
        tmp->next->type = d_func;
        tmp->next->dec.func = func;
    }
    return head;
}

func_body* create_body_var(var_dec* var) {
    func_body* body = (func_body*)malloc(sizeof(func_body));
    body->type = b_var;
    body->dec.var = var;
    body->next = NULL;

    return body;
}

func_body* insert_to_body(func_body* node, func_body* chain) {
    if (!node) return chain;
    if (chain) node->next = chain;
    else node->next = NULL;

    return node;
}

param_dec* create_param(char* id, v_type typespec, param_dec* chain) {
    param_dec* param = (param_dec*)malloc(sizeof(param_dec));
    param->id = id;
    param->typespec = typespec;
    if (chain) param->next = chain;
    else param->next = NULL;
    
    return param;
}

func_header* create_func_header(char* id, v_type typespec, param_dec* param_list) {
    func_header* header = (func_header*)malloc(sizeof(func_header));
    header->id = id;
    header->typespec = typespec;
    if (param_list) header->param = param_list;
    return header;
}

func_dec* create_func(char* id, v_type typespec, param_dec* param_list, func_body* body) {
    func_dec* func = (func_dec*)malloc(sizeof(func_dec));
    func->f_header = create_func_header(id, typespec, param_list);
    func->f_body = body;
    return func;
}

/* ------ Pretty printer ------ */

int spacing;

void space(char* str) {
    for (int i = 0; i < spacing; i++) printf("..");
    if (str) printf("%s", str);
}

void print_type(v_type type) {
    switch (type) {
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
    default:
        break;
    }
}

void print_var(const var_dec* node) {
    space("VarDecl\n");
    spacing++;
    space(NULL);
    print_type(node->typespec);
    space(NULL);
    printf("Id(%s)\n", node->id);
}

void print_body(const func_body* body) {
    spacing++;
    while (body) {
        switch(body->type) {
            case b_var:
                print_var(body->dec.var);
                break;
            case b_stmt:
                break;
            default:
                break;
        }
        body = body->next;
    }
}

void print_func(const func_dec* node) {
    func_header* header = node->f_header;
    param_dec* param = header->param;
    func_body* body = node->f_body;
    space("FuncDecl\n");
    spacing++;
    space("FuncHeader\n");
    spacing++;
    space(NULL);
    printf("Id(%s)\n", header->id);
    if (header->typespec != v_void) {
        space(NULL);
        print_type(header->typespec);
    }
    if (param) { 
        space("FuncParams\n");
        spacing++;
        while(param) {
            space("ParamDecl\n");
            spacing++;
            space(NULL);
            print_type(param->typespec);
            space(NULL);
            printf("Id(%s)\n", param->id);
            param = param->next;
            spacing--;
        }
        spacing--;
    }
    spacing--;
    if (body) {
        space("FuncBody\n");
        print_body(body);
    }
}

void print_ast(const prog_node* head) {
    printf("Program\n");
    dec_node* d_list = head->dlist;
    while (d_list) {
        spacing = 1;
        switch (d_list->type) {
        case d_var:
            print_var(d_list->dec.var);
            break;
        case d_func:
            print_func(d_list->dec.func);
            break;
        default:
            return;
        }
        d_list = d_list->next;
    }
}
