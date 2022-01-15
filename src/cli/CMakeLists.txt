file(GLOB_RECURSE encrypter_SRCS *.cpp *.cc *.hpp *.h)
set(encrypter_SRCS ${encrypter_SRCS})
add_executable(encrypter ${encrypter_SRCS} )

if (APPLE)
  target_link_libraries(encrypter
    PRIVATE nanolog encrypter-lib /opt/local/lib/libssl.a /opt/local/lib/libcrypto.a /opt/local/lib/libz.a
  )
else()
  target_link_libraries(encrypter
    PRIVATE nanolog encrypter-lib ${OPENSSL_LIBRARIES} z
  )
endif()

install(TARGETS encrypter DESTINATION bin)
