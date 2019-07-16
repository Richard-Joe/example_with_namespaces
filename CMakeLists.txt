cmake_minimum_required(VERSION 3.7.1)

project (example_with_namespaces)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_subdirectory(api)
add_subdirectory(pid)
add_subdirectory(user)
add_subdirectory(net)
add_subdirectory(mnt)
add_subdirectory(ipc)
