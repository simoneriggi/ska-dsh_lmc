## INC & SOURCE DIRS
set(DSEmulator_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DSEmulator_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "DSEmulator_INC_DIR: ${DSEmulator_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${DSEmulator_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${DSEmulator_SRC_DIR}/DSEmulatorClass.cpp 
  ${DSEmulator_SRC_DIR}/DSEmulator.cpp 
  ${DSEmulator_SRC_DIR}/DSEmulatorStateMachine.cpp 
  ${DSEmulator_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${DSEmulator_INC_DIR}/*.h)

set(THIS_HEADERS_WPATH "")
foreach (_headerFile ${THIS_HEADERS})
	get_filename_component(_basefilename ${_headerFile} NAME)
  list (APPEND THIS_HEADERS_WPATH ${_basefilename})
endforeach()
list(REMOVE_DUPLICATES THIS_HEADERS_WPATH)


MESSAGE(STATUS "THIS_SRCS: ${THIS_SRCS}")
MESSAGE(STATUS "THIS_HEADERS: ${THIS_HEADERS}")
MESSAGE(STATUS "THIS_HEADERS_WPATH: ${THIS_HEADERS_WPATH}")

## Append srcs and heads to project srcs and heads
list (APPEND DSEmulator_SRCS ${THIS_SRCS})
list (APPEND DSEmulator_HEADERS ${THIS_HEADERS})
list (APPEND DSEmulator_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_DSEmulator_SRCS ${DSEmulator_SRC_DIR}/main.cpp)

set (DSEmulator_SRCS ${DSEmulator_SRCS} PARENT_SCOPE)
set (DSEmulator_HEADERS ${DSEmulator_HEADERS} PARENT_SCOPE)
set (DSEmulator_HEADERS_WPATH ${DSEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_DSEmulator_SRCS ${Main_DSEmulator_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

