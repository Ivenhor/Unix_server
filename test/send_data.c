#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

ssize_t __wrap_send(int sockfd, const void *buf, size_t len, int flags)
{
    // Проверяем переданные значения
    check_expected(sockfd);
    check_expected_ptr(buf);
    check_expected(len);
    check_expected(flags);

    // Здесь можно добавить дополнительную логику или просто вернуть значение из мока
    return mock_type(ssize_t);
}

// Define a test case for successful data sending
static void test_send_data_success(void **state)
{
    unix_server_t server;
    expect_value(__wrap_send, sockfd, server.client_fd);
    expect_string(__wrap_send, buf, "Test data");
    expect_value(__wrap_send, len, strlen("Test data"));
    expect_value(__wrap_send, flags, 0);

    // Предполагаем, что send будет успешным
    will_return(__wrap_send, 0); // Пусть send вернет 0

    // Вызываем функцию для тестирования
    socket_error_t result = send_data(&server, "Test data");

    // Проверяем, что функция завершается без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);
}

// Define a test case for NULL pointer error
static void test_send_data_null_pointer(void **state)
{
    // Вызываем функцию с NULL указателем на сервер
    socket_error_t result = send_data(NULL, "Test data");

    // Проверяем, что функция возвращает ожидаемую ошибку
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

// Define a test case for send failure
static void test_send_data_failure(void **state)
{
    unix_server_t server;
    expect_value(__wrap_send, sockfd, server.client_fd);
    expect_string(__wrap_send, buf, "Test data");
    expect_value(__wrap_send, len, strlen("Test data"));
    expect_value(__wrap_send, flags, 0);

    // Предполагаем, что send завершится с ошибкой
    will_return(__wrap_send, -1); // Пусть send вернет ошибку

    // Вызываем функцию для тестирования
    socket_error_t result = send_data(&server, "Test data");

    // Проверяем, что функция возвращает ожидаемую ошибку
    assert_int_equal(result, SOCKET_ERR_SEND);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_send_data_success),
        cmocka_unit_test(test_send_data_failure),
        cmocka_unit_test(test_send_data_null_pointer),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}