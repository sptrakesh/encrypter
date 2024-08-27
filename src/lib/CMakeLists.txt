set(Target_Name "lib")
file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_library(${Target_Name} STATIC ${encrypter_SRCS})
add_library(encrypter::lib ALIAS ${Target_Name})
set_target_properties(${Target_Name} PROPERTIES OUTPUT_NAME encrypter)
target_compile_definitions(${Target_Name} PRIVATE _FORTIFY_SOURCE=2)
target_include_directories(${Target_Name}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
if (UNIX)
  if (APPLE)
    target_link_libraries(${Target_Name} INTERFACE nanolog ${OPENSSL_LIBRARIES} ${JEMALLOC_LIBRARY})
  else (APPLE)
    target_link_libraries(${Target_Name} INTERFACE nanolog crypto ssl jemalloc)
  endif (APPLE)
else (UNIX)
  target_link_libraries(${Target_Name} INTERFACE nanolog OpenSSL::SSL OpenSSL::Crypto)
endif (UNIX)
install(TARGETS ${Target_Name} EXPORT EncrypterLibTargets DESTINATION lib)
install(TARGETS ${Target_Name} DESTINATION lib)
install (
  DIRECTORY ${CMAKE_SOURCE_DIR}/src/lib
  DESTINATION include/encrypter
  FILES_MATCHING PATTERN "*.h*")
install(EXPORT EncrypterLibTargets
  FILE EncrypterLibTargets.cmake
  NAMESPACE encrypter::
  DESTINATION lib/cmake/Encrypter
)
