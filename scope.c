#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope.h"
#include "utilities.h"

//Allocates new scope symbol table and returns a pointer to it
scope_t* scope_create()
{
    scope_t* new_s = (scope_t*) malloc(sizeof(scope_t));
    if (new_s == NULL) bail_with_error("No space for new scope_t!");
    new_s->size = 0;
    new_s->loc_count = 0;
    for (int i = 0; i < MAX_SCOPE_SIZE; i++){
        new_s->entries[i] = NULL;
    }
    return new_s;
}

//Returns the number of constant and variable declarations in the scope
unsigned int scope_loc_count(scope_t* s)
{
    return s->loc_count;
}

//Returns the scopes current size (number of declared idents)
unsigned int scope_size(scope_t* s)
{
    return s->size;
}

//Returns weather the scope is full or not
bool scope_full(scope_t* s)
{
    return scope_size(s) >= MAX_SCOPE_SIZE;
}

//Returns weather the name is in the scope
bool scope_declared(scope_t* s, const char* name)
{
    return scope_lookup(s, name) != NULL;
}

//Adds an association into the scope
static void scope_add(scope_t* s, scope_assoc_t* assoc)
{
    assoc->attrs->offset_count = s->loc_count + 1;
    s->size += 1;
    s->entries[s->size] = assoc;
}

//Adds an association into the scope
void scope_insert(scope_t* s, const char* name, id_attrs* attrs)
{
    scope_assoc_t* new_assoc = malloc(sizeof(scope_assoc_t));
    if (new_assoc == NULL) bail_with_error("No space for association!");
    new_assoc->id = name;
    new_assoc->attrs = attrs;
    scope_add(s, new_assoc);
    free(new_assoc);
}

//Returns the attributes of a given name in the scope
id_attrs* scope_lookup(scope_t* s, const char* name)
{
    for (int i = 0; i < s->size; i++)
    {
        if (strcmp(s->entries[i]->id, name) == 0) return s->entries[i]->attrs;
    }
    return NULL;
}
