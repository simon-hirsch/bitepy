cmake_minimum_required(VERSION 3.15)
project(bite LANGUAGES CXX)

# --- Find Python and pybind11 ---
# Request both the interpreter and development components.
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 REQUIRED)

# --- Set your desired C++ standard and compile options ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# --- Add your library/executable, in this case a Python extension module ---
# We'll call the library "_bite" so that scikit-build produces _bite.<ext> 
pybind11_add_module(_bite
    bite/_bite.cpp
    bite/cpp-source-submodule/bite/src/Simulation.cpp
    bite/cpp-source-submodule/bite/src/GeneralStageProblem.cpp
    bite/cpp-source-submodule/bite/src/LimitOrderQueue.cpp
    bite/cpp-source-submodule/bite/src/LimitOrder.cpp
    bite/cpp-source-submodule/bite/src/MarketOrder.cpp
    bite/cpp-source-submodule/bite/src/LogLimitOrder.cpp
    bite/cpp-source-submodule/bite/src/ExecMarketOrder.cpp
    bite/cpp-source-submodule/bite/src/LogAcceptedOrder.cpp
    bite/cpp-source-submodule/bite/src/OrderBook.cpp
    bite/cpp-source-submodule/bite/src/ForeLogOrder.cpp
    bite/cpp-source-submodule/bite/src/SimulationParameters.cpp
    bite/cpp-source-submodule/bite/src/Util.cpp
    bite/cpp-source-submodule/bite/src/MILPStats.cpp
    bite/cpp-source-submodule/bite/src/BalancingOrder.cpp
)

set_target_properties(_bite
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# OLD
# --- Include directories ---
# target_include_directories(_bite
#     PRIVATE
#         ${PROJECT_SOURCE_DIR}/_bite/include
#         /opt/homebrew/include
#         /Library/gurobi1200/macos_universal2/include
# )

# target_link_directories(_bite
#     PRIVATE
#         /Library/gurobi1200/macos_universal2/lib
# )

# NEW
# # Attempt to find Gurobi includes
# find_path(GUROBI_INCLUDE_DIR
#     NAMES gurobi_c++.h
#     HINTS
#         /Library/gurobi1200/macos_universal2/include
#         # You can add more guess paths or rely on the user's environment
# )

# # Attempt to find Gurobi libraries
# find_library(GUROBI_LIB
#     NAMES gurobi_c++
#     HINTS
#         /Library/gurobi1200/macos_universal2/lib
#         # Again, you can add more guess paths here
# )

# # If either isn't found, you can issue an error or warning
# if (NOT GUROBI_INCLUDE_DIR OR NOT GUROBI_LIB)
#     message(FATAL_ERROR "Could NOT find Gurobi. Please specify GUROBI_INCLUDE_DIR and GUROBI_LIB.")
# endif()

# Then add them to your target:
target_include_directories(_bite PRIVATE
    "${PROJECT_SOURCE_DIR}/bite/cpp-source-submodule/bite/src"
    #"${GUROBI_INCLUDE_DIR}"
)

#target_link_libraries(_bite PRIVATE "${GUROBI_LIB}")

target_link_libraries(_bite
    PRIVATE
        pybind11::module  # pybind11
        # gurobi_c++
        # gurobi120
)

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
        /arch:AVX2  # Example; adjust based on your needs
    )
    target_link_options(_bite PRIVATE
        /LTCG
    )
endif()

# Tell CMake to name the target _bite.<so/pyd>: (does not work)
# set_target_properties(_bite PROPERTIES
#     PREFIX "${Python3_MODULE_PREFIX}"
#     SUFFIX "${Python3_MODULE_EXTENSION}"
# )

install(TARGETS _bite
        COMPONENT python
        LIBRARY DESTINATION bite  # or '.'
)