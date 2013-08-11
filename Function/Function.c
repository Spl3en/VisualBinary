#include "Function.h"
#include <stdlib.h>
#include <stdio.h>

Function *
_function_new (void *(*function)(), void *arg)
{
	Function *this;

	if ((this = function_alloc()) == NULL)
		return NULL;

	function_init(this, function, arg);

	return this;
}

Function *
function_alloc (void)
{
	return calloc (1, sizeof(Function));
}

void
function_init (Function *this, void *(*call)(), void *arg)
{
	this->call = call;
	this->arg  = arg;
}

void *
function_call (Function *this)
{
	if (this == NULL || this->call == NULL)
	{
		printf("%x cannot be called!\n", this);
		return NULL;
	}

	else
		return this->call (this->arg);
}

void
function_free (Function *function)
{
	if (function != NULL)
	{
		free (function);
	}
}
