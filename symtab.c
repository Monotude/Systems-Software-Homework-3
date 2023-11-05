#include <stddef.h>
#include "scope.h"
#include "symtab.h"
#include "utilities.h"

// index of the top of the stack of scopes
static int symtab_top_idx = -1;

// the symbol table itself
static scope_t *symtab[MAX_NESTING];

// initialize the symbol table
void symtab_initialize()
{
    symtab_top_idx = -1;
    for (int i = 0; i < MAX_NESTING; i++)
    {
        symtab[i] = NULL;
    }
}

// Return the number of scopes currently in the symbol table.
unsigned int symtab_size()
{
    return symtab_top_idx + 1;
}

// Does this symbol table have any scopes in it?
bool symtab_empty()
{
    return symtab_size() == 0;
}

// Return the current scope's next location count (of variables).
unsigned int symtab_scope_loc_count()
{
    return scope_loc_count(symtab[symtab_top_idx]);
}

// Return the current scope's size (the number of declared ids).
unsigned int symtab_scope_size()
{
    return scope_size(symtab[symtab_top_idx]);
}

// Is the current scope full?
bool symtab_scope_full()
{
    return scope_full(symtab[symtab_top_idx]);
}

// Return the current nesting level of the symbol table
unsigned int symtab_current_nesting_level()
{
    // assert(symtab_top_idx >= 0);
    return symtab_top_idx;
}

// Is the symbol table itself full
bool symtab_full()
{
    return symtab_current_nesting_level() == MAX_NESTING - 1;
}

// Is the given name associated with some attributes?
bool symtab_declared(const char *name)
{
    return symtab_lookup(name) != NULL;
}

// Is the given name associated with some attributes in the current scope?
bool symtab_declared_in_current_scope(const char *name)
{
    id_attrs *attrs = scope_lookup(symtab[symtab_top_idx], name);
    return attrs != NULL;
}

// add identifier to scope
static void add_ident(scope_t *s, const char *name, id_attrs *attrs)
{
    id_attrs *old_attrs = scope_lookup(s, name);
    if (old_attrs != NULL)
    {
        bail_with_prog_error(attrs->file_loc, "symtab_insert called with an already declared variable\"%s\"!", name);
    }
    else
    {
        scope_insert(s, name, attrs);
    }
}

// add to symble table
void symtab_insert(const char *name, id_attrs *attrs)
{
    add_ident(symtab[symtab_top_idx], name, attrs);
}

// Start a new scope
void symtab_enter_scope()
{
    symtab_top_idx++;
    symtab[symtab_top_idx] = scope_create();
}

// End current scope
void symtab_leave_scope()
{
    if (symtab_top_idx < 0)
    {
        bail_with_error("Cannot leave scope, no scope on symtab's stack!");
    }
    symtab_top_idx--;
}

// Returns the attributes of a given name in the scope
id_use *symtab_lookup(const char *name)
{
    unsigned int levelsOut = 0;
    for (int level = symtab_top_idx; 0 <= level; level--)
    {
        id_attrs *attrs = scope_lookup(symtab[level], name);
        if (attrs != NULL)
        {
            return id_use_create(attrs, levelsOut);
        }
        levelsOut++;
    }
    return NULL;
}

// To be used in HW4
/*
lexical_address *symtab_lexical_address(const char *name)
{
    // maintaining: -1 <= level <= symtab_top_idx;
    // maintaining: (for all int j:
    //                level < j <= symtab_top_idx
    //                   ==> !scope_declared(symtab[j], name))
    for (int level = symtab_top_idx; 0 <= level; level--)
    {
        id_attrs *attrs = scope_lookup(symtab[level], name);
        if (attrs != NULL)
        {
            return lexical_address_create(symtab_top_idx - level, attrs->loc_offset);
        }
    }
    bail_with_error("Couldn't find %s for symtab_lexical_address!", name);
    return NULL;
}
*/