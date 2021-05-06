#pragma once
#include "LinkedList.h"
#include <string>

//a function that executes the Ford-Bellman algorithm, it builds a one-dimensional array,
//the elements of which are the values of the shortest paths to all the vertices from the selected one,
//and also returns the path of the shortest path from one vertex to another and outputs it
void Bellman_Ford_Algorithm(string* Cities, int** Adjacency_Matrix, unsigned CSize);

void File_Processing(string filename);