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

        // IP�A�h���X��inet_pton()�ŕϊ�
        inet_pton(AF_INET, "192.168.43.26", &serverAddr.sin_addr);
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

        // �Q�[���J�n���b�Z�[�W��ҋ@
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);

            std::cout << "�T�[�o�[: " << buffer << "\n";

            // �T�[�o�[����u�Q�[���J�n�v�̃��b�Z�[�W���󂯎������A�Q�[���J�n
            if (strstr(buffer, "�Q�[���J�n") != nullptr) {
                std::cout << "�Q�[�����J�n����܂����I\n";
                break;
            }
        }

        // �Q�[�����n�܂�����P����̓��[�v�ɓ���
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);

            std::cout << "�T�[�o�[: " << buffer << "\n";

            // �Q�[���I�[�o�[�܂��̓Q�[���I���̃��b�Z�[�W��������I��
            if (strstr(buffer, "�Q�[���I�[�o�[") || strstr(buffer, "�Q�[���I��")) {
                break;
            }

            // �P������
            std::cout << "�P������: ";
            std::cin.ignore();  // �O��̓��͂̉e����h�����߂ɓ��̓o�b�t�@���N���A

            std::string word;
            std::getline(std::cin, word);

            // ���͂����P����T�[�o�[�ɑ��M
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
