//
// Created by Kristina Theroux on 1/23/20.
//

#include "TcpConnection.h"

#ifndef WIN32
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#endif

sockaddr_in TcpConnection::mSockAddr = { 0 };

TcpConnection::TcpConnection()
        : mPort(80)
        , mAddr(0)
        , mSocket(-1)
{
    InitializeWinsock(); //for windows
}


TcpConnection::~TcpConnection()
{
    if(IsOpen())
        Close();
}


//////////////////////////////////////
//
// Method:    Open
//
//////////////////////////////////////
bool TcpConnection::Open()
{
    struct sockaddr_in server = mSockAddr;
    server.sin_family = AF_INET;
    server.sin_port = htons(mPort);

    static size_t addrsize = sizeof(mSockAddr);

    if(!mAddr)
        return false;


    struct addrinfo hints = { 0 };
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* result = 0;
    int r = getaddrinfo(mAddr, 0, &hints, &result);
    if(r != 0 || !result)
        return false;

#ifdef WIN32
    server.sin_addr.S_un.S_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr.S_un.S_addr;
#else
    server.sin_addr.s_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
#endif
    freeaddrinfo(result);

    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocket == (SOCKET)-1)
        return false;

#ifdef WIN32
    int opt = 30 * 1000;
    setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt, sizeof(opt));
    setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&opt, sizeof(opt));
#else
    struct timeval tv = { 0 };
    tv.tv_sec = 30;
    setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif

    r = connect(mSocket, (struct sockaddr*) &server, addrsize);
    if(-1 == r)
        return false;

    return true;

}

//////////////////////////////////////
//
// Method:    Close
//
//////////////////////////////////////
void TcpConnection::Close()
{
    if(IsOpen())
    {
#ifdef WIN32
        closesocket(mSocket);
#else
        close(mSocket);
#endif
    }
}

//////////////////////////////////////
//
// Method:    Write
//
//////////////////////////////////////
size_t TcpConnection::Write(const char* buffer, size_t length)
{
#ifdef WIN32
    return send(mSocket, buffer, length, 0);
#else
    return send(mSocket, buffer, length, 0);
#endif
}

//////////////////////////////////////
//
// Method:    Read
//
//////////////////////////////////////
int TcpConnection::Read(char* buffer, size_t size)
{
    return recv(mSocket, buffer, size, 0);
}


//////////////////////////////////////
//
// Method:    InitializeWinsock
//
//////////////////////////////////////
void TcpConnection::InitializeWinsock()
{
#ifdef WIN32
    WSADATA wsaData;
    WORD wVer = MAKEWORD(2, 2);
    WSAStartup(wVer, &wsaData);
#endif
}

////////////////////////////////////
//
// Method:    SetPort
//
////////////////////////////////////
void TcpConnection::SetPort(int port)
{
    mPort = port;
}

//////////////////////////////////////
//
// Method:    SetAddress
//
//////////////////////////////////////
void TcpConnection::SetAddress(const char* addr)
{
    if(strstr(addr, "http://") != 0)
        addr += 7;

    if(strstr(addr, "https://") != 0)
    {
        addr += 8;
        mPort = 443;
    }

    mAddr = addr;
}