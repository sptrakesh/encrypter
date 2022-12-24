file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_executable(encrypter-service ${encrypter_SRCS} )

if (APPLE)
  target_link_libraries(encrypter-service
    PRIVATE nanolog encrypter-lib Boost::boost ${OPENSSL_LIBRARIES} jemalloc z
  )
else()
  target_link_libraries(encrypter-service
    PRIVATE nanolog encrypter-lib Boost::boost crypto ssl pthread jemalloc z
  )
endif()

install(TARGETS encrypter-service DESTINATION bin)
