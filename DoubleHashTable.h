#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H
#include <iostream>

/****************************************
 * UW User ID:  uwuserid
 * Submitted for ECE 250
 * Semester of Submission:  (Winter|Spring|Fall) 20NN
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ****************************************/

#include "Exception.h"
#include "ece250.h"


enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count;
		int power;
		int array_size;
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;
		int capacity() const;		
		bool empty() const;
		bool member( T const & ) const;
		T bin( int ) const;

		void print() const;

		void insert( T const & );
		bool remove( T const & );
		void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {
	delete[] array;			// delete the array holding the bin values
	delete[] array_state;	// delete the array holding the bin statuses
}

template<typename T >
int DoubleHashTable<T >::size() const {
	return count;		// return the current count, which is the number of elements currently stored
}

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size;	// return array_size (or M), which is the maximum possible elements
}

template<typename T >
bool DoubleHashTable<T >::empty() const {
    if ( count == 0 ) {	// check if the current count is zero
    	return true;	// if so, then the array is empty, so return true
    }
	return false;		// otherwise, return false, since count is not zero
}

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {
	// first hash function
	// take the floor of the input plus the array_size (M)
	// floor is in case the input is of type Double
	// explicity convert to an integer, to use modulus (%)
	// take modulus by array_size (M), and return
	return ( (int)(floor( obj + array_size )) % array_size );
}

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {
	// second hash function
	int k = ( (int)(floor( obj / array_size )) + array_size ) % array_size;
	// take the floor of the input divided by array_size (M)
	// explicitly convert the result to an integer, so that modulus (%) works
	// add array_size in case result is negative, so that modulus (%) works as intended
	// otherwise, the modulus of a negative in c++ is just the negative number
	// take modulus by array_size (M)
	if ( ( k % 2 ) == 0) {		// check if k is an even number, and if so
		k = k + 1;				// make it odd by adding one
								// this preserves the co-prime property
	}
	return k;	// return the result
}

template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {
	int probe = h1(obj);	// call hash function 1 on the value we want to locate
	int offset = h2(obj);	// call hash function 2 on the value we want to locate
	int i = 0;				// initialize the loop iteration counter
	while ( array[probe] != obj ) {		// while the value in the table has not been found
		if ( ( i == array_size ) || ( array_state[probe] == EMPTY ) ) {
			// check if either the number of times iterated is equal to array_size (M)
			// if so, it means that we have iterated through all elements of the table
			// therefore the value does not exist in the table
			// OR
			// if the index given is EMPTY status
			// it means that a value has not yet been set in the index
			// therefore, the value we are looking for is not in the table either
			//
			// in either case, return false, since the value does not reside in any bin
			return false;
		}
		probe = ( probe + offset ) % array_size;	// get the next possible index for the value
		i++;	// increment the number of times we have iterated
	}
	// once at this part of the code,
	// it means we have found the value in the table
	return true;	// therefore, return true, since the value is indeed a member
}

template<typename T >
T DoubleHashTable<T >::bin( int n ) const {              
	return array[n];	// return the value at the bin index specified
}

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {
	if ( count == array_size ) {	// if the number of current elements is already at max
		throw overflow();			// throw an overflow exception
	}
	int probe = h1(obj);	// call hash function 1 on the value we want to insert
	int offset = h2(obj);	// call hash function 2 on the value we want to insert
	while ( array_state[probe] == OCCUPIED ) {		// iterate until there is a free bin
		if ( array[probe] == obj ) {	// if the value is already in the table,
			return;						// then return without inserting anything
		}
		probe = ( probe + offset ) % array_size;	// offset to next index for value
	}
	array_state[probe] = OCCUPIED;		// change state of bin at index to OCCUPIED
	array[probe] = obj;					// put the value into the bin at index
	count++;							// increment count of current elements
	return;
}

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {
	int probe = h1(obj);	// call hash function 1 on the value we want to delete
	int offset = h2(obj);	// call hash function 2 on the value we want to delete
	int i = 0;	// initialize the loop iteration counter
	while ( array[probe] != obj ) {		// while the value in the table has not been found
		if ( ( i == array_size ) || ( array_state[probe] == EMPTY ) ) {
			// check if either the number of times iterated is equal to array_size (M)
			// if so, it means that we have iterated through all elements of the table
			// therefore the value does not exist in the table
			// OR
			// if the index given is EMPTY status
			// it means that a value has not yet been set in the index
			// therefore, the value we are looking for is not in the table either
			//
			// in either case, return false, since deletion not possible
			return false;
		}
		probe = ( probe + offset ) % array_size;	// get the next possible index for the value
		i++;	// increment the number of times we have iterated
	}
	// once at this part of the code,
	// it means we have found an index that contains the value we want to delete
	array_state[probe] = DELETED;	// set the state of the bin at index to DELETED
	array[probe] = 0;				// reset the value to default, which is zero
	count--;						// decrement the count, since we removed an object
	return true;					// return true, since delete was successful
}

template<typename T >
void DoubleHashTable<T >::clear() {
	for ( int i = 0; i < array_size; ++i ) {	// iterate through each element
		array[i] = 0;				// reset value back to default, which is zero
		array_state[i] = DELETED;	// set the state of each bin to DELETED
	}
	count = 0;		// reset count of all current elements to zero
	return; 
}

template<typename T >
void DoubleHashTable<T >::print() const {
	
	return;
}

#endif

