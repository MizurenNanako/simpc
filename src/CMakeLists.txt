# Add source files
add_executable(SimpC
    source/main.cc
    source/lexer.cc
)

# Link any external libraries from thirdparty directory
# add_subdirectory(thirdparty) # Uncomment and configure if needed
# target_link_libraries(SimpC PUBLIC <external_library_name>) # Example

# Link any libraries if needed
# target_link_libraries(SimpC PRIVATE <library_name>) # Example

# This is why we need cmake 3.16 and higher.
target_precompile_headers(SimpC PRIVATE header/stdafx.hh)

# Formatting, FORMAT MUST DO.
add_custom_target(format
    COMMAND clang-format -i -style=file ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/header/*.hh
    COMMENT "If you don't have a clang-format, I will doubt your tidiness."
)

add_custom_target(lexer-gen
    COMMAND re2c --case-ranges --loop-switch ${CMAKE_CURRENT_SOURCE_DIR}/source/rules.txt -o ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.ccpart --api custom
    COMMAND re2c ${CMAKE_CURRENT_SOURCE_DIR}/source/rules.txt -o ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.debug.dot -D
    COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.debug.dot | dot -Tpng -o ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.debug.png
)