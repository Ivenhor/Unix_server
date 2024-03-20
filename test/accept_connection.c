#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

int __wrap_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    check_expected(sockfd);
    check_expected_ptr(addr);
    check_expected_ptr(addrlen);

    // Возвращаем желаемое значение
    return mock_type(int);
}

// Define a test case for NULL pointer error
static void test_accept_connection_null_pointer(void **state)
{
    // Вызываем функцию с NULL указателем на сервер
    socket_error_t result = accept_connection(NULL);

    // Проверяем, что функция возвращает ожидаемую ошибку
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

// Define a test case for successful connection acceptance
static void test_accept_connection_success(void **state)
{
    unix_server_t      server;
    struct sockaddr_un addr;
    socklen_t          addrlen = sizeof(addr);

    // Устанавливаем ожидаемые значения для accept
    expect_value(__wrap_accept, sockfd, server.server_fd);
    expect_any(__wrap_accept, addr);
    expect_any(__wrap_accept, addrlen);

    // Предполагаем, что accept будет успешным
    will_return(__wrap_accept, 0); // Пусть accept вернет 0

    // Вызываем функцию для тестирования
    socket_error_t result = accept_connection(&server);

    // Проверяем, что функция завершается без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);
}

// Define a test case for accept failure
static void test_accept_connection_failure(void **state)
{
    unix_server_t      server;
    struct sockaddr_un addr;
    socklen_t          addrlen = sizeof(addr);

    // Устанавливаем ожидаемые значения для accept
    expect_value(__wrap_accept, sockfd, server.server_fd);
    expect_any(__wrap_accept, addr);
    expect_any(__wrap_accept, addrlen);

    // Предполагаем, что accept завершится с ошибкой
    will_return(__wrap_accept, -1); // Пусть accept вернет ошибку

    // Вызываем функцию для тестирования
    socket_error_t result = accept_connection(&server);

    // Проверяем, что функция возвращает ожидаемую ошибку
    assert_int_equal(result, SOCKET_ERR_ACCEPT);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_accept_connection_success),
        cmocka_unit_test(test_accept_connection_null_pointer),
        cmocka_unit_test(test_accept_connection_failure),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}