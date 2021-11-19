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

dec_node* insert_var_dec_list(dec_node* head, dec_node* list) {
    if (!head) {
        head = list;
    }     else {
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

var_dec* create_var(char* id, const v_type typespec) {
    var_dec* var = (var_dec*)malloc(sizeof(var_dec));
    var->typespec = typespec;
    var->id = id;

    return var;
}

dec_node* set_id_reps_head(dec_node* head, char* id, v_type typespec) {
    var_dec* var = create_var(id, typespec);
    dec_node* n_head = insert_var_dec(NULL, var);

    for (dec_node* tmp = head; tmp; tmp = tmp->next) {
        tmp->dec.var->typespec = typespec;
    }

    n_head->next = head;
    head = n_head;
    return head;
}

dec_node* save_id_reps(dec_node* head, char* id) {
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

    for (dec_node* tmp = head; tmp; tmp = tmp->next) {
        body->type = b_var;
        body->dec.var = tmp->dec.var;
        if (tmp->next) {
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

stmt_dec* create_pargs(char* id, expr* index) {
    parse_args* args = (parse_args*)malloc(sizeof(parse_args));
    args->id = id;
    // args->index = index;
    stmt_dec* stmt = create_stmt(s_parse);
    stmt->dec.d_args = args;

    return stmt;
}

stmt_dec* create_print(char* strlit, expr* expression) {
    print_stmt* print = (print_stmt*)malloc(sizeof(print_stmt));
    print->strlit = strlit;
    print->expression = expression;

    stmt_dec* stmt = create_stmt(s_print);
    stmt->dec.d_print = print;

    return stmt;
}

stmt_dec* create_assign(char* id, expr* expression) {
    assign_stmt* assign = (assign_stmt*)malloc(sizeof(assign_stmt));
    assign->id = id;
    assign->expression = expression;

    stmt_dec* stmt = create_stmt(s_assign);
    stmt->dec.d_assign = assign;

    return stmt;
}

stmt_dec* create_return(expr* expression) {
    stmt_dec* stmt = create_stmt(s_return);
    stmt->dec.d_expr = expression;
    return stmt;
}

stmt_dec* create_call(func_invoc* call) {
    stmt_dec* stmt = create_stmt(s_call);
    stmt->dec.d_fi = call;

    return stmt;
}

expr* create_expr(e_type type, op operator, void* arg1, expr* arg2) {
    expr* expression = (expr*)malloc(sizeof(expr));
    expression->type = type;
    expression->operator = operator;
    expression->arg2 = arg2;

    switch (type) {
    case e_expr:
        expression->arg1.exp_1 = (expr*)arg1;
        break;
    case e_func:
        expression->operator = nop;
        expression->arg1.call = (func_invoc*)arg1;
        break;
    case e_int:
    case e_real:
    case e_id:
        expression->operator = nop;
        expression->arg1.token = (char*)arg1;
        break;
    }
    return expression;
}

stmt_dec* create_stmt_block(stmt_block* block) {
    stmt_dec* stmt = create_stmt(s_block);
    stmt->type = s_block;
    stmt->dec.d_block = block;

    return stmt;
}

stmt_dec* create_stmt_block_nullable(stmt_block* block) {
    if (!block) return NULL;
    return create_stmt_block(block);
}

stmt_dec* create_for(expr* expression, stmt_block* block) {
    for_stmt* d_for = (for_stmt*)malloc(sizeof(for_stmt));
    d_for->condition = expression;
    d_for->block = create_stmt_block(block);
    stmt_dec* stmt = create_stmt(s_for);
    stmt->dec.d_for = d_for;

    return stmt;
}

stmt_dec* create_if(expr* condition, stmt_block* block1, stmt_block* block2) {
    if_stmt* d_if = (if_stmt*)malloc(sizeof(if_stmt));

    d_if->condition = condition;
    d_if->block1 = create_stmt_block(block1);
    d_if->block2 = create_stmt_block(block2);

    stmt_dec* stmt = create_stmt(s_if);
    stmt->dec.d_if = d_if;

    return stmt;
}

stmt_block* add_block_stmt(stmt_block* block, stmt_dec* stmt) {
    stmt_block* new_block = (stmt_block*)malloc(sizeof(stmt_block));
    new_block->stmt = stmt;
    new_block->next = NULL;
    if (!block) return new_block;
    stmt_block* last = block;
    for (; last->next; last = last->next);
    last->next = new_block;
    return block;
}

stmt_block* create_block(stmt_block* chain, stmt_dec* stmt) {
    stmt_block* block = (stmt_block*)malloc(sizeof(stmt_block));
    block->stmt = stmt;
    if (chain) block->next = chain;
    else block->next = NULL;   
    return block;
}

stmt_block* block_or_null(stmt_block* chain, stmt_dec* stmt) {
    if (!stmt) {
        if (chain) return chain;
        else return NULL;
    }
    return create_block(chain, stmt);
}

f_invoc_opts* create_fi_opts(f_invoc_opts* chain, expr* expression) {
    f_invoc_opts* fi_opts = (f_invoc_opts*)malloc(sizeof(f_invoc_opts));
    fi_opts->opt = expression;
    fi_opts->next = chain;
    return fi_opts;
}

func_invoc* create_func_invocation(char* id, f_invoc_opts* opts) {
    func_invoc* fi = (func_invoc*)malloc(sizeof(func_invoc));
    fi->id = id;
    fi->opts = opts;
    return fi;
}

/* ------ Pretty printers ------ */

int spacing = 0;

void space(const char* str) {
    for (int i = 0; i < spacing; i++) printf("..");
    if (str) printf("%s", str);
}

void printer_type(v_type type) {
    const char* types[4] = { "Int\n", "Float32\n", "String\n", "Bool\n" };
    space(types[type]);
}

void printer_var(const var_dec* node) {
    if (!node) return;
    space("VarDecl\n");
    spacing++;
    printer_type(node->typespec);
    space(NULL);
    printf("Id(%s)\n", node->id);
    spacing--;
}

void printer_expr(const expr*);

void printer_fi_opts(const f_invoc_opts* node) {
    if (!node) return;
    printer_expr(node->opt);
    printer_fi_opts(node->next);
}

void printer_fi(const func_invoc* node) {
    space("Call\n");
    spacing++;
    space(NULL);
    printf("Id(%s)\n", node->id);
    printer_fi_opts(node->opts);
    spacing--;
}

void printer_op(const expr* node) {
    if (!node) return;
    const char* type[] = {
        "Add\n", "And\n", "Call\n", "Div\n",
        "Eq\n", "Ge\n", "Gt\n", "Le\n", "Lt\n",
        "Minus\n", "Mod\n", "Mul\n", "Ne\n",
        "Not\n", "Or\n", "Plus\n", "Sub\n"
    };
    space(type[node->operator]);
    spacing++;
    printer_expr(node->arg1.exp_1);
    printer_expr(node->arg2);
    spacing--;
}

void printer_expr(const expr* node) {
    if (!node) return;
    switch (node->type) {
    case e_int:
        space(NULL);
        printf("IntLit(%s)\n", node->arg1.token);
        break;
    case e_real:
        space(NULL);
        printf("RealLit(%s)\n", node->arg1.token);
        break;
    case e_id:
        space(NULL);
        printf("Id(%s)\n", node->arg1.token);
        break;
    case e_expr:
        printer_op(node);
        break;
    case e_func:
        printer_fi(node->arg1.call);
        break;
    }
}

void printer_stmt(const stmt_dec*);

void printer_block(const stmt_block* block) {
    if (!block) return;
    printer_stmt(block->stmt);
    printer_block(block->next);
}

void printer_if(const if_stmt* stmt) {
    if (!stmt) return;
    space("If\n");
    spacing++;
    printer_expr(stmt->condition);
    spacing--;
    if (stmt->block1) {
        spacing++;
        printer_stmt(stmt->block1);
        spacing--;
    }
    if (stmt->block2) {
        spacing++;
        printer_stmt(stmt->block2);
        spacing--;
    }
}

void printer_for(const for_stmt* node) {
    if (!node) return;
    space("For\n");
    spacing++;
    printer_expr(node->condition);
    spacing--;
    if (node->block) {
        spacing++;
        printer_stmt(node->block);
        spacing--;
    }
    // spacing--;
}

void printer_assign(const assign_stmt* stmt) {
    if (!stmt) return;
    space("Assign\n");
    spacing++;
    space(NULL);
    printf("Id(%s)\n", stmt->id);
    printer_expr(stmt->expression);
    spacing--;
}

void printer_stmt(const stmt_dec* stmt) {
    if (!stmt) return;
    switch (stmt->type) {
    case s_assign:
        printer_assign(stmt->dec.d_assign);
        break;
    case s_if:
        printer_if(stmt->dec.d_if);
        break;
    case s_for:
        printer_for(stmt->dec.d_for);        
        break;
    case s_return:
        space("Return\n");
        spacing++;
        if (stmt->dec.d_expr) printer_expr(stmt->dec.d_expr);
        spacing--;
        break;
    case s_call:
        printer_fi(stmt->dec.d_fi);
        break;
    case s_print:
        space("Print\n");
        spacing++;
        space(NULL);
        if (stmt->dec.d_print->strlit)
            printf("StrLit(%s)\n", stmt->dec.d_print->strlit);
        else printer_expr(stmt->dec.d_print->expression);
        spacing--;
        break;
    case s_parse:
        space("ParseArgs\n");
        spacing++;
        space(NULL);
        printf("Id(%s)\n", stmt->dec.d_args->id);
        spacing--;
        break;
    case s_block:
        space("Block\n");
        spacing++;
        printer_block(stmt->dec.d_block);
        spacing--;
        break;
    default:
        break;
    }
}

void print_body(const func_body* body) {
    if (!body) return;
    spacing++;
    while (body) {
        switch (body->type) {
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
    if (!node) return;
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
        while (param) {
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
    if (!head) return;
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
