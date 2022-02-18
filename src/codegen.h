#include "yacc.h"

void cgen_expression(expr* expression);
void cgen_block(stmt_block* block);
void cgen_stmt(stmt_dec* stmt);

void codegen(prog_node* program, symtab* global);