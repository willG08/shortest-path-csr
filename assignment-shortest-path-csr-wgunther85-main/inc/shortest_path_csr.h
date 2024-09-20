//Copyright 2023, Bradley Peterson, Weber State University, All rights reserved. (12/2023)
#ifndef SHORTEST_PATH_CSR_H
#define SHORTEST_PATH_CSR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

// Declaring global variables
extern int* graphWeights;
extern int* columns;
extern int* rows;

extern int* pathCost;
extern int* thisCameFrom;

extern int numEdges;
extern int numNodes;

extern const int LARGE_NUMBER;

//The collection of edge objects
struct graphEdge;
extern vector<graphEdge> edges;

//The class for an edge object.  
struct graphEdge {
public:
  int sourceNode{ 0 };
  int destNode{ 0 };
  int weight{ 0 };
  graphEdge() {};
  graphEdge(int sourceNode, int destNode, int weight) {
    this->sourceNode = sourceNode;
    this->destNode = destNode;
    this->weight = weight;

  }
};


// Functions
int getEdgeWeight(int sourceIndex, int destIndex);

void shortestPath(int vertex);

string getPath(int sourceIndex, int destIndex);

void testRun();

void readFile();

void createCsrArrays();

void deleteArrays();

#endif // SHORTEST_PATH_CSR_H

