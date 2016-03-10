## INC & SOURCE DIRS
set(LMCInterface_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LMCInterface_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "LMCInterface_INC_DIR: ${LMCInterface_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${LMCInterface_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${LMCInterface_SRC_DIR}/LMCInterfaceClass.cpp 
  ${LMCInterface_SRC_DIR}/LMCInterface.cpp 
  ${LMCInterface_SRC_DIR}/LMCInterfaceStateMachine.cpp 
  ${LMCInterface_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${LMCInterface_INC_DIR}/*.h)

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
list (APPEND LMCInterface_SRCS ${THIS_SRCS})
list (APPEND LMCInterface_HEADERS ${THIS_HEADERS})
list (APPEND LMCInterface_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_LMCInterface_SRCS ${LMCInterface_SRC_DIR}/main.cpp)

set (LMCInterface_SRCS ${LMCInterface_SRCS} PARENT_SCOPE)
set (LMCInterface_HEADERS ${LMCInterface_HEADERS} PARENT_SCOPE)
set (LMCInterface_HEADERS_WPATH ${LMCInterface_HEADERS_WPATH} PARENT_SCOPE)
set (Main_LMCInterface_SRCS ${Main_LMCInterface_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

