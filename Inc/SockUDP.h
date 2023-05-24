#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SOCK_ERROR_NONE             0       // Socket初期化成功
#define SOCK_ERROR_INITIALIZE       -1      // Socket初期化失敗
#define SOCK_ERROR_OPEN             -2      // Socket Open失敗
#define SOCK_ERROR_CLOSE            -3      // Socket Close失敗

#define SOCK_TYPE_RECV              0       // 受信Socket
#define SOCK_TYPE_SEND              1       // 送信Socket

#define SOCK_BLOCKING               0       // ブロッキング
#define SOCK_NONBLOCKING            1       // ノンブロッキング

#define SOCK_STATUS_CLOSE           0       // Socket接続状態：CLOSE
#define SOCK_STATUS_OPEN            1       // Socket接続状態：OPEN

#define LOOPBACK_ADDRESS            "127.0.0.1"
#define SERVICE_PORT                50000

class CSockUDPLib
{
public:
    CSockUDPLib();
    ~CSockUDPLib();

    int initialize(int* errcode = NULL);                                            // Winsockの初期化
    int open(UINT socktype = SOCK_TYPE_RECV, u_long blocking = SOCK_BLOCKING,
             const char* ip = NULL, UINT port = SERVICE_PORT, int* errcode = NULL); // Socketのオープン
    int send_data(char* data, int size, int* errcode = NULL);                       // データ送信
    int recv_data(char* data, int size, PSOCKADDR_IN addrs, int* errcode = NULL);   // データ受信
    int close(int* errcode = NULL);                                                 // Socketのクローズ
    int get_sock_status(void);                                                      // Socket状態の取得

private:
    UINT        m_socktype;     //
    SOCKET      m_sock;         // Socket
    SOCKADDR_IN m_sockaddrs;    //
    u_long      m_sockblocking; //
    UINT        m_sockstat;     // Socket状態
};
