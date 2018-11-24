cmake_minimum_required(VERSION 3.10)
project(smash C)

set(CMAKE_C_STANDARD 99)

add_executable(smash commands.c signals.c smash.c job.c)