
PROJECT("IoU")

cmake_minimum_required (VERSION 3.0)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)


SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}    -fPIC ")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC ")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV 3.0 QUIET COMPONENTS core highgui imgproc)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET COMPONENTS core highgui imgproc)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

if(OpenCV_FOUND)
	# include path
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/test)

	# file(GLOB SRCS *.cpp) search required src and store as SRCS
	file(GLOB_RECURSE SRCS src/*.cpp test/*.cpp)

	# add execute file
	ADD_EXECUTABLE(demo ${SRCS})
	target_link_libraries(demo ${OpenCV_LIBS})
endif()












