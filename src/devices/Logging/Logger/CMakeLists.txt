## INC & SOURCE DIRS
set(Logger_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(Logger_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "Logger_INC_DIR: ${Logger_INC_DIR}")

## GET SOURCES and HEADERS
set(THIS_SRCS ${THIS_SRCS} 
  ${Logger_SRC_DIR}/LMCLoggerClass.cpp 
  ${Logger_SRC_DIR}/LMCLogger.cpp 
  ${Logger_SRC_DIR}/LMCLoggerStateMachine.cpp 
  ${Logger_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${Logger_INC_DIR}/*.h)

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
list (APPEND Logger_SRCS ${THIS_SRCS})
list (APPEND Logger_HEADERS ${THIS_HEADERS})
list (APPEND Logger_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_Logger_SRCS ${Logger_SRC_DIR}/main.cpp)

set (Logger_SRCS ${Logger_SRCS} PARENT_SCOPE)
set (Logger_HEADERS ${Logger_HEADERS} PARENT_SCOPE)
set (Logger_HEADERS_WPATH ${Logger_HEADERS_WPATH} PARENT_SCOPE)
set (Main_Logger_SRCS ${Main_Logger_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

