cmake_minimum_required(VERSION 3.21)

project(test)

if(MSVC)
    add_library(test_dll SHARED "main.cpp")
    set_target_properties(test_dll PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMMON_LANGUAGE_RUNTIME "")
    target_compile_options(test_dll PRIVATE /MP /doc)
else()
    add_compile_options(-march=native -Ofast -ggdb3 -Wall -Wextra)
    add_library(test_dll SHARED "main.cpp")
endif()
