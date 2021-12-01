#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "semantic.h"

extern void print_sym_type(t_type);
int check_statement(symtab** globaltab, symtab** functab, stmt_dec* stmt);

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
    if (insert_el(tab, var->tkn->value, t_bool, 0, 0, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_float_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_float32, 0, 0, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_int_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_int, 0, 0, 0, 0))
        return 0;

    printf("Line %d, column %d: Symbol %s already defined\n",
        var->tkn->line, var->tkn->col, var->tkn->value);
    return 1;
}

int check_string_var(symtab** tab, var_dec* var) {
    if (insert_el(tab, var->tkn->value, t_string, 0, 0, 0, 0))
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

t_type check_expr(symtab* globaltab, symtab* functab, expr* expression) {
    // TODO Finish all posible expression checking needed
    symtab* symbol;
    char* id;
    switch (expression->type) {
        case e_expr:
            switch (expression->operator) {
            case op_not:
                return check_expr(globaltab, functab, expression->arg1.exp_1);
            case op_and:
            case op_or:
            case op_eq:
            case op_ge:
            case op_gt:
            case op_le:
            case op_lt:
            case op_ne:
                return t_bool;
            }         
        case e_func:
            // TODO Check function call params
            symbol = search_el(globaltab, expression->arg1.call->tkn->value);
            if (symbol) return symbol->type;
            printf("Line %d, column %d: Cannot find symbol %s",
                expression->arg1.call->tkn->line, expression->arg1.call->tkn->col, expression->arg1.call->tkn->value);
            return t_undef;
            break;
        case e_id:
            // TODO pass some sort of error in case ID is not in symbol tables
            id = expression->tkn->value;
            symbol = search_el(functab, id);
            if (symbol) return symbol->type;
            symbol = search_el(globaltab, id);
            if (symbol) return symbol->type;
            return t_undef;
        default:
            return convert_e_type(expression->type);    
    }
}

int check_call_params(func_invoc* func, symtab* symbol) {
    f_params* paux = symbol->params;
    return 0;
}

int check_call(symtab** tab, func_invoc* call) {
    char* id = call->tkn->value;
    symtab* symbol = search_el(*tab, id);
    if (!symbol) printf("WIP: CANNOT FIND SYMBOL\n");
    if (!check_call_params(call, symbol)) return 0;
    printf("WIP: PARAMS MISMATCH\n");
    return 1;
}

int check_return(symtab* globaltab, symtab* functab, expr* expression) {
    t_type type = check_expr(globaltab, functab, expression);
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
        if (type == t_undef) {
            token* tkn = stmt->condition->tkn;
            printf("Line %d, column %d: Cannot find symbol %s\n", 
                tkn->line, tkn->col, tkn->value);
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
        if (type == t_undef) {
            printf("Cannot find symbol %s\n", stmt->condition->tkn->value);
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

int check_statement(symtab** globaltab, symtab** functab, stmt_dec* stmt) {
    stmt_block* aux;
    switch (stmt->type) {
    case s_assign:
        // return check_assign(tab, stmt->dec.d_assign);
        break;
    case s_block:
        aux = stmt->dec.d_block;
        for (; aux; aux = aux->next)
            check_statement(globaltab, functab, aux->stmt);
        break;
    case s_call:
        // return check_call(globaltab, stmt->dec.d_fi);
        break;
    case s_for:
        return check_for(*globaltab, *functab, stmt->dec.d_for);
        break;
    case s_if:
        return check_if(*globaltab, *functab, stmt->dec.d_if);
        break;
    case s_parse:
        // return check_parse(tab, stmt->dec.d_args);
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
    // TODO symtab struct needs to hold line and column for unused warnings
    symtab* aux = table;
    for (; aux; aux = aux->next) {
        int line, col;  // Placeholders for true line and col
        if (!aux->used)
            printf("Line %d, column %d: Symbol %s declared but never used\n",
                line, col, aux->id);
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
    // check_unused(*functab); // TODO uncomment when finished
    return error;
}

f_params* check_params(symtab** funtab, param_dec* params) {
    if (!params) return NULL;
    f_params* fparams = (f_params*)malloc(sizeof(f_params));
    fparams->type = convert_v_type(params->typespec);
    fparams->next = NULL;
    insert_el(funtab, params->tkn->value, fparams->type, 0, 0, 1, 0);

    f_params* faux = fparams;
    param_dec* aux = params->next;
    for (; aux; aux = aux->next) {
        faux->next = (f_params*)malloc(sizeof(f_params));
        faux = faux->next;
        faux->type = convert_v_type(aux->typespec);
        faux->next = NULL;
        insert_el(funtab, aux->tkn->value, faux->type, 0, 0, 1, 0);
    }

    return fparams;
}

int check_func(symtab** tab, func_dec* func) {
    symtab** localtab = &func->localsym;
    return check_func_body(tab, localtab, func->f_body);
}

int define_func(symtab** tab, func_dec* func) {
    symtab** localtab = &func->localsym;
    char* id = func->f_header->tkn->value;
    t_type type = convert_v_type(func->f_header->typespec);
    insert_el(localtab, 0, type, 0, 0, 0, 1);
    f_params* params = check_params(localtab, func->f_header->param);
    if (insert_el(tab, id, type, 1, params, 0, 0))
        return 0;
    
    token* tkn = func->f_header->tkn;
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
    }

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
        if (aux->type == d_func)
            print_func_table(global, aux->dec.func);
    }
}