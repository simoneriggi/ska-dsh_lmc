## INC & SOURCE DIRS
set(SEEmulatorBase_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SEEmulatorBase_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "SEEmulatorBase_INC_DIR: ${SEEmulatorBase_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${SEEmulatorBase_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${SEEmulatorBase_SRC_DIR}/SEEmulatorBaseClass.cpp 
  ${SEEmulatorBase_SRC_DIR}/SEEmulatorBase.cpp 
  ${SEEmulatorBase_SRC_DIR}/SEEmulatorBaseStateMachine.cpp 
  ${SEEmulatorBase_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${SEEmulatorBase_INC_DIR}/*.h)

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
list (APPEND SEEmulatorBase_SRCS ${THIS_SRCS})
list (APPEND SEEmulatorBase_HEADERS ${THIS_HEADERS})
list (APPEND SEEmulatorBase_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_SEEmulatorBase_SRCS ${SEEmulatorBase_SRC_DIR}/main.cpp)

set (SEEmulatorBase_SRCS ${SEEmulatorBase_SRCS} PARENT_SCOPE)
set (SEEmulatorBase_HEADERS ${SEEmulatorBase_HEADERS} PARENT_SCOPE)
set (SEEmulatorBase_HEADERS_WPATH ${SEEmulatorBase_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SEEmulatorBase_SRCS ${Main_SEEmulatorBase_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

