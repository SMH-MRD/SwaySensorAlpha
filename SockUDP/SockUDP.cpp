// SockUDP.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"
#include "SockUDP.h"

/// @brief 
/// @param
/// @return 
/// @note
CSockUDPLib::CSockUDPLib()
{
    ZeroMemory(&m_sockaddrs, sizeof(m_sockaddrs));
    m_sockstat = SOCK_STATUS_CLOSE; // Socket状態
}

/// @brief 
/// @param
/// @return 
/// @note
CSockUDPLib::~CSockUDPLib()
{
    WSACleanup();
}

/// @brief 
/// @param
/// @return 
/// @note
int CSockUDPLib::Initialize(UINT socktype, char* ip, UINT port, int* errcode)
{
    int     ret = SOCK_ERROR_NONE;
    WSADATA wsaData;
    int     err;

    m_socktype = socktype;
    if (errcode != NULL) {*errcode = 0;}
    if ((err = WSAStartup(MAKEWORD(2,0), &wsaData)) != 0)
    {
        if (errcode != NULL) {*errcode = err;}
        ret =  SOCK_ERROR_INITIALIZE;
    }
    else
    {
        switch (m_socktype)
        {
        case SOCK_TYPE_RECV:                        // 受信Socket
            m_sockaddrs.sin_family = AF_INET;
            if (ip != NULL) {inet_pton(m_sockaddrs.sin_family, (PCSTR)ip, &m_sockaddrs.sin_addr.S_un.S_addr);}
            else            {m_sockaddrs.sin_addr.S_un.S_addr = INADDR_ANY;}
            m_sockaddrs.sin_port = htons(port);
            break;
        case SOCK_TYPE_SEND:                        // 送信Socket
            m_sockaddrs.sin_family = AF_INET;
            if (ip != NULL) {inet_pton(m_sockaddrs.sin_family, (PCSTR)ip, &m_sockaddrs.sin_addr.S_un.S_addr);}
            else            {inet_pton(m_sockaddrs.sin_family, (PCSTR)LOOPBACK_ADDRESS, &m_sockaddrs.sin_addr.S_un.S_addr);}
            m_sockaddrs.sin_port = htons(port);
            break;
        default:
            ret =  SOCK_ERROR_INITIALIZE;
            break;
        }   // switch (m_socktype)
    }
    return ret;
}

/// @brief 
/// @param
/// @return 
/// @note
int CSockUDPLib::Open(u_long blocking, int* errcode)
{
    int ret = SOCK_ERROR_NONE;

    if (errcode != NULL) {*errcode = 0;}
    if (m_sockstat == SOCK_STATUS_OPEN) {Close();}
    if((m_sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        if (errcode != NULL) {*errcode = WSAGetLastError();}
        ret =  SOCK_ERROR_OPEN;
    }
    else
    {
        switch (m_socktype)
        {
        case SOCK_TYPE_RECV:                    // 受信Socket
	        if (bind(m_sock, (struct sockaddr *)&m_sockaddrs, sizeof(m_sockaddrs)) == SOCKET_ERROR)
	        {
                if (errcode != NULL) {*errcode = WSAGetLastError();}
                Close();
                m_sockstat = SOCK_STATUS_CLOSE; // Socket状態
                ret        = SOCK_ERROR_OPEN;
            }
            else
            {
                m_sockblocking = blocking;
                ioctlsocket(m_sock, FIONBIO, &m_sockblocking);
                m_sockstat = SOCK_STATUS_OPEN;  // Socket状態
            }
            break;
        case SOCK_TYPE_SEND:                    // 送信Socket
            m_sockstat = SOCK_STATUS_OPEN;      // Socket状態
            break;
        default:
            ret =  SOCK_ERROR_OPEN;
            break;
        }
    }
    return ret;
}

/// @brief 
/// @param
/// @return 
/// @note
int CSockUDPLib::RecvData(char* data, int size, int* errcode)
{
    struct sockaddr_in from;
	int                sockaddr_in_size = sizeof(struct sockaddr_in);   // sockaddr_in構造体のサイズ
    int                recvsize = 0;

    if (errcode != NULL) {*errcode = 0;}
    if (m_sockstat == SOCK_STATUS_OPEN)
    {
		if ((recvsize = recvfrom(m_sock, data, size, 0, (struct sockaddr *)&from, &sockaddr_in_size)) == SOCKET_ERROR)
        {
            if (errcode != NULL) {*errcode = WSAGetLastError();}
        }
    }
    else
    {
        recvsize = 0;
    }
    return recvsize;
}

/// @brief 
/// @param
/// @return 
/// @note
int CSockUDPLib::SendData(char* data, int size, int* errcode)
{
	int sockaddr_in_size = sizeof(struct sockaddr_in);   // sockaddr_in構造体のサイズ
    int sendsize = 0;

    if (errcode != NULL) {*errcode = 0;}
    if (m_sockstat == SOCK_STATUS_OPEN)
    {
		if ((sendsize = sendto(m_sock, data, size, 0, (struct sockaddr *)&m_sockaddrs, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
        {
            if (errcode != NULL) {*errcode = WSAGetLastError();}
        }
    }
    else
    {
        sendsize = 0;
    }
    return sendsize;
}

/// @brief 
/// @param
/// @return 
/// @note
int CSockUDPLib::Close(int* errcode)
{
    int ret = SOCK_ERROR_NONE;

    if (errcode != NULL) {*errcode = 0;}
    if (shutdown(m_sock, SD_BOTH) == SOCKET_ERROR)
    {
        if (errcode != NULL) {*errcode = WSAGetLastError();}
        ret = SOCK_ERROR_CLOSE;
    }
	if (closesocket(m_sock) == SOCKET_ERROR)
    {
        if (errcode != NULL) {*errcode = WSAGetLastError();}
        ret = SOCK_ERROR_CLOSE;
    }
    m_sockstat = SOCK_STATUS_CLOSE; // Socket状態
    return ret;
}
