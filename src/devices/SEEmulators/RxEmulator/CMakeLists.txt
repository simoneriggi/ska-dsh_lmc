## INC & SOURCE DIRS
set(RxEmulator_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(RxEmulator_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "RxEmulator_INC_DIR: ${RxEmulator_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${RxEmulator_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${RxEmulator_SRC_DIR}/RxEmulatorClass.cpp 
  ${RxEmulator_SRC_DIR}/RxEmulator.cpp 
  ${RxEmulator_SRC_DIR}/RxEmulatorStateMachine.cpp 
  ${RxEmulator_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${RxEmulator_INC_DIR}/*.h)

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
list (APPEND RxEmulator_SRCS ${THIS_SRCS})
list (APPEND RxEmulator_HEADERS ${THIS_HEADERS})
list (APPEND RxEmulator_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_RxEmulator_SRCS ${RxEmulator_SRC_DIR}/main.cpp)

set (RxEmulator_SRCS ${RxEmulator_SRCS} PARENT_SCOPE)
set (RxEmulator_HEADERS ${RxEmulator_HEADERS} PARENT_SCOPE)
set (RxEmulator_HEADERS_WPATH ${RxEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_RxEmulator_SRCS ${Main_RxEmulator_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

