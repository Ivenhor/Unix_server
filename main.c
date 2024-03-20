#include <stdio.h>
#include "unix_socket/unix_socket.h"

int main()
{
    unix_server_t  library;
    socket_error_t err;

    err = initialize_socket(&library, "/tmp/socket_test");
    if (err != SOCKET_ERR_NONE) {
        fprintf(stderr, "Ошибка инициализации сокета.\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        err = accept_connection(&library);
        if (err != SOCKET_ERR_NONE) {
            fprintf(stderr, "Ошибка принятия подключения.\n");
            continue;
        }

        while (1) {
            err = receive_data(&library);
            if (err != SOCKET_ERR_NONE) {
                fprintf(stderr, "Ошибка при получении данных.\n");
                break;
            }

            // Обработка команды
            err = process_command(&library);
            if (err != SOCKET_ERR_NONE) {
                fprintf(stderr, "Ошибка при обработке данных.\n");
                //break;
            }
            if (library.command == COMMAND_EXIT) {
                break;
            }

            err = send_data(&library, library.buf);
            if (err != SOCKET_ERR_NONE) {
                fprintf(stderr, "Ошибка при отправке данных.\n");
                break;
            }
            if (library.command == COMMAND_SHUTDOWN) {
                shutdown_server(&library);
                exit(EXIT_SUCCESS);
            }

            memset(library.buf, 0, sizeof(library.buf));
        }
        memset(library.buf, 0, sizeof(library.buf));
        // Закрытие соединения с клиентом
        close(library.client_fd);
    }

    return 0;
}
