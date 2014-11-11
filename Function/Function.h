// --- Author : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------


// ---------- Defines -------------
#define FUNCTION_NO_RETURN(func)( (void * (*)())func)

#define function_new(function, arg) \
	_function_new (FUNCTION_NO_RETURN (function), arg)

#define draw_function_new(function, arg, view) \
	_draw_function_new (FUNCTION_NO_RETURN (function), arg, view)

// ------ Class declaration -------
typedef struct _Function
{
	void * (*call)();
	void *arg;

}	Function;

typedef struct _DrawFunction
{
	Function *function;
	float *view;

} DrawFunction;


// --------- Constructors ---------

Function *
_function_new (void * (*call)(), void *arg);

DrawFunction *
_draw_function_new (void * (*call)(), void *arg, void *view);

Function *
function_alloc (void);

DrawFunction *
draw_function_alloc (void);

// ----------- Methods ------------

void
function_init (Function *function, void * (*call)(), void *arg);

void
draw_function_init (DrawFunction *this, void * (*call)(), void *arg, float *view);

void *
function_call (Function *function);

void *
draw_function_call (DrawFunction *function);

// --------- Destructors ----------

void
function_free (Function *function);


