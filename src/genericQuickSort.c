/*
 * genericQuickSort.c
 *
 *  Created on: Apr 25, 2016
 *      Author: Tania
 */


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aux_macros.h"
#include "list_mtm.h"

typedef void* Element;

typedef struct {

	int id;
	char* value;

} element;

typedef int (*compareFunction)( void* a, void* b);

void freeElement( Element element ){

	 free( element );
	 element = NULL;
}

Element copyElement( Element element ){

	void *temp = malloc( sizeof( *element ) );
	memcpy( temp, element, sizeof( *element ) );

	return temp;
}

void swap ( void** array, int size, int firstIndex, int secondIndex ) ;

int partition ( void** array, int size, int first, int last,
										compareFunction cmprFunc );

void quickSort (void** array, int size, int start, int end,
				compareFunction cmprFunc );

int compareElementId(void *a, void *b) {

	int a_ = ( (element*) a )->id;
	int b_ = ( (element*) b )->id;

	if( a_ < b_ ) return 1;
	if( a_ == b_ ) return 0;

	else return -1;
}

void printArray( void** array, int len ){

	printf( "\n" );

	for( int i = 0; i < len; i++ ){
		printf( "%d ", ((element*)array[i])->id );
	}

	printf( "\n" );
}

bool quickSort_TEST() {

	bool final = true;
	element* el1 = (element*) malloc( sizeof( element ));
	element* el2 = (element*) malloc( sizeof( element ));
	element* el3 = (element*) malloc( sizeof( element ));
	element* el4 = (element*) malloc( sizeof( element ));
	element* el5 = (element*) malloc( sizeof( element ));

	if( el1 && el2 && el3 && el4 && el5 ) {

		el1->id = 1;
		el2->id = 2;
		el3->id = 3;
		el4->id = 4;
		el5->id = 5;

		Element els[5] = { el1, el2, el3, el4, el5 };

		TEST_EQUALS(final, 1, compareElementId( el1, el2 ));
		TEST_EQUALS(final, -1, compareElementId( el3, el2 ));

		swap(els, sizeof( element ), 0, 4 );
		TEST_EQUALS(final, 5, el1->id );
		TEST_EQUALS(final, 1, el5->id );

		printArray( els, 5 );

		int q = partition( els, sizeof( element ), 0, 3, compareElementId );

		printArray( els, 5 );

		TEST_EQUALS(final, 2, q );

		 q = partition( els, sizeof( element ), 0, 4, compareElementId );

		TEST_EQUALS(final, 0, q );

		printArray( els, 5 );

		quickSort( els, sizeof( element ), 0, 4 , compareElementId );

		TEST_EQUALS(final, 2, ( (element*) els[1] )->id );

		printArray( els, 5 );
	}

	free( el1 );
	free( el2 );
	free( el3 );
	free( el4 );
	free( el5 );

	return final;
}

bool list_mtm_TEST() {

	bool final = true;

	Element el1 = malloc( sizeof( element ));
	Element el2 = malloc( sizeof( element ));
	Element el3 = malloc( sizeof( element ));

	if( el1 && el2 && el3  ) {

		( (element*) el1 )->id = 1;
		( (element*) el2 )->id = 2;
		( (element*) el3 )->id = 3;

		List list = NULL;

		TEST_EQUALS(final, -1, listGetSize( list ));

		list = listCreate( copyElement, freeElement );

		TEST_EQUALS(final, 0, listGetSize( list ));
		TEST_EQUALS(final, NULL, listGetCurrent( list ));
		TEST_EQUALS(final, NULL, listGetFirst( list ));
		TEST_EQUALS(final, NULL, listGetNext( list ));

		ListResult r = listInsertFirst( list, el1 );

		TEST_EQUALS(final, LIST_SUCCESS, r );

		TEST_EQUALS(final, 0 ,compareElementId( listGetFirst( list ), el1 ) );
	}

	freeElement( el1 );
	freeElement( el2 );
	freeElement( el3 );

	return final;
}

int main() {

	RUN_TEST( quickSort_TEST);
	RUN_TEST( list_mtm_TEST );

	return 0;
}


// Swaps two items in an array, changing the original array
void swap ( void** array, int size, int firstIndex, int secondIndex ) {

	 char temp[size];
	 memcpy( temp, array[firstIndex], size );
	 memcpy( array[firstIndex], array[secondIndex], size );
	 memcpy( array[secondIndex], temp, size);
}

int partition ( void** array, int size, int first, int last,
										compareFunction cmprFunc ) {

	char pivot[size];
	memcpy( pivot, array[last], size );

	int sortedIndx = first;

	for( int currIndx = first; currIndx < last; currIndx++ ){

		if ( cmprFunc( array[currIndx], pivot ) != -1 ){

			swap( array, size, currIndx, sortedIndx );
			sortedIndx++;
		}
	}

	swap( array, size, sortedIndx, last );

	return( sortedIndx );
}

void quickSort (void** array, int size, int start, int end,
				compareFunction cmprFunc ) {

    if( start == end ){
       return;

    } else if ( end > start ){
        int pivotIndx = partition( array, size, start, end, cmprFunc );

        quickSort( array, size, start, pivotIndx -1, cmprFunc );
        quickSort( array, size, pivotIndx + 1, end, cmprFunc );
    }
}


