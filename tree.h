#include "structs.h"

prog_node* new_prog(dec_node* declarations);
dec_node* insert_dec(dec_node* head, dec_node* node);
dec_node* insert_var(char* id, char* token);

void print_ast(const prog_node* head);