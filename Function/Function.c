#include "Function.h"
#include <stdlib.h>
#include <stdio.h>

Function *
_function_new (void * (*call) () , void *arg)
{
	Function *this;

	if ((this = function_alloc ()) == NULL)
		return NULL;

	function_init (this, call, arg);

	return this;
}

DrawFunction *
_draw_function_new (void * (*call) () , void *arg, void *view)
{
	DrawFunction *this;

	if ((this = draw_function_alloc ()) == NULL)
		return NULL;

	draw_function_init (this, call, arg, view);

	return this;
}

Function *
function_alloc (void)
{
	return calloc (1, sizeof (Function));
}

DrawFunction *
draw_function_alloc (void)
{
	return calloc (1, sizeof (DrawFunction));
}

void
function_init (Function *this, void * (*call) () , void *arg)
{
	this->call = call;
	this->arg  = arg;
}

void
draw_function_init (DrawFunction *this, void * (*call) () , void *arg, float *view)
{
	this->function = function_new (call, arg);
	this->view = view;
}

void *
function_call (Function *this)
{
	if (this == NULL || this->call == NULL)
	{
		printf ("%x cannot be called!\n", (int) this);
		return NULL;
	}

	else
		return this->call (this->arg);
}

void *
draw_function_call (DrawFunction *this)
{
	if (this == NULL || this->function == NULL)
	{
		printf ("%x cannot be called!\n", (int) this);
		return NULL;
	}

	else
		return this->function->call (this->function->arg, this->view);
}

void
function_free (Function *function)
{
	if (function != NULL)
	{
		free (function);
	}
}
