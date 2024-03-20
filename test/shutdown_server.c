#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

// Mock for close function
int __wrap_close(int fd)
{
    check_expected(fd);
    return mock_type(int);
}

// Mock for unlink function
int __wrap_unlink(const char *path)
{
    check_expected_ptr(path);
    return mock_type(int);
}

// Define a test case for successful server shutdown
static void test_shutdown_server_success(void **state)
{
    unix_server_t server;

    // Expectation for successful server_fd closing
    expect_value(__wrap_close, fd, server.server_fd);

    // Assume that close will be successful
    will_return(__wrap_close, 0);

    // Expectation for unlinking the socket path
    expect_string(__wrap_unlink, path, server.socket_path);

    // Assume that unlink will be successful
    will_return(__wrap_unlink, 0);

    // Call the function under test
    socket_error_t result = shutdown_server(&server);

    // Check if the function completes without errors
    assert_int_equal(result, SOCKET_ERR_NONE);
}

// Define a test case for NULL pointer error
static void test_shutdown_server_null_pointer(void **state)
{
    // Call the function with a NULL pointer to the server
    socket_error_t result = shutdown_server(NULL);

    // Check if the function returns the expected error
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

// Define a test case for server_fd closing failure
static void test_shutdown_server_close_failure(void **state)
{
    unix_server_t server;

    // Expectation for unsuccessful server_fd closing
    expect_value(__wrap_close, fd, server.server_fd);

    // Assume that close will fail
    will_return(__wrap_close, -1);

    // Call the function under test
    socket_error_t result = shutdown_server(&server);

    // Check if the function returns the expected error
    assert_int_equal(result, SOCKET_ERR_CLOSE);
}

// Define a test case for unlinking the socket path failure
static void test_shutdown_server_unlink_failure(void **state)
{
    unix_server_t server;

    // Expectation for successful server_fd closing
    expect_value(__wrap_close, fd, server.server_fd);

    // Assume that close will be successful
    will_return(__wrap_close, 0);

    // Expectation for unsuccessful unlinking of the socket path
    expect_string(__wrap_unlink, path, server.socket_path);

    // Assume that unlink will fail
    will_return(__wrap_unlink, -1);

    // Call the function under test
    socket_error_t result = shutdown_server(&server);

    // Check if the function returns the expected error
    assert_int_equal(result, SOCKET_ERR_UNLINK);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_shutdown_server_success),
        cmocka_unit_test(test_shutdown_server_null_pointer),
        cmocka_unit_test(test_shutdown_server_close_failure),
        cmocka_unit_test(test_shutdown_server_unlink_failure),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}