#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list_mtm.h"

typedef struct ListItem_t *ListItem;

struct ListItem_t {
	ListElement Element;
	ListItem Next;
};

struct List_t {
	CopyListElement copyElementFunc;
	FreeListElement freeElementFunc;
	ListItem First;
	ListItem Current;
};

static ListItem CreateListItem( CopyListElement copyElement,
		ListElement element );

static void DestroyListItem( FreeListElement freeElement, ListItem item );

static int GetListLength( List list );

ListElement listGetFirst(List list){

	ListElement first = NULL;

	if ( list && list->First ){

		first = list->First->Element;
	}

	return first;
}

int listGetSize(List list){

	int size = -1;

	if( list ){

		size = GetListLength( list );
	}

	return size;
}

ListElement listGetNext(List list){

	ListElement next = NULL;

	return next;
}

ListElement listGetCurrent( List list ){

	ListElement current = NULL;

	return current;
}

List listCreate( CopyListElement copyElement, FreeListElement freeElement ) {

	List list = malloc( sizeof( *list ) );

	if ( list && copyElement && freeElement ) {

		list->copyElementFunc = copyElement;
		list->freeElementFunc = freeElement;
		list->First = NULL;
		list->Current = NULL;
	}

	return list;
}

List listCopy( List list ) {

	List copy = NULL;

	if( list )
		copy = listCreate( list->copyElementFunc, list->freeElementFunc );

	if( copy && list->First ){

		list->Current = list->First;

		while( list->Current ){

			ListItem copyItem = CreateListItem( copy->copyElementFunc,
												list->Current );

			if( copyItem ) {

				if ( listInsertLast( copy, copyItem ) == LIST_SUCCESS ) {

					list->Current = list->Current->Next;

				} else {

					list->Current = NULL;
				}
			}
		}
	}

	return copy;
}

/**
 * Adds a new element to the list, the new element will be the first element.
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertFirst( List list, ListElement element ) {

	if ( list == NULL || element == NULL )
		return LIST_NULL_ARGUMENT;

	ListItem newListItem = CreateListItem( list->copyElementFunc, element );

	if ( newListItem == NULL )
		return LIST_OUT_OF_MEMORY;


	newListItem->Next = list->First;
	list->First = newListItem;
	return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be the last element
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertLast( List list, ListElement element ) {

	if ( list == NULL || element == NULL )
		return LIST_NULL_ARGUMENT;

	ListItem newListItem = CreateListItem( list->copyElementFunc, element );

	if ( newListItem == NULL )
		return LIST_OUT_OF_MEMORY;

	if ( list->First == NULL ) {

		list->First = newListItem;

	} else {

		ListItem toAddTo = list->First;

		while ( toAddTo->Next != NULL ) {

			toAddTo = toAddTo->Next;
		}

		toAddTo->Next = newListItem;
	}
	return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be place right before
 * the current element (As pointed by the inner iterator of the list)
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertBeforeCurrent( List list, ListElement element ) {

	if ( list == NULL || element == NULL )
		return LIST_NULL_ARGUMENT;

	if ( list->Current == NULL )
		return LIST_INVALID_CURRENT;

	ListItem newListItem = CreateListItem( list->copyElementFunc, element );

	if ( newListItem == NULL )
		return LIST_OUT_OF_MEMORY;

	if ( list->First == list->Current ) {

		newListItem->Next = list->First;
		list->First = newListItem;

	} else {

		ListItem toAddTo = list->First;

		while ( toAddTo->Next != list->Current ) {
			toAddTo = toAddTo->Next;
		}

		newListItem->Next = toAddTo->Next;
		toAddTo->Next = newListItem;
	}
	return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (As pointed by the inner iterator be of the list)
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertAfterCurrent( List list, ListElement element ) {

	if ( list == NULL || element == NULL )
		return LIST_NULL_ARGUMENT;

	if ( list->Current == NULL )
		return LIST_INVALID_CURRENT;

	ListItem newListItem = CreateListItem( list->copyElementFunc, element );

	if ( newListItem == NULL )
		return LIST_OUT_OF_MEMORY;

	newListItem->Next = list->Current->Next;
	list->Current->Next = newListItem;
	return LIST_SUCCESS;
}

/**
 * Creates a new ListItem that contains a copy of the given ListElement.
 *
 * @param copyElement Function pointer to be used for copying the element.
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function
 * @return
 * The newly created ListItem, or NULL in case of allocation failure (Meaning
 * the function for copying an element or allocating a ListItem struct failed)
 */
static ListItem CreateListItem( CopyListElement copyElement,
		ListElement element ) {

	ListItem item = malloc( sizeof( *item ) );

	if ( item == NULL ) {
		return NULL;
	}

	item->Next = NULL;
	item->Element = copyElement( element );

	if ( item->Element == NULL ) {

		free( item );
		item = NULL;
	}
	return item;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key){

	List filtered = NULL;

	if( list && filterElement ){

		filtered = listCopy( list );

		if( filtered && filtered->First ){

			filtered->Current = filtered->First;

			while( filtered && filtered->Current ){

				if( !filterElement( filtered->Current->Element, key ) ){

					if ( listRemoveCurrent( filtered ) == LIST_SUCCESS ){

						filtered->Current = filtered->Current->Next;

					} else {

						filtered->Current = NULL;
						listDestroy( filtered );
					}
				}
			}
		}
	}

	return filtered;
}

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function
 *
 * @param list The list for which the current element will be removed
 * @return
 * LIST_NULL_ARGUMENT if list is NULL
 * LIST_INVALID_CURRENT if the current pointer of the list is in invalid state
 * LIST_SUCCESS the current element was removed successfully
 */
ListResult listRemoveCurrent( List list ) {

	if ( list == NULL )
		return LIST_NULL_ARGUMENT;

	if ( list->Current == NULL )
		return LIST_INVALID_CURRENT;

	if ( list->First == list->Current ) {

		list->First = list->Current->Next;
	} else {
		ListItem toRemoveNext = list->First;

		while ( toRemoveNext->Next != list->Current ) {
			toRemoveNext = toRemoveNext->Next;
		}

		toRemoveNext->Next = toRemoveNext->Next->Next;
	}

	DestroyListItem( list->freeElementFunc, list->Current );
	list->Current = NULL;
	return LIST_SUCCESS;
}

/**
 * Sorts the list according to the given function, using a max sort sorting
 * method.
 *
 * For example, the following code will sort a list of integers according to
 * their distance from 0.
 * @code
 * int closerTo(ListElement num1, ListElement num2, ListSortKey value) {
 *   int distance1 = abs(*(int*)num1 - *(int*)value);
 *   int distance2 = abs(*(int*)num2 - *(int*)value);
 *   return distance1 - distance2;
 * }
 *
 * void sortInts(List listOfInts) {
 *   listSort(listOfInts, closerTo);
 * }
 * @endcode
 *
 * @param list the target list to sort
 * @param compareElement A comparison function as defined in the type
 * CompareListElements. This function should return an integer indicating the
 * relation between two elements in the list
 *
 * @return
 * LIST_NULL_ARGUMENT if list or compareElement are NULL
 * LIST_OUT_OF_MEMORY if a memory allocation failed, the list will be intact
 * in this case.
 * LIST_SUCCESS if sorting completed successfully.
 */
ListResult listSort( List list, CompareListElements compareElement ) {

	if ( ( list == NULL ) || ( compareElement == NULL ) )
		return LIST_NULL_ARGUMENT;

	int listLength = GetListLength( list );

	if ( listLength > 1 ) {

		for ( int i = listLength - 1; listLength > 1; listLength-- ) {

			ListItem current = list->First;

			for ( int j = 0; j < i; j++ ) {

				if ( compareElement( current->Element, current->Next->Element )
						< 0 ) {

					ListElement element = current->Element;
					current->Element = current->Next->Element;
					current->Next->Element = element;
				}
				current = current->Next;
			}
		}
	}
	return LIST_SUCCESS;
}

/**
 * Returns the number of elements in the target list.
 *
 * @param list Target list to count its elements
 * @return
 * The list elements count.
 */
static int GetListLength( List list ) {

	int size = 0;
	ListItem curItem = list->First;

	while ( curItem != NULL ) {

		size++;
		curItem = curItem->Next;
	}

	return ( size );
}

/**
 * Removes all elements from target list.
 *
 * The elements are deallocated using the stored freeing function
 * @param list Target list to remove all element from
 * @return
 * LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * LIST_SUCCESS - Otherwise.
 */
ListResult listClear( List list ) {

	if ( list == NULL )
		return LIST_NULL_ARGUMENT;

	ListItem current = list->First;
	list->First = NULL;

	while ( current != NULL ) {

		ListItem temp = current;
		current = current->Next;
		DestroyListItem( list->freeElementFunc, temp );
		temp = NULL;
	}

	list->Current = NULL;
	return LIST_SUCCESS;
}

/**
 * DestroyListItem: Deallocates an existing ListItem. Clears the element by
 * using the given free function.
 *
 * @param freeElement Function pointer to be used for removing the element of
 * the ListItem.
 * @param item The ListItem to Deallocate.
 */
static void DestroyListItem( FreeListElement freeElement, ListItem item ) {

	if ( item != NULL ) {

		if ( item->Element != NULL )
			freeElement( item->Element );

		free( item );
		item = NULL;
	}
}

void listDestroy( List list ) {

	if ( listClear( list ) ) {
		free( list );
		list = NULL;
	}
}

