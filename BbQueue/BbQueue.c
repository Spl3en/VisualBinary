#include "BbQueue.h"

EXPORT_FUNCTION BbQueue *
bb_queue_new (void)
{
	BbQueue *q = malloc (sizeof(BbQueue));

	bb_queue_init(q);

	return q;
}

EXPORT_FUNCTION void
bb_queue_init (BbQueue *q)
{
	q->len   = 0;
	q->first = NULL;
	q->last  = NULL;
}

EXPORT_FUNCTION BbQueue *
bb_queue_copy (BbQueue *q)
{
	BbQueue *cp 	= bb_queue_new();
	void 	*item 	= NULL;

	foreach_bbqueue_item (q, item)
	{
		bb_queue_add (cp, item);
	}

	return cp;
}

EXPORT_FUNCTION void
bb_queue_add (BbQueue *q, void *data)
{
	BbChild *c = bb_child_new(data);

	bb_queue_add_child(q, c);
}

EXPORT_FUNCTION void
bb_queue_add_child (BbQueue *q, BbChild *c)
{
	if (bb_queue_get_length(q) == 0)
	{
		q->first = c;
		q->last  = c;
	}

	else
	{
		q->last->next = c;
		c->prev = q->last;
		q->last = c;
	}

	bb_queue_inc_length(q);
}

EXPORT_FUNCTION void
bb_queue_add_nth (BbQueue *q, void *data, int pos)
{
	if (pos < 0)
	{
		printf("%s : pos < 0\n", __FUNCTION__);
		return;
	}

	BbChild *childPrev = NULL;
	BbChild *childNext = NULL;
	BbChild *c = bb_child_new(data);

	int len = bb_queue_get_length(q);

	if (pos > len || len == 0)
	{
		bb_queue_add(q, data);
		return;
	}

	if (pos == 0)
	{
		c->next = q->first;
		q->first->prev = c;
		q->first = c;
	}

	else if (pos == len)
	{
		c->prev = q->last;
		q->last->next = c;
		q->last = c;
	}

	else
	{
		childPrev = bb_queue_pick_child_nth(q, pos);

		if (childPrev == NULL)
			childPrev = bb_queue_pick_last_child(q);

		childNext = childPrev->next;

		childPrev->next = c;
		c->prev = childPrev;

		if (childNext != NULL)
			childNext->prev = c;

		c->next = childNext;
	}

	bb_queue_inc_length(q);
}

/* Starts at 0 */
EXPORT_FUNCTION void *
bb_queue_pick_nth (BbQueue *q, int pos)
{
	if (pos < 0)
		pos = bb_queue_get_length(q) + pos + 1;

	if (pos >= 0)
	{
		if (bb_queue_get_length(q) < pos + 1)
		{
			printf("%s: queue is not long enough\n", __FUNCTION__);
			return NULL;
		}
	}

	BbChild *c = NULL;

	c = bb_queue_pick_child_nth(q, pos);

	if (c == NULL)
		return NULL;

	return c->data;
}

EXPORT_FUNCTION void *
bb_queue_replace_nth (BbQueue *q, void *data, int nth)
{
	BbChild *c = bb_queue_pick_child_nth(q, nth);
	void *ret = NULL;

	if (c  == NULL)
	{
		printf("%s : child is NULL\n", __FUNCTION__);
		return NULL;
	}

	ret = c->data;
	c->data = data;

	return ret;
}

EXPORT_FUNCTION void
bb_queue_replace (BbQueue *q, void *data1, void *data2)
{
	BbChild *c = bb_queue_pick_child(q, data1);
	if (c == NULL)
	{
		printf("%s : child is NULL\n", __FUNCTION__);
		return;
	}

	c->data = data2;
}

EXPORT_FUNCTION int
bb_queue_put_last (BbQueue *q, void *data)
{
	BbChild *c = NULL;

	if (bb_queue_get_length(q) == 1)
	{
		// Il n'y a qu'un seul élément, celui recherché;
		// Il est donc déjà à la fin
		return 1;
	}

	c = bb_queue_pick_child(q, data);

	if (c == NULL)
		return 0;

	if (q->last == c)
	{
		// Déjà à la fin
		return 1;
	}

	if (q->first == c)
	{
		q->first = q->first->next;
		q->first->prev = NULL;
	}

	else
	{
		c->next->prev = c->prev;
		c->prev->next = c->next;
	}

	q->last->next = c;
	c->prev = q->last;
	q->last = c;
	c->next = NULL;

	return 1;
}

EXPORT_FUNCTION int
bb_queue_put_first (BbQueue *q, void *data)
{
	BbChild *c = NULL;

	if (bb_queue_get_length(q) == 1)
	{
		// Il n'y a qu'un seul élément, celui recherché;
		// Il est donc déjà au début
		return 1;
	}

	c = bb_queue_pick_child(q, data);

	if (c == NULL)
		return 0;

	if (q->first == c)
	{
		// Déjà au début
		return 1;
	}

	if (q->last == c)
	{
		q->last = q->last->prev;
		q->last->next = NULL;
	}

	else
	{
		c->prev->next = c->next;
		c->next->prev = c->prev;
	}

	q->first->prev = c;
	c->next = q->first;
	q->first = c;
	c->prev = NULL;

	return 1;
}

EXPORT_FUNCTION int
bb_queue_remv (BbQueue *q, void *data)
{
	BbChild *c = NULL;

	c = bb_queue_pick_child(q, data);

	if (c == NULL)
		return 0;

	if (bb_queue_get_length(q) == 1)
	{
		q->first = NULL;
		q->last = NULL;
	}

	else if (q->first == c)
	{
		q->first = c->next;
		q->first->prev = NULL;
	}

	else if (q->last == c)
	{
		q->last = c->prev;
		q->last->next = NULL;
	}

	else
	{
		c->next->prev = c->prev;
		c->prev->next = c->next;
	}

	bb_child_unref(c);
	bb_queue_dec_length(q);

	return 1;
}

EXPORT_FUNCTION void *
bb_queue_remv_nth (BbQueue *q, int nth)
{
	void *data = NULL;
	BbChild *c = NULL;

	c = bb_queue_pick_child_nth(q, nth);

	if (c == NULL)
		return NULL;

	data = c->data;

	if (bb_queue_get_length(q) == 1)
	{
		q->first = NULL;
		q->last = NULL;
	}

	else if (q->first == c)
	{
		q->first = c->next;
		q->first->prev = NULL;
	}

	else if (q->last == c)
	{
		q->last = c->prev;
		q->last->next = NULL;
	}

	else
	{
		c->next->prev = c->prev;
		c->prev->next = c->next;
	}

	bb_child_unref(c);
	bb_queue_dec_length(q);

	return data;
}

EXPORT_FUNCTION int
bb_queue_exists (BbQueue *q, void *data)
{
	BbChild *c = NULL;

	foreach_bbqueue(q, c)
	{
		if (c->data == data)
			return 1;
	}

	return 0;
}

EXPORT_FUNCTION void
bb_queue_push (BbQueue *q, void *data)
{
	BbChild *c = bb_child_new(data);

	if (bb_queue_get_length(q) == 0)
	{
		q->first = c;
		q->last = c;
	}

	else
	{
		q->first->prev = c;
		c->next = q->first;
		q->first = c;
	}

	bb_queue_inc_length(q);
}

EXPORT_FUNCTION void *
bb_queue_get_first (BbQueue *q)
{
	if (bb_queue_get_length(q) <= 0)
		return NULL;

	BbChild *c = q->first;
	void *data = c->data;

	if (bb_queue_get_length(q) == 1)
	{
		q->first = NULL;
		q->last = NULL;
	}

	else
	{
		q->first->next->prev = NULL;
		q->first = q->first->next;
	}

	bb_queue_dec_length(q);
	bb_child_unref(c);

	return data;
}

EXPORT_FUNCTION void *
bb_queue_pop (BbQueue *q)
{
	if (bb_queue_get_length(q) <= 0)
		return NULL;

	BbChild *c = bb_queue_pick_last_child(q);

	void *data = c->data;

	if (bb_queue_get_length(q) == 1)
	{
		q->first = NULL;
		q->last = NULL;
	}

	else
	{
		q->last->prev->next = NULL;
		q->last = q->last->prev;
	}

	bb_queue_dec_length(q);
	bb_child_unref(c);

	return data;
}

EXPORT_FUNCTION int
bb_queue_get_index (BbQueue *q, void *data)
{
	BbChild *c;
	int index = 0;

	foreach_bbqueue(q, c)
	{
		if (c->data == data)
			return index;

		index++;
	}

	return -1;
}

EXPORT_FUNCTION void
bb_queue_insert_after_bb_child (BbQueue *q, BbChild *before, void *data)
{
	BbChild *newchild = NULL;

	if (before != bb_queue_pick_last_child(q))
	{
		newchild = bb_child_new(data);

		before->next->prev = newchild;
		newchild->next = before->next;
		newchild->prev = before;
		before->next = newchild;

		bb_queue_inc_length(q);
	}

	else
		bb_queue_add(q, data);
}

EXPORT_FUNCTION void
bb_queue_insert_after (BbQueue *q, void *before, void *data)
{
	BbChild *before_child = bb_queue_pick_child(q, before);

	bb_queue_insert_after_bb_child(q, before_child, data);
}


EXPORT_FUNCTION void
bb_queue_insert_before_bb_child (BbQueue *q, BbChild *after, void *data)
{
	BbChild *newchild = NULL;

	if (after != bb_queue_pick_first_child(q))
	{
		newchild = bb_child_new(data);

		after->prev->next = newchild;
		newchild->prev = after->prev;
		newchild->next = after;
		after->prev = newchild;

		bb_queue_inc_length(q);
	}

	else
		bb_queue_push(q, data);
}


EXPORT_FUNCTION void
bb_queue_insert_before (BbQueue *q, void *after, void *data)
{
	BbChild *before_child = bb_queue_pick_child(q, after);

	bb_queue_insert_after_bb_child(q, before_child, data);
}

EXPORT_FUNCTION void *
bb_queue_pick_first (BbQueue *q)
{
	BbChild *child;

	child = bb_queue_pick_first_child(q);

	if (child == NULL)
		return NULL;

	return child->data;
}

EXPORT_FUNCTION void *
bb_queue_pick_last (BbQueue *q)
{
	BbChild *child;

	child = bb_queue_pick_last_child(q);

	if (child == NULL)
		return NULL;

	return child->data;
}

EXPORT_FUNCTION void *
bb_queue_get_nth (BbQueue *q, int nth)
{
	void *data = bb_queue_pick_nth(q, nth);

	if (data != NULL)
		bb_queue_remv(q, data);

	return data;
}

EXPORT_FUNCTION void
bb_queue_switch (BbQueue *q, void *data1, void *data2)
{
	BbChild *c1 = bb_queue_pick_child(q, data1);
	BbChild *c2 = bb_queue_pick_child(q, data2);

	if (c1 == NULL || c2 == NULL)
	{
		printf("%s : child is NULL\n", __FUNCTION__);
		return;
	}

	c1->data = data2;
	c2->data = data1;
}

EXPORT_FUNCTION void
bb_queue_debug (BbQueue *q)
{
	bb_queue_do_enumerate(q, bb_child_debug);
	printf("\n");
}

EXPORT_FUNCTION void
bb_queue_debug_custom (BbQueue *q, void (*debug_function)())
{
	bb_queue_do_enumerate(q, debug_function);
	printf("\n");
}

EXPORT_FUNCTION void
bb_queue_debug_custom_data (BbQueue *q, void (*debug_function)())
{
	bb_queue_do_enumerate_data (q, debug_function);
	printf("\n");
}

EXPORT_FUNCTION void
bb_queue_debug_string (BbQueue *q)
{
	bb_queue_do_enumerate(q, bb_child_debug_string);
	printf("\n");
}

EXPORT_FUNCTION void
bb_queue_debug_integer (BbQueue *q)
{
	bb_queue_do_enumerate(q, bb_child_debug_integer);
	printf("\n");
}

EXPORT_FUNCTION void
bb_queue_debug_raw_integer (BbQueue *q)
{
	bb_queue_do_enumerate(q, bb_child_debug_raw_integer);
	printf("\n");
}

EXPORT_FUNCTION BbChild *
bb_queue_pick_child (BbQueue *q, void *data)
{
	BbChild *c = NULL;
	int pos = 0;

	foreach_bbqueue(q, c)
	{
		if (c->data == data)
		{
			return c;
		}

		pos++;
	}

	return NULL;
}

EXPORT_FUNCTION BbChild *
bb_queue_pick_child_nth (BbQueue *q, int pos)
{
	if (pos < 0)
	{
		printf("%s : pos is < 0\n", __FUNCTION__);
		return NULL;
	}

	BbChild *c = NULL;

	int loop,
		len = bb_queue_get_length(q);

	if (len == pos)
		return bb_queue_pick_last_child(q);

	if (pos == 0)
		return bb_queue_pick_first_child(q);

	if (pos <= (len / 2))
	{
		loop = 1;

		foreach_bbqueue(q, c)
		{
			if (loop++ == pos)
				return c;
		}
	}

	else
	{
		loop = len;

		for (c = q->last;
			 c != NULL;
			 c = c->prev)
		{
			if (loop-- == pos)
				return c;
		}
	}

	return NULL;
}

EXPORT_FUNCTION void
bb_queue_concat (BbQueue *q1, BbQueue *q2)
{
	void *data = NULL;

	foreach_bbqueue_item(q2, data)
	{
		bb_queue_add(q1, data);
	}
}

EXPORT_FUNCTION void
bb_queue_concat_nth (BbQueue *q1, BbQueue *q2, int nth)
{
	void *data = NULL;

	foreach_bbqueue_item_reversed(q2, data)
	{
		bb_queue_add_nth(q1, data, nth);
	}
}

EXPORT_FUNCTION void
bb_queue_free (BbQueue *p)
{
	int i;
	int len = bb_queue_get_length(p);

	for (i = 0; i < len; i++)
	{
		// bb_queue_pop s'occupe de free le bbchild
		bb_queue_pop(p);
	}

	free(p);
}

EXPORT_FUNCTION void bb_queue_bubble_sort (BbQueue *q)
{
	int en_desordre = 1;
	BbChild *child = NULL;
	void *tmp;
	int *tj, *tj2;

	while (en_desordre)
	{
		en_desordre = 0;

		for (child = q->first; child != q->last; child = child->next)
		{
			tj = child->data;
			tj2 = child->next->data;

			if (*tj > *tj2)
			{
				tmp = child->next->data;
				child->next->data = child->data;
				child->data = tmp;
				en_desordre = 1;
			}
		}
	}
}

EXPORT_FUNCTION void
bb_queue_do (BbQueue *q, void (*do_func)())
{
	BbChild *child;

	foreach_bbqueue(q, child)
	{
		do_func(child);
	}
}

EXPORT_FUNCTION void
bb_queue_do_enumerate (BbQueue *q, void (*do_func)())
{
	BbChild *child;
	int i = 0;

	foreach_bbqueue(q, child)
	{
		printf("[%d] : ", i++);
		do_func(child);
	}
}

EXPORT_FUNCTION void
bb_queue_do_enumerate_data (BbQueue *q, void (*do_func)())
{
	BbChild *child;
	int i = 0;

	foreach_bbqueue(q, child)
	{
		printf("[%d] : ", i++);
		do_func(child->data);
	}
}

EXPORT_FUNCTION void
bb_queue_free_all (BbQueue *q, void (* free_func)())
{
	void *data = NULL;

	while (bb_queue_get_length(q))
	{
		data = bb_queue_pop(q);
		free_func(data);
	}

	free(q);
}

EXPORT_FUNCTION void
bb_queue_clear (BbQueue *q)
{
	while (bb_queue_get_length(q))
	{
		bb_queue_pop(q);
	}
}

/**===========================
	  BbChild functions
==============================*/

EXPORT_FUNCTION BbChild *
bb_child_new (void *data)
{
	BbChild *p = malloc (sizeof(BbChild));
	if (p == NULL)
		return NULL;

	p->next = NULL;
	p->prev = NULL;
	p->data = data;

	return p;
}

EXPORT_FUNCTION BbChild *
bb_child_get_next (BbChild *child)
{
	if (child == NULL)
		return NULL;

	return __bb_child_get_next(child);
}

EXPORT_FUNCTION BbChild *
bb_child_get_prev (BbChild *child)
{
	if (child == NULL)
		return NULL;

	return __bb_child_get_prev(child);
}

EXPORT_FUNCTION void
bb_child_debug (BbChild *child)
{
	printf("0x%x\n", (int)child->data);
}

EXPORT_FUNCTION void
bb_child_debug_string (BbChild *child)
{
	printf("%s\n", (char *)child->data);
}

EXPORT_FUNCTION void
bb_child_debug_integer (BbChild *child)
{
	printf("%d\n", (*(int*)child->data));
}

EXPORT_FUNCTION void
bb_child_debug_raw_integer (BbChild *child)
{
	printf("%d\n", (int)child->data);
}

EXPORT_FUNCTION void
bb_child_unref (BbChild *p)
{
	free(p);
}
