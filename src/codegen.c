#include "codegen.h"
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char fstr[] = "@.str = private unnamed_addr constant [4 x i8] c\"%s\\0A\\00\"";
const char fint[] = "@.int = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\"";
const char fdouble[] = "@.double = private unnamed_addr constant [7 x i8] c\"%.08f\\0A\\00\"";
const char ftrue[] = "@.true = private unnamed_addr constant [5 x i8] c\"true\\00\"";
const char ffalse[] = "@.false = private unnamed_addr constant [6 x i8] c\"false\\00\"";

char ret = 0, declare_prints = 0, declare_atoi = 0;
int tmp, if_cnt = 0, for_cnt = 0, bool_cnt = 0, str_count = 0;

typedef struct {
    int size;
    int capacity;
    int *data;
} int_vector;

typedef struct {
    char *string;
    int size;
} str1;

typedef struct {
    int size;
    int capacity;
    str1* data;
} str_vector;

str_vector strings;

int strsize(char* str) {
    int size = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        size++;
        if (str[i] == '\\') {
            if (str[i + 1] == '\\' || str[i + 1] == '9')
                i++;
            else
                i += 2;
        }
    }

    // Size + Null terminator
    return size+1;
}

char* strshift(char* str, int from, int* size) {
    *size = *size + 1;
    char* new_str = malloc(*size);
    strcpy(new_str, str);
    for (int i = *size; i > from; i--) {
        *(new_str+i) = *(new_str+i-1);
    }
    free(str);
    return new_str;
}

// Catches \" and replaces with \22
void strclean(char** str, int size) {
    (*str)[size-1] = '\0';
    char c;
    for (int i = 0; i < size; i++) {
        if (*(*str+i) == '\\') {
            size = strlen(*str);
            c = *(*str+i+1);
            switch (c) {
                case '\\':
                    *str = strshift(*str, i+1, &size);
                    *(*str+i+1) = '5';
                    *(*str+i+2) = 'C';
                    break;
                case '"':
                    *str = strshift(*str, i+1, &size);
                    *(*str+i+1) = '2';
                    *(*str+i+2) = '2';
                    break;
                case 'f':
                    *str = strshift(*str, i+1, &size);
                    *(*str+i+1) = '0';
                    *(*str+i+2) = 'C';
                    break;
                case 'n':
                    *str = strshift(*str, i+1, &size);
                    *(*str+i+1) = '0';
                    *(*str+i+2) = 'A';
                    break;
                case 'r':
                    *str = strshift(*str, i+1, &size);
                    *(*str+i+1) = '0';
                    *(*str+i+2) = 'D';
                    break;
                case 't':
                    *str = strshift(*str, i+1, &size);
                    *(*str+i+1) = '0';
                    *(*str+i+2) = '9';
                    break;
                default:
                    break;
            }
        }
    }
}

void int_init(int_vector* vector) {
    vector->size = 0;
    vector->capacity = 10;
    vector->data = malloc(vector->capacity * sizeof(*vector->data));
}

void int_clean(int_vector* vector) {
    vector->size = 0;
    vector->capacity = 0;
    if (vector->data) free(vector->data);
    vector->data = 0;
}

void int_push(int_vector* vector, int value) {
    if (vector->size == vector->capacity) {
        vector->capacity += 10;
        vector->data = realloc(vector->data, vector->capacity * sizeof(*vector->data));
    }
    vector->data[vector->size++] = value;
}

int int_get(int_vector* vector, int index) {
    if (index >= vector->size || index < 0) return 0;
    return vector->data[index];
}

void str_init(str_vector* vector) {
    vector->size = 0;
    vector->capacity = 10;
    vector->data = malloc(vector->capacity * sizeof(*vector->data));
}

void str_clean(str_vector* vector) {
    vector->size = 0;
    vector->capacity = 0;
    if (vector->data) {
        for (int i = 0; i < vector->capacity; i++) {
            free(vector->data[i].string);
        }
        free(vector->data);
    }
    vector->data = 0;
}

void str_push(str_vector* vector, char* string) {
    if (vector->size == vector->capacity) {
        vector->capacity += 10;
        vector->data = realloc(vector->data, vector->capacity * sizeof(*vector->data));
    }
    str1* data = &vector->data[vector->size];
    char* str = strdup(string+1);
    int size = strlen(str);
    strclean(&str, size);
    data->size = strsize(str);
    data->string = str;
    vector->size++;
}

char* str_get(str_vector* vector, int index) {
    if (index >= vector->size || index < 0) return 0;
    return vector->data[index].string;
}

int size_get(str_vector* vector, int index) {
    if (index >= vector->size || index < 0) return 0;
    return vector->data[index].size;
}

struct symtables {
    symtab *global;
    symtab *local;
} tables;

void str_double(char** str) {
    char dot = 0, c;
    int size = strlen(*str);
    if (*(*str) == '.') {
        *str = strshift(*str, 0, &size);
        *(*str) = '0';
        printf("\t; str = %s\n", *str);
    }
    for (int i = 1; i < size; i++) {
        c = *(*str+i);
        if (c == '.') dot = 1;
        if (tolower(c) == 'e') {
            if (!dot) {
                dot = 1;
                *str = strshift(*str, i+1, &size);
                *str = strshift(*str, i+1, &size);
                *(*str+i) = '.';
                *(*str+i+1) = '0';
                *(*str+i+2) = c;
                printf("\t; str = %s\n", *str);
                continue;
            }
            if (*(*str+i+1) != '+' && *(*str+i+1) != '-') {
                *str = strshift(*str, i+1, &size);
                *(*str+i+1) = '+';
                printf("\t; str = %s\n", *str);
            }
        }
    }
}

const char t_types[6][7] = {"i32", "double", "i1", "i8*", "i32", ""};

void cgen_load(const t_type type, char* id) {
    symtab* is_local = search_el(tables.local, id);
    if (type == t_string) {
        if (is_local)
            printf("\t%%%d = load i8*, i8** %%%s\n", tmp++, id);
        else
            printf("\t%%%d = getelementptr [1 x i8], [1 x i8]* @%s, i1 0, i1 0\n", tmp++, id);
    } else {
        if (is_local)
            printf("\t%%%d = load %s, %s* %%%s\n", tmp++, t_types[type], t_types[type], id);
        else
            printf("\t%%%d = load %s, %s* @%s\n", tmp++, t_types[type], t_types[type], id);
    }
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

void cgen_call_expr(expr* expression) {
    func_invoc* call = expression->arg1.call;
    symtab* function = search_el(tables.global, call->tkn->value);
    if (!call) {
        printf("call %s @%s()", t_types[function->type], function->id);
        return;
    }
    f_invoc_opts* opts = call->opts;
    f_params* params = call->params;
    int_vector vector;
    int_init(&vector);
    for (;opts;opts = opts->next) {
        cgen_expression(opts->opt);
        int_push(&vector, tmp-1);
    }
    printf("\t");
    if (function->type != t_void) {
        printf("%%%d = ", tmp++);
    }
    printf("call %s @%s(", t_types[function->type], function->id);
    int i = 0;
    for (;params;params = params->next) {
        if (params != call->params) printf(", ");
        printf("%s %%%d", t_types[params->type],int_get(&vector, i++));
    }
    puts(")");
    int_clean(&vector);
}

void cgen_call_fi(func_invoc* call) {
    symtab* function = search_el(tables.global, call->tkn->value);
    if (!call) {
        printf("call %s @%s()", t_types[function->type], function->id);
        return;
    }
    f_invoc_opts* opts = call->opts;
    f_params* params = call->params;
    int_vector vector;
    int_init(&vector);
    for (;opts;opts = opts->next) {
        cgen_expression(opts->opt);
        int_push(&vector, tmp-1);
    }
    printf("\t");
    if (function->type != t_void) {
        printf("%%%d = ", tmp);
    }
    printf("call %s @%s(", t_types[function->type], function->id);
    int i = 0;
    for (;params;params = params->next) {
        if (params != call->params) printf(", ");
        printf("%s %%%d", t_types[params->type], int_get(&vector, i++));
    }
    puts(")");
    int_clean(&vector);
    tmp++;
}

void cgen_intlit(expr* expression) {
    if (*(expression->tkn->value) == '0') {
        int intlit;
        if (*((expression->tkn->value)+1) == 'x') {
            intlit = strtol(expression->tkn->value, NULL, 16);
        } else {
            intlit = strtol(expression->tkn->value, NULL, 8);
        }
        printf("\t%%%d = add i32 %d, 0\n",
                tmp++, intlit);
    } else {
        printf("\t%%%d = add i32 %s, 0\n",
                tmp++, expression->tkn->value);
    }
}

void cgen_expression(expr* expression) {
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
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fmul double -1.0");
            else
                printf("mul i32 -1");
            printf(", %%%d\n", tmp1);
            break;
        case op_and:
            printf("\t%%%d = and i1 %%%d, %%%d\n", tmp++, tmp1, tmp2);
            break;
        case op_or:
            printf("\t%%%d = or i1 %%%d, %%%d\n", tmp++, tmp1, tmp2);
            break;
        case op_eq:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp oeq");
            else
                printf("icmp eq");
            printf(" %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_ne:
        printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp one");
            else
                printf("icmp ne");
            printf(" %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_ge:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp oge");
            else
                printf("icmp sge");
            printf(" %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_gt:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp ogt");
            else
                printf("icmp sgt");
            printf(" %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_le:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp ole");
            else
                printf("icmp sle");
            printf(" %s %%%d, %%%d\n",
                t_types[type], tmp1, tmp2);
            break;
        case op_lt:
            printf("\t%%%d = ", tmp++);
            if (type == t_float32)
                printf("fcmp olt");
            else
                printf("icmp slt");
            printf(" %s %%%d, %%%d\n",
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
        cgen_call_fi(expression->arg1.call);
        break;
    case e_id:
        cgen_load(expression->annotation, expression->tkn->value);
        break;
    case e_int:
        cgen_intlit(expression);
        break;
    case e_real:
        str_double(&expression->tkn->value);
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

void cgen_print(print_stmt* stmt) {
    const char call_printf[] = "\tcall i32 (i8*, ...) @printf(i8* getelementptr inbounds";
    const char call_puts[] = "\tcall i32 @puts(i8* getelementptr inbounds";
    declare_prints = 1;
    int tmp1;
    if (stmt->expression) {
        t_type type = stmt->expression->annotation;
        cgen_expression(stmt->expression);
        tmp1 = tmp-1;
        switch (type) {
        case t_int:
            printf("%s ([4 x i8], [4 x i8]* @.int, i1 0, i1 0), %s %%%d)\n",
                call_printf, t_types[type], tmp1);
            break;
        case t_float32:
            printf("%s ([7 x i8], [7 x i8]* @.double, i1 0, i1 0), %s %%%d)\n",
            call_printf, t_types[type], tmp1);
            break;
        case t_bool:
            bool_cnt++;
            printf("\tbr i1 %%%d, label %%true%d, label  %%false%d\n", tmp1, bool_cnt, bool_cnt);
            printf("\ttrue%d:\n", bool_cnt);
            printf("%s ([4 x i8], [4 x i8]* @.str, i1 0, i1 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.true, i1 0, i1 0))\n", call_printf);
            printf("\tbr label %%print%d\n", bool_cnt);
            printf("\tfalse%d:\n", bool_cnt);
            printf("%s ([4 x i8], [4 x i8]* @.str, i1 0, i1 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.false, i1 0, i1 0))\n", call_printf);
            printf("\tbr label %%print%d\n", bool_cnt);
            printf("\tprint%d:\n", bool_cnt);
            tmp++;
            break;
        case t_string:
            printf("\tcall i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i1 0, i1 0), i8* %%%d)\n", tmp1);
            break;
        default:
            break;
        }
    } else {
        str_push(&strings, stmt->strlit);
        int size = size_get(&strings, strings.size-1);
        printf("%s ([%d x i8], [%d x i8]* @.str.%d, i1 0, i1 0))\n",
            call_puts, size, size, str_count++);
    }
    tmp++;
}

void cgen_if(if_stmt* stmt) {
    char is_else;
    int cnt = if_cnt++;
    if (stmt->block2->dec.d_block) is_else = 1;
    else is_else = 0;
    cgen_expression(stmt->condition);
    printf("\tbr i1 %%%d, label %%if%d, label ",
        tmp-1, cnt);
    if (!is_else)
        printf("%%fi%d\n", cnt);
    else
        printf("%%else%d\n", cnt);
    printf("\tif%d:\n", cnt);
    cgen_stmt(stmt->block1);
    if (!ret) printf("\tbr label %%fi%d\n", cnt);
    else ret = 0;
    if (is_else) {
        printf("\telse%d:\n", cnt);
        cgen_stmt(stmt->block2);
        if (!ret) printf("\tbr label %%fi%d\n", cnt);
        else ret = 0;
    }
    printf("\tfi%d:\n", cnt);
}

void cgen_block(stmt_block* block) {
    ret = 0;
    if (!block) return;
    stmt_block* aux = block;
    for (;aux;aux = aux->next) {
        cgen_stmt(aux->stmt);
        if (aux->stmt->type == s_return) ret = 1;
    }
}

void cgen_for(for_stmt* stmt) {
    printf("\tbr label %%cfor%d\n", ++for_cnt);
    printf("\tcfor%d:\n", for_cnt);
    cgen_expression(stmt->condition);
    printf("\tbr i1 %%%d, label %%loop%d, label %%pool%d", tmp-1, for_cnt, for_cnt);
    printf("\tloop%d:\n", for_cnt);
    cgen_stmt(stmt->block);
    printf("\tbr label %%cfor%d\n", for_cnt);
    printf("\tpool%d:\n", for_cnt);
}

void cgen_parse(parse_args* parse) {
    char global;
    declare_atoi = 1;
    cgen_expression(parse->index);
    if (search_el(tables.local, parse->var->value))
        global = 0;
    else global = 1;
    printf("\t%%%d = getelementptr inbounds i8*, i8** %%argv, i32 %%%d\n",
        tmp, tmp-1);
    tmp++;
    printf("\t%%%d = load i8*, i8** %%%d\n", tmp, tmp-1);
    tmp++;
    printf("\t%%%d = call i32 @atoi(i8* %%%d)\n", tmp, tmp-1);
    tmp++;
    if (global)
        printf("\tstore i32 %%%d, i32* @%s\n", tmp-1, parse->var->value);
    else
        printf("\tstore i32 %%%d, i32* %%%s\n", tmp-1, parse->var->value);
}

void cgen_stmt(stmt_dec* stmt) {
    switch (stmt->type) {
        case s_assign:
            cgen_assign(stmt->dec.d_assign);
            break;
        case s_block:
            cgen_block(stmt->dec.d_block);
            break;
        case s_call:
            cgen_call_fi(stmt->dec.d_fi);
            break;
        case s_for:
            cgen_for(stmt->dec.d_for);
            break;
        case s_if:
            cgen_if(stmt->dec.d_if);
            break;
        case s_parse:
            cgen_parse(stmt->dec.d_args);
            break;
        case s_print:
            cgen_print(stmt->dec.d_print);
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
    switch (var->type) {
    case t_int:
        printf("\tstore i32 0, i32* %%%s\n", var->tkn->value);
        break;
    case t_float32:
        printf("\tstore double 0.0, double* %%%s\n", var->tkn->value);
        break;
    case t_bool:
        printf("\tstore i1 0, i1* %%%s\n", var->tkn->value);
        break;
    case t_string:
        printf("\tstore i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.empty, i1 0, i1 0), i8** %%%s\n", var->tkn->value);
        break;
    default:
        break;
    }
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
        if (aux != params) printf(", ");
        printf("%s %%a.%s", t_types[convert_v_type(aux->typespec)], aux->tkn->value);
    }
}

void cgen_func(func_dec* func) {
    tmp = 1;
    tables.local = func->localsym;
    printf("define %s", t_types[func->localsym->type]);
    printf(" @%s(", func->f_header->tkn->value);
    if (strcmp("main", func->f_header->tkn->value) == 0) {
        printf("i32 %%argc, i8** %%argv");
        if (func->f_header->param) printf(", ");
    }
    cgen_func_params(func->f_header->param);
    puts(") {");
    cgen_func_body(func->f_body);
    if (tables.local->type != t_string) {
        printf("\tret %s ", t_types[tables.local->type]);
        if (tables.local->type == t_float32) puts("0.0");
        else puts("0");
    }
    puts("}\n");
}

void cgen_global(var_dec* var) {
    if (var->type == t_string) {
        printf("@%s = global [1 x i8] zeroinitializer\n", var->tkn->value);
    } else {
        printf("@%s = global %s", var->tkn->value, t_types[var->type]);
        if (var->type == t_float32) puts(" 0.0\n");
        else puts(" 0");
    }
}

void codegen(prog_node* program, symtab* global) {
    tables.global = global;
    str_init(&strings);
    if (!program) return;
    dec_node* dec = program->dlist;
    if (!dec) {
        puts("define i32 @main(i32, i8**) {\n\tret i32 0\n}");
        return;
    }
    for (;dec;dec = dec->next) {
        if (dec->type == d_var)
            cgen_global(dec->dec.var);
        else if (dec->type == d_func)
            cgen_func(dec->dec.func);
    }

    if (declare_atoi)
        puts("declare i32 @atoi(i8*) nounwind\n");

    if (declare_prints) {
        puts("declare i32 @printf(i8* nocapture, ...) nounwind");
        puts("declare i32 @puts(i8* nocapture) nounwind\n");
        printf("%s\n", fstr);
        printf("%s\n", fint);
        printf("%s\n", fdouble);
        printf("%s\n", ftrue);
        printf("%s\n", ffalse);
    }

    printf("@.str.empty = private unnamed_addr constant [1 x i8] zeroinitializer\n");
    for (int i = 0; i < strings.size; i++) {
        printf("@.str.%d = private unnamed_addr constant [%d x i8] c\"%s\\00\"\n",
            i, size_get(&strings, i), str_get(&strings, i));
    }

    str_clean(&strings);
}