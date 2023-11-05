#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H
#include "ast.h"
#include "id_use.h"

// Checks the programs scope
extern void scope_check_program(block_t progast);

// Checks the blocks scope
extern void scope_check_block(block_t blk);

// Checks multiple constant declarations scope
extern void scope_check_constDecls(const_decls_t cds);

// Checks the const declaration scope
extern void scope_check_constDecl(const_decl_t cd);

// Checks all the constant definitions scope
extern void scope_check_constDefs(const_defs_t cdfs);

// Checks the constant definition scope
extern void scope_check_constDef(const_def_t cdf);

// Checks the identifier's scope
extern void scope_check_declare_ident(const char *name, AST_type type, file_location *fl);

// Checks multiple variable's scope
extern void scope_check_varDecls(var_decls_t vds);

// Checks the variable's scope
extern void scope_check_varDecl(var_decl_t vd);

// Checks all the identifiers scopes
extern void scope_check_idents(idents_t ids);

// Checks all the procedures scope
extern void scope_check_procDecls(proc_decls_t vds);

// Checks the procedure scope
extern void scope_check_procDecl(proc_decl_t pd);

// Checks the statments' scope
extern void scope_check_stmt(stmt_t stmt);

// Checks the assignment statement scope
extern void scope_check_assignStmt(assign_stmt_t stmt);

// Checks the call statement's scope
extern void scope_check_callStmt(call_stmt_t stmt);

// Checks the begin statement's scope
extern void scope_check_beginStmt(begin_stmt_t stmt);

// Checks the if statement's scope
extern void scope_check_ifStmt(if_stmt_t stmt);

// Checks the while statement's scope
extern void scope_check_whileStmt(while_stmt_t stmt);

// Checks the read statement's scope
extern void scope_check_readStmt(read_stmt_t stmt);

// Checks the write statement's scope
extern void scope_check_writeStmt(write_stmt_t stmt);

// Checks the identifier declaration scope
extern id_use *scope_check_ident_declared(file_location floc, const char *name);

// Checks the expressions' scope
extern void scope_check_expr(expr_t exp);

// Checks expression to see if identifiers have been declared
extern void scope_check_binary_op_expr(binary_op_expr_t exp);

// Checks identifiers in scope if they have been declared or not
extern void scope_check_ident_expr(ident_t id);

// Checks the condition if it's odd or relOp
extern void scope_check_condition(condition_t condition);

// Checks the expressions scope
extern void scope_check_condition_odd(odd_condition_t condition);

// Checks both expression1 and expression2 scope
extern void scope_check_condition_rel(rel_op_condition_t condition);

#endif