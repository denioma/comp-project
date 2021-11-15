#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum
{
    program,
    var_decl,
    id,
    typespec,
    func_decl,
    func_header,
    func_body,
    func_params,
    param_decl,
    block_stm,
    if_stm,
    else_stm,
    for_stm,
    return_stm,
    call_stm,
    print_stm,
    parse_stm
} node_type;

typedef struct token
{
    char *sym;
    int ln, col;
} token;

typedef struct node
{
    node_type type;
    token *token;
    node_list *children;
} node;

typedef struct node_list
{
    node *val;
    node_list *next;
} node_list;

// typedef struct func_decl
// {

// } func_decl;

// typedef struct var_dec
// {

// } var_decl;

// typedef struct program
// {
//     /* data */
// } program;

#endif