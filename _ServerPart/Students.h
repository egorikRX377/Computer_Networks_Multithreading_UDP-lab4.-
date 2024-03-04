#pragma once
#include <string>
#include <sstream>

using namespace std;




class Student
{
private:
	string name;
	int groupNum;
	double scholarship;
	double averageRating;
public:
	Student();
	Student(string name, int groupNum, double scholarship, double averageRating);

	string getName();
	int getGroupNum();
	double getScholarship();
	double getAverageRating();

	void setName(string name);
	void setGroupNum(int groupNum);
	void setScholarship(double scholarship);
	void setAverageRating(double averageRating);


	string toString() const;
	static Student fromString(string& data);

	friend ostream& operator<<(ostream& buf, Student& student);
	friend istream& operator>>(istream& buf, Student& student);
};