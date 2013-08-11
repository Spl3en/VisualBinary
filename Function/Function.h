// --- Author : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------


// ---------- Defines -------------
#define FUNCTION_NO_RETURN(func) ((void *(*)())func)

#define function_new(function, arg) \
	_function_new(FUNCTION_NO_RETURN(function), arg)

// ------ Class declaration -------
typedef struct _Function
{
	void *(*call)();
	void *arg;

}	Function;



// --------- Constructors ---------

Function *
_function_new (void *(*call)(), void *arg);

Function *
function_alloc (void);

// ----------- Methods ------------

void
function_init (Function *function, void *(*call)(), void *arg);

void *
function_call (Function *function);

// --------- Destructors ----------

void
function_free (Function *function);


