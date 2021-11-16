#include "structs.h"

prog_node* new_prog(dec_node* declarations);
dec_node* insert_var_dec(dec_node* head, var_dec* new);
dec_node* insert_func_dec(dec_node* head, func_dec* func);
var_dec* create_var(char* id, v_type typespec);
func_dec* create_func(char* id, v_type return_type);

void print_ast(const prog_node* head);