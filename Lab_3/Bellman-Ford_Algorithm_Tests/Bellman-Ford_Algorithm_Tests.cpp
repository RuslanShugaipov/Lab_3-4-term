#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\Руслан\source\repos\Lab_3\Lab_3\Bellman-Ford_Algorithm_N_File_Processing.h"
#include "C:\Users\Руслан\source\repos\Lab_3\Lab_3\LinkedList.h"
#include <fstream>

#define INF 999

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BellmanFordAlgorithmTests
{
	TEST_CLASS(BellmanFordAlgorithmTests)
	{
	public:

		TEST_METHOD(Bellman_Ford_Algorithm_Test)
		{
			string Cities[3] = { "A", "B", "C" };
			int To_City_Number = 0;
			string From_City = "B", To_City = "A";
			unsigned CSize = 3;
			int Adjacency_Matrix[3][3] = { { 0, INF, 1 }, { 7, 0, 3 }, { 2, 4, 0 } };
			int Shortest_Distances_Test[3] = { 5, 0, 3 };
			int Route_Test[3] = { 2, 1, 1 };
			LinkedList<string> Road_Test;
			Road_Test.push_front("A");
			Road_Test.push_front("C");
			Road_Test.push_front("B");
			
			LinkedList<string> Road;
			int* Shortest_Distances = new int[CSize];
			int* Route = new int[CSize];

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

			//intermediate check of a variable that stores the number of the city we need to get to			Assert::AreEqual(To_City_Number, 0);

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

			//compare the array of shortest paths and the array of vertex numbers to restore the paths
			//obtained as a result of the algorithm with the known correct ones
			for (int j = 0; j < 3; ++j)
			{
				Assert::AreEqual(Shortest_Distances[j], Shortest_Distances_Test[j]);
				Assert::AreEqual(Route[j], Route_Test[j]);
			}

			int Temp_Number = To_City_Number;
			Road.push_front(To_City);
			while (Cities[Route[To_City_Number]] != From_City)
			{
				Road.push_front(Cities[Route[To_City_Number]]);
				To_City_Number = Route[To_City_Number];
			}
			Road.push_front(From_City);

			//we compare the path restored by the algorithm with the obviously correct one
			for (int j = 0; j < 3; ++j)
				Assert::AreEqual(Road.at(j), Road_Test.at(j));

			delete[] Shortest_Distances;
			delete[] Route;
		}
		
		TEST_METHOD(File_Processing_Test)
		{
			ifstream read("flights_test_1");
			string City;
			string* Temp_Cities = new string[100];
			char ch;
			int i = 0;
			unsigned number_of_cities = 0;
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
			//we compare the array, the elements of which are the names of cities, with the obviously correct one
			string Cities_Test[3] = { "First", "Second", "Third" };
			for (int j = 0; j < 3; ++j)
				Assert::AreEqual(Cities[j], Cities_Test[j]);

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

			//go back to the beginning of the file and fill in the adjacency matrix with the values that we will find in the file
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

			//we compare the adjacency matrix obtained as a result of the algorithm with the obviously correct one
			int Adjacency_Matrix_Test[3][3] = { { 0, INF, 1 }, { 7, 0, 3 }, { 2, 4, 0 } };
			for (unsigned j = 0; j < 3; ++j)
				for (unsigned k = 0; k < 3; ++k)
					Assert::AreEqual(Adjacency_Matrix[j][k], Adjacency_Matrix_Test[j][k]);

			for (unsigned j = 0; j < CSize; ++j)
				delete[] Adjacency_Matrix[j];
			delete[] Adjacency_Matrix;
			delete[] Cities;
			delete[] Temp_Cities;
			read.close();
		}
	};
}