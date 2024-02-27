# Add test files
add_executable(test_lexer test_lexer.cc)
add_executable(test_parser test_parser.cc)
add_executable(test_tree test_tree.cc)

# Link with GoogleTest
find_package(GTest REQUIRED)
target_link_libraries(test_lexer PRIVATE GTest::GTest GTest::Main)
target_link_libraries(test_parser PRIVATE GTest::GTest GTest::Main)
target_link_libraries(test_tree PRIVATE GTest::GTest GTest::Main)

# Add tests
add_test(NAME TestLexer COMMAND test_lexer)
add_test(NAME TestParser COMMAND test_parser)
add_test(NAME TestTree COMMAND test_tree)
