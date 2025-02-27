#include <iostream>

#include <cstring>

#include <winsock2.h>

#include <ws2tcpip.h>

#include <string>

#pragma comment(lib, "ws2_32.lib")

class ShiritoriClient {

private:

    SOCKET clientSocket;

public:

    ShiritoriClient() {

        WSADATA wsa;

        WSAStartup(MAKEWORD(2, 2), &wsa);

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddr{};

        serverAddr.sin_family = AF_INET;

        // �ύX�_: inet_pton() ���g�p

        inet_pton(AF_INET, " 192.168.43.26", &serverAddr.sin_addr);

        serverAddr.sin_port = htons(12345);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {

            std::cout << "�T�[�o�[�ɐڑ����܂����I\n";

        }
        else {

            std::cerr << "�ڑ����s...\n";

            exit(1);

        }

    }

    void PlayGame() {

        char buffer[256];

        while (true) {

            memset(buffer, 0, sizeof(buffer));

            recv(clientSocket, buffer, sizeof(buffer), 0);

            std::cout << "�T�[�o�[: " << buffer << "\n";

            if (strstr(buffer, "�Q�[���I�[�o�[") || strstr(buffer, "�Q�[���I��")) break;

            std::cout << "�P������: ";

            std::cin.ignore();  // �� ������ǉ��i�O��̓��͂̉e����h���j

            std::string word;

            std::getline(std::cin, word);

            send(clientSocket, word.c_str(), word.size(), 0);

        }

    }

    ~ShiritoriClient() {

        closesocket(clientSocket);

        WSACleanup();

    }

};

int main() {

    ShiritoriClient client;

    client.PlayGame();

    return 0;

}

