//Copyright 2023, Bradley Peterson, Weber State University, All rights reserved. (12/2023)

#include "shortest_path_csr.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>


using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stoi;
using std::ifstream;
using std::istringstream;

//This helps with testing, comment it in when ready, but do not modify the code.
template <typename T>
bool checkTest(int testNum, int& correct, T* whatItShouldBe, T* whatItIs) {

  if (strcmp(whatItShouldBe, whatItIs) == 0) {
    correct++;
    cout << "Passed " << testNum << endl;
    return true;
  }
  else {
    if (whatItShouldBe == "") {
      cout << "***Failed test " << testNum << " *** " << endl << "   Output was " << whatItIs << endl << "   Output should have been blank. " << endl;
    }
    else {
      cout << "***Failed test " << testNum << " *** " << endl << "   Output was " << whatItIs << endl << "   Output should have been " << whatItShouldBe << endl;
    }
    cout << "The unit test checker is closing down the program now due to a failed test" << endl;
    exit(1);
    return false;
  }
}

template <typename T>
bool checkTest(int testNum, int& correct, T whatItShouldBe, T whatItIs) {

  if (whatItShouldBe == whatItIs) {
    correct++;
    cout << "Passed " << testNum << endl;
    return true;
  }
  else {

    cout << "***Failed test " << testNum << " *** " << endl << "   Output was " << whatItIs << endl << "   Output should have been " << whatItShouldBe << endl;
    cout << "The unit test checker is closing down the program now due to a failed test" << endl;
    exit(1);
    return false;
  }
}


//Some functions I wrote to help the stable sort know what to sort against.  
//You don't need to worry about these or modify them.  
bool compareFirstColumn(const graphEdge& x, const graphEdge& y) {
  if (x.sourceNode < y.sourceNode) {
    return true;
  }
  else {
    return false;
  }
}
bool compareSecondColumn(const graphEdge& x, const graphEdge& y) {
  if (x.destNode < y.destNode) {
    return true;
  }
  else {
    return false;
  }
}

void testRun() {
  //This loads the same values found in the book:
  edges.push_back(graphEdge(0, 1, 16));
  edges.push_back(graphEdge(0, 3, 2));
  edges.push_back(graphEdge(0, 4, 3));
  edges.push_back(graphEdge(1, 2, 5));
  edges.push_back(graphEdge(2, 1, 3));
  edges.push_back(graphEdge(3, 1, 12));
  edges.push_back(graphEdge(3, 4, 7));
  edges.push_back(graphEdge(4, 1, 10));
  edges.push_back(graphEdge(4, 2, 4));
  edges.push_back(graphEdge(4, 3, 5));

  numNodes = 5;
  numEdges = 10;

}

void readFile() {

  //std::filesystem::path cwd = std::filesystem::current_path() / "filename.txt";
  //std::ofstream file(cwd.string());
  //file.close();

  ifstream inFile("rome99.gr");
  int counter = 0;
  int largestNode = 0;
  char throwaway;
  if (!inFile.good())
  {
    cerr << "The file wasn't found. Pester Brad Peterson, something went wrong with CMake here" << endl;
    exit(-1);
  }
  else {
    string line;
    graphEdge edge;
    while (getline(inFile, line)) {


      if (line.at(0) == 'a' && line.at(1) == ' ') {
        if (counter % 10000 == 0) {
          cout << "Reading edge # " << counter << endl;
        }
        //this line is one we keep, read in the data
        istringstream iss(line);
        iss >> throwaway >> edge.sourceNode >> edge.destNode >> edge.weight;
        if (edge.sourceNode > largestNode) {
          largestNode = edge.sourceNode;
        }

        if (edge.destNode > largestNode) {
          largestNode = edge.destNode;
        }
        edges.push_back(edge);
        counter++;
      }
    }
    numNodes = largestNode;
    numEdges = counter;


    //Create a zero node with an edge that points to itself with a weight of 0.
    //The file node data starts at node #1, so we want to make 
    //everything clean by letting edge 1 take index 1 in our graphWeights array,
    //and this zero node can take index 0.  
    graphEdge zeroEdge(0, 0, 0);
    edges.push_back(zeroEdge);
    numNodes++;
    numEdges++;
    //Because we started a zero node, increase the numNodes by 1., 
    cout << "Finished reading " << numNodes << " nodes and " << numEdges << " edges." << endl;
  }
}


//This helps with testing, do not modify.
bool testBasicGraph() {

  int testNum = 1;
  int correct = 0;
  cout << "--------testBasicGraph Tests--------" << endl;

  testRun();
  //The collection of edge objects is just an unsorted collection.  
  //So use a stable sort to sort by first column and second column so 
  //they are in a clean order ready to go into CSR format.
  std::stable_sort(edges.begin(), edges.end(), compareSecondColumn);
  cout << "Halfway done sorting..." << endl;
  std::stable_sort(edges.begin(), edges.end(), compareFirstColumn);
  cout << "Finished sorting..." << endl;

  createCsrArrays();

  cout << "Basic run on a graph of five nodes. Your CSR arrays are: " << endl;
  cout << "weights: ";
  string weightsString;
  for (int i = 0; i < numEdges; i++) {
    cout << graphWeights[i] << " ";
    weightsString += std::to_string(graphWeights[i]) + " ";
  }
  cout << endl;
  cout << "columns: ";
  string columnsString;
  for (int i = 0; i < numEdges; i++) {
    cout << columns[i] << " ";
    columnsString += std::to_string(columns[i]) + " ";
  }
  cout << endl;
  cout << "row: ";
  string rowString;
  for (int i = 0; i < numNodes + 1; i++) {
    cout << rows[i] << " ";
    rowString += std::to_string(rows[i]) + " ";
  }
  cout << endl;
  
  checkTest(testNum++, correct, string("16 2 3 5 3 12 7 10 4 5 "), weightsString); // 1
  checkTest(testNum++, correct, string("1 3 4 2 1 1 4 1 2 3 "), columnsString); // 2
  checkTest(testNum++, correct, string("0 3 4 5 7 10 "), rowString); // 3

  // Check the getPathCost() function
  checkTest(testNum++, correct, 16, getEdgeWeight(0, 1)); // 4
  checkTest(testNum++, correct, 2, getEdgeWeight(0, 3)); // 5
  checkTest(testNum++, correct, 3, getEdgeWeight(0, 4)); // 6
  checkTest(testNum++, correct, 5, getEdgeWeight(1, 2)); // 7
  checkTest(testNum++, correct, 3, getEdgeWeight(2, 1)); // 8
  checkTest(testNum++, correct, 12, getEdgeWeight(3, 1)); // 9
  checkTest(testNum++, correct, 7, getEdgeWeight(3, 4)); // 10
  checkTest(testNum++, correct, 10, getEdgeWeight(4, 1)); // 11
  checkTest(testNum++, correct, 4, getEdgeWeight(4, 2)); // 12
  checkTest(testNum++, correct, 5, getEdgeWeight(4, 3)); // 13
  checkTest(testNum++, correct, LARGE_NUMBER, getEdgeWeight(0, 2)); // 14
  checkTest(testNum++, correct, LARGE_NUMBER, getEdgeWeight(1, 4)); // 15
  checkTest(testNum++, correct, LARGE_NUMBER, getEdgeWeight(4, 4)); // 16

  //call our shortest path algorithm
  shortestPath(0);

  cout << "The shortest path from node 0..." << endl;
  cout << " to node 1 is: " << pathCost[1] << endl;
  cout << " to node 2 is: " << pathCost[2] << endl;
  cout << " to node 3 is: " << pathCost[3] << endl;
  cout << " to node 4 is: " << pathCost[4] << endl;
  checkTest(testNum++, correct, pathCost[1], 10); // 17
  checkTest(testNum++, correct, pathCost[2], 7); // 18
  checkTest(testNum++, correct, pathCost[3], 2); // 19
  checkTest(testNum++, correct, pathCost[4], 3); // 20
 
  string path = getPath(0, 1);
  cout << "The path from node 0 node 1 is " << path << endl;
  checkTest(testNum++, correct, path, string("0 -> 4 -> 2 -> 1")); // 21
  path = getPath(0, 2);
  cout << "The path from node 0 node 2 is " << path << endl;
  checkTest(testNum++, correct, path, string("0 -> 4 -> 2")); // 22
  path = getPath(0, 3);
  cout << "The path from node 0 node 3 is " << path << endl;
  checkTest(testNum++, correct, path, string("0 -> 3")); // 23
  path = getPath(0, 4);
  cout << "The path from node 0 node 4 is " << path << endl;
  checkTest(testNum++, correct, path, string("0 -> 4")); // 24

  deleteArrays();
  edges.clear();

  return testNum - 1 == correct;

}


//This helps with testing, do not modify.
bool testLargeGraph() {

  int testNum = 1;
  int correct = 0;
  cout << "--------testLargeGraph Tests--------" << endl;

  readFile();
  //The collection of edge objects is just an unsorted collection.  
  //So use a stable sort to sort by first column and second column so 
  //they are in a clean order ready to go into CSR format.
  std::stable_sort(edges.begin(), edges.end(), compareSecondColumn);
  cout << "Halfway done sorting..." << endl;
  std::stable_sort(edges.begin(), edges.end(), compareFirstColumn);
  cout << "Finished sorting..." << endl;

  createCsrArrays();

  cout << "Checking the first ***20*** entries of each CSR array: " << endl;
  cout << "weights: ";
  string weightsString;
  for (int i = 0; i < 20; i++) {
    cout << graphWeights[i] << " ";
    weightsString += std::to_string(graphWeights[i]) + " ";
  }
  cout << endl;
  cout << "columns: ";
  string columnsString;
  for (int i = 0; i < 20; i++) {
    cout << columns[i] << " ";
    columnsString += std::to_string(columns[i]) + " ";
  }
  cout << endl;
  cout << "row: ";
  string rowString;
  for (int i = 0; i < 20 + 1; i++) {
    cout << rows[i] << " ";
    rowString += std::to_string(rows[i]) + " ";
  }
  cout << endl;

  checkTest(testNum++, correct, string("0 193 2172 193 188 403 188 403 2295 2007 2295 1807 126 760 126 1807 760 1642 96 2815 "), weightsString); // 1
  checkTest(testNum++, correct, string("0 2 22 1 3 4 2 2 5 21 4 8 7 8 6 5 6 12 10 11 "), columnsString); // 2
  checkTest(testNum++, correct, string("0 1 3 6 7 10 12 14 15 18 21 22 25 28 29 33 36 39 42 45 48 "), rowString); // 3
  
  checkTest(testNum++, correct, 42, getEdgeWeight(1947, 1943)); // 4
  checkTest(testNum++, correct, 326, getEdgeWeight(2794, 2796)); // 5
  checkTest(testNum++, correct, 105, getEdgeWeight(1337, 1336)); // 6
  checkTest(testNum++, correct, LARGE_NUMBER, getEdgeWeight(420, 69)); // 7
  checkTest(testNum++, correct, LARGE_NUMBER, getEdgeWeight(42, 84)); // 8
  checkTest(testNum++, correct, LARGE_NUMBER, getEdgeWeight(1, 4444)); // 9
  
  //call our shortest path algorithm
  shortestPath(1);

  cout << "The shortest path from node 1..." << endl;
  cout << " to node 1 is: " << pathCost[1] << endl;
  cout << " to node 2 is: " << pathCost[2] << endl;
  cout << " to node 3 is: " << pathCost[3] << endl;
  cout << " to node 4 is: " << pathCost[4] << endl;
  cout << " to node 5 is: " << pathCost[5] << endl;
  cout << " to node 6 is: " << pathCost[6] << endl;
  cout << " to node 7 is: " << pathCost[7] << endl;
  cout << " to node 8 is: " << pathCost[8] << endl;
  cout << " to node 9 is: " << pathCost[9] << endl;
  cout << " to node 10 is: " << pathCost[10] << endl;
  cout << " to node 1000 is: " << pathCost[1000] << endl;
  cout << " to node 2000 is: " << pathCost[2000] << endl;
  cout << " to node 3000 is: " << pathCost[3000] << endl;

  checkTest(testNum++, correct, pathCost[1], 0); // 10
  checkTest(testNum++, correct, pathCost[2], 193); // 11
  checkTest(testNum++, correct, pathCost[3], 381); // 12
  checkTest(testNum++, correct, pathCost[4], 596); // 13
  checkTest(testNum++, correct, pathCost[5], 2891); // 14
  checkTest(testNum++, correct, pathCost[6], 5458); // 15
  checkTest(testNum++, correct, pathCost[7], 5584); // 16
  checkTest(testNum++, correct, pathCost[8], 4698); // 17
  checkTest(testNum++, correct, pathCost[9], 8839); // 18
  checkTest(testNum++, correct, pathCost[10], 8935); // 19
  checkTest(testNum++, correct, pathCost[1000], 33725); // 20
  checkTest(testNum++, correct, pathCost[2000], 33517); // 21
  checkTest(testNum++, correct, pathCost[3000], 28376); // 22
  string path = getPath(1, 1000);
  cout << "The path from node 1 node 1000 is " << path << endl;
  checkTest(testNum++, correct, path, string("1 -> 22 -> 165 -> 162 -> 167 -> 164 -> 171 -> 190 -> 191 -> 336 -> 338 -> 343 -> 344 -> 340 -> 347 -> 348 -> 335 -> 515 -> 407 -> 524 -> 575 -> 574 -> 573 -> 572 -> 571 -> 601 -> 605 -> 508 -> 507 -> 509 -> 512 -> 511 -> 959 -> 963 -> 964 -> 971 -> 972 -> 970 -> 1014 -> 1016 -> 1015 -> 990 -> 989 -> 987 -> 988 -> 986 -> 1001 -> 995 -> 996 -> 997 -> 1000")); // 23
  path = getPath(1, 2000);
  cout << "The path from node 1 node 2000 is " << path << endl;
  checkTest(testNum++, correct, path, string("1 -> 22 -> 165 -> 162 -> 167 -> 164 -> 171 -> 190 -> 191 -> 336 -> 338 -> 343 -> 344 -> 340 -> 347 -> 348 -> 335 -> 515 -> 407 -> 524 -> 582 -> 589 -> 596 -> 597 -> 636 -> 641 -> 642 -> 632 -> 650 -> 653 -> 654 -> 1431 -> 1432 -> 1440 -> 1443 -> 1463 -> 1465 -> 1466 -> 1547 -> 1567 -> 1569 -> 1572 -> 1585 -> 1586 -> 1589 -> 1611 -> 1612 -> 1613 -> 1616 -> 1617 -> 1975 -> 1971 -> 1977 -> 1978 -> 1979 -> 1998 -> 2001 -> 2000")); // 24
  path = getPath(1, 3000);
  cout << "The path from node 1 node 3000 is " << path << endl;
  checkTest(testNum++, correct, path, string("1 -> 22 -> 165 -> 166 -> 2970 -> 2973 -> 2978 -> 2979 -> 2980 -> 2999 -> 3000")); // 25

  deleteArrays();
  edges.clear();

  return testNum - 1 == correct;

}

int main(int argc, char** argv) {

  int test = 0;
  int count = 0;

  if (argc > 1) {
    test = stoi(argv[1]);
  }
  switch (test) {
  case 0:
    if (testBasicGraph()) count++;
    if (testLargeGraph()) count++;
    cout << "----------------" << endl;
    cout << "Passed " << count << " out of 2 group tests" << endl;
    cout << "--End of tests--" << endl;
    return count != 2;
  case 1:
    return !testBasicGraph();
  case 2:
    return !testLargeGraph();
  }
}

