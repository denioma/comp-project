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

dec_node* insert_var_dec_list(dec_node* head, dec_node* list){
    if (!head) {
        head = list;
    }
    else {
        dec_node* tmp = head;
        for (; tmp->next; tmp = tmp->next);
        tmp->next = list;
    }
    return head;
}

dec_node* insert_var_dec(dec_node* head, var_dec* var) {
    if (!head) {
        head = alloc_node();
        head->type = d_var;
        head->dec.var = var;
        //head->next = NULL;
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

dec_node* set_id_reps_head(dec_node* head, char* id, v_type typespec){
    var_dec* var = create_var(id, typespec);
    dec_node* n_head = insert_var_dec(NULL, var);

    for (dec_node* tmp = head; tmp; tmp = tmp->next){
        tmp->dec.var->typespec = typespec;
    }

    n_head->next = head;
    head = n_head;
    return head;
}

dec_node* save_id_reps(dec_node* head, char* id){
    var_dec* var = (var_dec*)malloc(sizeof(var_dec));
    var->id = id;
    head = insert_var_dec(head, var);
    return head;
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

func_body* create_body_var(dec_node* head) {
    func_body* f_body_head = (func_body*)malloc(sizeof(func_body));
    func_body* body = f_body_head;

    for (dec_node* tmp = head; tmp; tmp = tmp->next){
        body->type = b_var;
        body->dec.var = tmp->dec.var;
        if(tmp->next) {
            body->next = (func_body*)malloc(sizeof(func_body));
            body = body->next;
        } else body->next = NULL;
    }

    return f_body_head;
}

func_body* create_body_stmt(stmt_dec* stmt) {
    func_body* body = (func_body*)malloc(sizeof(func_body));
    body->type = b_stmt;
    body->dec.stmt = stmt;
    body->next = NULL;

    return body;
}

func_body* insert_to_body(func_body* node, func_body* chain) {
    if (!node) return chain;
    if (!chain) return node;
    func_body* last = chain;
    for (; last->next; last = last->next);
    last->next = node;

    return chain;
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

stmt_dec* create_stmt(s_type type) {
    stmt_dec* stmt = (stmt_dec*)malloc(sizeof(stmt_dec));
    stmt->type = type;

    return stmt;
}

stmt_dec* create_pargs(char* id /*, int index*/) {
    parse_args* args = (parse_args*)malloc(sizeof(parse_args));
    args->id = id;
    // args->index = index;
    stmt_dec* stmt = create_stmt(s_parse);
    stmt->dec.args = args;

    return stmt;
}

stmt_dec* create_print(char* strlit, expr* expression) {
    print_stmt* print = (print_stmt*)malloc(sizeof(print_stmt));
    print->strlit = strlit;
    print->expression = expression;

    stmt_dec* stmt = create_stmt(s_print);
    stmt->dec.print = print;

    return stmt;
}

stmt_dec* create_assign(char* id, expr* expression) {
    assign_stmt* assign = (assign_stmt*)malloc(sizeof(assign_stmt));
    assign->id = id;
    assign->expression = expression;

    stmt_dec* stmt = create_stmt(s_assign);
    stmt->dec.assign = assign;

    return stmt;
}

/* ------ Pretty printers ------ */

int spacing = 0;

void space(char* str) {
    for (int i = 0; i < spacing; i++) printf("..");
    if (str) printf("%s", str);
}

void printer_type(v_type type) {
    switch (type) {
    case v_int:
        space("Int\n");
        break;
    case v_float:
        space("Float32\n");
        break;
    case v_string:
        space("String\n");
        break;
    case v_bool:
        space("Bool\n");
        break;
    default:
        break;
    }
}

void printer_var(const var_dec* node) {
    space("VarDecl\n");
    spacing++;
    printer_type(node->typespec);
    space(NULL);
    printf("Id(%s)\n", node->id);
    spacing--;
}

void printer_stmt(const stmt_dec* stmt) {
    switch(stmt->type) {
        case s_assign:
            space("Assign\n");
            spacing++;
            space(NULL);
            printf("Id(%s)\n", stmt->dec.assign->id);
            // TODO print exp
            spacing--;
            break;
        case s_if:
            space("If\n");
            break;
        case s_for:
            space("For\n");
            break;
        case s_return:
            space("Return\n");
            break;
        case s_call:
            space("Call\n");
            break;
        case s_print:
            space("Print\n");
            spacing++;
            space(NULL);
            if (stmt->dec.print->strlit)
                printf("Strlit(%s)\n", stmt->dec.print->strlit);
            // TODO else print expr
            spacing--;
            break;
        case s_parse:
            space("ParseArgs\n");
            spacing++;
            space(NULL);
            printf("Id(%s)\n", stmt->dec.args->id);
            spacing--;
            break;
        case s_block:
            space("Block\n");
            break;
        default:
            break;
    }
}

void print_body(const func_body* body) {
    spacing++;
    while (body) {
        switch(body->type) {
            case b_var:
                printer_var(body->dec.var);
                break;
            case b_stmt:
                printer_stmt(body->dec.stmt);
                break;
            default:
                break;
        }
        body = body->next;
    }
}

void printer_func(const func_dec* node) {
    func_header* header = node->f_header;
    param_dec* param = header->param;
    func_body* body = node->f_body;
    space("FuncDecl\n");
    spacing++;
    space("FuncHeader\n");
    spacing++;
    space(NULL);
    printf("Id(%s)\n", header->id);
    if (header->typespec != v_void) printer_type(header->typespec);
    space("FuncParams\n");
    if (param) { 
        spacing++;
        while(param) {
            space("ParamDecl\n");
            spacing++;
            printer_type(param->typespec);
            space(NULL);
            printf("Id(%s)\n", param->id);
            param = param->next;
            spacing--;
        }
        spacing--;
    }
    spacing--;
    space("FuncBody\n");
    if (body) print_body(body);
}

void print_ast(const prog_node* head) {
    printf("Program\n");
    dec_node* d_list = head->dlist;
    while (d_list) {
        spacing = 1;
        switch (d_list->type) {
        case d_var:
            printer_var(d_list->dec.var);
            break;
        case d_func:
            printer_func(d_list->dec.func);
            break;
        default:
            return;
        }
        d_list = d_list->next;
    }
}
