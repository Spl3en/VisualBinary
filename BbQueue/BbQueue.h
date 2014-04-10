 /**
 *  @author	 :   Spl3en
 *  @file	   :   BbQueue.h
 *  @version	:   1.8
 *
 *  BbQueue provides functions for stacks and queues.
 *  It allows the addition or removal of elements from a list in a simple and efficient manner.
 *
 *  Its usage is twofold:
 *	  - It is possible to use it as a queue/stack, via bbq_add / bbq_pop and its derivatives;
 * 	  - It is possible to use it as a double-chained list, first by retrieving the corresponding BbChild via bb_queue_pick_child and its derivatives.
 *	  Thus, the getters bb_child_get_next/_prev are available, and the data pointed by its children as well via ->data.
 *	  Note that __bb_child_get_next/_prev are faster, but will crash the program if the BbChild passed as parameter is invalid (no check).  
 *
 *	Changelog :
 *
 *	  [+] v1.1 :	(10 / 11 / 2010)
 *		  [ADD]	   bb_queue_remv_nth (BbQueue *, int nth)
 *		  [ADD]	   bb_queue_get_nth  (BbQueue *, int nth)
 *		  [FIX]	   bb_queue_remv : Length fixed
 *
 *	  [+] v1.2		(18 / 11 / 2010)
 *		  [ADD]	   The structure now contains two new elements: index and current
 *					  - index is a simple integer; It can freely be modified by the user. It is useful when remembering the index of an element is necessary.
 *					  - current is a pointer to the last BbChild picked. 
 *					  It is useful in the case that we want to get access to the next BbChild via q->current->next without having to use bb_queue_pick_nth,
 *					  if the pointer to the BbChild happens to be lost.
 *
 *		  [ADD]	   bb_queue_get_current (BbQueue *queue) : returns the data pointed by the current BbChild in a proper way, or NULL if it does not exist.
 *					  bb_queue_pick_next (BbQueue *queue, int refresh) : returns the data pointed by the next BbChild. refresh is to specify if the current pointer has to be refreshed or not.
 *					  bb_queue_pick_prev (BbQueue *queue, int refresh) : same as bb_queue_get_next for the previous BbChild.
 *
 *		  [ADD]	   bb_queue_free_all (BbQueue *queue, (void *) free_func()); : Allows to free the memory of the BbQueue as well as the one of the queued elements.
 *					  It is the most efficient function to erase all the data bound to BbQueue.
 *
 *	  [+] v1.3		(03 / 12 / 2010)
 *		  [ADD]	   bb_queue_do (BbQueue *queue, void (* do_func)()) : Allows to perform a function onto every BbQueue
 *		  [ADD]	   bb_queue_replace_nth : Replaces a data by another at a fixed index, and returns the removed one
 *		  [ADD]	   bb_queue_replace	 : Replaces a data by another
 *		  [FIX]	   bb_queue_add_nth	 : Fixes the length of the chained list
 *	      [ADD]	   foreach_bb_queue_item : Implements foreach for the BbQueue
 *	      [ADD]	   break_foreach : The foreach loop cannot be broken using a simple "break". break_foreach *has* to be used.
 *
 *	  [+] v.1.4	   (07 / 02 / 2011)
 *		  [FIX]	   bb_queue_pop : Correction of a conception bug (!)
 *		  [FIX]	   bb_queue_pick_first : The macro has in been implemented as a function for security reasons
 *
 *	  [+] v1.5		(12 / 03 / 2011)
 *		  [FIX]	   The "current" pointer and its associated fucntions (cf v1.2) are now deprecated, bad conception and useless
 *		  [FIX]	   Fusion of BbQueue.c and BbChild.c for practical reasons
 *		  [ADD]	   bb_queue_add_array : transforms the one dimension array into a BbQueue, while respecting the ascending order
 *		  [ADD]	   bb_queue_copy : Copies a BbQueue
 *
 *	  [+] v1.6		(19 / 10 / 2011)
 *		  [FIX]	   Optimization of bb_queue_insert_before/after
 *		  [ADD]	   bb_queue_insert_before/after_child()
 *		  [ADD]	   bb_queue_concat
 *		  [ADD]	   foreach_bb_queue_item_reversed
 *
 *	  [+] v1.7		(08 / 01 / 2012)
 *		  [FIX]	   Fixes a minor bug for bb_queue_foreach_*
 *		  [ADD]	   bb_queue_put_last : takes an element and puts it at the end, optimized equivalent of remv + add
 *		  [ADD]	   bb_queue_put_first : takes an element and puts it at the beginning, optimized equivalent of remv + push
 *		  [ADD]	   bb_queue_concat_nth : concatenates two BbQueue from the nth position
 *		  [FIX]	   bb_queue_remv_nth : In the case that "nth" data was duplicated in the list, the first encountered in the list was deleted without taking into acount the nth
 *		  [FIX]	   bb_queue_add_nth : The child wasn't inserted at the correct position, there was a +1 offset
 *		  [FIX]	   bb_queue_remv : The first et last pointers weren't correctly reseted to NULL if the size of the of bb_queue is equal to 1 before remv
 *
 *
 *		[+] v1.8		(06 / 03 / 2012)
 *	      [ADD]	   bb_queue_pick_nth : now accepts negative integers as a second argument; The element picked will be taken from the end of the BbQueue. (-1 will retru nthe last one)
 *
 *
 */

#ifndef bb_queue_H_INCLUDED
#define bb_queue_H_INCLUDED

/* Dependances */
#include <stdio.h>
#include <stdlib.h>

#ifdef BBQ_EXPORT_FUNCTION
	#ifdef WIN32
		#define EXPORT_FUNCTION __declspec(dllexport)
	#endif
#endif

#ifndef EXPORT_FUNCTION
	#define EXPORT_FUNCTION
#endif

/*
*   Working Macros
*/
/* BbChild */
#define __bb_child_get_next(child)	((child)->next)
#define __bb_child_get_prev(child)	((child)->prev)

/* BbQueue */
#define bb_queue_get_length(q)		  ((q)->len)
#define bb_queue_inc_length(q)		  ((q)->len++)
#define bb_queue_dec_length(q)		  ((q)->len--)
#define bb_queue_pick_last_child(q)	 ((q)->last)
#define bb_queue_pick_first_child(q)	((q)->first)
#define bb_queue_is_empty(q)			(bb_queue_get_length((q)) == 0)

#define break_foreach                 \
do {                                  \
	__child_foreach = (BbChild *) -1; \
	continue;                         \
} while (0)

#define foreach_bbqueue(queue, child) \
	for (                             \
		(child) = (queue)->first;     \
		(child) != NULL;              \
		(child) = (child)->next       \
	)


#define foreach_bbqueue_item(queue, item)					   \
	for (													   \
		BbChild *__child_foreach = (queue)->first,             \
		*__bb_loop = 0;                                        \
		__child_foreach != NULL;                               \
		__child_foreach = (__child_foreach != (BbChild *)-1) ? \
		 __child_foreach->next : NULL,                         \
		__bb_loop = (BbChild*)0                                \
	)                                                          \
		for (                                                  \
			item = __child_foreach->data;                      \
			__bb_loop != (BbChild*) 1;                         \
			__bb_loop = (BbChild*) 1                           \
		)

#define is_last_bbqueue_item(queue)      \
	((__child_foreach == (queue)->last))

#define foreach_bbqueue_item_raw(queue, item)				   \
	for (													   \
		BbChild *__child_foreach = (queue)->first,             \
		*__bb_loop = 0;                                        \
		__child_foreach != NULL;                               \
		__child_foreach = (__child_foreach != (BbChild *)-1) ? \
		 __child_foreach->next : NULL,                         \
		__bb_loop = (BbChild*)0                                \
	)                                                          \
		for (                                                  \
			item = (int) __child_foreach->data;                \
			__bb_loop != (BbChild*) 1;                         \
			__bb_loop = (BbChild*) 1                           \
		)

#define foreach_bbqueue_item_reversed(queue, item)             \
	for (                                                      \
		BbChild *__child_foreach = (queue)->last,              \
		*__bb_loop = 0;                                        \
		__child_foreach != NULL;                               \
		__child_foreach = (__child_foreach != (BbChild *)-1) ? \
		 __child_foreach->prev : NULL,                         \
		__bb_loop = (BbChild*)0                                \
	)                                                          \
		for (                                                  \
			item = __child_foreach->data;                      \
			__bb_loop != (BbChild*) 1;                         \
			__bb_loop = (BbChild*) 1                           \
		)


#define foreach_bbqueue_item_reversed_raw(queue, item)         \
	for (                                                      \
		BbChild *__child_foreach = (queue)->last,              \
		*__bb_loop = 0;                                        \
		__child_foreach != NULL;                               \
		__child_foreach = (__child_foreach != (BbChild *)-1) ? \
		 __child_foreach->prev : NULL,                         \
		__bb_loop = (BbChild*)0                                \
	)                                                          \
		for (                                                  \
			item = (int) __child_foreach->data;                \
			__bb_loop != (BbChild*) 1;                         \
			__bb_loop = (BbChild*) 1                           \
		)


#define bb_queue_add_array(queue, data, count)          \
do {                                                    \
	int __array_counter;                                \
	for (__array_counter = 0;                           \
		 __array_counter < (count);                     \
		 __array_counter++) {                           \
		bb_queue_add((queue), (data)[__array_counter]);	\
	}                                                   \
} while (0)

#define bb_queue_add_raw(queue, data)    \
	(bb_queue_add(queue, (void*)(data)))

#define bb_queue_local_decl() \
{                             \
	.len = 0,                 \
	.first = NULL,            \
	.last = NULL              \
}

/**
*   Typedef
*/

typedef
struct _BbChild
{
	void *data;
	struct _BbChild *next,
					*prev;
}	BbChild;

typedef
struct _BbQueue
{
	int len;

	BbChild *first,
			*last;

}	BbQueue;

	/**=================
		@Constructors
	===================*/

/* BbChild */
BbChild *
bb_child_new                 (void *data);

/* BbQueue */
BbQueue *
bb_queue_new                 (void);

BbQueue *
bb_queue_copy                (BbQueue *queue);

void
bb_queue_init                (BbQueue *q);

	/**=================
		  @Functions
	===================*/

/* BbChild */
BbChild *
bb_child_get_next            (BbChild *child);

BbChild *
bb_child_get_prev            (BbChild *child);

void
bb_child_debug               (BbChild *child);

void
bb_child_debug_string        (BbChild *child);

void
bb_child_debug_integer       (BbChild *child);

void
bb_child_debug_raw_integer   (BbChild *child);

/* BbQueue */
	/*! Add */
void
bb_queue_add                 (BbQueue *queue, void *data);
/* [head][][][][tail] <- [] */

void
bb_queue_push                (BbQueue *queue, void *data);
/* [] -> [head][][][][tail] */

void
bb_queue_add_nth             (BbQueue *queue, void *data, int pos);
/* [head][] -> [nth] -> [][tail] */

	/*! Get : remove from the BbQueue and return the data */
void *
bb_queue_get_first           (BbQueue *queue);
/* [head] <- [][][][tail] */

void *
bb_queue_pop                 (BbQueue *queue);
/* [head][][][] -> [tail] */

void *
bb_queue_get_nth             (BbQueue *queue, int nth);
/* [head][] - - [nth] - -> [][tail] */

void *
bb_queue_replace_nth         (BbQueue *queue, void *data, int nth);

void
bb_queue_replace             (BbQueue *queue, void *data1, void *data2);

	/*!
		- Pick : Same as get, but prevent removing the child from the BbQueue
		- Getters of the BbQueue on Read-Only
	*/
void *
bb_queue_pick_nth            (BbQueue *queue, int pos);

void *
bb_queue_pick_first          (BbQueue *queue);

void *
bb_queue_pick_last           (BbQueue *queue);

	/*! Child Setters : For optimizations */

void
bb_queue_add_child           (BbQueue *queue, BbChild *c);

	/*! Child Getters : For optimizations */

BbChild *
bb_queue_pick_child          (BbQueue *queue, void *data);

BbChild *
bb_queue_pick_child_nth      (BbQueue *queue, int nth);

	/*! Removal functions */
int
bb_queue_remv                (BbQueue *queue, void *data);

void *
bb_queue_remv_nth            (BbQueue *queue, int nth);

	/*! Debugging functions */
void
bb_queue_debug               (BbQueue *queue);

void
bb_queue_debug_string        (BbQueue *queue);

void
bb_queue_debug_integer       (BbQueue *queue);

void
bb_queue_debug_custom        (BbQueue *queue, void (*debug_function)());

void
bb_queue_debug_custom_data   (BbQueue *queue, void (*debug_function)());

void
bb_queue_debug_raw_integer   (BbQueue *queue);

	/*! Misc functions */

void
bb_queue_insert_after_child	 (BbQueue *queue, BbChild *before, void *data);

void
bb_queue_insert_before_child (BbQueue *queue, BbChild *after, void *data);

void
bb_queue_insert_after        (BbQueue *queue, void *after, void *data);

void
bb_queue_insert_before       (BbQueue *queue, void *before, void *data);

void
bb_queue_switch              (BbQueue *queue, void *data1, void *data2);

int
bb_queue_exists	             (BbQueue *queue, void *data);

void
bb_queue_do                  (BbQueue *queue, void (* do_func)());

void
bb_queue_do_enumerate        (BbQueue *queue, void (* do_func)());

void
bb_queue_do_enumerate_data   (BbQueue *q, void (*do_func)());

void
bb_queue_concat				 (BbQueue *queue_dest, BbQueue *queue);

void
bb_queue_concat_nth          (BbQueue *q1, BbQueue *q2, int nth);

int
bb_queue_put_first           (BbQueue *queue, void *data);

int
bb_queue_put_last            (BbQueue *queue, void *data);

	/*! Sort functions */

void
bb_queue_bubble_sort         (BbQueue *queue);

	/**=================
		@Destructors
	====================*/

/* BbChild */
void
bb_child_unref               (BbChild *child);

/* BbQueue */
void
bb_queue_free                (BbQueue *queue);

void
bb_queue_free_all            (BbQueue *queue, void (*free_func)());

void
bb_queue_clear               (BbQueue *queue);

void
bb_queue_free_elements       (BbQueue *q, void (* free_func)());

#endif
