#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SOCK_ERROR_NONE             0       // Socket����������
#define SOCK_ERROR_INITIALIZE       -1      // Socket���������s
#define SOCK_ERROR_OPEN             -2      // Socket Open���s
#define SOCK_ERROR_CLOSE            -3      // Socket Close���s

#define SOCK_TYPE_RECV              0       // ��MSocket
#define SOCK_TYPE_SEND              1       // ���MSocket

#define SOCK_BLOCKING               0       // �u���b�L���O
#define SOCK_NONBLOCKING            1       // �m���u���b�L���O

#define SOCK_STATUS_CLOSE           0       // Socket�ڑ���ԁFCLOSE
#define SOCK_STATUS_OPEN            1       // Socket�ڑ���ԁFOPEN

#define LOOPBACK_ADDRESS            "127.0.0.1"
#define SERVICE_PORT                50000

class CSockUDPLib
{
public:
    CSockUDPLib();
    ~CSockUDPLib();

    int initialize(int* errcode = NULL);                                            // Winsock�̏�����
    int open(UINT socktype = SOCK_TYPE_RECV, u_long blocking = SOCK_BLOCKING,
             const char* ip = NULL, UINT port = SERVICE_PORT, int* errcode = NULL); // Socket�̃I�[�v��
    int send_data(char* data, int size, int* errcode = NULL);                       // �f�[�^���M
    int recv_data(char* data, int size, PSOCKADDR_IN addrs, int* errcode = NULL);   // �f�[�^��M
    int close(int* errcode = NULL);                                                 // Socket�̃N���[�Y
    int get_sock_status(void);                                                      // Socket��Ԃ̎擾

private:
    UINT        m_socktype;     //
    SOCKET      m_sock;         // Socket
    SOCKADDR_IN m_sockaddrs;    //
    u_long      m_sockblocking; //
    UINT        m_sockstat;     // Socket���
};
