file(GLOB_RECURSE encrypter_SRCS *.cpp *.cc *.hpp *.h)
set(encrypter_SRCS ${encrypter_SRCS})
add_executable(encrypter ${encrypter_SRCS} )

if (APPLE)
  target_link_libraries(encrypter PRIVATE lib api OpenSSL::SSL OpenSSL::Crypto)
else()
  target_link_libraries(encrypter PRIVATE lib api ${OPENSSL_LIBRARIES} pthread z)
endif()

install(TARGETS encrypter DESTINATION bin)
