//
// Created by Kristina Theroux on 1/23/20.
//

#ifndef DISNEYPROTO_TCPCONNECTION_H
#define DISNEYPROTO_TCPCONNECTION_H


#include <string>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <netinet/in.h>
typedef int SOCKET;
#endif

class TcpConnection
{
public:
    TcpConnection();

    ~TcpConnection();

    void SetPort(int port);

    void SetAddress(const char *addr);

    static void InitializeWinsock();

    bool Open();

    void Close();

    bool IsOpen() const
    {
        return mSocket != -1;
    }

    size_t Write(const char *buffer, size_t length);

    int Read(char *buffer, size_t size);


protected:

    bool InitializeSSL();

    void FreeSSL();

    void FreeSSLContext();

    const char* mAddr;
    uint32_t mPort;
    SOCKET mSocket;
    static sockaddr_in mSockAddr;

};


#endif //DISNEYPROTO_TCPCONNECTION_H
