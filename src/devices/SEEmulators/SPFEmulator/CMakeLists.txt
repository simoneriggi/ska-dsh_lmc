## INC & SOURCE DIRS
set(SPFEmulator_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SPFEmulator_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "SPFEmulator_INC_DIR: ${SPFEmulator_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${SPFEmulator_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${SPFEmulator_SRC_DIR}/SPFEmulatorClass.cpp 
  ${SPFEmulator_SRC_DIR}/SPFEmulator.cpp 
  ${SPFEmulator_SRC_DIR}/SPFEmulatorStateMachine.cpp 
  ${SPFEmulator_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${SPFEmulator_INC_DIR}/*.h)

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
list (APPEND SPFEmulator_SRCS ${THIS_SRCS})
list (APPEND SPFEmulator_HEADERS ${THIS_HEADERS})
list (APPEND SPFEmulator_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_SPFEmulator_SRCS ${SPFEmulator_SRC_DIR}/main.cpp)

set (SPFEmulator_SRCS ${SPFEmulator_SRCS} PARENT_SCOPE)
set (SPFEmulator_HEADERS ${SPFEmulator_HEADERS} PARENT_SCOPE)
set (SPFEmulator_HEADERS_WPATH ${SPFEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SPFEmulator_SRCS ${Main_SPFEmulator_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

