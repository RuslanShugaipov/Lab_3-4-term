#include "Bellman-Ford_Algorithm_N_File_Processing.h"
#include <iostream>
#include <fstream>

//to perform the Bellman-Ford algorithm, you must enter a numeric constant
//that is obviously greater than any flight cost
#define INF 999

using namespace std;

void Bellman_Ford_Algorithm(string* Cities, int** Adjacency_Matrix, unsigned CSize)
{
	string From_City, To_City;
	LinkedList<string> Road;
	int To_City_Number;
	int* Shortest_Distances = new int[CSize];
	int* Route = new int[CSize];
	cout << "Enter the city you want to get from: ";
	cin >> From_City;
	cout << "\nEnter the city you want to get to: ";
	cin >> To_City;

	for (unsigned j = 0; j < CSize; j++)
	{
		if (From_City == Cities[j])
		{
			for (unsigned k = 0; k < CSize; k++)
			{
				Shortest_Distances[k] = Adjacency_Matrix[j][k];
				Route[k] = j;
			}
		}
		if (To_City == Cities[j])
			To_City_Number = j;
	}

	for (unsigned m = 1; m <= CSize - 2; ++m)
	{
		for (unsigned j = 0; j < CSize; ++j)
		{
			for (unsigned k = 0; k < CSize; ++k)
			{
				if ((Shortest_Distances[k] + Adjacency_Matrix[k][j]) < Shortest_Distances[j])
				{
					Shortest_Distances[j] = Shortest_Distances[k] + Adjacency_Matrix[k][j];
					Route[j] = k;
				}
			}
		}
	}

	int Temp_Number = To_City_Number;
	Road.push_front(To_City);
	while (Cities[Route[To_City_Number]] != From_City)
	{
		Road.push_front(Cities[Route[To_City_Number]]);
		To_City_Number = Route[To_City_Number];
	}
	Road.push_front(From_City);
	cout << '\n';
	Road.print_to_console();
	switch (Shortest_Distances[Temp_Number])
	{
	case INF:
		cout << "\nThere is no way from " << From_City << " to " << To_City << endl;
		break;
	case 0:
		cout << "\nThere is no point in flying to a city you are already in\n";
		break;
	default:
		cout << "\nThe most cost-effective flight: " << Shortest_Distances[Temp_Number] << endl;
		break;
	}

	delete[] Shortest_Distances;
	delete[] Route;
}

void File_Processing(string filename)
{
	ifstream read(filename.c_str());
	//this variable will store the name of the city
	string City;
	//this array of strings will store the names of the cities that will  
	//occur in the file. Since we do not know the number of cities in
	//advance, we simply set the array size to 1000, because there
	//are more efficient algorithms, so it is assumed that the
	//number of vertices of the graph will not be too large
	string* Temp_Cities = new string[100];
	char ch;
	int i = 0;
	unsigned number_of_cities = 0;

	//using this loop, we fill the array of lines with the names
	//of the cities that we encountered in the file
	while (true)
	{
		ch = read.get();
		if (read.eof())
			break;
		if (isdigit(ch) || ch == '\n')
			continue;
		while (true)
		{
			if (ch == ';')
			{
				if (City == "")
					goto end_cycle;
				for (unsigned j = 0; j < number_of_cities; ++j)
				{
					if (Temp_Cities[j] == City)
					{
						City.clear();
						goto end_cycle;
					}
				}
				Temp_Cities[i++] = City;
				number_of_cities++;
				City.clear();
			end_cycle:
				break;
			}
			City += ch;
			ch = read.get();
			if (ch == '\n' || City == "N/A")
			{
				City.clear();
				break;
			}
		}
	}

	string* Cities = new string[number_of_cities];
	for (unsigned j = 0; j < number_of_cities; ++j)
		Cities[j] = Temp_Cities[j];

	unsigned CSize = number_of_cities;

	//creating an adjacency matrix with as many columns and rows as there are cities in the file
	int** Adjacency_Matrix = new int* [CSize];
	for (unsigned j = 0; j < CSize; ++j)
		Adjacency_Matrix[j] = new int[CSize];
	for (unsigned j = 0; j < CSize; ++j)
	{
		for (unsigned k = 0; k < CSize; ++k)
		{
			if (k == j)
				Adjacency_Matrix[j][k] = 0;
			else
				Adjacency_Matrix[j][k] = INF;
		}
	}

	//go back to the beginning of the file and fill in the adjacency matrix with
	//the values that we will find in the file
	read.clear();
	read.seekg(0, ios::beg);
	string Flight_Price_String;
	bool from_or_to = false, second_city = false;
	int Flight_Price = 0, From_City_Number = 0, To_City_Number = 0;
	ch = ' ';
	i = 0;
	while (true)
	{
		ch = read.get();
		if (read.eof())
			break;
		if (ch == '\n')
			continue;
		while (true)
		{
			if (ch == ';')
			{
				if (isdigit(ch = read.get()) || second_city == true)
				{
					while (ch != '\n')
					{
						while (ch != ';' && ch != '\n' && !read.eof())
						{
							Flight_Price_String += ch;
							ch = read.get();
						}
						if (Flight_Price_String == "N/A")
							Flight_Price = INF;
						else
							Flight_Price = atoi(Flight_Price_String.c_str());
						Flight_Price_String.clear();
						if (!from_or_to)
						{
							for (unsigned j = 0; j < CSize; ++j)
							{
								if (Cities[j] == City)
								{
									To_City_Number = j;
									City.clear();
									break;
								}
							}
							Adjacency_Matrix[From_City_Number][To_City_Number] = Flight_Price;
							from_or_to = true;
						}
						else
							Adjacency_Matrix[To_City_Number][From_City_Number] = Flight_Price;
						if (ch == '\n')
						{
							ch = read.get();
							break;
						}
						else if (read.eof())
							break;
						ch = read.get();
					}
					second_city = false;
				}
				else
				{
					for (unsigned j = 0; j < CSize; ++j)
					{
						if (Cities[j] == City)
						{
							From_City_Number = j;
							from_or_to = false;
							second_city = true;
							City.clear();
							break;
						}
					}
				}
			}
			if (read.eof())
				break;
			City += ch;
			ch = read.get();
			if (ch == '\n')
			{
				City.clear();
				break;
			}
		}
		if (read.eof())
			break;
	}

	//after getting all the necessary data for the algorithm to work, we call it
	Bellman_Ford_Algorithm(Cities, Adjacency_Matrix, CSize);

	//freeing up all dynamically allocated memory
	for (unsigned j = 0; j < CSize; ++j)
		delete[] Adjacency_Matrix[j];
	delete[] Adjacency_Matrix;
	delete[] Cities;
	delete[] Temp_Cities;
	read.close();
}