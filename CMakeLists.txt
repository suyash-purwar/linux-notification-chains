cmake_minimum_required(VERSION 3.31)
project(linux_notification_chain C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_notification_chain glthread.c notification_chain.c main.c)

target_include_directories(linux_notification_chain include)
