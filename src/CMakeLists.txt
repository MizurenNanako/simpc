# Add source files
add_executable(SimpC
    source/main.cc
)

# Link any external libraries from thirdparty directory
# add_subdirectory(thirdparty) # Uncomment and configure if needed
# target_link_libraries(SimpC PUBLIC <external_library_name>) # Example

# Link any libraries if needed
# target_link_libraries(SimpC PRIVATE <library_name>) # Example

# Formatting, FORMAT MUST DO.
add_custom_command(TARGET SimpC PRE_BUILD
    COMMAND clang-format -i -style=file ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/source/*.hh
    COMMENT "If you don't have a clang-format, I will doubt your tidiness."
)