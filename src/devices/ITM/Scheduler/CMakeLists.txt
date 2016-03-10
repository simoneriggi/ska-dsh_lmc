## INC & SOURCE DIRS
set(Scheduler_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(Scheduler_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "Scheduler_INC_DIR: ${Scheduler_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${LMCInterface_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${Scheduler_SRC_DIR}/SchedulerClass.cpp 
  ${Scheduler_SRC_DIR}/Scheduler.cpp 
  ${Scheduler_SRC_DIR}/SchedulerStateMachine.cpp 
  ${Scheduler_SRC_DIR}/ClassFactory.cpp
  ${Scheduler_SRC_DIR}/SchedulerThread.cpp
  ${Scheduler_SRC_DIR}/TaskManager.cpp
  ${Scheduler_SRC_DIR}/TaskCallBack.cpp
)

file(GLOB_RECURSE THIS_HEADERS ${Scheduler_INC_DIR}/*.h)

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
list (APPEND Scheduler_SRCS ${THIS_SRCS})
list (APPEND Scheduler_HEADERS ${THIS_HEADERS})
list (APPEND Scheduler_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_Scheduler_SRCS ${Scheduler_SRC_DIR}/main.cpp)

set (Scheduler_SRCS ${Scheduler_SRCS} PARENT_SCOPE)
set (Scheduler_HEADERS ${Scheduler_HEADERS} PARENT_SCOPE)
set (Scheduler_HEADERS_WPATH ${Scheduler_HEADERS_WPATH} PARENT_SCOPE)
set (Main_Scheduler_SRCS ${Main_Scheduler_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

