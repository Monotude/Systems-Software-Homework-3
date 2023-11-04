#ifndef _SCOPE_H
#define _SCOPE_H

#include <stdbool.h>
#include "ast.h"
#include "machine_types.h"
#include "id_attrs.h"

#define MAX_SCOPE_SIZE 4096

typedef struct
{
    const char* id;
    id_attrs* attrs;
} scope_assoc_t;

typedef struct scope_s
{
    unsigned int size;
    unsigned int loc_count;
    scope_assoc_t* entries[MAX_SCOPE_SIZE];
} scope_t;

//Allocates new scope symbol table and returns a pointer to it
extern scope_t* scope_create();

//Returns the number of constant and variable declarations in the scope
extern address_type scope_loc_count(scope_t* s);

//Returns the scopes current size (number of declared idents)
extern unsigned int scope_size(scope_t* s);

//Returns weather the scope is full or not
extern bool scope_full(scope_t* s);

//Returns weather the name is in the scope
extern bool scope_declared(scope_t* s, const char* name);

//Adds an association into the scope
extern void scope_insert(scope_t* s, const char* name, id_attrs* attrs);

//Returns the attributes of a given name in the scope
extern id_attrs* scope_lookup(scope_t* s, const char* name);

#endif
