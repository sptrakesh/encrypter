file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_library(encrypter-api STATIC ${encrypter_SRCS})
set_target_properties(encrypter-api PROPERTIES OUTPUT_NAME encrypterapi)
install(TARGETS encrypter-api DESTINATION lib)
install (
  DIRECTORY ${CMAKE_SOURCE_DIR}/src/api
  DESTINATION include/encrypter
  FILES_MATCHING PATTERN "*.h*")
