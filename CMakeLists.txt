# Define a biblioteca como "interface", pois é só header
add_library(cpp-datastructures INTERFACE)

# Expõe o caminho dos headers via "include <list/linear_list.hpp>"
target_include_directories(cpp-datastructures INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
