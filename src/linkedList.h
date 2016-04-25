#ifndef __LIST_H
#define __LIST_H

// a common function used to free malloc'd objects
typedef void (*freeFunction)(void *);

typedef enum { FALSE, TRUE } bool;

typedef bool (*listIterator)(void *);

typedef struct _listNode {
  void *data;
  struct _listNode *next;
} listNode;

typedef struct {
  int logicalLength;
  int elementSize;
  listNode *head;
  listNode *tail;
  freeFunction freeFn;
} list;

void listCreate( list *list, int elementSize, freeFunction freeFn);
void listDestroy( list *list);
int listGetSize( list *list);

void listInsertFirst( list *list, void *element);
void listInsertLast( list *list, void *element);

void listInsertBeforeCurrent( list *list, void *element);
void listInsertAfterCurrent( list *list, void *element);

void listRemoveCurrent( list *list, void *element );
void listSort( list *list );
void listClear( list *list );

void listForEach( list *list, listIterator iterator);
void listGetFirst( list *list, void *element, bool removeFromList);
void listGetNext( list *list, void *element, bool removeFromList);

#endif
