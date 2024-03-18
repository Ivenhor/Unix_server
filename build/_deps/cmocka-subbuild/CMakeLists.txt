# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.22.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(cmocka-populate NONE)


# Pass through things we've already detected in the main project to avoid
# paying the cost of redetecting them again in ExternalProject_Add()
set(GIT_EXECUTABLE [==[/usr/bin/git]==])
set(GIT_VERSION_STRING [==[2.34.1]==])
set_property(GLOBAL PROPERTY _CMAKE_FindGit_GIT_EXECUTABLE_VERSION
  [==[/usr/bin/git;2.34.1]==]
)


include(ExternalProject)
ExternalProject_Add(cmocka-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://git.cryptomilk.org/projects/cmocka.git" "GIT_TAG" "cmocka-1.1.5" "GIT_SHALLOW" "1"
                    SOURCE_DIR          "/home/oleksandr/workspace/C_C++/Unix_server/build/_deps/cmocka-src"
                    BINARY_DIR          "/home/oleksandr/workspace/C_C++/Unix_server/build/_deps/cmocka-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


