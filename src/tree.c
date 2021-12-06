#include "structs.h"
#include <stdlib.h>
#include <string.h>

// Only useful for debug printing, remove later!
#include <stdio.h>

token* create_tkn(char* value, int line, int col) {
    token* tkn = (token*)malloc(sizeof(token));
    tkn->value = strdup(value);
    tkn->col = col;
    tkn->line = line;
    return tkn;
}

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
    } else {
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

var_dec* create_var(token* tkn, const v_type typespec) {
    var_dec* var = (var_dec*)malloc(sizeof(var_dec));
    var->typespec = typespec;
    var->tkn = tkn;

    return var;
}

dec_node* set_id_reps_head(dec_node* head, token* tkn, v_type typespec) {
    var_dec* var = create_var(tkn, typespec);
    dec_node* n_head = insert_var_dec(NULL, var);

    for (dec_node* tmp = head; tmp; tmp = tmp->next) {
        tmp->dec.var->typespec = typespec;
    }

    n_head->next = head;
    ;
    return n_head;
}

dec_node* save_id_reps(dec_node* head, token* tkn) {
    var_dec* var = (var_dec*)malloc(sizeof(var_dec));
    var->tkn = tkn;
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

param_dec* create_param(token* tkn, v_type typespec, param_dec* chain) {
    param_dec* param = (param_dec*)malloc(sizeof(param_dec));
    param->tkn = tkn;
    param->typespec = typespec;
    if (chain) param->next = chain;
    else param->next = NULL;

    return param;
}

func_header* create_func_header(token* tkn, v_type typespec, param_dec* param_list) {
    func_header* header = (func_header*)malloc(sizeof(func_header));
    header->tkn = tkn;
    header->typespec = typespec;
    header->param = NULL;
    if (param_list) header->param = param_list;

    return header;
}

func_dec* create_func(token* tkn, v_type typespec, param_dec* param_list, func_body* body) {
    func_dec* func = (func_dec*)malloc(sizeof(func_dec));
    func->f_header = create_func_header(tkn, typespec, param_list);
    func->f_body = body;
    func->localsym = NULL;
    return func;
}

stmt_dec* create_stmt(s_type type) {
    stmt_dec* stmt = (stmt_dec*)malloc(sizeof(stmt_dec));
    stmt->type = type;

    return stmt;
}

stmt_dec* create_pargs(token* tkn, token* var, expr* index) {
    parse_args* args = (parse_args*)malloc(sizeof(parse_args));
    args->tkn = tkn;
    args->var = var;
    args->index = index;
    stmt_dec* stmt = create_stmt(s_parse);
    stmt->dec.d_args = args;

    return stmt;
}

stmt_dec* create_print(token* tkn, token* strlit, expr* expression) {
    print_stmt* print = (print_stmt*)malloc(sizeof(print_stmt));
    print->tkn = tkn;
    if (strlit) print->strlit = strlit->value;
    else print->strlit = NULL;
    print->expression = expression;

    stmt_dec* stmt = create_stmt(s_print);
    stmt->dec.d_print = print;

    return stmt;
}

stmt_dec* create_assign(token* tkn, token* var, expr* expression) {
    assign_stmt* assign = (assign_stmt*)malloc(sizeof(assign_stmt));
    assign->tkn = tkn;
    assign->var = var;
    assign->expression = expression;

    stmt_dec* stmt = create_stmt(s_assign);
    stmt->dec.d_assign = assign;

    return stmt;
}

stmt_dec* create_return(token* tkn, expr* expression) {
    stmt_dec* stmt = create_stmt(s_return);
    stmt->dec.d_expr = expression;
    stmt->tkn = tkn;
    return stmt;
}

stmt_dec* create_call(func_invoc* call) {
    stmt_dec* stmt = create_stmt(s_call);
    stmt->dec.d_fi = call;

    return stmt;
}

expr* create_expr(e_type type, op operator, token* tkn, void* arg1, expr* arg2) {
    expr* expression = (expr*)malloc(sizeof(expr));
    expression->type = type;
    expression->tkn = tkn;
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
        expression->tkn = (token*)arg1;
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
    else if (!block->next) return block->stmt;
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
    if (chain) fi_opts->next = chain;
    else fi_opts->next = NULL;
    return fi_opts;
}

func_invoc* create_func_invocation(token* tkn, f_invoc_opts* opts) {
    func_invoc* fi = (func_invoc*)malloc(sizeof(func_invoc));
    fi->tkn = tkn;
    fi->opts = opts;

    return fi;
}

/* ------ AST DESTRUCTION ------ */

// destroy token
void destroy_tkn(token* tkn) {
    if (!tkn) return;
    free(tkn->value);
    free(tkn);
}

// recursivly destroys declarations, then program root node
void destroy(prog_node* program) {
    if (program->dlist) {
        destroy_dec(program->dlist);
        // free(program);
    }
}

// destroy declaration list
void destroy_dec(dec_node* node) {
    if (!node) return;
    // recursivly destroy list from tail to head
    if (node->next) destroy_dec(node->next);
    switch (node->type) {
    case d_func:
        // destroy function declaration
        destroy_func(node->dec.func);
        break;
    case d_var:
        // destroy variable declaration
        break;
    }
    // free(node);
}

// destroy function declaration
void destroy_func(func_dec* node) {
    if (!node) return;
    destroy_func_header(node->f_header);
    destroy_func_body(node->f_body);
    // destroy table
    // free(node);
}

// destroy function parameter list, then function header
void destroy_func_header(func_header* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    destroy_func_params(node->param);
    // free(node);
}

// destroy function body declarations, then the node itself
void destroy_func_body(func_body* node) {
    if (!node) return;
    if (node->next) destroy_func_body(node->next);
    switch (node->type) {
    case b_var:
        // destroy variable declaration
        destroy_var_dec(node->dec.var);
        break;
    case b_stmt:
        // destroy statement declaration
        destroy_stmt_dec(node->dec.stmt);
        break;
    }
    // free(node);
}

// destroy function parameters, then the node itself
void destroy_func_params(param_dec* node) {
    if (!node) return;
    if (node->next) destroy_params(node->next);
    destroy_tkn(node->tkn);
    // free(node);
}

// destroy variable declaration
void destroy_var_dec(var_dec* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    // free(node);
}

// destroy statement declaration
void destroy_stmt_dec(stmt_dec* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    switch (node->type) {
    case s_assign:
        destroy_assign_smt(node->dec.d_assign);
        break;
    case s_print:
        destroy_print_stmt(node->dec.d_print);
        break;
    case s_parse:
        destroy_parse_args(node->dec.d_args);
        break;
    case s_if:
        destroy_if_stmt(node->dec.d_if);
        break;
    case s_for:
        destroy_for_stmt(node->dec.d_for);
        break;
    case s_block:
        destroy_stmt_block(node->dec.d_block);
        break;
    case s_call:
        destroy_func_invoc(node->dec.d_fi);
        break;
    case s_return:
        destroy_expr(node->dec.d_expr);
        break;
    }
    // free(node);
}

// destroy assignment statement
void destroy_assign_stmt(assign_stmt* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    destroy_tkn(node->var);
    destroy_expr(node->expression);
    // free(node);
}

// destroy print statement
void destroy_print_stmt(print_stmt* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    // destroy string literal ???
    if (node->expression) destroy_expr(node->expression);
    // free(node);
}

// destroy argument parsing statement
void destroy_parse_args(parse_args* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    destroy_tkn(node->var);
    destroy_expr(node->index);
    // free(node);
}

// destroy if statement
void destroy_if_stmt(if_stmt* node) {
    if (!node) return;
    destroy_expr(node->condition);
    destroy_stmt_dec(node->block1);
    destroy_stmt_dec(node->block2);
    // free(node);
}

// destroy for statement
void destroy_for_stmt(for_stmt* node) {
    if (!node) return;
    destroy_expr(node->condition);
    destroy_stmt_dec(node->block);
    // free(node);
}

void destroy_stmt_block(stmt_block* node) {
    if (!node) return;
    if (node->next) destroy_stmt_block(node->next);
    destroy_stmt_dec(node->stmt);
    // free(node);
}
void destroy_func_invoc(func_invoc* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    // destroy f_params
    // destroy f_invoc_opts
    // free(node);
}

// destroy expression node
void destroy_expr(expr* node) {
    if (!node) return;
    destroy_tkn(node->tkn);
    // destroy expression operands
    if (node->type == e_func)
        destroy_func_invoc(node->arg1.call);
    else 
        destroy_expr(node->arg1.exp_1);
    destroy_expr(node->arg2);
    // free(node);
}

/* ------ Pretty printers ------ */

int spacing = 0;

extern void print_sym_type(t_type);

void space(const char* str) {
    for (int i = 0; i < spacing; i++) printf("..");
    if (str) printf("%s", str);
}

void printer_type(v_type type) {
    const char* types[4] = { "Int\n", "Float32\n", "Bool\n", "String\n" };
    space(types[type]);
}

void printer_var(const var_dec* node) {
    if (!node) return;
    space("VarDecl\n");
    spacing++;
    printer_type(node->typespec);
    space(NULL);
    printf("Id(%s)\n", node->tkn->value);
    spacing--;
}

void printer_expr(const expr*, const char annotations);

void printer_fi_opts(const f_invoc_opts* node, const char annotations) {
    if (!node) return;
    printer_expr(node->opt, annotations);
    printer_fi_opts(node->next, annotations);
}

void printer_call(const func_invoc* node, const char annotations) {
    space("Call");
    if (annotations && node->annotation != t_void) {
        printf(" - ");
        print_sym_type(node->annotation);
    }
    printf("\n");
    spacing++;
    space(NULL);
    printf("Id(%s)", node->tkn->value);
    if (annotations) {
        printf(" - (");
        f_params* aux = node->params;
        for (;aux;aux = aux->next) {
            if (aux != node->params) printf(",");
            print_sym_type(aux->type);
        }
        printf(")");
    }
    printf("\n");
    printer_fi_opts(node->opts, annotations);
    spacing--;
}

void printer_op(const expr* node, const char annotations) {
    if (!node) return;
    const char* type[] = {
        "Add", "And", "Call", "Div",
        "Eq", "Ge", "Gt", "Le", "Lt",
        "Minus", "Mod", "Mul", "Ne",
        "Not", "Or", "Plus", "Sub"
    };
    space(type[node->operator]);
    if (annotations) {
        printf(" - ");
        print_sym_type(node->annotation);
    }
    printf("\n");
    spacing++;
    printer_expr(node->arg1.exp_1, annotations);
    printer_expr(node->arg2, annotations);
    spacing--;
}

void printer_expr(const expr* node, const char annotations) {
    if (!node) return;
    switch (node->type) {
    case e_int:
        space(NULL);
        printf("IntLit(%s)", node->tkn->value);
        if (annotations) {
            printf(" - ");
            print_sym_type(node->annotation);
        }
        printf("\n");
        break;
    case e_real:
        space(NULL);
        printf("RealLit(%s)", node->tkn->value);
        if (annotations) {
            printf(" - ");
            print_sym_type(node->annotation);
        }
        printf("\n");
        break;
    case e_id:
        space(NULL);
        printf("Id(%s)", node->tkn->value);
        if (annotations) {
            printf(" - ");
            print_sym_type(node->annotation);
        }
        printf("\n");
        break;
    case e_expr:
        printer_op(node, annotations);
        break;
    case e_func:
        printer_call(node->arg1.call, annotations);
        break;
    }
}

void printer_stmt(const stmt_dec*, const char annotations);

void printer_block(const stmt_block* block, const char annotations) {
    if (!block) return;
    printer_stmt(block->stmt, annotations);
    printer_block(block->next, annotations);
}

void printer_if(const if_stmt* stmt, const char annotations) {
    if (!stmt) return;
    space("If\n");
    spacing++;
    printer_expr(stmt->condition, annotations);
    spacing--;
    if (stmt->block1) {
        spacing++;
        printer_stmt(stmt->block1, annotations);
        spacing--;
    }
    if (stmt->block2) {
        spacing++;
        printer_stmt(stmt->block2, annotations);
        spacing--;
    }
}

void printer_for(const for_stmt* node, const char annotations) {
    if (!node) return;
    space("For\n");
    spacing++;
    printer_expr(node->condition, annotations);
    spacing--;
    if (node->block) {
        spacing++;
        printer_stmt(node->block, annotations);
        spacing--;
    }
    // spacing--;
}

void printer_assign(const assign_stmt* stmt, const char annotations) {
    if (!stmt) return;
    space("Assign");
    if (annotations) {
        printf(" - ");
        print_sym_type(stmt->type);
    }
    puts("");
    spacing++;
    space(NULL);
    printf("Id(%s)", stmt->var->value);
    if (annotations) {
        printf(" - ");
        print_sym_type(stmt->type);
    }
    puts("");
    printer_expr(stmt->expression, annotations);
    spacing--;
}

void printer_parse(const parse_args* node, const char annotations) {
    space("ParseArgs");
    if (annotations) {
        printf(" - ");
        print_sym_type(node->type);
    }
    puts("");
    spacing++;
    space(NULL);
    printf("Id(%s)", node->var->value);
    if (annotations) {
        printf(" - ");
        print_sym_type(node->type);
    }
    puts("");
    printer_expr(node->index, annotations);
    spacing--;
}

void printer_return(const stmt_dec* stmt, const char annotations) {
    space("Return\n");
    spacing++;
    if (stmt->dec.d_expr) printer_expr(stmt->dec.d_expr, annotations);
    spacing--;
}

void printer_print(const print_stmt* node, const char annotations) {
    space("Print\n");
    spacing++;
    if (node->strlit) {
        space(NULL);
        printf("StrLit(%s)\n", node->strlit);
    }     else printer_expr(node->expression, annotations);
    spacing--;
}

void printer_stmt(const stmt_dec* stmt, const char annotations) {
    if (!stmt) return;
    switch (stmt->type) {
    case s_assign:
        printer_assign(stmt->dec.d_assign, annotations);
        break;
    case s_if:
        printer_if(stmt->dec.d_if, annotations);
        break;
    case s_for:
        printer_for(stmt->dec.d_for, annotations);
        break;
    case s_return:
        printer_return(stmt, annotations);
        break;
    case s_call:
        printer_call(stmt->dec.d_fi, annotations);
        break;
    case s_print:
        printer_print(stmt->dec.d_print, annotations);
        break;
    case s_parse:
        printer_parse(stmt->dec.d_args, annotations);
        break;
    case s_block:
        space("Block\n");
        spacing++;
        printer_block(stmt->dec.d_block, annotations);
        spacing--;
        break;
    default:
        break;
    }
}

void print_body(const func_body* body, const char annotations) {
    if (!body) return;
    spacing++;
    while (body) {
        switch (body->type) {
        case b_var:
            printer_var(body->dec.var);
            break;
        case b_stmt:
            printer_stmt(body->dec.stmt, annotations);
            break;
        default:
            break;
        }
        body = body->next;
    }
}

void printer_func(const func_dec* node, const char annotations) {
    if (!node) return;
    func_header* header = node->f_header;
    param_dec* param = header->param;
    func_body* body = node->f_body;
    space("FuncDecl\n");
    spacing++;
    space("FuncHeader\n");
    spacing++;
    space(NULL);
    printf("Id(%s)\n", header->tkn->value);
    if (header->typespec != v_void) printer_type(header->typespec);
    space("FuncParams\n");
    if (param) {
        spacing++;
        while (param) {
            space("ParamDecl\n");
            spacing++;
            printer_type(param->typespec);
            space(NULL);
            printf("Id(%s)\n", param->tkn->value);
            param = param->next;
            spacing--;
        }
        spacing--;
    }
    spacing--;
    space("FuncBody\n");
    if (body) print_body(body, annotations);
}

void print_tree(const prog_node* head, const char annotations) {
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
            printer_func(d_list->dec.func, annotations);
            break;
        default:
            return;
        }
        d_list = d_list->next;
    }
}

void print_ast(const prog_node* head) {
    print_tree(head, 0);
}

void print_annotations(const prog_node* head) {
    print_tree(head, 1);
}