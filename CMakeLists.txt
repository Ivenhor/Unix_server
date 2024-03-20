cmake_minimum_required(VERSION 3.0.0)
project(Unix_server VERSION 0.1.0 LANGUAGES C)

enable_testing()

include(cmake/FetchCMocka.cmake)

add_library(Unix_socket unix_socket/unix_socket.c unix_socket/unix_socket.h)

add_executable(Unix_server main.c)

target_link_libraries(Unix_server
	PRIVATE
    Unix_socket
)

add_executable(unit_tests
	test/main.c
)
target_link_libraries(unit_tests PRIVATE cmocka-static Unix_socket)
add_test(NAME unit_tests COMMAND unit_tests)
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(initialize_socket test/initialize_socket.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(initialize_socket PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME initialize_socket COMMAND initialize_socket)

set_target_properties(initialize_socket PROPERTIES LINK_FLAGS "-Wl,--wrap=socket -Wl,--wrap=bind -Wl,--wrap=listen")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(accept_connection test/accept_connection.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(accept_connection PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME accept_connection COMMAND accept_connection)

set_target_properties(accept_connection PROPERTIES LINK_FLAGS "-Wl,--wrap=accept")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(receive_data test/receive_data.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(receive_data PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME receive_data COMMAND receive_data)
set_target_properties(receive_data PROPERTIES LINK_FLAGS "-Wl,--wrap=recv")

##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(send_data test/send_data.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(send_data PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME send_data COMMAND send_data)
set_target_properties(send_data PROPERTIES LINK_FLAGS "-Wl,--wrap=send")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(shutdown_server test/shutdown_server.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(shutdown_server PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME shutdown_server COMMAND shutdown_server)
set_target_properties(shutdown_server PROPERTIES LINK_FLAGS "-Wl,--wrap=close -Wl,--wrap=unlink")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(process_command test/process_command.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(process_command PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME process_command COMMAND process_command)
##################################################################################
set(CPACK_PACKAGE_NAME "Unix_socket")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
