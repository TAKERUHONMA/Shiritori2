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
            std::cerr << "ソケットの作成に失敗しました。\n";
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(std::stoi(port));
        inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "サーバーへの接続に失敗しました。\n";
            return;
        }

        std::cout << "サーバーに接続しました！\n";
    }

    void StartGame() {
        char buffer[BUFFER_SIZE];
        while (true) {
            // サーバーからメッセージを受信
            int received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (received <= 0) {
                std::cerr << "サーバーからの接続が切れました。\n";
                break;
            }
            buffer[received] = '\0';
            std::string serverMessage(buffer);
            std::cout << serverMessage << "\n";

            // ゲーム終了メッセージが来たら終了
            if (serverMessage.find("ゲーム終了") != std::string::npos) {
                break;
            }

            // ユーザーに入力を促す
            if (serverMessage.find("あなたの番です") != std::string::npos) {
                std::string word;
                std::cout << "単語を入力してください: ";
                std::cin >> word;

                // "exit" と入力するとゲームを退出する
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
    std::string host = "192.168.43.26";  // サーバーのIPアドレス
    std::string port = "12345";       // サーバーのポート番号

    ShiritoriClient client(host, port);
    client.StartGame();

    return 0;
}
