#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#include <iostream>
#include <limits>
#include "Exception.h"

/****************************************
 * UW User ID:  h382zhan
 * Submitted for ECE 250
 * Semester of Submission:  Winter 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ****************************************/

class Weighted_graph {
	private:
		static const double INF;
		
		double **graph_matrix;					// An adjacency matrix
		int *degV;								// Record the degree of each vertex in an array
		int V;									// Count number of edges
		int E;									// Count number of vertices

	public:
		Weighted_graph( int = 50 );
		~Weighted_graph();

		int degree( int ) const;
		int edge_count() const;
		double adjacent( int, int ) const;
		double minimum_spanning_tree( int ) const;
		bool is_connected() const;

		void insert( int, int, double );

	// Friends

	friend std::ostream &operator << ( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

Weighted_graph::Weighted_graph( int n ):		// Constructor
V( 0 ),											// Initialize edge and vertex count as zero
E( 0 ) {
	if ( n < 0 ) {								// If the number of vertices wanted is less than zero
		throw illegal_argument();				// Then throw illegal argument
	}
	
	V = n;										// Set number of vertices to n
	graph_matrix = new double*[V];				// Start the matrix as an array, carrying arrays in each index
	degV = new int[V];							// Records the degree of each vertex in array of size V
	for ( int r = 0; r < V; r++ ) {				// Loop from 0 to V-1, representing the rows of the matrix
		graph_matrix[r] = new double[V];		// Declare each index of the matrix as an array of size V
		degV[r] = 0;							// Degree of each vertex is zero, no connections initially
		for ( int c = 0; c < V; c++ ) {			// Loop from 0 to V-1, representing the columns of the matrix
			graph_matrix[r][c] = INF;			// Edge value of each vertex zerp, no connections
			if ( r == c ) {						// If the row and column are the same (ie, a diagonal entry)
				graph_matrix[r][c] = 0;			// Make its edge value zero, since it is the same vertex
			}
		}
	}
}

Weighted_graph::~Weighted_graph() {				// Destructor
	for ( int r = 0; r < V; r++ ) {				// Loop from 0 to V-1
		delete[] graph_matrix[r];				// Delete each row of the matrix, since each row is an array
	}
	delete[] graph_matrix;						// Delete the now-empty matrix
	delete[] degV;								// Delete the degree array
	
	V = 0;										// Reset edge and vertex count to zero
	E = 0;
	
	return;
}

int Weighted_graph::degree( int n ) const {		// degree function, returns the degree of a vertex
	if ( n >= V || n < 0 ) {					// If the vertex does not exist (ie, larger than matrix index or negative)
		throw illegal_argument();				// Then throw illegal argument
	}
	
	return degV[n];								// Return the degree of the vertex, stored in the degree array
}

int Weighted_graph::edge_count() const {
	return E;									// Return the edge count
}

double Weighted_graph::adjacent( int m, int n ) const {
	if ( m >= V || n >= V || m < 0 || n < 0 ) { // If the vertex does not exist (ie, larger than matrix index or negative)
		throw illegal_argument();				// Then throw illegal argument
	}
	
	return graph_matrix[m][n];					// Return the value of the edge connecting the two vertices, which is stored inside the matrix
}

bool Weighted_graph::is_connected() const {
	if ( V == 1 ) {								// If the size of the graph is only one vertex
		return true;							// Then return true, since it is 'connected' by definition
	}
	
	int *visited = new int[V];					// Initialize an array to store visited nodes, up to the total number of vertices in the graph
	for ( int i = 0; i < V; i ++ ) {			// From 0 to V-1 (ie, the size of the array)
		visited[i] = V;							// Set the default value of each index to V, which is one larger than the size of the array, representing an unvisited node
	}
	visited[0] = 0;								// Start looking for connectivity at vertex 0, so mark it as visited by making the value of the index 0 equal to zero
	
	for ( int i = 0; i < V-1; i++ ) {			// Need to loop V-1 times, in case the graph is connected in a way that requires finding nodes at descending value (ie, graph looks like 0-4-3-2-1)
		for ( int r = 0; r < V; r++ ) {			// Loop through each node to find a node that we have visited
			if ( visited[r] == r ) {			// On a node that we have visited
				for ( int c = 0; c < V; c++ ) {	// Loop to check each of its edges
					if ( r != c && visited[c] != c && graph_matrix[r][c] != INF ) {
												// If r and c are not the same node AND c is not yet visited AND the edge from vertex r to vertex c exists (ie, not INF)
						visited[c] = c;			// Then in the visited array, set the value of index c equal to c, representing that we have visited node c
					}
				}
			}		
		}
	}
	
	for ( int j = 0; j < V; j++ ) {				// Loop through each element in the visited array
		if ( visited[j] != j ) {				// If the value of index j is not equal to j, it means that it has not been marked as visited, so there is an unvisited node
			delete[] visited;					// Delete the visited array to prevent memory leaks
			return false;						// Then return false, since there is at least one unvisited node, so the graph is not connected
		}
	}
	delete[] visited;							// Delete the visited array
	return true;								// Since the above loop did not catch one unvisited node, it means that all nodes must have been visited, so the graph is connected
}

double Weighted_graph::minimum_spanning_tree( int m ) const {
	if ( m >= V || m < 0 ) {					// If the vertex does not exist (ie, larger than matrix index or negative)
		throw illegal_argument();				// Then throw illegal argument
	}
	
	double MST_size = 0;						// Initialize the size of the MST as zero
	int newestNode = m;							// Declare a way to track the "newest node", which is initialized as m
	
	int *MST_nodes = new int[V];				// Create an array to store nodes we have visited, and have included in the MST
	for ( int i = 0; i < V; i ++ ) {
		MST_nodes[i] = V;						// Initialize each of the values in MST_nodes to V, indicating unvisited
	}
	MST_nodes[m] = m;							// Make the index of node m in the MST_nodes array as m, indicating that we have visited it, since it is the node we are starting on
	
	while ( 1 ) {								// Loop forever, condition inside will check if MST has been fully explored
		double minEdge = INF;					// Initialize the minimum edge value as Infinity
		
		for ( int r = 0; r < V; r++ ) {			// Loop from 0 to V-1
			if ( MST_nodes[r] == r ) {			// If the current node indicated by the loop value r has been marked as visited in the MST_nodes array
				for ( int c = 0; c < V; c++ ) {	// Then loop from 0 to V-1
					if ( r != c && MST_nodes[c] != c && graph_matrix[r][c] < minEdge ) {
														// If r and c are not the same node AND c is not yet visited AND the edge value between vertex r and vertex c is less than the current minimum edge value
						minEdge = graph_matrix[r][c];	// Then the minimum edge is the edge between vertex r and vertex c, since the third condition says that its value is less than the previous minimum
						newestNode = c;					// Make c the newest node
					}
				}
			}
		}
		
		if ( minEdge == INF ) {					// If the minimum edge found in this iteration of the infinite while loop is Infinity (ie, the initial value), it means that there are no nodes left to explore in the MST, since the minimum edge value has not changed this iteration
			delete[] MST_nodes;					// Delete the array containing the visited nodes to avoid memory leak
			return MST_size;					// Return the size (combined edge value) of the MST
		}
		
		MST_nodes[newestNode] = newestNode; 	// Mark the newest node, saved from the node that had the minimum edge value, as visited
		MST_size += minEdge;					// Add the minimum edge value of this iteration onto the current count of the size (combined edge value) of the MST
	}
}

void Weighted_graph::insert( int m, int n, double w ) {
	if ( w < 0 || w == INF || m == n || m >= V || n >= V || m < 0 || n < 0 ) {
												// If the vertex does not exist (ie, larger than matrix index or negative) OR the weight is an invalid value (ie, negative or infinity) OR if m and n are equal (ie, the same vertex)
		throw illegal_argument();				// Then throw illegal argument
	}
	
	if ( w == 0 ) {								// If the weight of the edge to be inserted is zero
		if ( graph_matrix[m][n] == INF ) {		// If the weight of the edge between vertex m and vertex n is Infinity (ie, there is no edge)
			return;								// Then return without doing anything, since inserting zero implies wanting to remove and edge, and there is no edge
		}										
		graph_matrix[m][n] = INF;				// Arriving at the code below means that there does exist an edge between m and n, since it passed the if statement
		graph_matrix[n][m] = INF;				// Therefore, set the entries in the matrix indicating the edge between m and n to Infinity, indicating that there is no edge
		degV[m]--;								// Decrement the degree of m, since one of its edges was removed
		degV[n]--;								// Decrement the degree of n, since one of its edges was removed
		E--;									// Decrement the total edge count, since an edge was removed from the graph
		return;									// Return so that the code below is not executed
	}
												// Arriving at this code means that the edge weight to be inserted is not zero
	if ( graph_matrix[m][n] == INF ) {			// If the weight of the edge between vertex m and vertex n is Infinity (ie, there is no edge)
		degV[m]++;								// Increment the degree of m, since an edge was added to is
		degV[n]++;								// Increment the degree of n, since an edge was added to is
		E++;									// Increment the total edge count, since an edge was added to the graph
	}
	graph_matrix[m][n] = w;						// Set the entries of the matrix indicating the edge between vertex m and vertex equal to the weight
	graph_matrix[n][m] = w;						// which indicates that the weight of the edge between m and n is equal to w
	return;
}

std::ostream &operator << ( std::ostream &out, Weighted_graph const &graph ) {
	// Your implementation

	return out;
}

#endif
