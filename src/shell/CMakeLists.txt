file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_executable(encrypter-shell ${encrypter_SRCS} )

if (APPLE)
  target_link_libraries(encrypter-shell PRIVATE lib api readline z)
else()
  target_link_libraries(encrypter-shell PRIVATE lib api pthread readline z)
endif()

set_target_properties(encrypter-shell PROPERTIES OUTPUT_NAME encryptsh)
install(TARGETS encrypter-shell DESTINATION bin)
