## INC & SOURCE DIRS
set(LogConsumer_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LogConsumer_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "LogConsumer_INC_DIR: ${LogConsumer_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${LogConsumer_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${LogConsumer_SRC_DIR}/LogConsumerClass.cpp 
  ${LogConsumer_SRC_DIR}/LogConsumer.cpp 
  ${LogConsumer_SRC_DIR}/LogConsumerStateMachine.cpp 
  ${LogConsumer_SRC_DIR}/ClassFactory.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${LogConsumer_INC_DIR}/*.h)

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
list (APPEND LogConsumer_SRCS ${THIS_SRCS})
list (APPEND LogConsumer_HEADERS ${THIS_HEADERS})
list (APPEND LogConsumer_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_LogConsumer_SRCS ${LogConsumer_SRC_DIR}/main.cpp)

set (LogConsumer_SRCS ${LogConsumer_SRCS} PARENT_SCOPE)
set (LogConsumer_HEADERS ${LogConsumer_HEADERS} PARENT_SCOPE)
set (LogConsumer_HEADERS_WPATH ${LogConsumer_HEADERS_WPATH} PARENT_SCOPE)
set (Main_LogConsumer_SRCS ${Main_LogConsumer_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

