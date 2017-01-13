//
// Created by bknun on 1/7/2017.
//

#ifndef SHARP_AST_H
#define SHARP_AST_H

#include <list>
#include "tokenizer/tokenentity.h"

enum ast_types
{
    ast_class_decl,
    ast_import_decl,
    ast_module_decl,
    ast_var_decl,
    ast_value,
    ast_method_inv,
    ast_method_params,
    ast_block,
    ast_method_return_type,
    ast_return_stmnt,

    ast_entity // the base level ast
};

class ast
{
public:
    ast(ast* parent, ast_types type)
    :
            type(type),
            parent(parent)
    {
        sub_asts = new list<ast>();
        entities = new list<token_entity>();
    }

    ast_types gettype();
    ast* getparent();
    long getsubastcount();
    ast *getsubast(long at);
    long getentitycount();
    token_entity getentity(long at);

    void add_entity(token_entity entity);
    void add_ast(ast _ast);

private:
    ast_types type;
    ast *parent;
    list<ast> *sub_asts;
    list<token_entity> *entities;
};

#endif //SHARP_AST_H
