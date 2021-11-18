#include "structs.h"

prog_node* new_prog(dec_node* declarations);
dec_node* insert_var_dec(dec_node* head, var_dec* new);
dec_node* insert_func_dec(dec_node* head, func_dec* func);
var_dec* create_var( char* id, const v_type typespec);
func_dec* create_func(char* id, const v_type typespec, param_dec* param_list, func_body* body);
param_dec* create_param(char* id, v_type typespec, param_dec* chain);
func_body* insert_to_body(func_body* head, func_body* node);
func_body* create_body_var(dec_node* var);
dec_node* insert_var_dec_list(dec_node* head, dec_node* list);
dec_node* set_id_reps_head(dec_node* head, char* id, v_type typespec);
dec_node* save_id_reps(dec_node* head, char* id);
func_body* create_body_stmt(stmt_dec* stmt);
stmt_dec* create_stmt(s_type type);
stmt_dec* create_pargs(char* id, expr* index);
stmt_dec* create_print(char* strlit, expr* expression);
stmt_dec* create_assign(char* id, expr* expression);
stmt_dec* create_if(expr* condition, stmt_dec* block1, stmt_dec* block2);
stmt_dec* create_for(expr* expression, stmt_dec* block);
stmt_dec* create_return(expr* expression);
stmt_dec* create_call();
stmt_dec* create_stmt_block(stmt_block* block);
stmt_block* create_block(stmt_block* chain, stmt_dec* stmt);
stmt_block* add_block_stmt(stmt_block* block, stmt_dec* stmt);
expr* create_expr(e_type type, op operator, void* arg1, expr* arg2);
f_invoc_opts* create_fi_opts(expr* first, f_invoc_opts* chain);
func_invoc* create_func_invocation(char* id, f_invoc_opts* opts);

void print_ast(const prog_node* head);
void printer_block(const stmt_block* block);