#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

// Тест для функции process_command
static void test_process_command(void **state)
{
    unix_server_t server;
    memset(&server, 0, sizeof(unix_server_t));

    socket_error_t error = process_command(NULL);
    assert_int_equal(error, SOCKET_ERR_NULL_POINTER);

    // Проверка команды "date"
    strcpy(server.buf, "date");
    error = process_command(&server);
    assert_int_equal(error, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_DATE);

    // Проверка команды "time"
    strcpy(server.buf, "time");
    error = process_command(&server);
    assert_int_equal(error, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_TIME);

    // Проверка команды "exit"
    strcpy(server.buf, "exit");
    error = process_command(&server);
    assert_int_equal(error, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_EXIT);

    // Проверка команды "shutdown"
    strcpy(server.buf, "shutdown");
    error = process_command(&server);
    assert_int_equal(error, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_SHUTDOWN);

    // Проверка неизвестной команды
    strcpy(server.buf, "random text");
    error = process_command(&server);
    assert_int_equal(error, SOCKET_ERR_UNKNOWN);
    assert_int_equal(server.command, COMMAND_UNKNOWN);

    // Создание слишком длинного сообщения
    char long_message[101]; // Больше максимально допустимой длины на 1 символ
    memset(long_message, 'a', sizeof(long_message) - 1);
    long_message[sizeof(long_message) - 1] = '\0'; // Добавляем завершающий символ
    strcpy(server.buf, long_message);

    // Проверка функции process_command
    error = process_command(&server);
    assert_int_equal(error, SOCKET_ERR_INPUT_TOO_LONG);
    assert_int_equal(server.command, COMMAND_UNKNOWN);
}

static void test_shutdown_server_null_pointer(void **state)
{
    socket_error_t result = shutdown_server(NULL);
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

ssize_t mock_send(int sockfd, const void *buf, size_t len, int flags)
{
    check_expected(sockfd);
    check_expected(buf);
    check_expected(len);
    check_expected(flags);
    return mock_type(ssize_t);
}

static void test_send_data_null_pointer(void **state)
{
    socket_error_t result = send_data(NULL, "Test message");
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

static void test_receive_data_null_pointer(void **state)
{
    socket_error_t result = receive_data(NULL);
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

static void test_initialize_socket(void **state)
{
    unix_server_t server;
    const char   *socket_path = "/tmp/test_socket";

    // Вызываем функцию инициализации сокета
    socket_error_t result = initialize_socket(&server, socket_path);

    // Проверяем, что функция завершилась без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);

    result = initialize_socket(NULL, socket_path);
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_process_command),        cmocka_unit_test(test_shutdown_server_null_pointer),
        cmocka_unit_test(test_send_data_null_pointer), cmocka_unit_test(test_receive_data_null_pointer),
        cmocka_unit_test(test_initialize_socket),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}