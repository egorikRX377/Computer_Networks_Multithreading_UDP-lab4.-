#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<winsock2.h>
#include <Windows.h>
#include <string>
#include <sstream>


using namespace std;


string dataToString(string name, int groupNum, double scholarship, double averageRating)
{
	ostringstream oss;
	oss << name << "," << groupNum << " " << scholarship << " " << averageRating;
	return oss.str();
}



string doubleToString(double value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}


void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in dest_addr;
	int destAddrSize = sizeof(dest_addr);

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sockaddr_in dest_addr1;
	int destAddrSize1 = sizeof(dest_addr);

	dest_addr1.sin_family = AF_INET;
	dest_addr1.sin_port = htons(1281);
	dest_addr1.sin_addr.s_addr = inet_addr("127.0.0.1");

	char startMessage = 'S';

	sendto(s, &startMessage, sizeof(startMessage), 0, (sockaddr*)&dest_addr1, destAddrSize1);

	while (1)
	{
		char choice;
		cout << "1. ����������� ���������� � ���� ���������" << endl;
		cout << "2. �������� ���������� � ��������" << endl;
		cout << "3. ������� ���������� � ��������" << endl;
		cout << "4. ������������� ���������� � ��������" << endl;
		cout << "5. ��������� �������������� �������" << endl << endl;
		cout << "6. ����� �� ���������" << endl;
		cout << "��� �����?" << endl;
		cin >> choice;

		char func = choice;

		sendto(s, &func, sizeof(func), 0, (sockaddr*)&dest_addr, destAddrSize);

		switch (choice)
		{
		case '1':
		{
			char buf[101];
			memset(buf, 0, sizeof(buf));


			cout << "���������� � ���� ���������: " << endl << endl;
			cout << "-------------------------------" << endl;
			while (true)
			{
				int bytesRead = recvfrom(s, buf, sizeof(buf) - 1, 0, (sockaddr*)&dest_addr, &destAddrSize);

				if (bytesRead == 0 || buf[0] == '\0')
				{
					break;
				}

				cout << buf;


				if (buf[bytesRead - 1] == '\n')
				{
					cout << "-------------------------------" << endl;
				}

				memset(buf, 0, sizeof(buf));
			}

			break;
		}
		case '2':
		{
			string name;
			int groupNum;
			double scholarship;
			double averageRating;

			cout << "������� ���: ";
			getline(cin >> ws, name);
			cout << "������� ����� ������: ";
			cin >> groupNum;
			cout << "������� ������ ���������: ";
			cin >> scholarship;
			cout << "������� ������� ����: ";
			cin >> averageRating;
			string data = dataToString(name, groupNum, scholarship, averageRating);

			sendto(s, data.c_str(), data.length(), 0, (sockaddr*)&dest_addr, destAddrSize);

			cout << "���������� ������� ��������� � ����!" << endl;
			break;
		}
		case '3':
		{
			string name;
			cout << "������� ���: ";
			getline(cin >> ws, name);

			sendto(s, name.c_str(), name.length(), 0, (sockaddr*)&dest_addr, destAddrSize);

			char isExist;

			recvfrom(s, &isExist, sizeof(isExist), 0, (sockaddr*)&dest_addr, &destAddrSize);

			if (isExist == 'Y')
			{
				cout << "���������� ������� ������� �� ����!" << endl;
			}
			else
			{
				cout << "������: �������� � ����� ������ �� ����������!" << endl;
			}

			break;
		}
		case '4':
		{
			string name;
			cout << "������� ���: ";
			getline(cin >> ws, name);

			sendto(s, name.c_str(), name.length(), 0, (sockaddr*)&dest_addr, destAddrSize);

			char isExist;

			recvfrom(s, &isExist, sizeof(isExist), 0, (sockaddr*)&dest_addr, &destAddrSize);
			if (isExist == 'N')
			{
				cout << "������: �������� � ����� ������ �� ����������!" << endl;
				break;
			}

			cout << "1. ������������� ���" << endl;
			cout << "2. ������������� ����� ������" << endl;
			cout << "3. ������������� ������ ���������" << endl;
			cout << "4. ������������� ������� ����" << endl;
			cout << "5. ����� �� ���� ��������������" << endl;
			char editChoice;

			cin >> editChoice;

			sendto(s, &editChoice, sizeof(editChoice), 0, (sockaddr*)&dest_addr, destAddrSize);

			switch (editChoice)
			{
			case '1':
			{
				string name;
				cout << "������� ���: ";
				getline(cin >> ws, name);

				sendto(s, name.c_str(), name.length(), 0, (sockaddr*)&dest_addr, destAddrSize);
				cout << "��� ������� ��������!" << endl;

				break;
			}
			case '2':
			{
				int groupNum;
				cout << "������� ����� ������: ";
				cin >> groupNum;

				sendto(s, reinterpret_cast<char*>(&groupNum), sizeof(groupNum), 0, (sockaddr*)&dest_addr, destAddrSize);
				cout << "����� ������ ������� ������!" << endl;

				break;
			}
			case '3':
			{
				double scholarship;
				cout << "������� ������ ���������: ";
				cin >> scholarship;

				string sch = doubleToString(scholarship);

				break;
			}
			case '4':
			{
				double averageRating;
				cout << "������� ������� ����: ";
				cin >> averageRating;

				string avgRat = doubleToString(averageRating);

				sendto(s, avgRat.c_str(), avgRat.length(), 0, (sockaddr*)&dest_addr, destAddrSize);
				cout << "������� ���� ������� ������!" << endl;

				break;
			}
			case '5':
			{
				break;
			}
			}
			break;
		}
		case '5':
		{
			char letter;
			cout << "������� �����: ";
			cin >> letter;
			sendto(s, &letter, sizeof(letter), 0, (sockaddr*)&dest_addr, destAddrSize);

			char buf[101];
			memset(buf, 0, sizeof(buf));

			char existing;
			recvfrom(s, &existing, sizeof(existing), 0, (sockaddr*)&dest_addr, &destAddrSize);
			if (existing == 'N')
			{
				cout << "������: ��� ��������� � �������� ������������ �� ������ �����!" << endl;
				break;
			}


			cout << "���������� � ��������� ��� ������ �������: " << endl << endl;
			cout << "-------------------------------" << endl;

			while (true)
			{
				int bytesRead = recvfrom(s, buf, sizeof(buf) - 1, 0, (sockaddr*)&dest_addr, &destAddrSize);

				if (bytesRead == 0 || buf[0] == '\0')
				{
					break;
				}
				else if (bytesRead < 0)
				{
					cerr << "������ ��������� ������" << endl;
					break;
				}

				cout << buf;


				if (buf[bytesRead - 1] == '\n')
				{
					cout << "-------------------------------" << endl;
				}

				memset(buf, 0, sizeof(buf));
			}

			break;
		}
		case '6':
		{
			closesocket(s);
			WSACleanup();
			return;
		}
		}

	}
}