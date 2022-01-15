file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_library(encrypter-lib STATIC ${encrypter_SRCS})
set_target_properties(encrypter-lib PROPERTIES OUTPUT_NAME encrypter)
install(TARGETS encrypter-lib DESTINATION lib)
install (
  DIRECTORY ${CMAKE_SOURCE_DIR}/src/lib
  DESTINATION include/encrypter
  FILES_MATCHING PATTERN "*.h*")
