/*
 * linkedList.c
 *
 *  Created on: Apr 23, 2016
 *      Author: Tania
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef void* Element;
typedef int (*compareFunction)( void* a, void* b);



int main(){

int compareInt(void *a, void *b) {
	int a_ = *(int *) a;
	int b_ = *(int *) b;

	if ( a_ < b_ ) return 1;

	if ( a_ > b_ ) return -1;

	return 0;

	}

	return 0;
}

// Swaps two items in an array, changing the original array
void swap ( void** array, int size, int firstIndex, int secondIndex ) {

	 char temp[size];
	  memcpy( temp, array[firstIndex], size );
	  memcpy( array[firstIndex], array[secondIndex], size );
	  memcpy( array[secondIndex], temp, size);
}

int partition ( void** array, int size, int first, int last, compareFunction cmprFunc ) {

	    char pivot[size];
	    memcpy( pivot, array[last], size );

	    int sortedIndx = first;

	    for( int currIndx = first; currIndx < last; currIndx++ ){

	    	// <=
	        if ( cmprFunc( array[currIndx], pivot ) == 1 ){
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
};


