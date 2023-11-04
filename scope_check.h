#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H
#include "ast.h"

extern void scope_check_program(block_t progast);
void scope_check_block(block_t blk);
void scope_check_constDecls(const_decls_t cds);
void scope_check_constDecl(const_decl_t cd);
void scope_check_constDefs(const_defs_t cdfs);
void scope_check_constDef(const_def_t cdf);
void scope_check_declare_ident(ident_t id);

void scope_check_varDecls(var_decls_t vds);
void scope_check_varDecl(var_decl_t vd);
void scope_check_idents(idents_t ids);


void scope_check_procDecls(proc_decls_t vds);
void scope_check_procDecl(proc_decl_t pd);

#endif