#ifndef _UNIX_SOCKET_H_
#define _UNIX_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <time.h>

#define TIMEOUT_SECONDS 60

typedef enum {
    SOCKET_ERR_NONE,
    SOCKET_ERR_SOCKET,
    SOCKET_ERR_BIND,
    SOCKET_ERR_LISTEN,
    SOCKET_ERR_ACCEPT,
    SOCKET_ERR_RECV,
    SOCKET_ERR_SEND,
    SOCKET_ERR_INPUT_TOO_LONG,
    SOCKET_ERR_NULL_POINTER,
    SOCKET_ERR_CLOSE,
    SOCKET_ERR_UNLINK,
    SOCKET_ERR_UNKNOWN
} socket_error_t;

typedef enum { COMMAND_DATE, COMMAND_TIME, COMMAND_EXIT, COMMAND_SHUTDOWN, COMMAND_UNKNOWN } command_t;

typedef struct {
    int       server_fd;
    int       client_fd;
    char      buf[100];
    char      socket_path[108]; // 108 - максимальная длина пути для AF_UNIX
    command_t command;
} unix_server_t;

socket_error_t initialize_socket(unix_server_t *server, const char *socket_path);
socket_error_t accept_connection(unix_server_t *server);
socket_error_t receive_data(unix_server_t *server);
socket_error_t send_data(unix_server_t *server, const char *buf);
socket_error_t shutdown_server(unix_server_t *server);
socket_error_t process_command(unix_server_t *server);

#endif /*_UNIX_SOCKET_H_*/