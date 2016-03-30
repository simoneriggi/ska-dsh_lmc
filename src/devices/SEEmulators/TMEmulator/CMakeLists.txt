## INC & SOURCE DIRS
set(TMEmulator_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TMEmulator_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "TMEmulator_INC_DIR: ${TMEmulator_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${TMEmulator_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${TMEmulator_SRC_DIR}/TMEmulatorClass.cpp 
  ${TMEmulator_SRC_DIR}/TMEmulator.cpp 
  ${TMEmulator_SRC_DIR}/TMEmulatorStateMachine.cpp 
  ${TMEmulator_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${TMEmulator_INC_DIR}/*.h)

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
list (APPEND TMEmulator_SRCS ${THIS_SRCS})
list (APPEND TMEmulator_HEADERS ${THIS_HEADERS})
list (APPEND TMEmulator_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_TMEmulator_SRCS ${TMEmulator_SRC_DIR}/main.cpp)

set (TMEmulator_SRCS ${TMEmulator_SRCS} PARENT_SCOPE)
set (TMEmulator_HEADERS ${TMEmulator_HEADERS} PARENT_SCOPE)
set (TMEmulator_HEADERS_WPATH ${TMEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_TMEmulator_SRCS ${Main_TMEmulator_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

