cmake_minimum_required(VERSION 3.10)
project(Wall_E_CoppeliaSim)

IF(CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -Wall -std=c++0x  -DBUILD_SHARED_LIBS=OFF")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(remoteApiDef WIN32)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(remoteApiDef __APPLE__)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(remoteApiDef __linux)
endif()

add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)
add_definitions(-DDO_NOT_USE_SHARED_MEMORY)
add_definitions(-D${remoteApiDef})


set(SOURCES
    remote_api/extApi.c
    remote_api/extApiPlatform.c
)

set(HEADERS
include/extApi.h
include/extApiInternal.h
include/shared_memory.h
include/simConst.h
include/simLib.h
include/simTypes.h
include/extApiPlatform.h
)

include_directories("include/")
include_directories("remote_api/")
include_directories("src/")

find_package(Threads)

add_executable(${PROJECT_NAME}
                line_follow.cpp
                ${SOURCES}
                ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} "C:/Windows/System32/winmm.dll")
    target_link_libraries(${PROJECT_NAME} "C:/Windows/System32/ws2_32.dll")
endif()



function(print)
    foreach(var ${ARGN})
        message("${var} : ${${var}}")
    endforeach()
    message("----------------------------------")
    message("Defining ${remoteApiDef}")
    message("To build this project")
    message("type mingw32-make in the terminal")
    message("Then Run the executable generated")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        message("By running ${PROJECT_NAME}.exe on the terminal")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
         message("By running ./${PROJECT_NAME} on the terminal")
    endif()
    message("----------------------------------")
endfunction()

print(PROJECT_NAME PROJECT_SOURCE_DIR PROJECT_BINARY_DIR)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
