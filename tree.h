#include "structs.h"

prog_node* new_prog(dec_node* declarations);
dec_node* insert_var_dec(dec_node* head, var_dec* new);
dec_node* insert_func_dec(dec_node* head, func_dec* func);
var_dec* create_var(char* id, v_type typespec);
func_dec* create_func(char* id, v_type typespec, param_dec* param_list, func_body* body);
param_dec* create_param(char* id, v_type typespec, param_dec* chain);

/* TODO */
func_body* insert_to_body(func_body* head, func_body* node);
func_body* create_body_var(var_dec* var);
func_body* create_body_stmt(stmt_dec* stmt);
/* End TODO */

void print_ast(const prog_node* head);