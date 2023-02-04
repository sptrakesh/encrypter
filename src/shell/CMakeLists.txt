file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_executable(encrypter-shell ${encrypter_SRCS} )

if (APPLE)
  target_link_libraries(encrypter-shell
    PRIVATE nanolog encrypter-lib encrypter-api OpenSSL::SSL OpenSSL::Crypto readline z
  )
else()
  target_link_libraries(encrypter-shell
    PRIVATE nanolog encrypter-lib encrypter-api crypto ssl readline pthread z
  )
endif()

set_target_properties(encrypter-shell PROPERTIES OUTPUT_NAME encryptsh)
install(TARGETS encrypter-shell DESTINATION bin)
