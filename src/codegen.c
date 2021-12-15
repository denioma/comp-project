#include "codegen.h"
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

char declare_puts = 0;
int tmp, if_cnt = 0;

typedef struct {
    int size;
    int capacity;
    int *data;
} int_vector;

void init(int_vector* vector) {
    vector->size = 0;
    vector->capacity = 10;
    vector->data = malloc(vector->capacity * sizeof(*vector->data));
}

void clean(int_vector* vector) {
    vector->size = 0;
    vector->capacity = 0;
    if (vector->data) free(vector->data);
    vector->data = 0;
}

void push(int_vector* vector, int value) {
    if (vector->size == vector->capacity) {
        vector->capacity += 10;
        vector->data = realloc(vector->data, vector->capacity * sizeof(*vector->data));
    }
    vector->data[vector->size++] = value;
}

int get(int_vector* vector, int index) {
    if (index >= vector->size || index < 0) return 0;
    return vector->data[index];
}

struct symtables {
    symtab *global;
    symtab *local;
} tables;

const char t_types[6][7] = {"i32", "double", "i1", "", "void", ""};

void cgen_load(const t_type type, char* id) {
    printf("\t%%%d = load %s, %s* ", tmp++, t_types[type], t_types[type]);
    if (search_el(tables.local, id))
        printf("%%%s\n", id);
    else
        printf("@%s\n", id);
}

void cgen_store(const t_type type, char* id) {
    printf("\tstore %s %%%d, %s* ", t_types[type], tmp-1, t_types[type]);
    if (search_el(tables.local, id))
        printf("%%%s\n", id);
    else
        printf("@%s\n", id);
}

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

void cgen_call(expr* expression) {
    func_invoc* call = expression->arg1.call;
    f_invoc_opts* opts = call->opts;
    f_params* params = call->params;
    symtab* function = search_el(tables.global, call->tkn->value);
    int_vector vector;
    init(&vector);
    for (;opts;opts = opts->next) {
        cgen_expression(opts->opt);
        push(&vector, tmp-1);
    }
    printf("\t");
    if (function->type != t_void) {
        printf("%%%d = ", tmp++);
    }
    printf("call %s @%s(", t_types[function->type], function->id);
    int i = 0;
    for (;params;params = params->next) {
        if (params != call->params) printf(", ");
        cgen_type(params->type);
        printf(" %%%d", get(&vector, i++));
    }
    puts(")");
    clean(&vector);
}

void cgen_expression(expr* expression) {
    /* TODO Finish expression code gen */
    int tmp1, tmp2;
    t_type type;
    switch (expression->type) {
    case e_expr:
        type = expression->arg1.exp_1->annotation;
        cgen_expression(expression->arg1.exp_1);
        tmp1 = tmp-1;
        if (expression->arg2) {
            cgen_expression(expression->arg2);
            tmp2 = tmp-1;
        }
        switch (expression->operator) {
        // Binary operators
        case op_add:
            printf("\t%%%d = ", tmp++);
            if (expression->annotation == t_float32)
                printf("fadd");
            else printf("add");
            printf(" %s %%%d, %%%d\n", t_types[expression->annotation], tmp1, tmp2);
            break;
        case op_sub:
            printf("\t%%%d = ", tmp++);
            if (expression->annotation == t_float32)
                printf("fsub");
            else printf("sub");
            printf(" %s %%%d, %%%d\n", t_types[expression->annotation], tmp1, tmp2);
            break;
        case op_div:
            printf("\t%%%d = ", tmp++);
            if (expression->annotation == t_float32)
                printf("fdiv");
            else printf("sdiv");
            printf(" %s %%%d, %%%d\n", t_types[expression->annotation], tmp1, tmp2);
            break;
        case op_mul:
            printf("\t%%%d = ", tmp++);
            if (expression->annotation == t_float32)
                printf("fmul");
            else printf("mul");
            printf(" %s %%%d, %%%d\n", t_types[expression->annotation], tmp1, tmp2);
            break;
        // Unary operators
        case op_plus:
            break;
        case op_minus:
            break;
        case op_and:
            break;
        case op_or:
            break;
        case op_eq:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp");
            else
                printf("icmp");
            printf(" eq %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_ne:
        printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp");
            else
                printf("icmp");
            printf(" ne %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_ge:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp");
            else
                printf("icmp");
            printf(" sge %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_gt:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp");
            else
                printf("icmp");
            printf(" sgt %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_le:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp");
            else
                printf("icmp");
            printf(" sle %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_lt:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp");
            else
                printf("icmp");
            printf(" slt %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_mod:
            printf("\t%%%d = srem %s %%%d, %%%d\n",
                tmp++, t_types[type], tmp1, tmp2);
            break;
        case op_not:
            printf("\t%%%d = sub %s 1, %%%d\n",
                tmp++, t_types[type], tmp1);
            break;
        case nop:
            break;
        }
        break;
    case e_func:
        cgen_call(expression);
        break;
    case e_id:
        cgen_load(expression->annotation, expression->tkn->value);
        break;
    case e_int:
        printf("\t%%%d = add i32 %s, 0\n",
            tmp++, expression->tkn->value);
        break;
    case e_real:
        printf("\t%%%d = fadd double %s, 0.0\n",
            tmp++, expression->tkn->value);
        break;
    }
}

void cgen_assign(assign_stmt* stmt) {
    cgen_expression(stmt->expression);
    cgen_store(stmt->type, stmt->var->value);
}

void cgen_return(expr* expression) {
    cgen_expression(expression);
    printf("\tret %s %%%d\n", t_types[tables.local->type], tmp-1);
}

void cgen_print() {
    declare_puts = 1;
    /* TODO figure out prints */
}

void cgen_if(if_stmt* stmt) {
    char is_else;
    if (stmt->block2->dec.d_block) is_else = 1;
    else is_else = 0;
    cgen_expression(stmt->condition);
    printf("\tbr i1 %%%d, label %%if%d, label ",
        tmp-1, ++if_cnt);
    if (!is_else)
        printf("%%fi%d\n", if_cnt);
    else 
        printf("%%else%d\n", if_cnt);
    printf("\tif%d:\n", if_cnt);
    cgen_stmt(stmt->block1);
    if (is_else) {
        printf("\tbr label %%fi%d\n", if_cnt);
        printf("\telse%d:\n", if_cnt);
        cgen_stmt(stmt->block2);
    }
    printf("\tfi%d:\n", if_cnt);
}

void cgen_block(stmt_block* block) {
    if (!block) return;
    stmt_block* aux = block;
    for (;aux;aux = aux->next)
        cgen_stmt(aux->stmt);
}

void cgen_stmt(stmt_dec* stmt) {
    stmt_block* block;
    switch (stmt->type) {
        case s_assign:
            cgen_assign(stmt->dec.d_assign);
            break;
        case s_block:
            cgen_block(stmt->dec.d_block);
            break;
        case s_call:
            cgen_call(stmt->dec.d_expr);
            break;
        case s_for:
            // cgen_for();
            break;
        case s_if:
            cgen_if(stmt->dec.d_if);
            break;
        case s_parse:
            // cgen_parse();
            break;
        case s_print:
            /* TODO not finished */
            cgen_print();
            break;
        case s_return:
            cgen_return(stmt->dec.d_expr);
            break;
        }
}

void cgen_alloca_params() {
    symtab* aux = tables.local->next;
    for (;aux && aux->is_param; aux = aux->next) {
        printf("\t%%%s = alloca %s", aux->id, t_types[aux->type]);
        puts("");
        printf("\tstore %s %%a.%s, %s* %%%s\n",
            t_types[aux->type], aux->id,
            t_types[aux->type], aux->id);
    }

}

void cgen_func_var(var_dec* var) {
    printf("\t%%%s = alloca %s\n",
        var->tkn->value, t_types[var->type]);
}

void cgen_func_body(func_body* body) {
    cgen_alloca_params();
    func_body *aux = body;
    for (;aux;aux = aux->next) {
        if (aux->type == b_var) cgen_func_var(aux->dec.var);
        else cgen_stmt(aux->dec.stmt);
    }
}

void cgen_func_params(param_dec* params) {
    param_dec* aux = params;
    for (;aux;aux = aux->next) {
        if (convert_v_type(aux->typespec) == t_string) {
            /* TODO Find out what to do with strings in params */
            continue;
        }
        if (aux != params) printf(", ");
        cgen_type(convert_v_type(aux->typespec));
        printf(" %%a.%s", aux->tkn->value);
    }
}

void cgen_func(func_dec* func) {
    tmp = 1;
    tables.local = func->localsym;
    printf("define ");
    cgen_type(func->localsym->type);
    printf(" @%s(", func->f_header->tkn->value);
    cgen_func_params(func->f_header->param);
    puts(") {");
    cgen_func_body(func->f_body);
    puts("}\n");
}

void cgen_global(var_dec* var) {
    if (var->typespec == v_string)
        /* TODO figure out what to do with strings */;
    else {
        printf("@%s = global ", var->tkn->value);
        cgen_type(var->type);
        puts(" 0\n");
    }
}

void codegen(prog_node* program, symtab* global) {
    tables.global = global;
    if (!program) return;
    dec_node* dec = program->dlist;
    for (;dec;dec = dec->next) {
        if (dec->type == d_var)
            cgen_global(dec->dec.var);
        else if (dec->type == d_func)
            cgen_func(dec->dec.func);
    }

    if (declare_puts)
        puts("declare i32 @puts(i8* nocapture) nounwind");
}