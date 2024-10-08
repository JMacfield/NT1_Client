#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main(void)
{
	// �l�b�g���[�N�W���֐��Q //
	WSADATA wsa;
	SOCKET clinet_socket;
	sockaddr_in server_addr;
	char buffer[1024];

	// WinSock�̏����� //
	std::cout << "WinSock��������..." << std::endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cerr << "���s �G���[�R�[�h : " << WSAGetLastError() << std::endl;

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
	server_addr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &server_addr.sin_addr.s_addr);
	server_addr.sin_port = htons(8888);

	// �T�[�o�ɐڑ�
	if (connect(clinet_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "�ڑ��Ɏ��s �G���[�R�[�h : " << WSAGetLastError() << std::endl;
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
			std::cout << "�ڑ����I��" << std::endl;
			break;
		}

		// �T�[�o�Ƀ��b�Z�[�W�𑗐M
		if (send(clinet_socket, buffer, strlen(buffer), 0) < 0)
		{
			std::cerr << "���b�Z�[�W�̑��M�Ɏ��s: " << WSAGetLastError() << std::endl;
			break;
		}

		std::cout << "���b�Z�[�W�𑗐M..." << std::endl;
	}

	// �\�P�b�g�̏I��
	closesocket(clinet_socket);

	WSACleanup();

	return 0;
}