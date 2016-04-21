#ifndef DEQUE_AS_ARRAY_H
#define DEQUE_AS_ARRAY_H
#include <algorithm>
#include <iostream>

/******************************************
 * UW User ID:  h382zhan
 * Submitted for ECE 250
 * Semester of Submission:  (Winter) 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 ******************************************/

#include "Exception.h"


class Deque_as_array {
	private:
		// int array_size; no need - just using count to keep track of current number of elements
		int array_capacity;	// for keeping track of maximum number of elements
		int*array;
		int ihead;
		int itail;
		int count;

	public:
		Deque_as_array( int = 10 );
		~Deque_as_array();

		int head() const;
		int tail() const;
		int size() const;
		bool empty() const;
		int capacity() const;

		void enqueue_head( int const & );
		void enqueue_tail( int const & );
		int dequeue_head();
		int dequeue_tail();
		void clear();
		void print();
	
};

 
Deque_as_array::Deque_as_array( int n ):
array_capacity( std::max( 1, n ) ), 
array( new int[array_capacity] ),
ihead( 1 ),		// Initialize head counter as 1. Otherwise (ie, if ihead <= 0), the first enqueue_head() will point to index (array_capacity - 1), which will mean that index 0 is not allocated
itail( 0 ),		// Initialize tail coutner as 0. First enqueue_tail() will point to index 1 of the array

				// This way, once both enqueue functions are used, the first head will be at index 0 and the first tail will be at index 1
count( 0 ) {	// Initialize count size counter of deque as 0
	if ( array_capacity <= 0 ){		// If the user tries to declare a deque with capacity < 1,
		array_capacity = 1;			// then make the capacity 1
									// This also prevents potential divison by zero, since it ensures that the capacity > 0
	}
}

void Deque_as_array::print() {
	return;
}
 
Deque_as_array::~Deque_as_array() {
	// destructor
	//
	// this method will crash if deque is initialized as static memory
	// (ie, Deque_as_array a(5))
	//
	// willl work if deque is initialized as dynamic memory
	//(ie, Deque_as_array *a = new Deque_as_array(5))
	
	delete [] array;
}

 
int Deque_as_array::size() const {
	return count;			// Returns the current number of elements queued in deque
}

 
int Deque_as_array::capacity() const {
	return array_capacity;	// Returns the maximum number of elements allowed in deque
}

 
bool Deque_as_array::empty() const {
	// Checks if the deque is empty, and returns a boolean value
	return ( count == 0 );	// Returns whether the current number of elements in deque is zero
}

 
int Deque_as_array::head() const {
	// Looks at element at head
	//try {
	if ( count == 0 ) {		// If the deque is empty,
		throw underflow();	// then throw underflow exception
	}
		
	return array[ihead];	// This returns the integer at head
	//}
	/*catch (...) {				// In this case there is only one exception possible, so "..." means it catches and handles all exceptions.
		//std::cout << "underflow exception - deque empty, unable to find element at head." << std::endl;	// Print exception message
		//return 0;				// Return 0, unable to return void since it is an int function
		throw underflow();	// then throw underflow exception
	}*/
}

 
int Deque_as_array::tail() const {
	// Looks at element at tail
	//try {
	if ( count == 0 ) {		// If the deque is empty,
		throw underflow();	// then throw underflow exception
	}
		
	return array[itail];	// This returns integer at tail
	//}
	/*catch (...) {				// In this case there is only one exception possible, so "..." means it catches and handles all exceptions.
		//std::cout << "underflow exception - deque empty, unable to find element at tail." << std::endl;	// Print exception message
		//return 0;				// Return 0, unable to return void since it is an int function
		throw underflow();	// then throw underflow exception
	}*/
}


void Deque_as_array::enqueue_head( int const &obj ) {
	// Pushes element to head
	//try {
	if ( count == array_capacity ) {	// If the deque is already full,
		throw overflow();				// then throw overflow exception
	}
	
	++count;							// Increment the current size of the deque
	ihead = ( ihead + array_capacity - 1 ) % array_capacity;	// Set new head pointer: Decrement the current head pointer by one, and add to it the maximum size of the deque. Then, take the modulus of the previous operation by the maximum size of the deque.
																// The modulus allows the array being used for the deque to be a circular array, since the index will always be a modulus of the total size.
																// The reason for adding the maximum size is added is to ensure that the number always remains positive.  Otherwise, a modulus with a negative operand could
																// return a negative head pointer.  This is because % in C++ returns the remainder of the division, rather than actual modulus (ie, -1 % 3 = -1).
	array[ihead] = obj;					// Set the input as the value for the new head pointer
	return;
	//}
	/*catch (...) {							// In this case there is only one exception possible, so "..." means it catches and handles all exceptions.
		//std::cout << "overflow exception - deque full, unable to enqueue element '" << obj << "' to head." << std::endl;	// Print exception message
		//return;
		throw overflow();				// then throw overflow exception
	}*/
}


void Deque_as_array::enqueue_tail( int const &obj ) {
	// Pushes element to tail
	//try {
	if ( count == array_capacity ) {	// If the deque is already full,
		throw overflow();				// then throw overflow exception
	}
	
	++count;							// Increment the current size of the deque
	itail = ( itail + 1 ) % array_capacity;		// Set new tail pointer: Increment the current tail pointer by one. Then, take the modulus of the previous operation by the maximum size of the deque
												// The modulus allows the array being used for the deque to be a circular array, since the index will always be a modulus of the total size.
												// No need to add maximum size like enqueue_head(), as modulus behaves properly for positive operands.
	array[itail] = obj;					// Set the input as the value for the new tail pointer
	return;
	//}
	/*catch (...) {							// In this case there is only one exception possible, so "..." means it catches and handles all exceptions.
		//std::cout << "overflow exception - deque full, unable to enqueue element '" << obj << "' to tail." << std::endl;	// Print exception message
		//return;
		throw overflow();				// then throw overflow exception
	}*/
}

 
int Deque_as_array::dequeue_head() {
	// Remove element from head, and look at it
	//try {
	if ( count == 0 ) {		// If the deque is already empty,
	throw underflow();	// then throw underflow exception
	}
	
	--count;				// Decrement the current size of the deque
	int a = array[ihead];	// Set an integer a to be value stored at the current head
	array[ihead] = 0;		// Make the value of the current head zero (default value)
	ihead = ( ihead + 1 ) % array_capacity;		// Set the new head pointer: Increment the current head pointer by one, and take its modulus by the maximum size of the deque.
												// Modulus allows for circular array.
	return a;     			// This returns a the value at the previous head of the deque (before clearing).
	//}
	/*catch (...) {				// In this case there is only one exception possible, so "..." means it catches and handles all exceptions.
		//std::cout << "underflow exception - deque empty, unable to dequeue from head" << std::endl;		// Print exception message
		//return 0;
		throw underflow();	// then throw underflow exception
	}*/
}

 
int Deque_as_array::dequeue_tail() {
	// Remove element from tail, and look at it
	//try {
	if ( count == 0) {		// If the deque is already empty,
		throw underflow();	// then throw underflow exception
	}
	--count;				// Decrement the current size of the deque
	int a = array[itail];	// Set an integer a to be value stored at the current head
	array[itail] = 0;		// Make the value of the current head zero (default value)
	itail = ( itail + array_capacity - 1 ) % array_capacity;	// Set the new head pointer: Increment the current head pointer by one and add the maximum size of the deque to it, and take the modulus by the maximum size.
																	// Modulus allows for circular array.
																	// The reason for adding the maximum size is added is to ensure that the number always remains positive.
	return a;     			// This returns a the value at the previous tail of the deque (before clearing).
	//}
	/*catch (...) {				// In this case there is only one exception possible, so "..." means it catches and handles all exceptions.
		//std::cout << "underflow exception - deque empty, unable to dequeue from tail" << std::endl;		// Print exception message
		//return 0;
		throw underflow();	// then throw underflow exception
	}*/
}


void Deque_as_array::clear() {
	// Empty the deque - remove all elements
	for ( int i = 0; i <= array_capacity; ++i ) {
		array[ i % array_capacity ] = 0;	// Makes every element in the deque 0, which is the default value.
	}
	
	ihead = 1;	// Reset head to original index
	itail = 0;	// Reset tail to original index
	count = 0;	// Reset size counter to original
	return;
}



// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
