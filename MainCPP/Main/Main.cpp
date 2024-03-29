// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>


class Tree
{
	std::vector<bool> heap;
	int n;
	int N;
	int cnt = 0;
	bool marked = false;

	bool checkIndex(int index)
	{
		if (index<0 || index>this->N)
			return false;

		return true;
	}
	int getLeftChild(int index)
	{
		if (!checkIndex(index))
			return -1;

		return 2 * index + 1;
	}
	int getRightChild(int index)
	{
		if (!checkIndex(index))
			return -1;

		return 2 * index + 2;
	}
	int getParent(int index)
	{

		if (!checkIndex(index) || index==0)
			return -1;

		return (index - 1) / 2;;
	}



public:
	Tree(int n) : n(n), N((1 << n)-1) { this->heap.assign(this->N, 0); }
	int size() { return this->N; }
	bool getState() { return marked; }
	bool mark(int index)
	{

		if (!checkIndex(index) || this->heap[index]) //if its marked do nothing
			return false;

		this->heap[index] = true;
		this->cnt++;

		if (cnt == this->N)
		{
			this->marked = true;
			return true;
		}

		//if its right child exists and is marked
		if (checkIndex(getRightChild(index)))
			if(this->heap[getRightChild(index)]) 
				mark(getLeftChild(index));
		//if its left child exists nad is marked
		else if (checkIndex(getLeftChild(index)))
			if(this->heap[getLeftChild(index)]) 
				mark(getRightChild(index));

		//if its parent exists
		if (checkIndex(getParent(index)))
		{
			//if it is left child
			if (index == getLeftChild(getParent(index)))
			{
				int parent = getParent(index);
				if (this->heap[parent]) //if parent is marked -> mark right child
					mark(getRightChild(parent));
				else if (this->heap[getRightChild(parent)]) //if sibling is marked -> mark parent
					mark(parent);

			}
			//if it is right child
			else
			{
				int parent = getParent(index);
				if (this->heap[parent]) //if parent is marked -> mark right child
					mark(getLeftChild(parent));
				else if (this->heap[getLeftChild(parent)]) //if sibling is marked -> mark parent
					mark(parent);
			}
			
		}

		return true;

	}
};

double mean(std::vector<int>& v)
{
	double m = 0;
	for (int each : v)
		m += each;

	m = m / v.size();

	return m;
}
double var(std::vector<int>& v, double m)
{
	double var = 0;
	for (auto each : v)
		var += (each - m)*(each - m);
	
	return var / (v.size() - 1);
}

//As rand() returns number between 0 and 2^15-1 the purpose of this function is to provide larger random numbers
//generate rand() then shift 15 bits to the left and generate another one and OR these two
int randomGen()
{
	int number = rand();

	number = number << 15;
	number = number | (rand());

	return number;
}
int Process_one(int n, int sample_size)
{
	std::ofstream myfile;
	myfile.open("data1.txt", std::ios::app | std::ios::out);

	std::vector<int> times;

	srand(time(nullptr));

	for (int i = 0; i < sample_size; i++)
	{
		Tree tr(n);
		int T = 0;

		while (!tr.getState())
		{
			int index = randomGen() % (tr.size());
			tr.mark(index);
			T++;
		}

		times.push_back(T);
	}


	myfile << n << ' ' << mean(times) << ' ' << std::sqrt(var(times, mean(times))) << std::endl;
	std::cout << "Size: 2^" << n << "-1" << " Mean: " << mean(times) << " Deviation: " << std::sqrt(var(times, mean(times))) << std::endl;

	return 1;
}
int Process_two(int n, int sample_size)
{
	std::ofstream myfile;
	myfile.open("data2.txt", std::ios::app | std::ios::out);

	std::vector<int> times;
	std::vector<bool> heap;

	srand(time(nullptr));

	for (int i = 0; i < sample_size; i++)
	{
		Tree tr(n);
		int T = 0;
		heap.assign(tr.size(), false);

		while (!tr.getState())
		{
			int index = randomGen() % (tr.size());
			if (!heap[index])
			{
				heap[index] = true;
				tr.mark(index);
				T++;
			}
		}

		times.push_back(T);
	}


	myfile << n << ' ' << mean(times) << ' ' << std::sqrt(var(times, mean(times))) << std::endl;
	std::cout << "Size: 2^" << n << "-1" << " Mean: " << mean(times) << " Deviation: " << std::sqrt(var(times, mean(times))) << std::endl;

	return 1;
}
int Process_three(int n, int sample_size)
{
	std::ofstream myfile;
	myfile.open("data3.txt", std::ios::app | std::ios::out);

	std::vector<int> times;

	srand(time(nullptr));

	for (int i = 0; i < sample_size; i++)
	{
		Tree tr(n);
		int T = 0;

		while (!tr.getState())
		{
			int index = randomGen() % (tr.size());
			if(tr.mark(index))
				T++;
		}

		times.push_back(T);
	}


	myfile << n << ' ' << mean(times) << ' ' << std::sqrt(var(times, mean(times))) << std::endl;
	std::cout << "Size: 2^" << n << "-1" << " Mean: " << mean(times) << " Deviation: " << std::sqrt(var(times, mean(times))) << std::endl;

	return 1;
}
//This is just a helper function that aims to test randomGen
void printBinary(int n)
{
	std::string s = "";


	for (int i = 0; i < 32; i++)
	{
		if (n % 2 == 0)
			s = '0' + s;
		else
			s = '1' + s;

		n = n / 2;
	}

	std::cout << s << std::endl;
}



int main()
{
	int t = clock();
	int sample_size = 30;

	for (int i = 1; i < 16; i++)
		Process_three(i,sample_size);
	std::cout << "Sample size: " << sample_size << std::endl;
	std::cout << "Execution time: " << (clock() - t)/1000  << std::endl;


	system("PAUSE");
	return 0;
}

