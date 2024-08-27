set(Target_Name "api")
file(GLOB_RECURSE encrypter_SRCS *.cpp *.h)
add_library(${Target_Name} STATIC ${encrypter_SRCS})
add_library(encrypter::api ALIAS ${Target_Name})
set_target_properties(${Target_Name} PROPERTIES OUTPUT_NAME encrypterapi)
target_compile_definitions(${Target_Name} PRIVATE _FORTIFY_SOURCE=2)
target_include_directories(${Target_Name}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${Target_Name} INTERFACE Boost::boost)
install(TARGETS ${Target_Name} EXPORT EncrypterApiTargets DESTINATION lib)
install(TARGETS ${Target_Name} DESTINATION lib)
install (
  DIRECTORY ${CMAKE_SOURCE_DIR}/src/api
  DESTINATION include/encrypter
  FILES_MATCHING PATTERN "*.h*")
install(EXPORT EncrypterApiTargets
    FILE EncrypterApiTargets.cmake
    NAMESPACE encrypter::
    DESTINATION lib/cmake/Encrypter
)
