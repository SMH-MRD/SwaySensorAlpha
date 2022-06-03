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

    int Initialize(UINT socktype, char* ip = NULL, UINT port = SERVICE_PORT, int* errcode = NULL);
    int Open(u_long blocking = 0, int* errcode = NULL);
    int SendData(char* data, int size, int* errcode = NULL);
    int RecvData(char* data, int size, int* errcode = NULL);
    int Close(int* errcode = NULL);
    int GetSockStatus() {return m_sockstat;}

private:
    UINT               m_socktype;      //
    SOCKET             m_sock;          // Socket
    struct sockaddr_in m_sockaddrs;     //
    u_long             m_sockblocking;  //
    UINT               m_sockstat;      // Socket���
};
