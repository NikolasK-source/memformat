#
# Copyright (C) 2022 Nikolas Koesling <nikolas@koesling.info>.
# This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
#

add_executable(test_${Target} test_main.cpp)

enable_testing()
add_test(NAME test_${Target}  COMMAND test_${Target})

target_link_libraries(test_${Target} ${Target})

# add clang format target
if(CLANG_FORMAT)
    set(CLANG_FORMAT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format)

    if(EXISTS ${CLANG_FORMAT_FILE})
        target_clangformat_setup(test_${Target})
        message(STATUS "Added clang format test target(s)")
    else()
        message(STATUS "no clang format file")
    endif()
else()
    message(STATUS "clang format disabled for test targets")
endif()
