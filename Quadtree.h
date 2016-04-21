#ifndef QUADTREE_H
#define QUADTREE_H

/***************************************
 * UW User ID:  h382zhan
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  Winter 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ***************************************/

#include "ece250.h"
#include "Quadtree_node.h"
#include "Exception.h"
#include <iostream>

template <typename T>
class Quadtree {
	private:
		Quadtree_node<T> *tree_root;
		int count;

	public:
		Quadtree();
		~Quadtree();

		// Accessors

		int size() const;
		bool empty() const;

		T min_x() const;
		T min_y() const;

		T max_x() const;
		T max_y() const;

		T sum_x() const;
		T sum_y() const;

		Quadtree_node<T> *root() const;
		bool member( T const &, T const & ) const;

		// Mutators

		void insert( T const &, T const & );
		void clear();

	
};

template <typename T>
Quadtree<T>::Quadtree():
tree_root( 0 ), 
count( 0 ) {
	// empty constructor
}

template <typename T>
Quadtree<T>::~Quadtree() {				// Destructor
	if ( tree_root != 0 ) {				// If the tree root is not a null reference
		clear();						// Call Quadtree.clear()
	} else {							// Otherwise, it means that there is no pointer to anything other than the tree root
		delete tree_root;				// Delete the root
	}
}

template <typename T>
int Quadtree<T>::size() const {
	return count;						// Return the count, which is the total number of elements currently in the Quadtree
}

template <typename T>
bool Quadtree<T>::empty() const {
	if ( count == 0 ) {					// If the count is zero,
		return true;					// then return true, since the Quadtree is empty
	}
	return false;						// Otherwise, return false, since count is not zero
}

template <typename T>
T Quadtree<T>::min_x() const {
	if ( tree_root == 0 ) {
		throw underflow();				// Underflow exception if tree root references a null pointer, since it means that no values have been inserted into the Quadtree yet
	}
	return tree_root->min_x();			// Call Quadtree_node.min_x() on the tree root, giving the minimum x value of all nodes accessible by tree_root
}

template <typename T>
T Quadtree<T>::min_y() const {
	if ( tree_root == 0 ) {
		throw underflow();				// Underflow exception if tree root references a null pointer, since it means that no values have been inserted into the Quadtree yet
	}
	return tree_root->min_y();			// Call Quadtree_node.min_y() on the tree root, giving the minimum y value of all nodes accessible by tree_root
}

template <typename T>
T Quadtree<T>::max_x() const {
	if ( tree_root == 0 ) {
		throw underflow();				// Underflow exception if tree root references a null pointer, since it means that no values have been inserted into the Quadtree yet
	}
	return tree_root->max_x();			// Call Quadtree_node.max_x() on the tree root, giving the maximmum x value of all nodes accessible by tree_root
}

template <typename T>
T Quadtree<T>::max_y() const {
	if ( tree_root == 0 ) {
		throw underflow();				// Underflow exception if tree root references a null pointer, since it means that no values have been inserted into the Quadtree yet
	}
	return tree_root->max_y();			// Call Quadtree_node.max_y() on the tree root, giving the maximum y value of all nodes accessible by tree_root
}

template <typename T>
T Quadtree<T>::sum_x() const {
	if ( tree_root == 0 ) {
		return 0;						// Return zero if the tree root is zero, since there are no values to sum
	}
	return tree_root->sum_x();			// Call Quadtree_node.sum_x() on the tree root, giving the sum of x values of all nodes accessible to it
}

template <typename T>
T Quadtree<T>::sum_y() const {
	if ( tree_root == 0 ) {
		return 0;						// Return zero if the tree root is zero, since there are no values to sum
	}
	return tree_root->sum_y();			// Call Quadtree_node.sum_y() on the tree root, giving the sum of y values of all nodes accessible to it
}

template <typename T>
Quadtree_node<T> *Quadtree<T>::root() const {
	return tree_root;					// Return the pointer to the tree root
}

template <typename T>
bool Quadtree<T>::member( T const &x, T const &y ) const {
	return tree_root->member(x,y);		// Call Quadtree_node.member(x,y) on the tree root, which will return true if (x,y) is found in a node accessible to the tree root, and false otherwise
}

template <typename T>
void Quadtree<T>::insert( T const &x, T const &y ) {
	if( tree_root == 0 ) {
		tree_root = new Quadtree_node<T>(x,y);	// If the tree root is a null pointer, then insert the value as the tree root, since the Quadtree is empty
		++count;								// Increment count, since a value was inserted
	} 
	bool inserted = tree_root->insert(x,y);		// Return a Boolean value based on whether or not insertion was successful, by calling Quadtree_node.insert(x,y), which will return true if a value was successfully inserted
	if ( inserted ) {							// Check if insertion was successful
		++count;								// Increment count, since a value was inserted
	}
}

template <typename T>
void Quadtree<T>::clear() {				// Clear all values in the Quadtree, and delete the nodes, but keep a reference to the tree root
	if ( tree_root != 0 ) {				// If the tree root is not a null reference
		tree_root->clear();				// Call Quadtree_node.clear(), which will clear nodes accessible at the tree root
	}
	delete tree_root;					// Delete the value stored at the root
	tree_root = 0;						// Set the tree root to a null pointer, which is the default status upon initialization
	count = 0;							// Reset the count to zero, since everything has been cleared, and the Quadtree is now empty
}



// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif

