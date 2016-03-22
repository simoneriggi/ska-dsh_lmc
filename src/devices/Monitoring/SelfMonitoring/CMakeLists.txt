## INC & SOURCE DIRS
set(SelfMonitoring_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SelfMonitoring_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "SelfMonitoring_INC_DIR: ${SelfMonitoring_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${SelfMonitoring_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${SelfMonitoring_SRC_DIR}/SelfMonitoringClass.cpp 
  ${SelfMonitoring_SRC_DIR}/SelfMonitoring.cpp 
  ${SelfMonitoring_SRC_DIR}/SelfMonitoringStateMachine.cpp 
  ${SelfMonitoring_SRC_DIR}/ClassFactory.cpp
	${SelfMonitoring_SRC_DIR}/NagiosListenerThread.cpp 
)

file(GLOB_RECURSE THIS_HEADERS ${SelfMonitoring_INC_DIR}/*.h)

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
list (APPEND SelfMonitoring_SRCS ${THIS_SRCS})
list (APPEND SelfMonitoring_HEADERS ${THIS_HEADERS})
list (APPEND SelfMonitoring_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_SelfMonitoring_SRCS ${SelfMonitoring_SRC_DIR}/main.cpp)

set (SelfMonitoring_SRCS ${SelfMonitoring_SRCS} PARENT_SCOPE)
set (SelfMonitoring_HEADERS ${SelfMonitoring_HEADERS} PARENT_SCOPE)
set (SelfMonitoring_HEADERS_WPATH ${SelfMonitoring_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SelfMonitoring_SRCS ${Main_SelfMonitoring_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

