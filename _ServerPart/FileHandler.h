#pragma once
#include <iostream>
#include <fstream>

using namespace std;



template<class T>
class FileHandler
{
public:
	static void writeToFile(const string& filePath, vector<T>& BASE)
	{
		ofstream file(filePath);

		if (!file.is_open())
		{
			cerr << "Невозможно открыть файл для записи!" << endl;
			exit(1);
		}

		for (auto& item : BASE)
		{
			file << item;
		}

		file.close();
	}

	static vector<T> readFromFile(const string& filePath)
	{
		vector<T> BASE;
		ifstream file(filePath);

		if (!file.good())
		{
			return BASE;
		}

		if (!file.is_open())
		{
			cerr << "Невозможно октрыть файл для чтения!" << endl;
			exit(1);
		}

		T item;

		while (file >> item)
		{
			BASE.push_back(item);
			item = T();
		}

		file.close();

		return BASE;
	}
};
