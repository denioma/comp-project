#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "semantic.h"

extern void print_sym_type(t_type);
t_type check_expr(symtab* globaltab, symtab* functab, expr* expression);
int check_statement(symtab** globaltab, symtab** functab, stmt_dec* stmt);

void no_symbol(int line, int col, char* id, char is_func, f_invoc_opts* params, symtab* global, symtab* func) {
    if (!is_func) {
        printf("Line %d, column %d: Cannot find symbol %s\n", line, col, id);
        return;
    }

    // TODO
    f_invoc_opts* aux = params;
    const char types[6][8] = {"int", "float32", "bool", "string", "void", "undef"};
    char errorstr[1024];
    t_type type;
    snprintf(errorstr, sizeof errorstr, "Line %d, column %d: Cannot find symbol %s(", line, col, id);
    for (;aux;aux = aux->next) {
        if (aux != params) strcat(errorstr, ",");
        type = check_expr(global, func, aux->opt);
        strcat(errorstr, types[type]);
    }
    printf("%s)\n", errorstr);

}

void op_types2(int line, int col, char* op, t_type type1, t_type type2) {
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
}

int check_bool_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_bool, 0, 0, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_float_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_float32, 0, 0, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_int_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_int, 0, 0, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_string_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, var->tkn->line, var->tkn->col, t_string, 0, 0, 0, 0))
        return 0;

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
}

int check_call_params(symtab* global, symtab* func, f_params* params, f_invoc_opts* call_opts) {
    if (!params) {
        if (!call_opts) return 1;
        else return 0;
    }
    
    if (!call_opts) return 0;
    
    f_params* aux = params;
    f_invoc_opts* caux = call_opts;
    t_type expr_type;
    for (;aux;aux = aux->next) {
        if (!caux) return 0;
        expr_type = check_expr(global, func, caux->opt);
        if (expr_type != aux->type) return 0;
        caux = caux->next;
    }

    return 1;
}

t_type check_expr(symtab* globaltab, symtab* functab, expr* expression) {
    // TODO Finish all posible expression checking needed
    token* tkn = expression->tkn;
    symtab* symbol;
    func_invoc* call;
    char* id;
    t_type type1, type2;
    switch (expression->type) {
        case e_expr:
            switch (expression->operator) {
            case op_not:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                if (type1 == t_bool) return type1;
                return t_undef;
            case op_and:
            case op_or:
            case op_eq:
            case op_ge:
            case op_gt:
            case op_le:
            case op_lt:
            case op_ne:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                if (type1 == type2) return t_bool;
                return t_undef;
            case op_add:
            case op_sub:
            case op_div:
            case op_mul:
                type1 = check_expr(globaltab, functab, expression->arg1.exp_1);
                type2 = check_expr(globaltab, functab, expression->arg2);
                if (type1 == type2) return type1;
                op_types2(tkn->line, tkn->col, tkn->value, type1, type2);
                return t_undef;
            }
            break;         
        case e_func:
            // TODO Check function call params
            call = expression->arg1.call;
            symbol = search_el(functab, call->tkn->value);
            if (!symbol) symbol = search_el(globaltab, call->tkn->value);
            if (symbol && check_call_params(globaltab, functab, symbol->params, call->opts)) 
                return symbol->type;
            no_symbol(call->tkn->line, call->tkn->col, call->tkn->value, 1, call->opts, globaltab, functab);
            return t_undef;
            break;
        case e_id:
            id = expression->tkn->value;
            symbol = search_el(functab, id);
            if (!symbol) symbol = search_el(globaltab, id);
            if (symbol) return symbol->type;
            no_symbol(tkn->line, tkn->col, tkn->value, 0, 0, 0, 0);
            return t_undef;
        default:
            return convert_e_type(expression->type);    
    }
}

int check_call(symtab* global, symtab* func, func_invoc* call) {
    char* id = call->tkn->value;
    symtab* symbol = search_el(global, id);
    if (symbol && check_call_params(global, func, symbol->params, call->opts)) 
        return 0;
    no_symbol(call->tkn->line, call->tkn->col, id, 1, call->opts, global, func);
    return 1;
}

int check_return(symtab* globaltab, symtab* functab, expr* expression) {
    t_type type;
    if (expression) type = check_expr(globaltab, functab, expression);
    else type = t_void;
    if (type == functab->type) return 0;
    
    
    printf("Line %d, column %d: Incompatible type ", 
        expression->tkn->line, expression->tkn->col);
    print_sym_type(type);
    printf(" in return statement\n");
    return 1;
}

int check_for(symtab* globaltab, symtab* functab, for_stmt* stmt) {
    if (stmt->condition) {
        t_type type = check_expr(globaltab, functab, stmt->condition);
        if (type == t_undef && stmt->condition->type == e_func) {
            token* tkn = stmt->condition->tkn;
            func_invoc* call = stmt->condition->arg1.call;
            no_symbol(tkn->line, tkn->col, tkn->value, 1, call->opts, globaltab, functab);
        } else if (type != t_bool) {
            printf("Line %d, column %d: Incompatible type ", 
                stmt->condition->tkn->line, stmt->condition->tkn->col);
            print_sym_type(type);
            printf(" in for statement\n");
        }
    }
    if (stmt->block)
        return check_statement(&globaltab, &functab, stmt->block);
    return 0;
}

int check_if(symtab* globaltab, symtab* functab, if_stmt* stmt) {
    if (stmt->condition) {
        t_type type = check_expr(globaltab, functab, stmt->condition);
        if (type == t_undef && stmt->condition->type == e_func) {
            token* tkn = stmt->condition->tkn;
            no_symbol(tkn->line, tkn->col, tkn->value, 0, 0, 0, 0);
        } else if (type != t_bool) {
            printf("Line %d, column %d: Incompatible type ", 
                stmt->condition->tkn->line, stmt->condition->tkn->col);
            print_sym_type(type);
            printf(" in if statement\n");
        }
    }
    int errors = 0;
    if (stmt->block1)
        errors += check_statement(&globaltab, &functab, stmt->block1);
    if (stmt->block2) 
        errors += check_statement(&globaltab, &functab, stmt->block2);
    return errors;
}

int check_print(symtab* globaltab, symtab* functab, print_stmt* stmt) {
    if (!stmt->expression) return 0;
    t_type type = check_expr(globaltab, functab, stmt->expression);
    if (stmt->expression->type != e_func && type == t_undef) {
        printf("Line %d, column %d: Incompatible type undef in %s statement\n", 
                stmt->tkn->line, stmt->tkn->col, stmt->tkn->value);
        return 1;
    }
    return 0;
}

int check_parse(symtab* globaltab, symtab* functab, parse_args* stmt) {
    int error = 0;
    symtab* symbol = search_el(functab, stmt->var->value);
    if (!symbol) symbol = search_el(globaltab, stmt->tkn->value);
    if (!symbol) {
        no_symbol(stmt->var->line, stmt->var->col, stmt->var->value, 0, 0, 0, 0);
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

    return 0;
}

int check_assign(symtab* globaltab, symtab* functab, assign_stmt* stmt) {
    int error = 0;
    symtab* symbol = search_el(functab, stmt->var->value);
    if (!symbol) symbol = search_el(globaltab, stmt->var->value);
    if (!symbol) {
        no_symbol(stmt->var->line, stmt->var->col, stmt->var->value, 0, 0, 0, 0);
        return 1;
    }
    t_type expr_type = check_expr(globaltab, functab, stmt->expression);
    if (symbol->type != expr_type) {
        op_types2(stmt->tkn->line, stmt->tkn->col, stmt->tkn->value, symbol->type, expr_type);
        return 1;
    }

    return 0;
}

int check_statement(symtab** globaltab, symtab** functab, stmt_dec* stmt) {
    stmt_block* aux;
    switch (stmt->type) {
    case s_assign:
        return check_assign(*globaltab, *functab, stmt->dec.d_assign);
        break;
    case s_block:
        aux = stmt->dec.d_block;
        for (; aux; aux = aux->next)
            check_statement(globaltab, functab, aux->stmt);
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
        return check_return(*globaltab, *functab, stmt->dec.d_expr);
        break;   
    }
    return 0;
}

void check_unused(symtab* table) {
    symtab* aux = table;
    for (; aux; aux = aux->next) {
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
    for (; aux; aux = aux->next) {
        faux->next = (f_params*)malloc(sizeof(f_params));
        faux = faux->next;
        faux->type = convert_v_type(aux->typespec);
        faux->next = NULL;
        insert_el(funtab, aux->tkn->value, aux->tkn->line, aux->tkn->col, faux->type, 0, 0, 1, 0);
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
    if (insert_el(tab, tkn->value, tkn->line, tkn->col, type, 1, params, 0, 0))
        return 0;
    
    printf("Line %d, column %d: Symbol %s already defined\n",
        tkn->line, tkn->col, tkn->value);
    return 1;
}

int semantic_check(symtab** tab, prog_node* program) {
    if (!program) return 0;
    int errors = 0;

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
            define_func(tab, dec->dec.func);
            break;
        }
    }

    dec = program->dlist;
    for (; dec; dec = dec->next) {
        if (dec->type == d_var) continue;
        // Check function here
        errors += check_func(tab, dec->dec.func);
        /* TODO
        ** enable unused checks after checking all symbols
        ** otherwise it spits a million warnings about unused symbols
        */
        // check_unused(dec->dec.func->localsym);
    }
    // check_unused(*tab);

    return errors;
}

void print_v_type(v_type type) { print_sym_type(convert_v_type(type)); }

void print_func_table(symtab* global, func_dec* fun) {
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
            print_func_table(global, aux->dec.func);
        }

    }
}