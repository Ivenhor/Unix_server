#include "unix_socket.h"

socket_error_t initialize_socket(unix_server_t *server, const char *socket_path)
{
    if (server == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    //TODO! add check for path
    struct sockaddr_un server_addr;
    memset(&server->buf, 0, sizeof(server->buf));

    close(server->server_fd);
    unlink(socket_path);

    // Создание сокета
    if ((server->server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return SOCKET_ERR_SOCKET;
    }

    // Установка адреса сервера
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    // Привязка сокета к адресу
    if (bind(server->server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        return SOCKET_ERR_BIND;
    }

    // Ожидание подключений от клиентов
    if (listen(server->server_fd, 5) == -1) {
        perror("listen");
        return SOCKET_ERR_LISTEN;
    }

    printf("Server started. Avaiting for connections...\n");

    strncpy(server->socket_path, socket_path, sizeof(server->socket_path) - 1);

    return SOCKET_ERR_NONE;
}

socket_error_t accept_connection(unix_server_t *server)
{
    if (server == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    struct sockaddr_un client_addr;
    socklen_t          len = sizeof(struct sockaddr_un);

    // Принятие подключения
    if ((server->client_fd = accept(server->server_fd, (struct sockaddr *)&client_addr, &len)) == -1) {
        perror("accept");
        return SOCKET_ERR_ACCEPT;
    }

    printf("Client connected.\n");

    return SOCKET_ERR_NONE;
}

socket_error_t receive_data(unix_server_t *server)
{
    if (server == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    // Получение данных от клиента
    if (recv(server->client_fd, server->buf, sizeof(server->buf), 0) == -1) {
        perror("recv");
        return SOCKET_ERR_RECV;
    }

    printf("Recived from client: %s\n", server->buf);

    return SOCKET_ERR_NONE;
}

socket_error_t send_data(unix_server_t *server, const char *buf)
{
    if (server == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    // Отправка данных клиенту
    if (send(server->client_fd, buf, strlen(buf), 0) == -1) {
        perror("send");
        return SOCKET_ERR_SEND;
    }

    printf("Response send.\n");

    return SOCKET_ERR_NONE;
}

socket_error_t shutdown_server(unix_server_t *server)
{
    if (server == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    // Закрытие сокета сервера
    if (close(server->server_fd) == -1) {
        perror("close");
        return SOCKET_ERR_CLOSE;
    }

    if (unlink(server->socket_path) == -1) {
        perror("unlink");
        return SOCKET_ERR_UNLINK;
    } // Удаление файла сокета

    return SOCKET_ERR_NONE;
}

socket_error_t process_command(unix_server_t *server)
{

    if (server == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    server->command = COMMAND_UNKNOWN;

    size_t input_length = strlen(server->buf);
    // Проверка длины входящего сообщения
    if (input_length >= sizeof(server->buf)) {
        strcpy(server->buf, "Error: Input message is too long");
        return (SOCKET_ERR_INPUT_TOO_LONG);
    }

    if (strcmp(server->buf, "date") == 0) {
        server->command    = COMMAND_DATE;
        time_t     t       = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(server->buf, sizeof(server->buf), "%Y-%m-%d", tm_info);
    } else if (strcmp(server->buf, "time") == 0) {
        server->command    = COMMAND_TIME;
        time_t     t       = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(server->buf, sizeof(server->buf), "%H:%M:%S", tm_info);
    } else if (strcmp(server->buf, "exit") == 0) {
        server->command = COMMAND_EXIT;
        printf("Exit command send. Close connection.\n");
        return SOCKET_ERR_NONE;
    } else if (strcmp(server->buf, "shutdown") == 0) {
        server->command = COMMAND_SHUTDOWN;
        printf("Server shutdown command send. Close connection.\n");
    } else {
        server->command = COMMAND_UNKNOWN;
        strcpy(server->buf, "Error: Unsupported command");
        return (SOCKET_ERR_UNKNOWN);
    }

    return SOCKET_ERR_NONE;
}