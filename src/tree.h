#include "structs.h"

token* create_tkn(char* value, int line, int col);
prog_node* new_prog(dec_node* declarations);
dec_node* insert_var_dec(dec_node* head, var_dec* new);
dec_node* insert_func_dec(dec_node* head, func_dec* func);
var_dec* create_var(token* tkn, const v_type typespec);
func_dec* create_func(token* tkn, const v_type typespec, param_dec* param_list, func_body* body);
param_dec* create_param(token* tkn, v_type typespec, param_dec* chain);
func_body* insert_to_body(func_body* head, func_body* node);
func_body* create_body_var(dec_node* var);
dec_node* insert_var_dec_list(dec_node* head, dec_node* list);
dec_node* set_id_reps_head(dec_node* head, token* tkn, v_type typespec);
dec_node* save_id_reps(dec_node* head, token* tkn);
func_body* create_body_stmt(stmt_dec* stmt);
stmt_dec* create_stmt(s_type type);
stmt_dec* create_pargs(token* tkn, expr* index);
stmt_dec* create_print(token* tkn, token* strlit, expr* expression);
stmt_dec* create_assign(token* tkn, expr* expression);
stmt_dec* create_if(expr* condition, stmt_block* block1, stmt_block* block2);
stmt_dec* create_for(expr* expression, stmt_block* block);
stmt_dec* create_return(expr* expression);
stmt_dec* create_call();
stmt_dec* create_stmt_block(stmt_block* block);
stmt_dec* create_stmt_block_nullable(stmt_block* block);
stmt_block* create_block(stmt_block* chain, stmt_dec* stmt);
stmt_block* block_or_null(stmt_block* chain, stmt_dec* stmt);
stmt_block* add_block_stmt(stmt_block* block, stmt_dec* stmt);
expr* create_expr(e_type type, op operator, token* tkn, void* arg1, expr* arg2);
f_invoc_opts* create_fi_opts(f_invoc_opts* chain, expr* expression);
func_invoc* create_func_invocation(token* tkn, f_invoc_opts* opts);

void print_ast(const prog_node* head);
void destroy(prog_node* program);