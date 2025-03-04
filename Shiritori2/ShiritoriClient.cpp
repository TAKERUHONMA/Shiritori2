#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 256;

class ShiritoriClient {
private:
    SOCKET clientSocket;
    std::string username;

public:
    ShiritoriClient(const std::string& host, const std::string& port) {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "�\�P�b�g�̍쐬�Ɏ��s���܂����B\n";
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(std::stoi(port));
        inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "�T�[�o�[�ւ̐ڑ��Ɏ��s���܂����B\n";
            return;
        }

        std::cout << "�T�[�o�[�ɐڑ����܂����I\n";
    }

    void StartGame() {
        char buffer[BUFFER_SIZE];
        while (true) {
            // �T�[�o�[���烁�b�Z�[�W����M
            int received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (received <= 0) {
                std::cerr << "�T�[�o�[����̐ڑ����؂�܂����B\n";
                break;
            }
            buffer[received] = '\0';
            std::string serverMessage(buffer);
            std::cout << serverMessage << "\n";

            // �Q�[���I�����b�Z�[�W��������I��
            if (serverMessage.find("�Q�[���I��") != std::string::npos) {
                break;
            }

            // ���[�U�[�ɓ��͂𑣂�
            if (serverMessage.find("���Ȃ��̔Ԃł�") != std::string::npos) {
                std::string word;
                std::cout << "�P�����͂��Ă�������: ";
                std::cin >> word;

                // "exit" �Ɠ��͂���ƃQ�[����ޏo����
                if (word == "exit") {
                    send(clientSocket, "exit", 4, 0);
                    break;
                }

                send(clientSocket, word.c_str(), word.length(), 0);
            }
        }

        closesocket(clientSocket);
        WSACleanup();
    }
};

int main() {
    std::string host = "192.168.43.26";  // �T�[�o�[��IP�A�h���X
    std::string port = "12345";       // �T�[�o�[�̃|�[�g�ԍ�

    ShiritoriClient client(host, port);
    client.StartGame();

    return 0;
}
