#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>          // std::string

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 256;

class ShiritoriClient {
private:
    SOCKET clientSocket;

public:
    ShiritoriClient() {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "�\�P�b�g�̍쐬�Ɏ��s���܂����B\n";
            return;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        inet_pton(AF_INET, "192.168.42.93", &serverAddr.sin_addr);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "�T�[�o�[�ւ̐ڑ��Ɏ��s���܂����B�G���[�R�[�h: " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "�T�[�o�[�ɐڑ����܂����I\n";
    }

    void StartGame() {
        char buffer[BUFFER_SIZE];
        while (true) {
            // �T�[�o�[����̃��b�Z�[�W����M
            int received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (received == SOCKET_ERROR) {
                std::cerr << "���b�Z�[�W��M�Ɏ��s���܂����B�G���[�R�[�h: " << WSAGetLastError() << std::endl;
                break;
            }
            if (received == 0) {
                std::cerr << "�T�[�o�[����̐ڑ����ؒf����܂����B\n";
                break;
            }

            buffer[received] = '\0';
            std::string serverMessage(buffer);

            std::cout << "�T�[�o�[: " << serverMessage << std::endl;

            // �����ȒP��ƃ��b�Z�[�W�������ꍇ�A�ēx�P�����͂�����
            if (serverMessage.find("�����ȒP��ł�") != std::string::npos) {
                std::cout << "�����ȒP��ł��B�ēx�P�����͂��Ă�������: ";

                // �ēx�P�����͂���O�Ƀo�b�t�@���N���A
                std::cin.clear();
                std::cin.sync();  // �o�b�t�@�̃N���A
                continue;
            }

            // �v���C���[�ɒP����͂𑣂�
            if (serverMessage.find("���Ȃ��̔Ԃł�") != std::string::npos) {
                std::cout << "�P�����͂��Ă�������: ";

                // �ēx�P�����͂���O�Ƀo�b�t�@���N���A
                std::cin.clear();
                std::cin.sync();

                std::string word;
                std::getline(std::cin, word);  // ���[�U�[����P������
                send(clientSocket, word.c_str(), word.length(), 0);
            }

            // �Q�[���I�����̏���
            if (serverMessage.find("�Q�[���I��") != std::string::npos) {
                break;
            }
        }
    }

    ~ShiritoriClient() {
        closesocket(clientSocket);
        WSACleanup();
    }
};

int main() {
    ShiritoriClient client;
    client.StartGame();
    return 0;
}
