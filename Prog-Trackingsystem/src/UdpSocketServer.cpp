/*
 * UdpSocketServer.cpp
 *
 * function: create an UDP server for remote connection and control
 *
 * author: Jannik Beyerstedt
 */

#include "UdpSocketServer.h"

UdpSocketServer::UdpSocketServer(int port) {
  clientConnected = false;
  newClientMessage = false;

#ifndef _WIN32
  serverLen = sizeof(server);
  remoteLen = sizeof(remote);

  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket < 0) {
    fprintf(stderr, "ERROR: in Socket::Socket - opening socket \n");
    exit(0);
  }
  bzero(&server, serverLen);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);
  rc = bind(udp_socket, (struct sockaddr *) &server, serverLen);
  if (rc < 0) {
    fprintf(stderr, "ERROR: in Socket::Socket - binding \n");
    exit(0);
  }

#else
  serverLen = sizeof(SOCKADDR_IN);
  remoteLen = sizeof(SOCKADDR_IN);

  WSADATA wsa;
  rc = WSAStartup(MAKEWORD(2, 0), &wsa);
  if (rc != 0) {
    fprintf(stderr, "ERROR: in Socket::Socket - startWinsock, code: %ld \n", rc);
    exit(0);
  }

  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket == INVALID_SOCKET) {
    fprintf(stderr, "ERROR: in Socket::Socket - opening socket, code: %d\n", WSAGetLastError());
    exit(0);
  }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = ADDR_ANY;
  server.sin_port = htons(port);
  rc = bind(udp_socket, (SOCKADDR*) &server, sizeof(SOCKADDR_IN));
  if (rc == SOCKET_ERROR) {
    printf("ERROR: in Socket::Socket - binding, code: %d\n", WSAGetLastError());
    exit(0);
  }
  u_long iMode = 1;
  ioctlsocket(udp_socket, FIONBIO, &iMode);
#endif

}

UdpSocketServer::~UdpSocketServer() {
#ifndef _WIN32
  close(udp_socket);
#else
  closesocket(udp_socket);
#endif
}

void UdpSocketServer::evaluate() {
#ifndef _WIN32
  rc = recvfrom(udp_socket, message, MESSAGE_LEN, MSG_DONTWAIT, (struct sockaddr *) &remote, &remoteLen);

  if (rc > 0) {
    newClientMessage = true;

    write(1, "[LOG] received packet: ", 23);
    write(1, message, rc);

    if (0 == strncmp(message, "connect", 7)) {
      clientConnected = true;
      newClientMessage = false;

      rc = sendto(udp_socket, "you are connected\n", 19, 0, (struct sockaddr *) &remote, remoteLen);
      if (rc < 0) {
        fprintf(stderr, "[ERROR] in Socket::evaluate - sendto\n");
        clientConnected = false;
      }
    } // only send ack if connection is requested, so there is no else

  }

#else
  rc = recvfrom(udp_socket, message, MESSAGE_LEN, 0, (SOCKADDR*) &remote, &remoteLen);

  if (rc != SOCKET_ERROR) {
    newClientMessage = true;

    fprintf(stdout, "[LOG] received packet: ");
    fprintf(stdout, "%s \n", message);

    if (0 == strncmp(message, "connect", 7)) {
      clientConnected = true;
      newClientMessage = false;

      rc = sendto(udp_socket, "you are connected\n", 19, 0, (SOCKADDR*) &remote, remoteLen);
      if (rc == SOCKET_ERROR) {
        fprintf(stderr, "[ERROR] in Socket::evaluate - sendto\n");
        clientConnected = false;
      }
    } // only send ack if connection is requested, so there is no else

  }

#endif

}

bool UdpSocketServer::is_clientConnected() {
  return clientConnected;
}

bool UdpSocketServer::get_message(std::string & message) {
  message = this->message;

  if (newClientMessage) {
    newClientMessage = false;
    return 1;
  } else {
    return 0;
  }

}

int UdpSocketServer::sendMessage(char message[MESSAGE_LEN], unsigned int messageLen) {
#ifndef _WIN32
  if (clientConnected) {
    rc = sendto(udp_socket, message, messageLen, 0, (struct sockaddr *) &remote, remoteLen);
    if (rc < 0) {
      fprintf(stderr, "[ERROR] in Socket::sendMessage - sendto\n");
      return MYERROR;
    }
  }
  return MYOK;

#else
  if (clientConnected) {
    rc = sendto(udp_socket, message, messageLen, 0, (SOCKADDR*) &remote, remoteLen);
    if (rc == SOCKET_ERROR) {
      fprintf(stderr, "[ERROR] in Socket::sendMessage - sendto\n");
      return MYERROR;
    }
  }
  return MYOK;
#endif
}
