#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H
#include "ast.h"
#include "id_use.h"

extern void scope_check_program(block_t progast);

extern void scope_check_block(block_t blk);

extern void scope_check_constDecls(const_decls_t cds);

extern void scope_check_constDecl(const_decl_t cd);

extern void scope_check_constDefs(const_defs_t cdfs);

extern void scope_check_constDef(const_def_t cdf);

extern void scope_check_declare_ident(ident_t id);

extern void scope_check_varDecls(var_decls_t vds);

extern void scope_check_varDecl(var_decl_t vd);

extern void scope_check_idents(idents_t ids);

extern void scope_check_procDecls(proc_decls_t vds);

extern void scope_check_procDecl(proc_decl_t pd);

extern void scope_check_stmt(stmt_t stmt);

extern void scope_check_assignStmt(assign_stmt_t stmt);

extern void scope_check_callStmt(call_stmt_t stmt);

extern void scope_check_beginStmt(begin_stmt_t stmt);

extern void scope_check_ifStmt(if_stmt_t stmt);

extern void scope_check_whileStmt(while_stmt_t stmt);

extern void scope_check_readStmt(read_stmt_t stmt);

extern void scope_check_writeStmt(write_stmt_t stmt);

extern id_use *scope_check_ident_declared(file_location floc, const char *name);

extern void scope_check_expr(expr_t exp);

extern void scope_check_binary_op_expr(binary_op_expr_t exp);

extern void scope_check_ident_expr(ident_t id);

#endif