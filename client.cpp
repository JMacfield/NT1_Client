#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main(void)
{
	// ネットワーク標準関数群 //
	WSADATA wsa;
	SOCKET clinet_socket;
	sockaddr_in server_addr;
	char buffer[1024];

	// WinSockの初期化 //
	std::cout << "WinSockを初期化..." << std::endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cerr << "失敗 エラーコード : " << WSAGetLastError() << std::endl;

		return 1;
	}

	std::cout << "WinSockが初期化されました..." << std::endl;

	// クライアントソケットの初期化 //
	if ((clinet_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cerr << "ソケットの作成に失敗..." << WSAGetLastError() << std::endl;

		return 1;
	}

	std::cout << "ソケットを作成" << std::endl;

	// サーバのIPアドレスとポートをバインド //
	server_addr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &server_addr.sin_addr.s_addr);
	server_addr.sin_port = htons(8888);

	// サーバに接続
	if (connect(clinet_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "接続に失敗 エラーコード : " << WSAGetLastError() << std::endl;
		closesocket(clinet_socket);
		WSACleanup();

		return 1;
	}

	std::cout << "サーバに接続 (終了する場合は'quit'を入力してください)" << std::endl;

	// メッセージ処理を抜け出すまでチャットを更新
	while (true)
	{
		std::cout << "送信するメッセージを入力してください: ";
		std::cin.getline(buffer, 1024);

		// quitを入力したら接続を終了
		if (strcmp(buffer, "quit") == 0)
		{
			std::cout << "接続を終了" << std::endl;
			break;
		}

		// サーバにメッセージを送信
		if (send(clinet_socket, buffer, strlen(buffer), 0) < 0)
		{
			std::cerr << "メッセージの送信に失敗: " << WSAGetLastError() << std::endl;
			break;
		}

		std::cout << "メッセージを送信..." << std::endl;
	}

	// ソケットの終了
	closesocket(clinet_socket);

	WSACleanup();

	return 0;
}