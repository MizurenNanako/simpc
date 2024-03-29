# Add source files
add_executable(SimpC
    source/main.cc
    source/lexer.cc
)
# add_dependencies(SimpC lexer-gen)

add_executable(test-lexer
test/test_lexer.cc
source/lexer.cc
)
add_dependencies(test-lexer lexer-gen)

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

add_custom_target(lexer-gen ALL=${CMAKE_CURRENT_SOURCE_DIR}/source/rules.txt
    COMMENT "Generate lexer.ccpart file and DFA graph from rules.txt with re2c."
    COMMAND re2c --case-ranges --loop-switch --api custom --no-generation-date --no-version -i ${CMAKE_CURRENT_SOURCE_DIR}/source/rules.txt -o ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.ccpart
    COMMAND sed '1,2d' ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.ccpart -i
    COMMAND clang-format -i -style=file ${CMAKE_CURRENT_SOURCE_DIR}/source/lexer.ccpart
)

add_custom_target(lexer-plot
    COMMAND re2c ${CMAKE_CURRENT_SOURCE_DIR}/source/rules.txt -o ${CMAKE_HOME_DIRECTORY}/document/lexer.debug.dot -D
    COMMAND sed -E -e 's\/\\[label=\\\".*?rules\\.txt:\/\\[label=\\\"rule-\/g' ${CMAKE_HOME_DIRECTORY}/document/lexer.debug.dot -i
    COMMAND cat ${CMAKE_HOME_DIRECTORY}/document/lexer.debug.dot | dot -Tpng -o ${CMAKE_HOME_DIRECTORY}/document/lexer.debug.png
)
