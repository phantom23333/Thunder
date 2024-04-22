# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set (PROJECT_NAME "Thunder")

project (${PROJECT_NAME} 
	LANGUAGES CXX
	VERSION 0.0.1)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

# glfw settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# add library
add_subdirectory(thirdparty)

file(GLOB_RECURSE SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${HEADER_DIR} "include/platform/assertion.h")

# resource path
target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
# compile version
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/include/platform
	${CMAKE_CURRENT_SOURCE_DIR}/include/gameLayer
)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glm glfw glad stb_image stb_truetype gl2d raudio imgui safeSave profilerLib glui)

