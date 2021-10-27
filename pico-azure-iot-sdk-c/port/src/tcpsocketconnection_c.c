// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// CAVEAT: This sample is to demonstrate azure IoT client concepts only and is not a guide design principles or style
// Checking of return codes and error values shall be omitted for brevity.  Please practice sound engineering practices
// when writing production code.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>     
#include <time.h>
#include "pico/stdlib.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "dns.h"
#include "netif.h"
#include <stdbool.h>
#include "azure_c_shared_utility/tcpsocketconnection_c.h"

/* structure for adaptation between Azure mbed socket IO and WIZnet W5XXX Connectivity */
typedef struct
{
  int32_t socket;
} TCPSocketConnection;

#define MAX_SOCK_NUM 2
// socket assgin
// 0, 1 - application, 2 channel avail
// 2 - DHCP
// 3 - DNS, SNTP

static TCPSocketConnection socketconnection[MAX_SOCK_NUM];
static uint16_t avail_local_port = 5555;

TCPSOCKETCONNECTION_HANDLE tcpsocketconnection_create(void)
{
  TCPSocketConnection *psock = NULL;
  uint8_t avail_socket_fd=0;
  for (int avail_socket_fd=0; avail_socket_fd<MAX_SOCK_NUM; avail_socket_fd++) {
    if (getSn_SR(avail_socket_fd) == SOCK_CLOSED) {
      psock = (TCPSocketConnection *)&socketconnection[avail_socket_fd];
printf("=== tcpsocketconnection_create enter avail_socket_fd(%d) ===\r\n", avail_socket_fd);
      psock->socket = socket(avail_socket_fd, Sn_MR_TCP, avail_local_port++, 0);
printf("=== tcpsocketconnection_create finish (%d) ===\r\n", psock->socket);
      break;
    }
  }
  if (avail_socket_fd >= MAX_SOCK_NUM) {
printf("=== tcpsocketconnection_create ERROR (%d) ===\r\n", avail_socket_fd);
    psock = NULL;
  }
  return (TCPSOCKETCONNECTION_HANDLE)psock;
}

void tcpsocketconnection_destroy(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    TCPSocketConnection *psock = (TCPSocketConnection *)(tcpSocketConnectionHandle);
    printf("=== 1. tcpsocketconnection_destroy (%d %d) === \n", psock, psock->socket);
    close(psock->socket);
    printf("=== 2. tcpsocketconnection_destroy (%d %d) === \n", psock, psock->socket);
  }
}

void tcpsocketconnection_set_blocking(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle, bool blocking, unsigned int timeout)
{
  printf("=== tcpsocketconnection_set_blocking ===\r\n");
  // need to complete later  
/*
  TCPSocketConnection *psock = (TCPSocketConnection *)(tcpSocketConnectionHandle);
  int32_t option_timeout = 0;
  int ret = NET_OK;
  
  if (blocking)
  {
    option_timeout = timeout;
  }
  else
  {
    option_timeout = 0;
  }
  
  ret = net_setsockopt(psock->socket, NET_SOL_SOCKET, NET_SO_RCVTIMEO, (void *)&option_timeout, sizeof(option_timeout));
  if ( ret != NET_OK)
  {
    msg_error("Error setting the socket receive timeout option (%d).\n", ret);
  }
  else
  {
    ret = net_setsockopt(psock->socket, NET_SOL_SOCKET, NET_SO_SNDTIMEO, (void *)&option_timeout, sizeof(option_timeout));
    if ( ret != NET_OK)
    {
      msg_error("Error setting the socket send timeout option (%d).\n", ret);
    }
  }
*/
}


int tcpsocketconnection_connect(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle, const char* host, const int port)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    uint8_t gettingip[4] = {0,};

    TCPSocketConnection *psock = (TCPSocketConnection *)(tcpSocketConnectionHandle);
  // 0 - succ, <0 - fail
    int rc = 0;

    printf("=== tcpsocketconnection_connect %s:%d (%d) ===\r\n", host, port, psock->socket);

    rc = wizchip_gethostbyname(host, gettingip);
    // rc = net_if_gethostbyname(NULL, (sockaddr_t *)&addr, (char_t *)host);
    if (rc != 1)
    {
      printf("Could not find hostname ipaddr %s (%d)\n", host, rc);
    }
    else
    {
      // rc = net_connect(psock->socket, (sockaddr_t *)&addr, sizeof(addr));
      rc = 0; //succ
      rc = connect(psock->socket, gettingip, port);
  /*    while(getSn_SR(psock->socket) != SOCK_ESTABLISHED)
      {
        if (getSn_SR(psock->socket) == SOCK_CLOSED)
        {
          setSn_IR(psock->socket, Sn_IR_TIMEOUT);
          printf("Could not connect to %s:%d (%d)\n", host, port, rc);
          rc = -1;
          break;
        }
      }
  */
      if (rc < 0) { 
          printf("=== tcpsocketconnection_connect failed ===\n");
          return -1; 
      }
      else {
          printf("=== tcpsocketconnection_connect ok ===\n");
          return 0;
      } 
    }
    return rc;
  }
  else {
    printf("=== tcpSocketConnectionHandle is NULL ===\n");
    return -1; 
  }
}

bool tcpsocketconnection_is_connected(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle)
{
  printf("=== enter tcpsocketconnection_is_connected ===\n");
	// TCPSocketConnection* tsc = (TCPSocketConnection*)tcpSocketConnectionHandle;
	// return tsc->is_connected();
  if (tcpSocketConnectionHandle != NULL)
  {
    uint8_t status = 0;
    TCPSocketConnection* psock = (TCPSocketConnection*)tcpSocketConnectionHandle;
    if ( getsockopt(psock->socket, SO_STATUS, &status) == SOCK_OK)
    {
      if ((status == SOCK_ESTABLISHED) || (status == SOCK_CLOSE_WAIT)) return true;
    }
  }
  return false;
}

void tcpsocketconnection_close(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    TCPSocketConnection* psock = (TCPSocketConnection*)tcpSocketConnectionHandle;
    printf("=== 1. tcpsocketconnection_close (%d %d) === \n", psock, psock->socket);
    close(psock->socket);
    printf("=== 2. tcpsocketconnection_close (%d %d) === \n", psock, psock->socket);
  }
}

int tcpsocketconnection_send(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle, const char* data, int length)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    TCPSocketConnection* psock = (TCPSocketConnection*)tcpSocketConnectionHandle;
    return send(psock->socket,  (uint8_t *)data, length);
  }
  else {
    printf("=== tcpSocketConnectionHandle is NULL ===\n");
    return -1; 
  }
}

int tcpsocketconnection_send_all(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle, const char* data, int length)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    TCPSocketConnection* psock = (TCPSocketConnection*)tcpSocketConnectionHandle;
    return send(psock->socket, (uint8_t *)data, length);
  }
  else {
    printf("=== tcpSocketConnectionHandle is NULL ===\n");
    return -1; 
  }
}

int tcpsocketconnection_receive(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle, char* data, int length)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    TCPSocketConnection* psock = (TCPSocketConnection*)tcpSocketConnectionHandle;
    int recvsize = 0;
    int timeout = 0;
    while (1) {
      recvsize = getSn_RX_RSR(psock->socket);
      if (recvsize > 0) return recv(psock->socket, (uint8_t*)data, length);
      //  if (recvsize >= length) return recv(psock->socket, (uint8_t*)data, length);
      sleep_ms(100);
      timeout++;
      if (timeout > 10) return 0;
    }
    // else return 0;
  }
  else {
    printf("=== tcpSocketConnectionHandle is NULL ===\n");
    return -1; 
  }
}

int tcpsocketconnection_receive_all(TCPSOCKETCONNECTION_HANDLE tcpSocketConnectionHandle, char* data, int length)
{
  if (tcpSocketConnectionHandle != NULL)
  {
    printf("=== tcpsocketconnection_receive_all ===\n");
    TCPSocketConnection* psock = (TCPSocketConnection*)tcpSocketConnectionHandle;
    int recvsize = getSn_RX_RSR(psock->socket);
    if (recvsize) return recv(psock->socket, (uint8_t*)data, length);
    else return 0;
  }
  else {
    printf("=== tcpSocketConnectionHandle is NULL ===\n");
    return -1; 
  }
}
