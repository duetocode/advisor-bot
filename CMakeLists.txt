cmake_minimum_required(VERSION 3.0.0)
project(advisor_bot VERSION 0.1.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/advisor_bot)


# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
include(CTest)
enable_testing()

# The executable target
# Add all cpp files under advisor_bot to the target
file(GLOB APP_SRC
    "advisor_bot/*.cpp"
)

list(FILTER APP_SRC EXCLUDE REGEX ".*main.cpp")
add_executable(advisorbot ${APP_SRC} advisor_bot/main.cpp)
target_include_directories(advisorbot PUBLIC advisor_bot)

# Add the test executable
file(GLOB TEST_SRC "test/*.cpp")  

add_executable(unit_test ${TEST_SRC} ${APP_SRC})
target_link_libraries(unit_test gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)