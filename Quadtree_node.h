#ifndef SINGLE_NODE_H
#define SINGLE_NODE_H

/****************************************
 * UW User ID:  h382zhan
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  Winter 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ****************************************/

#include "ece250.h"

template <typename T>
class Quadtree;

template <typename T>
class Quadtree_node {
	private:
		T x_value;
		T y_value;
		
		 
		Quadtree_node *north_west;
		Quadtree_node *north_east;
		Quadtree_node *south_west;
		Quadtree_node *south_east;
		/*
		Quadtree_node<T> *north_west;
		Quadtree_node<T> *north_east;
		Quadtree_node<T> *south_west;
		Quadtree_node<T> *south_east;
		*/

	public:
		Quadtree_node( T const & = T(), T const & = T() );
		~Quadtree_node();
		
		T retrieve_x() const;
		T retrieve_y() const;

		Quadtree_node *nw() const;
		Quadtree_node *ne() const;
		Quadtree_node *sw() const;
		Quadtree_node *se() const;

		T min_x() const;
		T min_y() const;

		T max_x() const;
		T max_y() const;

		T sum_x() const;
		T sum_y() const;

		bool member( T const &, T const & ) const;

		bool insert( T const &, T const & );
		void clear();

	friend class Quadtree<T>;
};

template <typename T>
Quadtree_node<T>::Quadtree_node( T const &x, T const &y ):
x_value( x ), 
y_value( y ), 
north_west( 0 ),
north_east( 0 ),
south_west( 0 ),
south_east( 0 ) {
	// empty constructor
}

template <typename T>
Quadtree_node<T>::~Quadtree_node() {		// Destructor
	if( north_east != 0 ) {					// If the NE subtree is not null (ie, something has been inserted there)
		north_east->clear();				// Call clear on the Quadtree rooted at the NE child
		delete north_east;					// Delete the NE child
	}
	if( north_west != 0 ) {					// If the NE subtree is not null (ie, something has been inserted there)
		north_west->clear();				// Call clear on the Quadtree rooted at the NW child
		delete north_west;					// Delete the NW child
	}
	if( south_east != 0 ) {					// If the NE subtree is not null (ie, something has been inserted there)
		south_east->clear();				// Call clear on the Quadtree rooted at the SE child
		delete south_east;					// Delete the SE child
	}
	if( south_west != 0 ) {					// If the NE subtree is not null (ie, something has been inserted there)
		south_west->clear();				// Call clear on the Quadtree rooted at the SW child
		delete south_west;					// Delete the SW child
	}
}

template <typename T>
T Quadtree_node<T>::retrieve_x() const {
	return x_value;							// Return the x-value stored at this
}

template <typename T>
T Quadtree_node<T>::retrieve_y() const {
	return y_value;							// Return the y-value stored at this node
}

template <typename T>
Quadtree_node<T> *Quadtree_node<T>::nw() const {
	return north_west;						// Return the pointer to the NW child of this node
}

template <typename T>
Quadtree_node<T> *Quadtree_node<T>::ne() const {
	return north_east;						// Return the pointer to the NE child of this node
}

template <typename T>
Quadtree_node<T> *Quadtree_node<T>::sw() const {
	return south_west;						// Return the pointer to the SW child of this node
}

template <typename T>
Quadtree_node<T> *Quadtree_node<T>::se() const {
	return south_east;						// Return the pointer to the SE child of this node
}

template <typename T>
T Quadtree_node<T>::min_x() const {
	T min_x = 0;														// Initialize a value, min_x, to store the minimum found
	
	if ( north_west == 0 && south_west == 0 ) {							// If the current node has two West children who are null, then it means that this must be the lowest x-value possible (for this branch/leaf)
		min_x = x_value;												// Therefore, return the x-value stored here, since it is the minimum
	} else if ( north_west != 0 && south_west != 0 ) {					// If both West children are not null,
		min_x = std::min( north_west->min_x(), south_west->min_x() );	// Then take the min _x between the minimum of the two Quadtrees rooted at each child (ie, NW or SW child)
	} else if ( north_west != 0 && south_west == 0 ) {					// If the NW child is not null, and the SW child is null,
		min_x = north_west->min_x();									// Then take the min as the minimum of the Quadtree rooted at the NW child
	} else if ( north_west == 0 && south_west != 0  ) {					// If the SW child is not null, and the NW child is null,
		min_x = south_west->min_x();									// Then take the min as the minimum of the Quadtree rooted at the SW child
	}
	
	return min_x;														// Return the minimum x-value found of the Quadtree rooted at the current node
}

template <typename T>
T Quadtree_node<T>::min_y() const {
	T min_y = 0;														// Initialize a value, min_y, to store the minimum found
	
	if ( south_east == 0 && south_west == 0 ) {							// If the current node has two South children who are null, then it means that this must be the lowest y-value possible (for this branch/leaf)
		min_y = y_value;												// Therefore, return the y-value stored here, since it is the minimum
	} else if ( south_east != 0 && south_west != 0 ) {					// If both South children are not null,
		min_y = std::min( south_east->min_y(), south_west->min_y() );	// Then take the min_y between the minimum of the two Quadtrees rooted at each child (ie, SE or SW child)
	} else if ( south_east != 0 && south_west == 0 ) {					// If the SE child is not null, and the SW child is null,
		min_y = south_east->min_y();									// Then take the min as the minimum of the Quadtree rooted at the SE child
	} else if ( south_east == 0 && south_west != 0  ) {					// If the SW child is not null, and the SE child is null,
		min_y = south_west->min_y();									// Then take the min as the minimum of the Quadtree rooted at the SW child
	}
	
	return min_y;														// Return the minimum y-value found of the Quadtree rooted at the current node
}

template <typename T>
T Quadtree_node<T>::max_x() const {
	T max_x = 0;														// Initialize a value, max_x, to store the maximum found
	
	if ( north_east == 0 && south_east == 0 ) {							// If the current node has two East children who are null, then it means that this must be the highest x-value possible (for this branch/leaf)
		max_x = x_value;												// Therefore, return the x-value stored here, since it is the maximum
	} else if ( north_east != 0 && south_east != 0 ) {					// If both East children are not null,
		max_x = std::max( north_east->max_x(), south_east->max_x());	// Then take the max_x between the maximum of the two Quadtrees rooted at each child (ie, NE or SE child)
	} else if ( north_east != 0 && south_east == 0 ) {					// If the NE child is not null, and the SE child is null,
		max_x = north_east ->max_x();									// Then take the max as the maximum of the Quadtree rooted at the NE child
	} else if ( north_east == 0 && south_east != 0  ) {					// If the SE child is not null, and the NE child is null,
		max_x = south_east->max_x();									// Then take the max as the maximum of the Quadtree rooted at the SE child
	}
	
	return max_x;														// Return the maximum x-value found of the Quadtree rooted at the current node
}

template <typename T>
T Quadtree_node<T>::max_y() const {
	T max_y = 0;														// Initialize a value, max_y, to store the maximum found
	
	if ( north_east == 0 && north_west == 0 ) {							// If the current node has two North children who are null, then it means that this must be the highest y-value possible (for this branch/leaf)
		max_y = y_value;												// Therefore, return the y-value stored here, since it is the maximum
	} else if ( north_east != 0 && north_west != 0 ) {					// If both South children are not null,
		max_y = std::max( north_east ->max_y(), north_west->max_y());	// Then take the max_y between the maximum of the two Quadtrees rooted at each child (ie, NE or NW child)
	} else if ( north_east != 0 && north_west == 0 ) {					// If the NE child is not null, and the NW child is null,
		max_y = north_east ->max_y();									// Then take the max as the maximum of the Quadtree rooted at the NE child
	} else if ( north_east == 0 && north_west != 0  ) {					// If the NW child is not null, and the NE child is null,
		max_y = north_west->max_y();									// Then take the max as the maximum of the Quadtree rooted at the NW child
	}
	
	return max_y;														// Return the maximum y-value found of the Quadtree rooted at the current node
}

template <typename T>
T Quadtree_node<T>::sum_x() const {
	T sum_x = 0;							// Initialize a value, sum_x, to continuously add up the sum found
	
	if ( this == 0 ) {						// If the current node is a null pointer,
		return 0;							// Then return zero, since it means that there is no value stored at this node, and attempting to access a value would cause an error
	}
											// Past here, the node is not a null pointer, meaning that there is some value stored here (even if it is zero)
	if ( north_east != 0 ) {				// If the NE child is not null,
		sum_x += north_east->sum_x();		// Then add to sum_x the sum of x-values of the Quadtree rooted at the NE child
	}
	if ( south_east != 0 ) {				// If the SE child is not null,
		sum_x += south_east->sum_x();		// Then add to sum_x the sum of x-values of the Quadtree rooted at the SE child
	}
	if ( north_west != 0 ) {				// If the NW child is not null,
		sum_x += north_west->sum_x();		// Then add to sum_x the sum of x-values of the Quadtree rooted at the NW child
	}
	if ( south_west != 0 ) {				// If the SW child is not null,
		sum_x += south_west->sum_x(); 		// Then add to sum_x the sum of x-values of the Quadtree rooted at the SW child
	}
	sum_x += x_value;						// Add to sum_x the x-value stored at the current node
	return sum_x;							// Return the sum of x-values of the Quadtree rooted at the current node
}

template <typename T>
T Quadtree_node<T>::sum_y() const {
	T sum_y = 0;							// Initialize a value, sum_x, to continuously add up the sum found
	
	if ( this == 0 ) {						// If the current node is a null pointer,
		return 0;							// Then return zero, since it means that there is no value stored at this node, and attempting to access a value would cause an error
	}
	
	if ( north_east != 0 ) {				// If the NE child is not null,
		sum_y += north_east->sum_y();		// Then add to sum_y the sum of y-values of the Quadtree rooted at the NE child
	}
	if ( south_east != 0 ) {				// If the SE child is not null,
		sum_y += south_east->sum_y();		// Then add to sum_y the sum of y-values of the Quadtree rooted at the SE child
	}
	if ( north_west != 0 ) {				// If the NW child is not null,
		sum_y += north_west->sum_y();		// Then add to sum_y the sum of y-values of the Quadtree rooted at the NW child
	}
	if ( south_west != 0 ) {				// If the SW child is not null,
		sum_y += south_west->sum_y();		// Then add to sum_y the sum of y-values of the Quadtree rooted at the SW child
	}
	sum_y += y_value;						// Add to sum_y the y-value stored at the current node
	return sum_y;							// Return the sum of y-values of the Quadtree rooted at the current node
}

template <typename T>
bool Quadtree_node<T>::member( T const &x, T const &y ) const {

	if( this == 0 ) {								// If the current node is a null pointer,
		return false;								// Return false, as the set being searched for is not a member of this Quadtree
	} else if ( x == x_value && y == y_value ) {  	// If the x and y values of the current node are equal to the x and y values being searched for,
		return true;								// Return true, as the member does exist in this Quadtree
	}
	
	if ( x >= x_value && y >= y_value  ) {			// If the wanted x-value and y-value are both respectively greater than the x-value and y-value of the current node, 
		return north_east->member(x,y);				// Then search for the wanted values in the NE child of this node
	} else if ( x >= x_value && y < y_value ) {		// If the wanted x-value is greater than or equal to the x-value of the current node, and the wanted y-value is less than the y-value of the current node, 
		return south_east->member(x,y);				// Then search for the wanted values in the SE child of this node
	} else if( x < x_value && y >= y_value ) {		// If the wanted x-value is less than the x-value of the current node, and the wanted y-value is greater than or equal to the y-value of the current node,
		return north_west->member(x,y);				// Then search for the wanted values in the NW child of this node
	} else if ( x < x_value && y < y_value ) {		// If the wanted x-value and y-value are both respectively less than the x-value and y-value of the current node, 
		return south_west->member(x,y);				// Then search for the wanted values in the SW child of this node
	}
}

template <typename T>
bool Quadtree_node<T>::insert( T const &x, T const &y ) {
	if( x == x_value && y == y_value ) {				// If the values to be inserted match an already existing set of values on the Quadtree,
		return false;									// Then return false, as insertion was not successful
	}
	
	if( x >= x_value && y >= y_value ) {				// If the inserting x-value and y-value are both respectively greater than the x-value and y-value of the current node, 
		if ( north_east == 0 ) {						// If the NE child is null (ie, does not exist as a part of the Quadtree),
			north_east = new Quadtree_node<T>(x,y);		// Then create the NE child with the values being inserted
			return true;								// Return true, since insertion was successful
		} else {
			north_east->insert(x,y);					// Otherwise, if the NE child exists, then look for a place to insert the values on the Quadtree rooted at the NE child
		}
	} else if( x >= x_value && y < y_value ) {			// If the inserting x-value is greater than or equal to the x-value of the current node, and the wanted y-value is less than the y-value of the current node, 
		if ( south_east == 0 ) {						// If the SE child is null (ie, does not exist as a part of the Quadtree),
			south_east = new Quadtree_node<T>(x,y);		// Then create the SE child with the values being inserted
			return true;								// Return true, since insertion was successful
		} else {
			south_east->insert(x,y);					// Otherwise, if the SE child exists, then look for a place to insert the values on the Quadtree rooted at the SE child
		}
	} else if ( x < x_value && y >= y_value ) {			// If the wanted x-value is less than the x-value of the current node, and the wanted y-value is greater than or equal to the y-value of the current node,
		if ( north_west == 0 ) {						// If the NW child is null (ie, does not exist as a part of the Quadtree),
			north_west = new Quadtree_node<T>(x,y);		// Then create the NW child with the values being inserted
			return true;								// Return true, since insertion was successful
		} else {
			north_west->insert(x,y);					// Otherwise, if the NW child exists, then look for a place to insert the values on the Quadtree rooted at the NW child
		}
	} else if ( x < x_value && y < y_value ) {			// If the wanted x-value and y-value are both respectively less than the x-value and y-value of the current node, 
		if ( south_west == 0 ) {						// If the SW child is null (ie, does not exist as a part of the Quadtree),
			south_west = new Quadtree_node<T>(x,y);		// Then create the SW child with the values being inserted
			return true;								// Return true, since insertion was successful
		} else {
			south_west->insert(x,y);					// Otherwise, if the SW child exists, then look for a place to insert the values on the Quadtree rooted at the SW child
		}
	}
}

template <typename T>
void Quadtree_node<T>::clear() {
	if( north_east != 0 ) {					// If the NE child is not null,
		north_east->clear();				// call clear() on the NE child
		delete north_east;					// delete the NE child
		north_east = 0;						// set the pointer of the NE child back to zero
	}
	if( north_west != 0 ) {					// If the NW child is not null,
		north_west->clear();				// call clear() on the NW child
		delete north_west;					// delete the NW child
		north_west = 0;						// set the pointer of the NW child back to zero
	}
	if( south_east != 0 ) {					// If the SE child is not null,
		south_east->clear();				// call clear() on the SE child
		delete south_east;					// delete the SE child
		south_east = 0;						// set the pointer of the SE child back to zero
	}
	if( south_west != 0 ) {					// If the SW child is not null,
		south_west->clear();				// call clear() on the SW child
		delete south_west;					// delete the SW child
		south_west = 0;						// set the pointer of the SW child back to zero
	}
	/*if ( this != 0 ) {
		north_east->clear();
		north_west->clear();
		south_east->clear();
		south_west->clear();
		this = 0;
	}*/
	return;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif

