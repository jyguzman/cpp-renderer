# CMakeList.txt : CMake project for renderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("renderer")

add_executable (renderer "src/main.cpp"  "src/math.cpp" "src/geometry.cpp"
"src/math.hpp" "src/renderer.cpp" "src/renderer.hpp")

# Define SDL3 path
set(SDL3_DIR "C:/SDL3")
set(SDL3_IMAGE_DIR "C:/SDL3_image")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL3_DIR}/include
    ${SDL3_IMAGE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL3_DIR}/lib/x64/SDL3.lib
    ${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.lib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_DIR}/lib/x64/SDL3.dll"
    "${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET renderer PROPERTY CXX_STANDARD 20)
endif()
