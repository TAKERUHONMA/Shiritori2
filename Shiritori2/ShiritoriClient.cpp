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
            std::cerr << "ソケットの作成に失敗しました。\n";
            return;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        inet_pton(AF_INET, "192.168.42.93", &serverAddr.sin_addr);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "サーバーへの接続に失敗しました。エラーコード: " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "サーバーに接続しました！\n";
    }

    void StartGame() {
        char buffer[BUFFER_SIZE];
        while (true) {
            // サーバーからのメッセージを受信
            int received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (received == SOCKET_ERROR) {
                std::cerr << "メッセージ受信に失敗しました。エラーコード: " << WSAGetLastError() << std::endl;
                break;
            }
            if (received == 0) {
                std::cerr << "サーバーからの接続が切断されました。\n";
                break;
            }

            buffer[received] = '\0';
            std::string serverMessage(buffer);

            std::cout << "サーバー: " << serverMessage << std::endl;

            // 無効な単語とメッセージが来た場合、再度単語を入力させる
            if (serverMessage.find("無効な単語です") != std::string::npos) {
                std::cout << "無効な単語です。再度単語を入力してください: ";

                // 再度単語を入力する前にバッファをクリア
                std::cin.clear();
                std::cin.sync();  // バッファのクリア
                continue;
            }

            // プレイヤーに単語入力を促す
            if (serverMessage.find("あなたの番です") != std::string::npos) {
                std::cout << "単語を入力してください: ";

                // 再度単語を入力する前にバッファをクリア
                std::cin.clear();
                std::cin.sync();

                std::string word;
                std::getline(std::cin, word);  // ユーザーから単語を入力
                send(clientSocket, word.c_str(), word.length(), 0);
            }

            // ゲーム終了時の処理
            if (serverMessage.find("ゲーム終了") != std::string::npos) {
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
