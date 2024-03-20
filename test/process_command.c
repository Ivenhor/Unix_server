#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

// Define a test case for successful processing of "date" command
static void test_process_command_date(void **state)
{
    unix_server_t server;
    strcpy(server.buf, "date");

    socket_error_t result = process_command(&server);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_DATE);
}

// Define a test case for successful processing of "time" command
static void test_process_command_time(void **state)
{
    unix_server_t server;
    strcpy(server.buf, "time");

    socket_error_t result = process_command(&server);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_TIME);
}

// Define a test case for successful processing of "exit" command
static void test_process_command_exit(void **state)
{
    unix_server_t server;
    strcpy(server.buf, "exit");

    socket_error_t result = process_command(&server);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_EXIT);
}

// Define a test case for successful processing of "shutdown" command
static void test_process_command_shutdown(void **state)
{
    unix_server_t server;
    strcpy(server.buf, "shutdown");

    socket_error_t result = process_command(&server);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(server.command, COMMAND_SHUTDOWN);
}

// Define a test case for processing of unsupported command
static void test_process_command_unknown(void **state)
{
    unix_server_t server;
    strcpy(server.buf, "unsupported_command");

    socket_error_t result = process_command(&server);

    assert_int_equal(result, SOCKET_ERR_UNKNOWN);
    assert_int_equal(server.command, COMMAND_UNKNOWN);
}

// Define a test case for processing of too long input message
static void test_process_command_input_too_long(void **state)
{
    unix_server_t server;
    char long_message[101]; // Больше максимально допустимой длины на 1 символ
    memset(long_message, 'a', sizeof(long_message) - 1);
    long_message[sizeof(long_message) - 1] = '\0'; // Добавляем завершающий символ
    strcpy(server.buf, long_message);

    socket_error_t result = process_command(&server);

    assert_int_equal(result, SOCKET_ERR_INPUT_TOO_LONG);
    assert_int_equal(server.command, COMMAND_UNKNOWN);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_process_command_date),    cmocka_unit_test(test_process_command_time),
        cmocka_unit_test(test_process_command_exit),    cmocka_unit_test(test_process_command_shutdown),
        cmocka_unit_test(test_process_command_unknown), cmocka_unit_test(test_process_command_input_too_long),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
