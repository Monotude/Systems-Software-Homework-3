#ifndef _SYMTAB_H
#define _SYMTAB_H
#include "id_use.h"
#include "scope.h"

// Maximum nesting of potential scopes
#define MAX_NESTING 100

// initialize the symbol table
extern void symtab_initialize();

// Return the number of scopes currently in the symbol table.
extern unsigned int symtab_size();

// Does this symbol table have any scopes in it?
extern bool symtab_empty();

// Return the current scope's count of variables declared
extern unsigned int symtab_scope_loc_count();

// Return the current scope's size
extern unsigned int symtab_scope_size();

// Is the current scope full?
extern bool symtab_scope_full();

// Return the current nesting level
extern unsigned int symtab_current_nesting_level();

// Is the symbol table itself full?
extern bool symtab_full();

// Is name declared?
extern bool symtab_declared(const char *name);

// Is name declared in the current scope?
extern bool symtab_declared_in_current_scope(const char *name);

// Add a variable
extern void symtab_insert(const char *name, id_attrs *attrs);

// Start a new scope
extern void symtab_enter_scope();

// End the current scope
extern void symtab_leave_scope();

// return id for name if it is declared
extern id_use *symtab_lookup(const char *name);

#endif
