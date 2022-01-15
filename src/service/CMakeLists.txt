file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_executable(encrypter-service ${encrypter_SRCS} )

if (APPLE)
  target_link_libraries(encrypter-service
    PRIVATE nanolog encrypter-lib Boost::boost ${OPENSSL_LIBRARIES} z
  )
else()
  target_link_libraries(encrypter-service
    PRIVATE nanolog encrypter-lib Boost::boost crypto ssl z
  )
endif()

install(TARGETS encrypter-service DESTINATION bin)
