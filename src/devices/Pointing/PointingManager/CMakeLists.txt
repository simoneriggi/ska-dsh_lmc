## INC & SOURCE DIRS
set(PointingMgr_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PointingMgr_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "PointingMgr_INC_DIR: ${PointingMgr_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${PointingMgr_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${PointingMgr_SRC_DIR}/PointingMgrClass.cpp 
  ${PointingMgr_SRC_DIR}/PointingMgr.cpp 
  ${PointingMgr_SRC_DIR}/PointingMgrStateMachine.cpp 
  ${PointingMgr_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${PointingMgr_INC_DIR}/*.h)

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
list (APPEND PointingMgr_SRCS ${THIS_SRCS})
list (APPEND PointingMgr_HEADERS ${THIS_HEADERS})
list (APPEND PointingMgr_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_PointingMgr_SRCS ${PointingMgr_SRC_DIR}/main.cpp)

set (PointingMgr_SRCS ${PointingMgr_SRCS} PARENT_SCOPE)
set (PointingMgr_HEADERS ${PointingMgr_HEADERS} PARENT_SCOPE)
set (PointingMgr_HEADERS_WPATH ${PointingMgr_HEADERS_WPATH} PARENT_SCOPE)
set (Main_PointingMgr_SRCS ${Main_PointingMgr_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

