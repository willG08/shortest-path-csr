//Copyright 2023, Bradley Peterson, Weber State University, All rights reserved. (12/2023)
#include "shortest_path_csr.h"


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <stack>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::stack;


//TODO: Declare *five* arrays 
// Define three CSR arrays: graphWeights, columns, rows
// Also define two Dijkstra arrays: pathCost, thisCameFrom
// Do not define another Disjkstra array, bestPathFound, as it is created in the shortestPath() algorithm itself.
// The five arrays defined allocated in another function.
int* graphWeights{ nullptr };
int* columns{ nullptr };
int* rows{ nullptr };

int* pathCost{ nullptr };
int* thisCameFrom{ nullptr };

int numEdges{ 0 };
int numNodes{ 0 };

//Here is where you create your global dynamic arrays
const int LARGE_NUMBER{ 99999999 };

// This is just here so the code compiles. It's filled with garbage data, so don't trust it.
int weightsIn2D[5][5];

//The collection of edge objects
vector<graphEdge> edges;

int getEdgeWeight(int sourceIndex, int destIndex) {

  // TODO: Go through your arrays to see if there is an edge between sourceIndex and endIndex, 
  // and if so, return the cost. If not, return the large number.
    int startIndex = rows[sourceIndex];
    int endIndex = rows[sourceIndex + 1];
    int weight = LARGE_NUMBER;

    //loop from startindex to endIndex in the columns array
    for (int i = startIndex; i < endIndex; i++) {
        //search each of these indexes for the destination Index. 
        if (columns[i] == destIndex) {
            weight = graphWeights[i];
        }
    }

    return weight;
}

void shortestPath(int vertex) {
  //The argument is the vertex to search from

  // TODO: Fix the code so that it works without 2D array data 

  for (int j = 0; j < numNodes; j++) {
    //intialization step
      pathCost[j] = getEdgeWeight(vertex, j);
      thisCameFrom[j] = vertex;
        //weightsIn2D[vertex][j];
  }
  //continue initializing
  unique_ptr<bool[]> bestPathFound = make_unique<bool[]>(numNodes);
  for (int j = 0; j < numNodes; j++) {
    bestPathFound[j] = false;
  }

  //The node we're at we assume we can get to with a cost of zero.
  bestPathFound[vertex] = true;
  pathCost[vertex] = 0;

  //For every node...
  for (int i = 0; i < numNodes - 1; i++) {
    if (i % 100 == 0) {
      cout << "Looking up shortest path for " << i << " of " << numNodes - 1 << " amount of nodes" << endl;

    }
    else if (i == numNodes - 2) {
      cout << "Looking up the shortest path for the last node" << endl;
    }
    int minWeight = LARGE_NUMBER;
    int v = -1;

    //Of the nodes who haven't been marked as completed,
    //or in other words, nodes which we aren't sure if we've found the
    //smallest path weight
    //Of those ndoes...find the node with the smallest current weight.
    for (int j = 0; j < numNodes; j++) {
      if (!bestPathFound[j]) {
        if (pathCost[j] < minWeight) {
          v = j;
          minWeight = pathCost[v];
        }
      }
    }

    //Ignore non connected nodes.
    if (v != -1) {

      bestPathFound[v] = true;
      //Now that we've found a new shortest possible weight (at node v)
      //look at all of v's neighborly costs, and see if we can get to v's neighbors
      //using v, at a better weight than what we already know.
      for (int j = 0; j < numNodes; j++) {
        if (!bestPathFound[j]) {
          if (minWeight + getEdgeWeight(v, j) < pathCost[j]) {
           pathCost[j] = minWeight + getEdgeWeight(v, j);
           thisCameFrom[j] = v;
          }
        }
      }
    }
  }
}

void createCsrArrays() {

  // TODO:
  // Take the collection of graphEdge objects in our edges vector, and convert them into CSR format
  // For example, this graph form figure 12-8 would look like this as a 2-D array:
  //     0         1         2         3         4
  //   +--------------------------------------------
  // 0 | 0         16        99999999  2         3
  // 1 | 99999999  0         5         99999999  99999999
  // 2 | 99999999  3         0         99999999  99999999
  // 3 | 99999999  12        99999999  0         7
  // 4 | 99999999  10        4         5         0
  // But we don't have a 2D array in this assignment.  We have a vector/collection object.
  // In this vector object, edges are simply listed one edge at a time, so the 0th
  // object should contain (0, 1, 16) for 0 goes to 1 with a cost of 16.

  // In CSR format the data looks like this:
  // graphWeights  : 16 2  3  5  3  12 7 10 4  5      
  // columns (dest): 1  3  4  2  1  1  4 1  2  3      
  // row (indexes):  0  3  4  5  7  10     

  // TODO: Allocate three global arrays, with names of graphWeights, columns, and row.
    graphWeights = new int[numEdges];
    columns = new int[numEdges];
    rows = new int[numEdges];

  // TODO: Also allocate the two arrays needed for the Dijkstra's shortest path algorithm: pathCost and thisCameFrom. Initialize values
    pathCost = new int[numEdges];
    thisCameFrom = new int[numEdges];
    for (int i = 0; i < numEdges; i++) {
        pathCost[i] = LARGE_NUMBER;
        thisCameFrom[i] = -1;
    }

  // TODO: Iterate through the edges vector to load the graphWeights, columns, and row array with correct data
  // Note: This only takes about 15 lines of code. But you need to carefully think this out. Do not
  // be discouraged when your first few attempts don't work.
    int countEdges = 0;
    int nextNode = 1;
    for (int i = 0; i < numEdges; i++) {

        graphWeights[i] = edges[i].weight;
        columns[i] = edges[i].destNode;
        if (i == 0) {
            rows[0] = 0;
            countEdges++;
           
        }
        else if(edges[i].sourceNode == edges[i-1].sourceNode) {
            countEdges++;
            
        }
        else {
            rows[nextNode] = rows[nextNode-1] + countEdges;
            nextNode++;
            countEdges = 1;
        }
    }
    //last row needs to be recorded
    rows[nextNode] = rows[nextNode - 1] + countEdges;
}


string getPath(int sourceIndex, int destIndex) {
  string retVal;
  vector<int> backwards;
  int myIndex = destIndex;
  
  // TODO: Return the string representation of the shortest path from sourceIndext to destIndex
  // If the shortest path between sourceIndex = 0 and sourceIndex = 1 were 0, 4, 2, 1, then 
  // return "0 -> 4 -> 2 -> 1"
  backwards.push_back( destIndex);
  if (thisCameFrom[sourceIndex] != thisCameFrom[myIndex]) {
      do {
          //for (int i = 0; i < numEdges; i++) {
          backwards.push_back(thisCameFrom[myIndex]);
          myIndex = thisCameFrom[myIndex];
      } while (thisCameFrom[sourceIndex] != thisCameFrom[myIndex]);
  }
  

  //if size is less than two we will be double printing the source node
  if (thisCameFrom[sourceIndex] != backwards[0]) {
      backwards.push_back(sourceIndex);
  }

  for (int i = backwards.size()-1; i > -1; i--) {
      retVal += std::to_string(backwards[i]);
      if (i > 0) {
          retVal += " -> ";
      }
  }

  cout << endl;
  cout << "source index " << sourceIndex << ", thisCameFrom " << thisCameFrom[sourceIndex] << endl;
  cout << "dest index " << destIndex << ", thisCameFrom " << thisCameFrom[destIndex] << endl;
  cout << endl;

  return retVal;
}


void deleteArrays() {
  //TODO: Delete the arrays allocated in createCsrArrays()
    delete[] graphWeights;
    delete[] columns;
    delete[] rows;
    delete[] pathCost;
    delete[] thisCameFrom;
}