#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope_check.h"
#include "id_attrs.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "symtab.h"
#include "scope_check.h"

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
    scope_check_declare_ident(cdf.ident);
}

void scope_check_declare_ident(ident_t id)
{
    if (symtab_declared_in_current_scope(id.name))
    {
        bail_with_prog_error(*(id.file_loc), "Variable \"%s\" has already been declared", id.name);
    }
    else
    {
        int ofst_cnt = symtab_scope_loc_count();
        id_attrs* attrs = id_attrs_loc_create(*(id.file_loc), ofst_cnt);
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
        scope_check_declare_ident(*idp);
        idp = idp->next;
    }
}

void scope_check_procDecls(proc_decls_t vds)
{
    proc_decl_t * pdl = vds.proc_decls;
    while (pdl != NULL)
    {
        scope_check_procDecl(*pdl);
        pdl = pdl->next;
    }
}

void scope_check_procDecl(proc_decl_t pd)
{
    scope_check_block(*(pd.block));
}