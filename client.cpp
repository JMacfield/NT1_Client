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
	// �l�b�g���[�N�W���֐��Q //
	WSADATA wsa;
	SOCKET clinet_socket;
	sockaddr_in server_addr;
	char buffer[1024];

	char server_ip[16];
	int server_port;

	// WinSock�̏����� //
	std::cout << "WinSock��������..." << std::endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cerr << "WinSock�̏������Ɏ��s �G���[�R�[�h: " << WSAGetLastError() << std::endl;

		return 1;
	}

	std::cout << "WinSock������������܂���..." << std::endl;

	// �N���C�A���g�\�P�b�g�̏����� //
	if ((clinet_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cerr << "�\�P�b�g�̍쐬�Ɏ��s..." << WSAGetLastError() << std::endl;

		return 1;
	}

	std::cout << "�\�P�b�g���쐬" << std::endl;

	// �T�[�o��IP�A�h���X�ƃ|�[�g���o�C���h //
	std::cout << "�ڑ���̃T�[�o��IP����͂��Ă�������: ";
	std::cin.getline(server_ip, 16);
	std::cout << "�ڑ���̃T�[�o�̃|�[�g����͂��Ă�������: ";
	std::cin >> server_port;
	std::cin.ignore();
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);

	wchar_t wc_server_ip[16];
	size_t converted_chars = 0;

	if (mbstowcs_s(&converted_chars, wc_server_ip, 16, server_ip, _TRUNCATE) != 0)
	{
		std::cerr << "IP�A�h���X�̕ϊ��Ɏ��s���܂��� " << std::endl;
		closesocket(clinet_socket);
		WSACleanup();

		return 1;
	}

	// IP�A�h���X���o�C�i���`���ɕϊ����Đݒ�
	if (InetPton(AF_INET, wc_server_ip, &server_addr.sin_addr) <= 0)
	{
		std::cerr << "IP�A�h���X�̐ݒ�Ɏ��s���܂���" << std::endl;
		closesocket(clinet_socket);
		WSACleanup();

		return 1;
	}

	// �T�[�o�ɐڑ�
	if (connect(clinet_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "�T�[�o�ڑ��Ɏ��s �G���[�R�[�h : " << WSAGetLastError() << std::endl;
		closesocket(clinet_socket);
		WSACleanup();

		return 1;
	}

	std::cout << "�T�[�o�ɐڑ� (�I������ꍇ��'quit'����͂��Ă�������)" << std::endl;

	// ���b�Z�[�W�����𔲂��o���܂Ń`���b�g���X�V
	while (true)
	{
		std::cout << "���M���郁�b�Z�[�W����͂��Ă�������: ";
		std::cin.getline(buffer, 1024);

		// quit����͂�����ڑ����I��
		if (strcmp(buffer, "quit") == 0)
		{
			std::cout << "�T�[�o�ڑ����I��" << std::endl;
			break;
		}

		// �T�[�o�Ƀ��b�Z�[�W�𑗐M
		if (send(clinet_socket, buffer, strlen(buffer), 0) < 0)
		{
			std::cerr << "���b�Z�[�W�̑��M�Ɏ��s: " << WSAGetLastError() << std::endl;
			break;
		}

		// ���b�Z�[�W�̎�M
		int recv_size = recv(clinet_socket, buffer, 1024, 0);

		if (recv_size == SOCKET_ERROR)
		{
			std::cerr << "���b�Z�[�W�̎�M�Ɏ��s �G���[�R�[�h: " << WSAGetLastError() << std::endl;
			break;
		}
		else if (recv_size == 0)
		{
			std::cout << "�T�[�o���ؒf����܂���" << std::endl;
			break;
		}

		buffer[recv_size] = '\0';

		std::cout << "�T�[�o����̃��b�Z�[�W: " << buffer << std::endl;
	}

	// �\�P�b�g�̏I��
	closesocket(clinet_socket);

	WSACleanup();

	return 0;
}