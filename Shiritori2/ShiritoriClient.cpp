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

        // IPアドレスをinet_pton()で変換
        inet_pton(AF_INET, "192.168.43.26", &serverAddr.sin_addr);
        serverAddr.sin_port = htons(12345);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            std::cout << "サーバーに接続しました！\n";
        }
        else {
            std::cerr << "接続失敗...\n";
            exit(1);
        }
    }

    void PlayGame() {
        char buffer[256];

        // ゲーム開始メッセージを待機
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);

            std::cout << "サーバー: " << buffer << "\n";

            // サーバーから「ゲーム開始」のメッセージを受け取ったら、ゲーム開始
            if (strstr(buffer, "ゲーム開始") != nullptr) {
                std::cout << "ゲームが開始されました！\n";
                break;
            }
        }

        // ゲームが始まったら単語入力ループに入る
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);

            std::cout << "サーバー: " << buffer << "\n";

            // ゲームオーバーまたはゲーム終了のメッセージが来たら終了
            if (strstr(buffer, "ゲームオーバー") || strstr(buffer, "ゲーム終了")) {
                break;
            }

            // 単語を入力
            std::cout << "単語を入力: ";
            std::cin.ignore();  // 前回の入力の影響を防ぐために入力バッファをクリア

            std::string word;
            std::getline(std::cin, word);

            // 入力した単語をサーバーに送信
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
