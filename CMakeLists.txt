cmake_minimum_required(VERSION 3.15)
project(bite LANGUAGES CXX)

find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Determine the correct precompiled simulation library ---
if(WIN32)
    if(Python3_VERSION_MAJOR EQUAL 3 AND Python3_VERSION_MINOR GREATER_EQUAL 8 AND Python3_VERSION_MINOR LESS_EQUAL 13)
        if(Python3_VERSION_MINOR EQUAL 8)
            set(CP_TAG "3.8")
        elseif(Python3_VERSION_MINOR EQUAL 9)
            set(CP_TAG "3.9")
        elseif(Python3_VERSION_MINOR EQUAL 10)
            set(CP_TAG "3.10")
        elseif(Python3_VERSION_MINOR EQUAL 11)
            set(CP_TAG "3.11")
        elseif(Python3_VERSION_MINOR EQUAL 12)
            set(CP_TAG "3.12")
        elseif(Python3_VERSION_MINOR EQUAL 13)
            set(CP_TAG "3.13")
        endif()
        set(SIM_LIB_PATH "${PROJECT_SOURCE_DIR}/libs/_bite_win_py${CP_TAG}.dll")
    else()
        message(FATAL_ERROR "Unsupported Python version on Windows")
    endif()
elseif(APPLE)
    if(Python3_VERSION_MAJOR EQUAL 3 AND Python3_VERSION_MINOR GREATER_EQUAL 8 AND Python3_VERSION_MINOR LESS_EQUAL 13)
        if(Python3_VERSION_MINOR EQUAL 8)
            set(CP_TAG "3.8")
        elseif(Python3_VERSION_MINOR EQUAL 9)
            set(CP_TAG "3.9")
        elseif(Python3_VERSION_MINOR EQUAL 10)
            set(CP_TAG "3.10")
        elseif(Python3_VERSION_MINOR EQUAL 11)
            set(CP_TAG "3.11")
        elseif(Python3_VERSION_MINOR EQUAL 12)
            set(CP_TAG "3.12")
        elseif(Python3_VERSION_MINOR EQUAL 13)
            set(CP_TAG "3.13")
        endif()
        set(SIM_LIB_PATH "${PROJECT_SOURCE_DIR}/libs/_bite_mac_py${CP_TAG}.dylib")
    else()
        message(FATAL_ERROR "Unsupported Python version on macOS")
    endif()
else()  # Assume Linux
    if(Python3_VERSION_MAJOR EQUAL 3 AND Python3_VERSION_MINOR GREATER_EQUAL 8 AND Python3_VERSION_MINOR LESS_EQUAL 13)
        if(Python3_VERSION_MINOR EQUAL 8)
            set(CP_TAG "3.8")
        elseif(Python3_VERSION_MINOR EQUAL 9)
            set(CP_TAG "3.9")
        elseif(Python3_VERSION_MINOR EQUAL 10)
            set(CP_TAG "3.10")
        elseif(Python3_VERSION_MINOR EQUAL 11)
            set(CP_TAG "3.11")
        elseif(Python3_VERSION_MINOR EQUAL 12)
            set(CP_TAG "3.12")
        elseif(Python3_VERSION_MINOR EQUAL 13)
            set(CP_TAG "3.13")
        endif()
        set(SIM_LIB_PATH "${PROJECT_SOURCE_DIR}/libs/_bite_linux_py${CP_TAG}.so")
    else()
        message(FATAL_ERROR "Unsupported Python version on Linux")
    endif()
endif()

# Declare an imported target for your precompiled simulation library.
add_library(bite_lib SHARED IMPORTED)
set_target_properties(bite_lib PROPERTIES
    IMPORTED_LOCATION "${SIM_LIB_PATH}"
)

# --- Build the Python wrapper ---
pybind11_add_module(_bite
    bite/_bite.cpp
)

set_target_properties(_bite
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(_bite PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

# Link the Python module with the precompiled simulation library.
target_link_libraries(_bite
    PRIVATE
        pybind11::module
        bite_lib
)

# --- Compiler-specific options ---
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(_bite PRIVATE
        -O3
        -march=native
        -funroll-loops
        -flto
        -std=c++17
    )
    target_link_options(_bite PRIVATE
        -flto
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(_bite PRIVATE
        /O2
        /GL
        /arch:AVX2 
    )
    target_link_options(_bite PRIVATE
        /LTCG
    )
endif()

install(TARGETS _bite
        COMPONENT python
        LIBRARY DESTINATION bite
)