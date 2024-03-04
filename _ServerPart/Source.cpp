#include <thread>
#include <vector>
#include <WinSock2.h>
#include <mutex>
#include "Students.h"
#include "FileHandler.h"
#include <algorithm>

using namespace std;

const string studentsBASE_PATH = "students.txt";

mutex mtx;

vector<Student> studentsBASE;

double stringToDouble(const string& data)
{
    double result;
    istringstream stream(data);
    stream >> result;
    return result;
}

bool operator==(const sockaddr_in& lhs, const sockaddr_in& rhs) 
{
    return (lhs.sin_addr.s_addr == rhs.sin_addr.s_addr) &&
        (lhs.sin_port == rhs.sin_port);
}

int HandleClient(SOCKET serverSocket, sockaddr_in clientAddr)
{
    while (true)
    {
        char letter;
        bool isExist = false;
        int clientAddrSize = sizeof(clientAddr);

        char func;

        recvfrom(serverSocket, &func, sizeof(func), 0, (sockaddr*)&clientAddr, &clientAddrSize);

        switch (func)
        {
        case '1':
        {

            for (auto& item : studentsBASE)
            {
                isExist = true;
                string studentStr = item.toString();
                sendto(serverSocket, studentStr.c_str(), studentStr.length(), 0, (sockaddr*)&clientAddr, clientAddrSize);

                char newline = '\n';
                sendto(serverSocket, &newline, sizeof(newline), 0, (sockaddr*)&clientAddr, clientAddrSize);
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            if (!isExist)
            {
                cout << "Студенты не найдены в базе!" << endl;
            }

            char endMarker = '\0';
            sendto(serverSocket, &endMarker, sizeof(endMarker), 0, (sockaddr*)&clientAddr, clientAddrSize);
            break;
        }
        case '2':
        {

            char buf[101];
            recvfrom(serverSocket, buf, sizeof(buf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);

            string data(buf);
            Student newStudent = Student::fromString(data);

            mtx.lock();

            studentsBASE.push_back(newStudent);
            FileHandler<Student>::writeToFile(studentsBASE_PATH, studentsBASE);

            mtx.unlock();

            break;
        }
        case '3':
        {
            bool isExist = false;
            char buf[101];
            int bytesRead = recvfrom(serverSocket, buf, sizeof(buf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
            buf[bytesRead] = '\0';
            string name(buf);
            char existing = 'N';

            for (auto it = studentsBASE.begin(); it != studentsBASE.end();)
            {
                if (it->getName() == name)
                {

                    mtx.lock();
                    it = studentsBASE.erase(it);
                    mtx.unlock();

                    isExist = true;
                    existing = 'Y';

                    sendto(serverSocket, &existing, sizeof(existing), 0, (sockaddr*)&clientAddr, clientAddrSize);
                    break;
                }
                else
                {
                    ++it;
                }
            }
            if (!isExist)
            {
                sendto(serverSocket, &existing, sizeof(existing), 0, (sockaddr*)&clientAddr, clientAddrSize);
                break;
            }

            mtx.lock();
            FileHandler<Student>::writeToFile(studentsBASE_PATH, studentsBASE);
            mtx.unlock();

            break;
        }
        case '4':
        {
            bool isExist = false;
            char existing = 'N';
            char buf[101];
            char editBuf[101];
            int bytesRead = recvfrom(serverSocket, buf, sizeof(buf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
            buf[bytesRead] = '\0';
            string name(buf);

            for (auto it = studentsBASE.begin(); it != studentsBASE.end();)
            {

                if (it->getName() == name)
                {
                    isExist = true;
                    existing = 'Y';
                    sendto(serverSocket, &existing, sizeof(existing), 0, (sockaddr*)&clientAddr, clientAddrSize);

                    char editChoice;
                    recvfrom(serverSocket, &editChoice, sizeof(editChoice), 0, (sockaddr*)&clientAddr, &clientAddrSize);
                    switch (editChoice)
                    {
                    case '1':
                    {
                        int received_bytes = recvfrom(serverSocket, editBuf, sizeof(editBuf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
                        editBuf[received_bytes] = '\0';
                        string studentName(editBuf);

                        mtx.lock();
                        it->setName(studentName);
                        mtx.unlock();

                        break;
                    }
                    case '2':
                    {
                        int numGroup = 0;
                        recvfrom(serverSocket, reinterpret_cast<char*>(&numGroup), sizeof(numGroup), 0, (sockaddr*)&clientAddr, &clientAddrSize);

                        mtx.lock();
                        it->setGroupNum(numGroup);
                        mtx.unlock();

                        break;
                    }
                    case '3':
                    {
                        int received_bytes = recvfrom(serverSocket, editBuf, sizeof(editBuf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
                        editBuf[received_bytes] = '\0';

                        double studentScholarship = stringToDouble(editBuf);

                        mtx.lock();
                        it->setScholarship(studentScholarship);
                        mtx.unlock();

                        break;
                    }
                    case '4':
                    {
                        int received_bytes = recvfrom(serverSocket, editBuf, sizeof(editBuf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrSize);
                        editBuf[received_bytes] = '\0';
                        double studentAVGRate = stringToDouble(editBuf);

                        mtx.lock();
                        it->setAverageRating(studentAVGRate);
                        mtx.unlock();

                        break;
                    }
                    }
                    break;
                }
                else
                {
                    ++it;
                }
            }
            if (!isExist)
            {
                sendto(serverSocket, &existing, sizeof(existing), 0, (sockaddr*)&clientAddr, clientAddrSize);
                break;
            }

            mtx.lock();
            FileHandler<Student>::writeToFile(studentsBASE_PATH, studentsBASE);
            mtx.unlock();

            break;
        }
        case '5':
        {
            bool isExist = false;
            char existing = 'N';
            int counter = 0;

            while (int result = recvfrom(serverSocket, &letter, sizeof(letter), 0, (sockaddr*)&clientAddr, &clientAddrSize))
            {
                for (auto& item : studentsBASE)
                {
                    if (item.getName()[0] == letter)
                    {
                        isExist = true;
                        counter++;
                        if (counter == 1)
                        {
                            existing = 'Y';
                            sendto(serverSocket, &existing, sizeof(existing), 0, (sockaddr*)&clientAddr, clientAddrSize);
                        }


                        string studentStr = item.toString();
                        sendto(serverSocket, studentStr.c_str(), studentStr.length(), 0, (sockaddr*)&clientAddr, clientAddrSize);


                        char newline = '\n';
                        sendto(serverSocket, &newline, sizeof(newline), 0, (sockaddr*)&clientAddr, clientAddrSize);

                        this_thread::sleep_for(chrono::milliseconds(10));
                    }
                }

                if (!isExist)
                {
                    sendto(serverSocket, &existing, sizeof(existing), 0, (sockaddr*)&clientAddr, clientAddrSize);
                    break;
                }


                char endMarker = '\0';
                sendto(serverSocket, &endMarker, sizeof(endMarker), 0, (sockaddr*)&clientAddr, clientAddrSize);


                if (endMarker == '\0') {
                    break;
                }
            }
            break;
        }
        case '6':
        {
            cout << "Клиент отключился от сервера!" << endl;
            closesocket(serverSocket);
            return 0;
        }
        }
    }
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    studentsBASE = FileHandler<Student>::readFromFile(studentsBASE_PATH);

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) { return 1; }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(1281);
    localAddr.sin_addr.s_addr = INADDR_ANY;

    SOCKET serverSocket1 = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in localAddr1;
    localAddr1.sin_family = AF_INET;
    localAddr1.sin_port = htons(1280);
    localAddr1.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&localAddr, sizeof(localAddr));
    bind(serverSocket1, (sockaddr*)&localAddr1, sizeof(localAddr1));

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    char addrMessage;

    while (true)
    {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        recvfrom(serverSocket, &addrMessage, sizeof(addrMessage), 0, (sockaddr*)&clientAddr, &clientAddrSize);
        thread(HandleClient, serverSocket1, clientAddr).detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}