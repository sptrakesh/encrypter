Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.7
)

FetchContent_MakeAvailable(Catch2)

FILE(GLOB_RECURSE unittest_SRCS *.cpp *.hpp *.h)
set(unittest_SRCS ${unittest_SRCS})
add_executable(unittest ${unittest_SRCS})

target_link_libraries(unittest PRIVATE lib Catch2::Catch2 ${OPENSSL_LIBRARIES} z)

add_test(NAME unittest COMMAND unittest)
