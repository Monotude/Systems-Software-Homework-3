#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "file_location.h"
#include "id_attrs.h"
#include "scope_check.h"
#include "symtab.h"
#include "utilities.h"

void scope_check_program(block_t progast)
{
    scope_check_block(progast);
}

void scope_check_block(block_t blk)
{
    symtab_enter_scope();
    scope_check_constDecls(blk.const_decls);
    scope_check_varDecls(blk.var_decls);
    scope_check_procDecls(blk.proc_decls);
    scope_check_stmt(blk.stmt);
    symtab_leave_scope();
}

void scope_check_constDecls(const_decls_t cds)
{
    const_decl_t *cdl = cds.const_decls;
    while (cdl != NULL)
    {
        scope_check_constDecl(*cdl);
        cdl = cdl->next;
    }
}

void scope_check_constDecl(const_decl_t cd)
{
    scope_check_constDefs(cd.const_defs);
}

void scope_check_constDefs(const_defs_t cdfs)
{
    const_def_t *cdp = cdfs.const_defs;
    while (cdp != NULL)
    {
        scope_check_constDef(*cdp);
        cdp = cdp->next;
    }
}

void scope_check_constDef(const_def_t cdf)
{
    scope_check_declare_ident(cdf.ident, cdf.type_tag);
}

void scope_check_declare_ident(ident_t id, AST_type type)
{
    if (symtab_declared_in_current_scope(id.name))
    {
        bail_with_prog_error(*(id.file_loc), "variable \"%s\" is already declared as a %s", id.name, kind2str(symtab_lookup(id.name)->attrs->kind));
    }
    else
    {
        int ofst_cnt = symtab_scope_loc_count();
        id_attrs *attrs = NULL;
        switch (type)
        {
        case const_def_ast:
            attrs = create_id_attrs(*(id.file_loc), constant_idk, ofst_cnt);
            break;
        case idents_ast:
            attrs = create_id_attrs(*(id.file_loc), variable_idk, ofst_cnt);
            break;
        default:
            bail_with_prog_error(*(id.file_loc), "error type of variable");
        }
        symtab_insert(id.name, attrs);
    }
}

void scope_check_varDecls(var_decls_t vds)
{
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL)
    {
        scope_check_varDecl(*vdp);
        vdp = vdp->next;
    }
}

void scope_check_varDecl(var_decl_t vd)
{
    scope_check_idents(vd.idents);
}

void scope_check_idents(idents_t ids)
{
    ident_t *idp = ids.idents;
    while (idp != NULL)
    {
        scope_check_declare_ident(*idp, ids.type_tag);
        idp = idp->next;
    }
}

void scope_check_procDecls(proc_decls_t vds)
{
    proc_decl_t *pdl = vds.proc_decls;
    while (pdl != NULL)
    {
        scope_check_procDecl(*pdl);
        pdl = pdl->next;
    }
}

void scope_check_procDecl(proc_decl_t pd)
{
    if (symtab_declared_in_current_scope(pd.name))
    {
        bail_with_prog_error(*(pd.file_loc), "variable \"%s\" is already declared as a %s", pd.name, kind2str(symtab_lookup(pd.name)->attrs->kind));
    }
    else
    {
        scope_check_block(*(pd.block));
    }
}

void scope_check_stmt(stmt_t stmt)
{
    switch (stmt.stmt_kind)
    {
    case assign_stmt:
        scope_check_assignStmt(stmt.data.assign_stmt);
        break;
    case call_stmt:
        scope_check_callStmt(stmt.data.call_stmt);
        break;
    case begin_stmt:
        scope_check_beginStmt(stmt.data.begin_stmt);
        break;
    case if_stmt:
        scope_check_ifStmt(stmt.data.if_stmt);
        break;
    case while_stmt:
        scope_check_whileStmt(stmt.data.while_stmt);
        break;
    case read_stmt:
        scope_check_readStmt(stmt.data.read_stmt);
        break;
    case write_stmt:
        scope_check_writeStmt(stmt.data.write_stmt);
        break;
    case skip_stmt:
        // no identifiers are possible in this case, so just return
        break;
    default:
        bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
        break;
    }
}

void scope_check_assignStmt(assign_stmt_t stmt)
{
    const char *name = stmt.name;
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), name);
    assert(idu != NULL);
    scope_check_expr(*(stmt.expr));
}

void scope_check_callStmt(call_stmt_t stmt)
{
    const char *name = stmt.name;
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), name);
    assert(idu != NULL);
}

void scope_check_beginStmt(begin_stmt_t stmt)
{
    stmt_t *stmts = stmt.stmts.stmts;
    while (stmts != NULL)
    {
        scope_check_stmt(*stmts);
        stmts = stmts->next;
    }
}

void scope_check_ifStmt(if_stmt_t stmt)
{
    scope_check_condition(stmt.condition);
    scope_check_stmt(*(stmt.then_stmt));
    scope_check_stmt(*(stmt.else_stmt));
}

void scope_check_whileStmt(while_stmt_t stmt)
{
    scope_check_condition(stmt.condition);
    scope_check_stmt(*(stmt.body));
}

void scope_check_readStmt(read_stmt_t stmt)
{
    scope_check_ident_declared(*(stmt.file_loc), stmt.name);
}

void scope_check_writeStmt(write_stmt_t stmt)
{
    scope_check_expr(stmt.expr);
}

id_use *scope_check_ident_declared(file_location floc, const char *name)
{
    id_use *ret = symtab_lookup(name);
    if (ret == NULL)
    {
        bail_with_prog_error(floc, "identifier \"%s\" is not declared!", name);
    }
    return ret;
}

void scope_check_expr(expr_t exp)
{
    switch (exp.expr_kind)
    {
    case expr_bin:
        scope_check_binary_op_expr(exp.data.binary);
        break;
    case expr_ident:
        scope_check_ident_expr(exp.data.ident);
        break;
    case expr_number:
        // no identifiers are possible in this case, so just return
        break;
    default:
        bail_with_error("Unexpected expr_kind_e (%d) in scope_check_expr", exp.expr_kind);
        break;
    }
}

void scope_check_binary_op_expr(binary_op_expr_t exp)
{
    scope_check_expr(*(exp.expr1));
    scope_check_expr(*(exp.expr2));
}

void scope_check_ident_expr(ident_t id)
{
    scope_check_ident_declared(*(id.file_loc), id.name);
}

void scope_check_condition(condition_t condition)
{
    switch (condition.cond_kind)
    {
    case ck_odd:
        scope_check_condition_odd(condition.data.odd_cond);
        break;
    case ck_rel:
        scope_check_condition_rel(condition.data.rel_op_cond);
        break;
    }
}

void scope_check_condition_odd(odd_condition_t condition)
{
    scope_check_expr(condition.expr);
}

void scope_check_condition_rel(rel_op_condition_t condition)
{
    scope_check_expr(condition.expr1);
    scope_check_expr(condition.expr2);
}