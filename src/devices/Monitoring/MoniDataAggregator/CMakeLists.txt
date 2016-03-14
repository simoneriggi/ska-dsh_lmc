## INC & SOURCE DIRS
set(MoniDataAggregator_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MoniDataAggregator_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "MoniDataAggregator_INC_DIR: ${MoniDataAggregator_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${MoniDataAggregator_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${MoniDataAggregator_SRC_DIR}/MoniDataAggregatorClass.cpp 
  ${MoniDataAggregator_SRC_DIR}/MoniDataAggregator.cpp 
  ${MoniDataAggregator_SRC_DIR}/MoniDataAggregatorStateMachine.cpp 
  ${MoniDataAggregator_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${MoniDataAggregator_INC_DIR}/*.h)

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
list (APPEND MoniDataAggregator_SRCS ${THIS_SRCS})
list (APPEND MoniDataAggregator_HEADERS ${THIS_HEADERS})
list (APPEND MoniDataAggregator_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_MoniDataAggregator_SRCS ${MoniDataAggregator_SRC_DIR}/main.cpp)

set (MoniDataAggregator_SRCS ${MoniDataAggregator_SRCS} PARENT_SCOPE)
set (MoniDataAggregator_HEADERS ${MoniDataAggregator_HEADERS} PARENT_SCOPE)
set (MoniDataAggregator_HEADERS_WPATH ${MoniDataAggregator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_MoniDataAggregator_SRCS ${Main_MoniDataAggregator_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

