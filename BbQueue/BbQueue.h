/**
 *  @author	 :   Spl3en
 *  @file	   :   BbQueue.h
 *  @version	:   1.8
 *
 *  BbQueue fournit des fonctions de file et de pile.
 *  Elle permet de manière simple et efficace d'ajouter ou de retirer un élément d'une liste.
 *
 *  Sa manière d'utilisation est double :
 *	  - Il est possible de l'utiliser comme une file/pile, via bbq_add / bbq_push / bbq_pop et ses dérivées
 *	  - Il est possible de l'utiliser comme une liste doublement chaînée, en récupérant tout d'abord le BbChild correspondant via bb_queue_pick_child et dérivées
 *	  Ainsi, les accesseurs bb_child_get_next/_prev sont disponibles, et les données pointées par ses childs par ->data.
 *	  A noter que __bb_child_get_next/_prev sont plus rapides, mais crashera le programme si le BbChild passé en paramètre est invalide (pas de check).
 *
 *	Changelog :
 *
 *		[+] v1.1 :		(10 / 11 / 2010)
 *			[ADD]	   bb_queue_remv_nth (BbQueue *, int nth)
 *			[ADD]	   bb_queue_get_nth  (BbQueue *, int nth)
 *		  [FIX]	   bb_queue_remv : Length fixed
 *
 *	  [+] v1.2		(18 / 11 / 2010)
 *		  [ADD]	   La structure contient maintenant deux nouveau éléments : index et current
 *					  - index est un simple entier; Il est modifiable à la convenance de l'utilisateur. Il est utile quand il faut se rappeler de l'index d'un élément.
 *					  - current est un pointeur sur le dernier BbChild pické.
 *					  Il est utile dans le cas où l'on voudrait accéder au BbChild suivant via q->current->next sans devoir utiliser bb_queue_pick_nth,
 *					  dans le cas où le pointeur sur BbChild serait perdu.
 *
 *		  [ADD]	   bb_queue_get_current (BbQueue *queue) : retourne la data pointé par le BbChild current de façon propre, ou NULL s'il n'existe pas
 *					  bb_queue_pick_next (BbQueue *queue, int refresh) : Retourne la data pointé par le BbChild suivant du current. refresh sert à spécifier si le pointeur courant doit être actualisé ou non
 *					  bb_queue_pick_prev (BbQueue *queue, int refresh) : idem que bb_queue_get_next pour le BbChild précédent.
 *
 *		  [ADD]	   bb_queue_free_all (BbQueue *queue, (void *) free_func()); : Permet de libérer la mémoire de la BbQueue ainsi que celle des éléments enfilés.
 *					  Il s'agit de la fonction la plus efficace pour effacer l'ensemble des données liées à la BbQueue.
 *
 *	  [+] v1.3		(03 / 12 / 2010)
 *		  [ADD]	   bb_queue_do (BbQueue *queue, void (* do_func)()) : Permet d'effectuer une fonction sur l'ensemble de tous les éléments de la BbQueue
 *		  [ADD]	   bb_queue_replace_nth : Remplace une data par une autre à un index fixé, et renvoie celle removée
 *		  [ADD]	   bb_queue_replace	 : Remplace une data par une autre
 *		  [FIX]	   bb_queue_add_nth	 : fix de la longueur de la liste chaînée
 *			[ADD]		foreach_bb_queue_item : foreach implémenté pour la BbQueue
 *			[ADD]		break_foreach : la boucle foreach ne peut pas être breakée avec un simple "break". Il *faut* utiliser break_foreach.
 *
 *	  [+] v.1.4	   (07 / 02 / 2011)
 *		  [FIX]	   bb_queue_pop : Correction d'un bug de conception (!)
 *		  [FIX]	   bb_queue_pick_first : La macro a été implémentée en fonction pour raison de sécurité
 *
 *	  [+] v1.5		(12 / 03 / 2011)
 *		  [FIX]	   Pointeur "current" et fonctions associées (cf v1.2) deprecated, mauvaise conception et aucune utilité
 *		  [FIX]	   Fusion de BbQueue.c et BbChild.c pour des raisons pratiques
 *		  [ADD]	   bb_queue_add_array : transforme le tableau 1 dimension en BbQueue, en respectant l'ordre croissant
 *		  [ADD]	   bb_queue_copy : Copie une BbQueue
 *
 *	  [+] v1.6		(19 / 10 / 2011)
 *		  [FIX]	   Optimisation de bb_queue_insert_before/after
 *		  [ADD]	   bb_queue_insert_before/after_child()
 *		  [ADD]	   bb_queue_concat
 *			[ADD]		foreach_bb_queue_item_reversed
 *
 *	  [+] v1.7		(08 / 01 / 2012)
 *		  [FIX]	   Fix d'un bug mineur sur bb_queue_foreach_*
 *		  [ADD]	   bb_queue_put_last : prend un élément et le met à la fin, équivalent optimisé d'un remv + add
 *		  [ADD]	   bb_queue_put_first : prend un élément et le met au début, équivalent optimisé d'un remv + push
 *		  [ADD]	   bb_queue_concat_nth : concatène 2 BbQueue à partir de la position nth
 *		  [FIX]	   bb_queue_remv_nth : Dans le cas où la data "nth" existait en double dans la liste, la première rencontrée dans la liste était supprimée sans tenir compte du nthième
 *		  [FIX]	   bb_queue_add_nth : Le child n'était pas inséré à la bonne position, il y avait un offset de +1
 *		  [FIX]	   bb_queue_remv : Les pointeurs first et last n'étaient pas correctement remis à NULL si la taille de la bb_queue est égale à 1 avant remv
 *
 *
 *		[+] v1.8		(06 / 03 / 2012)
 *			[ADD]		bb_queue_pick_nth : accepte dorénavant les entiers négatifs pour deuxième argument; L'élément pické sera pris à partir de la fin de la BbQueue. (-1 renverra le dernier)
 *
 *
 */

#ifndef bb_queue_H_INCLUDED
#define bb_queue_H_INCLUDED

/* Dépendances */
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

#define break_foreach					   	\
do {										\
	__child_foreach = (BbChild *) -1;	   	\
	continue;							   	\
} while (0)								   	\

#define foreach_bbqueue(queue, child)			   	\
	for (										   	\
		(child) = (queue)->first;					   \
		(child) != NULL;							  	\
		(child) = (child)->next						 \
	)											   	\


#define foreach_bbqueue_item(queue, item)					   \
	for (													   \
		BbChild *__child_foreach = (queue)->first,			  \
		*__bb_loop = 0;										 \
		__child_foreach != NULL;								\
		__child_foreach = (__child_foreach != (BbChild *)-1) ?  \
		 __child_foreach->next : NULL,						  \
		__bb_loop = (BbChild*)0								 \
	)														   \
		for (												   \
			item = __child_foreach->data;						\
			__bb_loop != (BbChild*) 1;						  \
			__bb_loop = (BbChild*) 1							\
		)													   \

#define is_last_bbqueue_item(queue)		\
	((__child_foreach == (queue)->last))

#define foreach_bbqueue_item_raw(queue, item)				   \
	for (													   \
		BbChild *__child_foreach = (queue)->first,			  \
		*__bb_loop = 0;										 \
		__child_foreach != NULL;								\
		__child_foreach = (__child_foreach != (BbChild *)-1) ?  \
		 __child_foreach->next : NULL,						  \
		__bb_loop = (BbChild*)0								 \
	)														   \
		for (												   \
			item = (int) __child_foreach->data;				\
			__bb_loop != (BbChild*) 1;						  \
			__bb_loop = (BbChild*) 1							\
		)													   \

#define foreach_bbqueue_item_reversed(queue, item)			  \
	for (													   \
		BbChild *__child_foreach = (queue)->last,				\
		*__bb_loop = 0;										 \
		__child_foreach != NULL;								\
		__child_foreach = (__child_foreach != (BbChild *)-1) ?  \
		 __child_foreach->prev : NULL,						  \
		__bb_loop = (BbChild*)0								 \
	)														   \
		for (												   \
			item = __child_foreach->data;					 \
			__bb_loop != (BbChild*) 1;						  \
			__bb_loop = (BbChild*) 1							\
		)													   \


#define foreach_bbqueue_item_reversed_raw(queue, item)			\
	for (													   	\
		BbChild *__child_foreach = (queue)->last,				\
		*__bb_loop = 0;										 	\
		__child_foreach != NULL;								\
		__child_foreach = (__child_foreach != (BbChild *)-1) ?  \
		 __child_foreach->prev : NULL,						  	\
		__bb_loop = (BbChild*)0								 	\
	)														   	\
		for (												   	\
			item = (int) __child_foreach->data;			 	\
			__bb_loop != (BbChild*) 1;						  	\
			__bb_loop = (BbChild*) 1							\
		)													   	\


#define bb_queue_add_array(queue, data, count)	  	\
do {													\
	int __array_counter;								\
	for (__array_counter = 0;					   	\
		 __array_counter < (count);				  	\
		 __array_counter++) {					   	\
		bb_queue_add((queue), (data)[__array_counter]);	\
	}											   	\
} while (0)										 	\

#define bb_queue_add_raw(queue, data) 		\
	(bb_queue_add(queue, (void*)(data)))

#define bb_queue_local_decl() \
{\
	.len = 0,\
	.first = NULL,\
	.last = NULL\
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
bb_child_new					(void *data);

/* BbQueue */
BbQueue *
bb_queue_new					(void);

BbQueue *
bb_queue_copy				   (BbQueue *queue);

void
bb_queue_init 					(BbQueue *q);

	/**=================
		  @Methods
	===================*/

/* BbChild */
BbChild *
bb_child_get_next			   (BbChild *child);

BbChild *
bb_child_get_prev			   (BbChild *child);

void
bb_child_debug				  (BbChild *child);

void
bb_child_debug_string		   (BbChild *child);

void
bb_child_debug_integer		  (BbChild *child);

void
bb_child_debug_raw_integer	  (BbChild *child);

/* BbQueue */
	/*! Add */
void
bb_queue_add					(BbQueue *queue, void *data);
/* [head][][][][tail] <- [] */

void
bb_queue_push				   (BbQueue *queue, void *data);
/* [] -> [head][][][][tail] */

void
bb_queue_add_nth				(BbQueue *queue, void *data, int pos);
/* [head][] -> [nth] -> [][tail] */

	/*! Get : remove from the BbQueue and return the data */
void *
bb_queue_get_first			  (BbQueue *queue);
/* [head] <- [][][][tail] */

void *
bb_queue_pop					(BbQueue *queue);
/* [head][][][] -> [tail] */

void *
bb_queue_get_nth				(BbQueue *queue, int nth);
/* [head][] - - [nth] - -> [][tail] */

void *
bb_queue_replace_nth			(BbQueue *queue, void *data, int nth);

void
bb_queue_replace				(BbQueue *queue, void *data1, void *data2);

	/*!
		- Pick : Same as get, but prevent removing the child from the BbQueue
		- Accessors of the BbQueue on Read-Only
	*/
void *
bb_queue_pick_nth			   (BbQueue *queue, int pos);

void *
bb_queue_pick_first 			(BbQueue *queue);

void *
bb_queue_pick_last			  (BbQueue *queue);

	/*! Child Accessors : For optimizations */

void
bb_queue_add_child 				(BbQueue *queue, BbChild *c);

BbChild *
bb_queue_pick_child			 (BbQueue *queue, void *data);

BbChild *
bb_queue_pick_child_nth		 (BbQueue *queue, int nth);

	/*! Removal functions */
int
bb_queue_remv				   (BbQueue *queue, void *data);

void *
bb_queue_remv_nth			   (BbQueue *queue, int nth);

	/*! Debugging functions */
void
bb_queue_debug				  (BbQueue *queue);

void
bb_queue_debug_string		   (BbQueue *queue);

void
bb_queue_debug_integer		  (BbQueue *queue);

void
bb_queue_debug_custom		   (BbQueue *queue, void (*debug_function)());

void
bb_queue_debug_custom_data 		(BbQueue *queue, void (*debug_function)());

void
bb_queue_debug_raw_integer	  (BbQueue *queue);

	/*! Misc functions */

void
bb_queue_insert_after_child	 (BbQueue *queue, BbChild *before, void *data);

void
bb_queue_insert_before_child	(BbQueue *queue, BbChild *after, void *data);

void
bb_queue_insert_after		   (BbQueue *queue, void *after, void *data);

void
bb_queue_insert_before		  (BbQueue *queue, void *before, void *data);

void
bb_queue_switch				 (BbQueue *queue, void *data1, void *data2);

int
bb_queue_exists				 (BbQueue *queue, void *data);

void
bb_queue_do					 (BbQueue *queue, void (* do_func)());

void
bb_queue_do_enumerate		   (BbQueue *queue, void (* do_func)());

void
bb_queue_do_enumerate_data 		(BbQueue *q, void (*do_func)());

void
bb_queue_concat				 (BbQueue *queue_dest, BbQueue *queue);

void
bb_queue_concat_nth 			(BbQueue *q1, BbQueue *q2, int nth);

int
bb_queue_put_first 				(BbQueue *queue, void *data);

int
bb_queue_put_last 				(BbQueue *queue, void *data);

	/*! Sort functions */

void
bb_queue_bubble_sort	   		(BbQueue *queue);

	/**=================
		@Destructors
	====================*/

/* BbChild */
void
bb_child_unref				  (BbChild *child);

/* BbQueue */
void
bb_queue_free				   (BbQueue *queue);

void
bb_queue_free_all			   (BbQueue *queue, void (*free_func)());

void
bb_queue_clear				  (BbQueue *queue);

#endif
