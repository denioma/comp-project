#include "codegen.h"
#include "semantic.h"
#include <stdio.h>

char declare_puts = 0;
int tmp;

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
    f_invoc_opts* params = call->opts;
    symtab* function = search_el(tables.global, call->tkn->value);
    printf("\t");
    if (function->type != t_void) {
        printf("%%%d = ", tmp++);
    }
    printf("call %s @%s(", t_types[function->type], function->id);
    for (;params;params = params->next) {
        /* TODO cgen call args */
    }
    puts(")");
}

void cgen_expression(expr* expression) {
    /* TODO Finish expression code gen */
    switch (expression->type) {
    case e_expr:
        switch (expression->operator) {
        // Binary operators
        case op_add:
        case op_sub:
        case op_div:
        case op_mul:
            cgen_expression(expression->arg1.exp_1);
            cgen_expression(expression->arg2);
            break;
        // Unary operators
        case op_plus:
        case op_minus:
        case op_and:
        case op_eq:
        case op_ge:
        case op_gt:
        case op_le:
        case op_lt:
        case op_mod:
        case op_ne:
        case op_not:
        case op_or:
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
        printf("\t%%%d = add double %s, 0\n",
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

void cgen_stmt(stmt_dec* stmt) {
    stmt_block* block;
    switch (stmt->type) {
        case s_assign:
            cgen_assign(stmt->dec.d_assign);
            break;
        case s_block:
            block = stmt->dec.d_block;
            for (;block;block = block->next)
                cgen_stmt(block->stmt);
            break;
        case s_call:
            cgen_call(stmt->dec.d_expr);
            break;
        case s_for:
            // cgen_for();
            break;
        case s_if:
            // cgen_if();
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
        tmp += 1;
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