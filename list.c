/* Blessy Gnanamani, list.c, 10-31-2023 
 * term project, data type for dynamic arrays
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

#define NODE struct node

struct node {
	void **data;
	NODE *next;
	NODE *prev;
	int length;
	int count; 
	int first;
};

struct list {
	NODE *head;
	int numNodes; 
	int numArrays; 
};

//creates list pointed to by lp, O(1) 
LIST *createList(void) {
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	lp->numNodes = 0;
	lp->head = NULL;
	lp->numArrays = 0;
	return lp;
}

// makes new node, O(1)
static void makeNode(LIST *lp, int s){ 
	NODE *prev;
	NODE *new = malloc(sizeof(NODE));
	assert(new != NULL);
	void **newArray = malloc(sizeof(void *) * s);
	assert(newArray != NULL);
	new->data = newArray;
	new->prev = NULL;
	new->next = NULL;
	prev = lp->head->prev;
	prev->next = new;
	new->next = lp->head;
	new->prev = prev;
	lp->head->prev = new;
	new->length = s;
	new->count = 0;
	new->first = 0;
	lp->numArrays++;
}

// frees node, O(1)
static void freeNode(LIST *lp, struct node *n) { //issue with freeing
	assert(lp != NULL && n != NULL);
	free(n->data);
	free(n);
	lp->numArrays--;
}

//destroys a list pointed to by lp, O(n)
void destroyList(LIST *lp){
	assert(lp!=NULL);
	int i;
	struct node *x;
	struct node *temp;
	int z = lp->numArrays;
	x = lp->head;
	for (i = 0; i < z; i++) {
		temp = x;
		x = x->next;
		freeNode(lp, temp);
	}
	free(lp);
}

//return number of items in list pointed to by lp, O(1)
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->numNodes;
}

//adds item to the beginning of the list pointed to by lp, O(1) 
void addFirst(LIST *lp, void *item) { 
	assert(lp != NULL && item != NULL);
	if(lp->numArrays == 0) {
		lp->head = malloc(sizeof(NODE));
		void **newArray = malloc(sizeof(void *) * 4);
		lp->head->data = newArray;
		lp->head->next = lp->head;
		lp->head->prev = lp->head;
		lp->head->length = 4;
		lp->head->count = 0;
		lp->numArrays = 1;
		lp->head->first = 0;
	}
	NODE *p = lp->head;
	int l;
	if(lp->head->count == lp->head->length) {
		if (lp->numNodes / 2 < p->length) {
			l = p->length;
		} else {
			l = lp->numNodes / 2;
		}
		makeNode(lp, l);
		p = lp->head->prev;
		lp->head = p;
	}
	p->first = (p->first + p->length - 1) % p->length;
	p->data[(p->first + p->length) % p->length] = item;
	lp->numNodes++;
	p->count++;
}

//removes and returns the first element of a list pointed to by lp, O(1)
void *removeFirst(LIST *lp) { 
	assert(lp != NULL);
	void *pDel;
	pDel = lp->head->data[lp->head->first % lp->head->length];
	lp->head->count--;
	lp->head->first = (lp->head->first + 1) % lp->head->length;
	lp->numNodes--;
	if (lp->head->count == 0) {
		NODE *z;
		NODE *y;
		NODE *temp;
		z = lp->head->next;
		y = lp->head->prev;
		temp = lp->head;
		z->prev = y;
		y->next = z;
		freeNode(lp,temp);
		if(lp->numArrays == 0) {
			lp->head = NULL;
		} else {
			lp->head = z;
		}
	}
	return pDel;
}

//adds item to the end of the list pointed to by lp, O(1)
void addLast (LIST *lp, void *item){ 
	assert(lp != NULL && item != NULL);
	if(lp->numArrays == 0) {
		lp->head = malloc(sizeof(struct node));
		void **newArray = malloc(sizeof(void *) * 4);
		lp->head->data = newArray;
		lp->head->prev=lp->head;
		lp->head->next=lp->head;
		lp->head->length = 4;
		lp->head->count = 0;
		lp->head->first = 0;
		lp->numArrays = 1;
	}
	int l;
	NODE *p = lp->head->prev;
	if(p->count == p->length) {
		if (lp->numNodes/2 < p->length) {
			l = p->length;
		} else {
			l = lp->numNodes/2;
		}   
		makeNode(lp,l);
		p = lp->head->prev;
	}
	p->data[(p->first + p->count) % p->length] = item;
	lp->numNodes++;
	p->count++;
}

//removes the last element of the list pointed to by lp, O(1)
void *removeLast(LIST *lp){
	assert(lp!=NULL);
	void *pDel;
	NODE *p = lp->head->prev;
	pDel = p->data[(p->first + p->count + p->length) % p->length];
	lp->numNodes--;
	p->count--;
	if (p->count == 0) {
		NODE *g;
		NODE *h;
		NODE *temp;
		g = p->next;
		h = p->prev;
		temp = p;
		g->prev = h;
		h->next = g;
		freeNode(lp, temp);
		if (lp->numArrays == 0) {
			lp->head = NULL;
		} else {
			lp->head = g;
		}
	}
	return pDel;
}

// gets the item at the specified index in the list pointed to by lp, O(n)
void *getItem(LIST *lp, int index){
	assert(lp != NULL);
	NODE *p = lp->head;
	int i;
	for (i=0; i<lp->numArrays; i++) {
		if(index >= p->length) {
			index = index - p->length;
			p = p->next;
		} else { 
			return p->data[index];
		}
	}
	return NULL;
}

//sets the item at a specific index as a specific item in the list pointed to by lp, O(1)
void setItem(LIST *lp, int index, void *item) { 
	assert(lp != NULL && index < lp->numNodes && item != NULL);
	NODE *f = lp->head;
	int i = index;
	while(i >= f->length) {
		i = i - f->length;
		f = f->next;
	}
	f->data[i] = item;
}

