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
		cout << "1. Просмотреть информацию о всех студентах" << endl;
		cout << "2. Добавить информацию о студенте" << endl;
		cout << "3. Удалить информацию о студенте" << endl;
		cout << "4. Редактировать информацию о студенте" << endl;
		cout << "5. Выполнить индивидуальное задание" << endl << endl;
		cout << "6. Выйти из программы" << endl;
		cout << "Ваш выбор?" << endl;
		cin >> choice;

		char func = choice;

		sendto(s, &func, sizeof(func), 0, (sockaddr*)&dest_addr, destAddrSize);

		switch (choice)
		{
		case '1':
		{
			char buf[101];
			memset(buf, 0, sizeof(buf));


			cout << "Информация о всех студентах: " << endl << endl;
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

			cout << "Введите ФИО: ";
			getline(cin >> ws, name);
			cout << "Введите номер группы: ";
			cin >> groupNum;
			cout << "Введите размер стипендии: ";
			cin >> scholarship;
			cout << "Введите средний балл: ";
			cin >> averageRating;
			string data = dataToString(name, groupNum, scholarship, averageRating);

			sendto(s, data.c_str(), data.length(), 0, (sockaddr*)&dest_addr, destAddrSize);

			cout << "Информация успешно добавлена в базу!" << endl;
			break;
		}
		case '3':
		{
			string name;
			cout << "Введите ФИО: ";
			getline(cin >> ws, name);

			sendto(s, name.c_str(), name.length(), 0, (sockaddr*)&dest_addr, destAddrSize);

			char isExist;

			recvfrom(s, &isExist, sizeof(isExist), 0, (sockaddr*)&dest_addr, &destAddrSize);

			if (isExist == 'Y')
			{
				cout << "Информация успешно удалена из базы!" << endl;
			}
			else
			{
				cout << "Ошибка: студента с таким именем не существует!" << endl;
			}

			break;
		}
		case '4':
		{
			string name;
			cout << "Введите ФИО: ";
			getline(cin >> ws, name);

			sendto(s, name.c_str(), name.length(), 0, (sockaddr*)&dest_addr, destAddrSize);

			char isExist;

			recvfrom(s, &isExist, sizeof(isExist), 0, (sockaddr*)&dest_addr, &destAddrSize);
			if (isExist == 'N')
			{
				cout << "Ошибка: студента с таким именем не существует!" << endl;
				break;
			}

			cout << "1. Редактировать ФИО" << endl;
			cout << "2. Редактировать номер группы" << endl;
			cout << "3. Редактировать размер стипендии" << endl;
			cout << "4. Редактировать средний балл" << endl;
			cout << "5. Выйти из меню редактирования" << endl;
			char editChoice;

			cin >> editChoice;

			sendto(s, &editChoice, sizeof(editChoice), 0, (sockaddr*)&dest_addr, destAddrSize);

			switch (editChoice)
			{
			case '1':
			{
				string name;
				cout << "Введите ФИО: ";
				getline(cin >> ws, name);

				sendto(s, name.c_str(), name.length(), 0, (sockaddr*)&dest_addr, destAddrSize);
				cout << "ФИО успешно изменено!" << endl;

				break;
			}
			case '2':
			{
				int groupNum;
				cout << "Введите номер группы: ";
				cin >> groupNum;

				sendto(s, reinterpret_cast<char*>(&groupNum), sizeof(groupNum), 0, (sockaddr*)&dest_addr, destAddrSize);
				cout << "Номер группы успешно изменён!" << endl;

				break;
			}
			case '3':
			{
				double scholarship;
				cout << "Введите размер стипендии: ";
				cin >> scholarship;

				string sch = doubleToString(scholarship);

				break;
			}
			case '4':
			{
				double averageRating;
				cout << "Введите средний балл: ";
				cin >> averageRating;

				string avgRat = doubleToString(averageRating);

				sendto(s, avgRat.c_str(), avgRat.length(), 0, (sockaddr*)&dest_addr, destAddrSize);
				cout << "Средний балл успешно изменён!" << endl;

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
			cout << "Введите букву: ";
			cin >> letter;
			sendto(s, &letter, sizeof(letter), 0, (sockaddr*)&dest_addr, destAddrSize);

			char buf[101];
			memset(buf, 0, sizeof(buf));

			char existing;
			recvfrom(s, &existing, sizeof(existing), 0, (sockaddr*)&dest_addr, &destAddrSize);
			if (existing == 'N')
			{
				cout << "Ошибка: нет студентов с фамилией начинавшейся на данную букву!" << endl;
				break;
			}


			cout << "Информация о студентах под данное условие: " << endl << endl;
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
					cerr << "Ошибка получения данных" << endl;
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