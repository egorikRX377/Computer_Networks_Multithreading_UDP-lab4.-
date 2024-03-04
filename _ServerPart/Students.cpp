#include "Students.h"

using namespace std;




Student::Student(string name, int groupNum, double scholarship, double averageRating) :
	name(name), groupNum(groupNum), scholarship(scholarship), averageRating(averageRating) {}

Student::Student() {}

string Student::getName()
{
	return name.c_str();
}

int Student::getGroupNum()
{
	return groupNum;
}

double Student::getScholarship()
{
	return scholarship;
}

double Student::getAverageRating()
{
	return averageRating;
}

void Student::setName(string name)
{
	this->name = name;
}

void Student::setGroupNum(int groupNum)
{
	this->groupNum = groupNum;
}

void Student::setScholarship(double scholarship)
{
	this->scholarship = scholarship;
}

void Student::setAverageRating(double averageRating)
{
	this->averageRating = averageRating;
}

string Student::toString() const
{
	ostringstream oss;
	oss << name << " " << groupNum << " " << scholarship << " " << averageRating;
	return oss.str();
}

Student Student::fromString(string& data)
{
	istringstream iss(data);

	string name;
	int groupNum;
	double scholarship;
	double averageRating;

	getline(iss >> ws, name, ',');

	iss >> groupNum >> scholarship >> averageRating;

	return Student(name, groupNum, scholarship, averageRating);
}


ostream& operator<<(ostream& buf, Student& student)
{
	buf << student.getName() << ',' << student.getGroupNum() << ' ' << student.getScholarship() << ' ' << student.getAverageRating() << endl;
	return buf;
}

istream& operator>>(istream& buf, Student& student)
{
	getline(buf >> ws, student.name, ',');
	buf >> student.groupNum >> student.scholarship >> student.averageRating;
	return buf;
}