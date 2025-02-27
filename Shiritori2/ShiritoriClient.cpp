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

        // 変更点: inet_pton() を使用

        inet_pton(AF_INET, " 192.168.43.26", &serverAddr.sin_addr);

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

        while (true) {

            memset(buffer, 0, sizeof(buffer));

            recv(clientSocket, buffer, sizeof(buffer), 0);

            std::cout << "サーバー: " << buffer << "\n";

            if (strstr(buffer, "ゲームオーバー") || strstr(buffer, "ゲーム終了")) break;

            std::cout << "単語を入力: ";

            std::cin.ignore();  // ← ここを追加（前回の入力の影響を防ぐ）

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

