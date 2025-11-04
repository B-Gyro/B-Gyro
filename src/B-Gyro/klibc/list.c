#include "memory/malloc/chunks.h"


void	printPages() {
	_ptr	*ptr = g_head;
	_page	*page;
	size_t	i = 0;

	if (!ptr)
		return;
	VGA_PRINT("==>PAGES=========================\n");
	while (ptr) {
		page = (_page *)(ptr);
		// VGA_PRINT(page->data);
		if (page->type == LARGE)
			VGA_PRINT("LARGE\n");
		else if (page->type == SMALL)
			VGA_PRINT("SMALL\n");
		else
			VGA_PRINT("TINY\n");
		ptr = ptr->next;
		i++;
	}
	VGA_PRINT("size: =========================\n%d\n", i);
}

// void	printList(_ptr **listHead, short offset) {
// 	_ptr	*ptr;
// 	_metaData	*node;

// 	if (!listHead || !*listHead)
// 		return;
// 	ptr = *listHead;
// 	while (ptr) {
// 		node = (_metaData *)(ptr - offset);
// 		// VGA_PRINT(node->data);
// 		VGA_PRINT("size :");
// 		printNumber((size_t)node->size, 10);
// 		VGA_PRINT("\n");
// 		if (node->isFree)
// 			VGA_PRINT("FREE\n");
// 		else
// 			VGA_PRINT("USED\n");

// 		ptr = ptr->next;
// 	}
// 	VGA_PRINT("=======================================\n");
	
// }

void	insertNode(_ptr **listHead, _ptr *node) {
	if (!listHead || !node)
		return;

	if (*listHead)
		(*listHead)->previous = node;
	node->next = *listHead;
	node->previous = NULL;
	*listHead = node;
}

void	insertNodeInList(_list *list, _node *node) {
	if (!list || !node)
		return;

	if (!list->first)
		list->first = node;
	else {
		list->last->next = node;
		node->previous = list->last;
	}

	list->last = node;
	list->size++;
}

void	makeItCircularList(_list *list) {
	list->last->next = list->first;
	list->first->previous = list->last;
}

void	deleteNode(_ptr **listHead, _ptr *node) {
	_ptr	*next, *prv;

	if (!listHead || !*listHead || !node)
		return;

	next = node->next;
	prv = node->previous;

	if (!prv) {
		(*listHead) = node->next;
	} else
		prv->next = node->next;

	if (next)
		next->previous = prv;

	// PROTECTION
	node->next = NULL;
	node->previous = NULL;
}