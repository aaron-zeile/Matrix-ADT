/***************************************
**** Name: Aaron Zeile *****************
**** ID: azeile ************************
**** Assignment: pa4 *******************
***************************************/
// List.c
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj *Node;

// private NodeObj type
typedef struct NodeObj {
	ListElement data;
	Node next;
	Node prev;
} NodeObj;

// private ListObj type
typedef struct ListObj {
	Node cursor;
	Node front;
	Node back;
	int length;
	int cursorIndex;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

// creates a new node
Node newNode(ListElement data) {
	Node n = malloc(sizeof(NodeObj));
	assert(n != NULL);
	n->data = data;
	n->next = NULL;
	n->prev = NULL;
	return n;
}
// frees heap memory pointed at by *pN and sets *pN to NULL
void freeNode(Node* pN) {
	if (pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

// creates a new empty list
List newList(void) {
	List L = malloc(sizeof(ListObj));
	assert(L != NULL);
	L->front = L->back = NULL;
	L->cursor = NULL;
	L->length = 0;
	L->cursorIndex = -1;
	return L;
}

// frees heap memory pointed at by *pL and sets *pL to NULL
void freeList(List *pL) {
	if (pL != NULL && *pL != NULL) {
		while (length(*pL) > 0) {
			deleteFront(*pL);
		}
		free(*pL);
		*pL = NULL;
	}
}

// Access functions -----------------------------------------------------------

// get length of list
int length(List L) {
	if (L == NULL) {
		fprintf(stderr, "Error. Trying to get length of NULL list.\n");
		exit(EXIT_FAILURE);
	}
	return L->length;
}

// get index of cursor
int index(List L) {
	// if cursor is undefined
	if (L->cursor == NULL) {
		L->cursorIndex = -1;
		return L->cursorIndex;
	}
	// if cursor is defined
	else {
		return L->cursorIndex; 
	}

}

// return front of list
void* front(List L) {
	// Preconditions
	if (L == NULL) {
		fprintf(stderr, "Error. Trying to get front of NULL list.\n");
		exit(EXIT_FAILURE);
	}
	else if (!(length(L) > 0)) {
		fprintf(stderr, "Error. Trying to get front of empty list.\n");
		exit(EXIT_FAILURE);
	}
	// return data of front node
	return L->front->data;
}

// return back element
void* back(List L) {
	if (!(length(L) > 0)) {
		fprintf(stderr, "Error. Trying to get back of empty list\n");
		exit(EXIT_FAILURE);
	}
	return L->back->data;
}

// returns the cursor element
void* get(List L) {
	// preconditions
	if (!(length(L) > 0)) {
		fprintf(stderr, "Error. No cursor on empty list\n");
		exit(EXIT_FAILURE);
	}
	else if (index(L) == -1) {
		fprintf(stderr, "Error. Cursor is undefined.\n");
		exit(EXIT_FAILURE);
	}	

	return L->cursor->data;
}

// Manipulation procedures ----------------------------------------------------

// function to reset list to its original empty state
void clear(List L) {
	if (L == NULL) {
		fprintf(stderr, "Error. Trying to clear NULL list.\n");
		exit(EXIT_FAILURE);
	}
	
	// delete nodes one by one	
	while (length(L) > 0) {
		deleteFront(L);
	}
}

// overwrites value of cursor element
void set(List L, void* x) {
	// Preconditions
	if (L == NULL) {
		fprintf(stderr, "Error. Trying to use set on NULL List.\n");
		exit(EXIT_FAILURE);
	}
	else if (!(length(L) > 0)) {
		fprintf(stderr, "Error. Trying to use set on empty List.\n");
		exit(EXIT_FAILURE);
	}
	else if (index(L) == -1) {
		fprintf(stderr, "Error. Trying to set value for undefined cursor.\n");
		exit(EXIT_FAILURE);
	}
	// set new value
	L->cursor->data = x;
}

// function to move cursor to front node
void moveFront(List L) {
	if (length(L) > 0) {
		L->cursor = L->front;
		L->cursorIndex = 0;
	}
}

// function to move cursor to back node
void moveBack(List L) {
	if (length(L) > 0) {
		L->cursor = L->back;
		L->cursorIndex = length(L)-1;
	}
}

// function to move cursor back one
void movePrev(List L) {
	// if cursor is defined
	if (index(L) != -1) {
		// if cursor is not at front
		if (L->cursor != L->front) {
			L->cursor = L->cursor->prev;
			L->cursorIndex--;
		}
		// if cursor moves before the front
		else if (L->cursor == L->front) {
			L->cursor = NULL;
			L->cursorIndex = -1;
		}
	}

}

// move cursor to the next node
void moveNext(List L) {
	// if cursor is defined
	if (index(L) != -1) {
		// if cursor is not at the back
		if (L->cursor != L->back) {
			L->cursor = L->cursor->next;
			L->cursorIndex++;
		}
		// if cursor moves past the back
		else if (L->cursor == L->back) {
			L->cursor = NULL;
			L->cursorIndex = -1;
		}
	}
}

// add new node to the front of list
void prepend(List L, void* x) {
	// create new node with given data
	Node n = newNode(x);

	// if L is NULL
	if (L == NULL) {
		fprintf(stderr, "Error. Trying to prepend on NULL List.\n");
		exit(EXIT_FAILURE);
	}

	// if list is empty
	if (!(length(L) > 0)) {
		L->front = L->back = n;
	}
	// if list is not empty
	else {
		n->next = L->front;
		L->front->prev = n;
		L->front = n;
	}

	L->length++;
	if (index(L) != -1) {
		L->cursorIndex++;
	}
}

// add new node to the end of the list
void append(List L, void* x) {
	// create new node with given data
	Node n = newNode(x);
	
	// if L is NULL
	if (L == NULL) {
		fprintf(stderr, "Error. Trying to append on NULL List.\n");
		exit(EXIT_FAILURE);
	}

	// if list is empty
	if (!(length(L) > 0)) {
		L->front = L->back = n;
	}
	// if list is not empty
	else {
		L->back->next = n;
		n->prev = L->back;
		L->back = n;
	}
	L->length++;
}

// inserts a new node just before the cursor
void insertBefore(List L, void* x) {
	if (!(length(L) > 0)) {
		fprintf(stderr, "Error. Trying insertBefore on empty list.\n");
		exit(EXIT_SUCCESS);
	}
	else if (index(L) == -1) {
		fprintf(stderr, "Error. Trying insertBefore with undefined cursor.\n");
		exit(EXIT_SUCCESS);
	}
	
	// if cursor is the front	
	if (L->cursor == L->front) {
		prepend(L, x);
		return;
	}
	// if cursor is not front
	Node n = newNode(x);
	Node p = L->cursor->prev;
	Node c = L->cursor;

	// insert new node
	n->prev = p;
	p->next = n;
	n->next = c;
	c->prev = n;

	// update length
	L->length++;
	if (index(L) != -1) {
		L->cursorIndex++;
	}	
}

// insert new node just after the cursor
void insertAfter(List L, void* x) {
	if (index(L) == -1) {
		fprintf(stderr, "Error. Trying insertAfter with undefined cursor.\n");
		exit(EXIT_FAILURE);
	}
	
	// if list is empty or cursor is the back
	if (!(length(L) > 0) || L->cursor == L->back) {
		append(L, x);
		return;
	}

	// if cursor not in back
	Node n = newNode(x);
	Node c = L->cursor;
	Node b = L->cursor->next;

	// insert node
	n->prev = c;
	c->next = n;
	b->prev = n;
	n->next = b;

	// update length
	L->length++;
}

// deletes front node in list
void deleteFront(List L) {
	// if list is empty
	if (!(length(L) > 0)) {
		fprintf(stderr, "List Error: trying to deleteFront on empty List\n");
		exit(EXIT_FAILURE);
	}
	// if front is only node
	else if (length(L) == 1 || L->cursor == L->front) {
		moveFront(L);
		delete(L);
		return;
	}
	
	Node f = L->front;
	Node n = L->front->next;
	
	n->prev = NULL;
	L->front = n;

	freeNode(&f);
	L->length--;
	L->cursorIndex--;
}

// deletes back node in list
void deleteBack(List L) {
	// if list is empty
	if (!(length(L) > 0)) {
		fprintf(stderr, "Error. Trying to deleteBack on empty list.\n");
		exit(EXIT_FAILURE);
	}
	// if back is the only node
	else if (length(L) == 1 || L->cursor == L->back) {
		moveBack(L);
		delete(L);
		return;
	}

	Node b = L->back;
	Node p = L->back->prev;
	
	p->next = NULL;
	L->back = p;
	
	freeNode(&b);
	L->length--;
}
void delete(List L) {	
	// if cursor is defined
	if (index(L) != -1) {
		// pointer to cursor
		Node c = L->cursor;

		// if cursor is the only node
		if (length(L) == 1) {
			L->front = L->back = NULL;
		}
		// if there are only 2 nodes
		else if (length(L) == 2) {
			// if cursor is in front
			if (c == L->front) {
				L->front = L->back;
			}
			// if cursor is in back
			else if (c == L->back) {
				L->back = L->front;
			}
		}
		// if there are 3 or more nodes
		else if (length(L) >= 3) {
			// if cursor is front
			if (c == L->front) {
				L->front = L->front->next;	
			}
			// if cursor is back
			else if (c == L->back) {
				L->back = L->back->prev;
			}
			// if cursor is between two nodes
			else {
				c->prev->next = c->next;
				c->next->prev = c->prev;
			}
		}
		// free the cursor node and update length
		freeNode(&c);
		L->cursorIndex = -1;
		L->cursor = NULL;
		L->length--;
	}
}

// Other operations -----------------------------------------------------------

// prints list node by node
void printList(FILE* out, List L) {

	if (length(L) == 0) {
		fprintf(stderr, "Error. Trying to print empty list.\n");
		exit(EXIT_FAILURE);
	}

	// create pointer to iterate over list
	Node n = L->front;
	// print every element before back
	while (n != L->back) {
		fprintf(out, FORMAT " ", n->data);
		n = n->next;
	}
	// print back
	fprintf(out, FORMAT "\n", n->data);
}
