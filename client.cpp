#define STRICT

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <cstring>

#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void)
{
	// ネットワーク標準関数群 //
	WSADATA wsa;
	SOCKET clinet_socket;
	sockaddr_in server_addr;
	char buffer[1024];

	char server_ip[16];
	int server_port;

	// WinSockの初期化 //
	std::cout << "WinSockを初期化..." << std::endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cerr << "WinSockの初期化に失敗 エラーコード: " << WSAGetLastError() << std::endl;

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
	std::cout << "接続先のサーバのIPを入力してください: ";
	std::cin.getline(server_ip, 16);
	std::cout << "接続先のサーバのポートを入力してください: ";
	std::cin >> server_port;
	std::cin.ignore();
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);

	wchar_t wc_server_ip[16];
	size_t converted_chars = 0;

	if (mbstowcs_s(&converted_chars, wc_server_ip, 16, server_ip, _TRUNCATE) != 0)
	{
		std::cerr << "IPアドレスの変換に失敗しました " << std::endl;
		closesocket(clinet_socket);
		WSACleanup();

		return 1;
	}

	// IPアドレスをバイナリ形式に変換して設定
	if (InetPton(AF_INET, wc_server_ip, &server_addr.sin_addr) <= 0)
	{
		std::cerr << "IPアドレスの設定に失敗しました" << std::endl;
		closesocket(clinet_socket);
		WSACleanup();

		return 1;
	}

	// サーバに接続
	if (connect(clinet_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "サーバ接続に失敗 エラーコード : " << WSAGetLastError() << std::endl;
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
			std::cout << "サーバ接続を終了" << std::endl;
			break;
		}

		// サーバにメッセージを送信
		if (send(clinet_socket, buffer, strlen(buffer), 0) < 0)
		{
			std::cerr << "メッセージの送信に失敗: " << WSAGetLastError() << std::endl;
			break;
		}

		// メッセージの受信
		int recv_size = recv(clinet_socket, buffer, 1024, 0);

		if (recv_size == SOCKET_ERROR)
		{
			std::cerr << "メッセージの受信に失敗 エラーコード: " << WSAGetLastError() << std::endl;
			break;
		}
		else if (recv_size == 0)
		{
			std::cout << "サーバが切断されました" << std::endl;
			break;
		}

		buffer[recv_size] = '\0';

		std::cout << "サーバからのメッセージ: " << buffer << std::endl;
	}

	// ソケットの終了
	closesocket(clinet_socket);

	WSACleanup();

	return 0;
}