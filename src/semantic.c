#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "semantic.h"

extern void print_sym_type(t_type);
t_type check_expr(symtab* globaltab, symtab* functab, expr* expression);
int check_statement(symtab** globaltab, symtab** functab, stmt_dec* stmt);

int errors = 0;

void no_symbol(int line, int col, char* id, char is_func, f_invoc_opts* params) {
    errors++;
    if (!is_func) {
        printf("Line %d, column %d: Cannot find symbol %s\n", line, col, id);
        return;
    }

    f_invoc_opts* aux = params;
    const char types[6][8] = {"int", "float32", "bool", "string", "none", "undef"};
    t_type type;
    printf("Line %d, column %d: Cannot find symbol %s(", line, col, id);
    for (;aux;aux = aux->next) {
        if (aux != params) printf(",");
        type = aux->opt->annotation;
        printf("%s", types[type]);
    }
    printf(")\n");

}

void op_type(int line, int col, char* op, t_type type1) {
    errors++;
    printf("Line %d, column %d: Operator %s cannot be applied to type ",
        line, col, op);
    print_sym_type(type1);
    printf("\n");
}

void op_types2(int line, int col, char* op, t_type type1, t_type type2) {
    errors++;
    printf("Line %d, column %d: Operator %s cannot be applied to types ",
        line, col, op);
    print_sym_type(type1);
    printf(", ");
    print_sym_type(type2);
    printf("\n");
}

t_type convert_v_type(v_type type) {
    switch (type) {
    case v_int:
        return t_int;
        break;
    case v_float:
        return t_float32;
        break;
    case v_bool:
        return t_bool;
        break;
    case v_string:
        return t_string;
        break;
    case v_void:
        return t_void;
        break;
    }
    return t_undef;
}

int check_bool_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_bool, 0, 0, 0, 0)) {
        var->type = t_bool;
        return 0;
    }

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_float_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_float32, 0, 0, 0, 0)) {
        var->type = t_float32;
        return 0;
    }

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_int_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_int, 0, 0, 0, 0)) {
        var->type = t_int;
        return 0;
    }

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_string_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_string, 0, 0, 0, 0)) {
        var->type = t_string;
        return 0;
    }

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}


int check_var(symtab** tab, var_dec* var) {
    switch (var->typespec) {
    case v_bool:
        return check_bool_var(tab, var);
    case v_float:
        return check_float_var(tab, var);
    case v_int:
        return check_int_var(tab, var);
    case v_string:
        return check_string_var(tab, var);
    case v_void:
    default:
        return 0;
    }
}

t_type convert_e_type(e_type type) {
    if (type == e_int)
        return t_int;
    else if (type == e_real)
        return t_float32;
    return t_undef;
}

int check_call_params(symtab* global, symtab* func, f_params* params, f_invoc_opts* call_opts) {
    f_invoc_opts* caux = call_opts;
    for (;caux;caux = caux->next) {
        caux->opt->annotation = check_expr(global, func, caux->opt);
    }

    if (!params) {
        if (!call_opts) return 1;
        else return 0;
    }

    if (!call_opts) return 0;

    int ret = 1;
    f_params* aux = params;
    caux = call_opts;
    for (;aux;aux = aux->next) {
        if (!caux) return 0;
        if (caux->opt->annotation != aux->type) ret = 0;
        caux = caux->next;
    }

    return ret;
}

int check_call(symtab* global, symtab* func, func_invoc* call);

t_type check_expr(symtab* globaltab, symtab* functab, expr* expression) {
    if (!expression) return t_undef;
    token* tkn = expression->tkn;
    symtab* symbol;
    func_invoc* call;
    char* id;
    t_type type1, type2;
    int check;
    switch (expression->type) {
        case e_int:
            expression->annotation = t_int;
            return t_int;
        case e_real:
            expression->annotation = t_float32;
            return t_float32;
        case e_expr:
            switch (expression->operator) {
            case op_not:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                expression->annotation = t_bool;
                if (type1 != t_bool)
                    op_type(tkn->line, tkn->col, tkn->value, type1);
                return t_bool;
            case op_eq:
            case op_ne:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                expression->annotation = t_bool;
                if (type1 != type2)
                    op_types2(tkn->line, tkn->col, tkn->value, type1, type2);
                return t_bool;
            case op_ge:
            case op_gt:
            case op_le:
            case op_lt:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                expression->annotation = t_bool;
                if (type1 != type2 || type1 == t_bool)
                    op_types2(tkn->line, tkn->col, tkn->value, type1, type2);
                return t_bool;
            case op_and:
            case op_or:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                expression->annotation = t_bool;
                if (type1 != t_bool || type2 != t_bool)
                    op_types2(tkn->line, tkn->col, tkn->value, type1, type2);
                return t_bool;
            case op_add:
            case op_sub:
            case op_div:
            case op_mul:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                if (type1 == type2 && type1 != t_bool && type1 != t_undef && type1 != t_void) {
                    expression->annotation = type1;
                    return type1;
                }
                expression->annotation = t_undef;
                op_types2(tkn->line, tkn->col, tkn->value, type1, type2);
                return t_undef;
            case op_mod:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                if (type1 == type2 && type1 == t_int)  {
                    expression->annotation = t_int;
                    return type1;
                }
                op_types2(tkn->line, tkn->col, tkn->value, type1, type2);
                expression->annotation = t_undef;
                return t_undef;
            case op_minus:
            case op_plus:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                if (type1 == t_bool || type1 == t_void || type1 == t_string) {
                    op_type(tkn->line, tkn->col, tkn->value, type1);
                    expression->annotation = t_undef;
                    return t_undef;
                }
                expression->annotation = type1;
                return type1;
            case nop:
                break;
            }
            break;
        case e_func:
            call = expression->arg1.call;
            symbol = search_el(globaltab, call->tkn->value);
            if (symbol) {
                expression->annotation = symbol->type;
                call->annotation = symbol->type;
                call->params = symbol->params;
                check = check_call_params(globaltab, functab, symbol->params, call->opts);
                type1 = symbol->type;
            } else check = check_call_params(globaltab, functab, NULL, call->opts);
            if (symbol && check) return type1;
            no_symbol(call->tkn->line, call->tkn->col, call->tkn->value, 1, call->opts);
            return t_undef;
            break;
        case e_id:
            id = expression->tkn->value;
            symbol = search_el(functab, id);
            if (!symbol) symbol = search_el(globaltab, id);
            if (symbol && !symbol->is_func) {
                expression->annotation = symbol->type;
                return symbol->type;
            }
            no_symbol(tkn->line, tkn->col, tkn->value, 0, 0);
            expression->annotation = t_undef;
            return t_undef;
        default:
            return convert_e_type(expression->type);
    }
    return t_undef;
}

int check_call(symtab* global, symtab* func, func_invoc* call) {
    char* id = call->tkn->value;
    symtab* symbol = search_el(global, id);
    int check;
    if (symbol) {
        call->annotation = symbol->type;
        call->params = symbol->params;
        check = check_call_params(global, func, symbol->params, call->opts);
    } else check_call_params(global, func, NULL, call->opts);
    if (symbol && check) return 0;
    no_symbol(call->tkn->line, call->tkn->col, id, 1, call->opts);
    return 1;
}

int check_return(symtab* globaltab, symtab* functab, stmt_dec* stmt) {
    expr* expression = stmt->dec.d_expr;
    t_type type;
    if (expression) type = check_expr(globaltab, functab, expression);
    else type = t_void;
    if (type == functab->type) return 0;


    printf("Line %d, column %d: Incompatible type ",
        stmt->tkn->line, stmt->tkn->col);
    print_sym_type(type);
    printf(" in return statement\n");
    return 1;
}

int check_for(symtab* globaltab, symtab* functab, for_stmt* stmt) {
    int errors = 0;
    if (stmt->condition) {
        t_type type = check_expr(globaltab, functab, stmt->condition);
        if (type == t_undef && stmt->condition->type == e_func) {
            token* tkn = stmt->condition->tkn;
            func_invoc* call = stmt->condition->arg1.call;
            no_symbol(tkn->line, tkn->col, tkn->value, 1, call->opts);
            errors++;
        } else if (type != t_bool) {
            printf("Line %d, column %d: Incompatible type ",
                stmt->condition->tkn->line, stmt->condition->tkn->col);
            print_sym_type(type);
            printf(" in for statement\n");
            errors++;
        }
    }
    if (stmt->block)
        errors += check_statement(&globaltab, &functab, stmt->block);
    return errors;
}

int check_if(symtab* globaltab, symtab* functab, if_stmt* stmt) {
    int errors = 0;
    if (stmt->condition) {
        t_type type = check_expr(globaltab, functab, stmt->condition);
        if (type == t_undef && stmt->condition->type == e_func) {
            token* tkn = stmt->condition->tkn;
            no_symbol(tkn->line, tkn->col, tkn->value, 0, 0);
            errors++;
        } else if (type != t_bool) {
            printf("Line %d, column %d: Incompatible type ",
                stmt->condition->tkn->line, stmt->condition->tkn->col);
            print_sym_type(type);
            printf(" in if statement\n");
            errors++;
        }
    }
    if (stmt->block1)
        errors += check_statement(&globaltab, &functab, stmt->block1);
    if (stmt->block2)
        errors += check_statement(&globaltab, &functab, stmt->block2);
    return errors;
}

int check_print(symtab* globaltab, symtab* functab, print_stmt* stmt) {
    if (!stmt->expression) return 0;
    t_type type = check_expr(globaltab, functab, stmt->expression);
    if (type == t_undef) {
        if (stmt->expression->type == e_func) {
            func_invoc* call = stmt->expression->arg1.call;
            printf("Line %d, column %d: Incompatible type undef in %s statement\n",
                call->tkn->line, call->tkn->col, stmt->tkn->value);
        } else if (stmt->expression->tkn) {
            printf("Line %d, column %d: Incompatible type undef in %s statement\n",
                stmt->expression->tkn->line, stmt->expression->tkn->col, stmt->tkn->value);
        } else {
            printf("Line %d, column %d: Incompatible type undef in %s statement\n",
                    stmt->tkn->line, stmt->tkn->col, stmt->tkn->value);
        }
        return 1;
    }
    return 0;
}

int check_parse(symtab* globaltab, symtab* functab, parse_args* stmt) {
    int error = 0;
    symtab* symbol = search_el(functab, stmt->var->value);
    if (!symbol) symbol = search_el(globaltab, stmt->var->value);
    if (!symbol) {
        no_symbol(stmt->var->line, stmt->var->col, stmt->var->value, 0, 0);
        return 1;
    }
    if (symbol->type != t_int) error = 1;
    t_type expr_type = check_expr(globaltab, functab, stmt->index);
    if (expr_type != t_int) error = 1;

    if (error) {
        token* tkn = stmt->tkn;
        op_types2(tkn->line, tkn->col, tkn->value, symbol->type, expr_type);
        return 1;
    }

    stmt->type = symbol->type;
    return 0;
}

int check_assign(symtab* globaltab, symtab* functab, assign_stmt* stmt) {
    t_type expr_type = check_expr(globaltab, functab, stmt->expression);
    symtab* symbol = search_el(functab, stmt->var->value);
    if (!symbol) symbol = search_el(globaltab, stmt->var->value);
    if (!symbol || symbol->is_func) {
        stmt->type = t_undef;
        no_symbol(stmt->var->line, stmt->var->col, stmt->var->value, 0, 0);
        op_types2(stmt->tkn->line, stmt->tkn->col, stmt->tkn->value, t_undef, expr_type);
        return 1;
    }
    if (symbol->type != expr_type) {
        op_types2(stmt->tkn->line, stmt->tkn->col, stmt->tkn->value, symbol->type, expr_type);
        return 1;
    }

    stmt->type = symbol->type;
    return 0;
}

int check_statement(symtab** globaltab, symtab** functab, stmt_dec* stmt) {
    int errors = 0;
    stmt_block* aux;
    switch (stmt->type) {
    case s_assign:
        return check_assign(*globaltab, *functab, stmt->dec.d_assign);
        break;
    case s_block:
        aux = stmt->dec.d_block;
        for (; aux; aux = aux->next)
            errors += check_statement(globaltab, functab, aux->stmt);
        break;
    case s_call:
        return check_call(*globaltab, *functab, stmt->dec.d_fi);
        break;
    case s_for:
        return check_for(*globaltab, *functab, stmt->dec.d_for);
        break;
    case s_if:
        return check_if(*globaltab, *functab, stmt->dec.d_if);
        break;
    case s_parse:
        return check_parse(*globaltab, *functab, stmt->dec.d_args);
        break;
    case s_print:
        return check_print(*globaltab, *functab, stmt->dec.d_print);
        break;
    case s_return:
        return check_return(*globaltab, *functab, stmt);
        break;
    }
    return errors;
}

void check_unused(symtab* table) {
    symtab* aux = table;
    for (; aux; aux = aux->next) {
        if (aux->is_return || aux->is_param) continue;
        if (!aux->used)
            printf("Line %d, column %d: Symbol %s declared but never used\n",
                aux->line, aux->col, aux->id);
    }
}

int check_func_body(symtab** globaltab, symtab** functab, func_body* body) {
    if (!body) return 0;
    int error = 0;
    func_body* aux = body;
    for (; aux; aux = aux->next) {
        switch(aux->type) {
        case b_var:
            error = check_var(functab, aux->dec.var);
            break;
        case b_stmt:
            error = check_statement(globaltab, functab, aux->dec.stmt);
            break;
        }
    }
    return error;
}

f_params* check_params(symtab** funtab, param_dec* params) {
    if (!params) return NULL;
    f_params* fparams = (f_params*)malloc(sizeof(f_params));
    fparams->type = convert_v_type(params->typespec);
    fparams->next = NULL;
    insert_el(funtab, params->tkn->value, params->tkn->line, params->tkn->col, fparams->type, 0, 0, 1, 0);

    f_params* faux = fparams;
    param_dec* aux = params->next;
    symtab* new_param;
    for (; aux; aux = aux->next) {
        faux->next = (f_params*)malloc(sizeof(f_params));
        faux = faux->next;
        faux->type = convert_v_type(aux->typespec);
        faux->next = NULL;
        new_param = insert_el(funtab, aux->tkn->value, aux->tkn->line, aux->tkn->col, faux->type, 0, 0, 1, 0);
        if (!new_param) {
            printf("Line %d, column %d: Symbol %s already defined\n",
                aux->tkn->line, aux->tkn->col, aux->tkn->value);
            return NULL;
        }
    }

    return fparams;
}

int check_func(symtab** tab, func_dec* func) {
    symtab** localtab = &func->localsym;
    return check_func_body(tab, localtab, func->f_body);
}

int define_func(symtab** tab, func_dec* func) {
    symtab** localtab = &func->localsym;
    token* tkn = func->f_header->tkn;
    t_type type = convert_v_type(func->f_header->typespec);
    // insert return type in local symbol table
    insert_el(localtab, 0, 0, 0, type, 0, 0, 0, 1);
    f_params* params = check_params(localtab, func->f_header->param);
    if (func->f_header->param && !params) return 1;
    if (insert_el(tab, tkn->value, tkn->line, tkn->col, type, 1, params, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        tkn->line, tkn->col, tkn->value);

    free(func->localsym);
    func->localsym = NULL;
    return 1;
}

int semantic_check(symtab** tab, prog_node* program) {
    if (!program) return 1;

    /*
    ** b() declared after a() can still be reachable/called by a()
    ** That means all global definitions are put in the global symtab
    ** Before checking each function against the global symtab
    */

    dec_node* dec = program->dlist;
    for (; dec; dec = dec->next) {
        switch (dec->type) {
        case d_var:
            errors += check_var(tab, dec->dec.var);
            break;
        case d_func:
            // Define functions in global symtab
            errors += define_func(tab, dec->dec.func);
            break;
        }
    }

    dec = program->dlist;
    for (; dec; dec = dec->next) {
        if (dec->type == d_var || !dec->dec.func->localsym) continue;
        // Check function here
        errors += check_func(tab, dec->dec.func);
        check_unused(dec->dec.func->localsym);
    }

    return errors;
}

void print_v_type(v_type type) { print_sym_type(convert_v_type(type)); }

void print_func_table(func_dec* fun) {
    func_header* header = fun->f_header;
    printf("\n===== Function %s(", header->tkn->value);
    // Print return types - parse params typespec
    param_dec* aux = header->param;
    for (; aux; aux = aux->next) {
        if (aux != header->param) printf(",");
        print_v_type(aux->typespec);
    }
    printf(") Symbol Table =====\n");
    show_table(fun->localsym);
}

void show_tables(symtab* global, prog_node* program) {
    printf("===== Global Symbol Table =====\n");
    show_table(global);
    if (!program->dlist) return;
    dec_node* aux = program->dlist;
    for (; aux; aux = aux->next) {
        if (aux->type == d_func) {
            print_func_table(aux->dec.func);
        }
    }
    puts("");
}
