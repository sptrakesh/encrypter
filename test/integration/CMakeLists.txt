Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.7
)

FetchContent_MakeAvailable(Catch2)

FILE(GLOB_RECURSE integration_SRCS *.cpp *.hpp *.h)
set(integration_SRCS ${integration_SRCS})
add_executable(integration ${integration_SRCS})

target_link_libraries(integration PRIVATE lib api Catch2::Catch2 ${OPENSSL_LIBRARIES} z)

add_test(NAME integration COMMAND integration)
