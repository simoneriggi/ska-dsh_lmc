## INC & SOURCE DIRS
set(BaseDevice_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BaseDevice_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "BaseDevice_INC_DIR: ${BaseDevice_INC_DIR}")

## GET SOURCES and HEADERS
#file(GLOB_RECURSE THIS_SRCS ${BaseDevice_SRC_DIR}/*.cpp)
set(THIS_SRCS ${THIS_SRCS} 
  ${BaseDevice_SRC_DIR}/BaseDeviceClass.cpp 
  ${BaseDevice_SRC_DIR}/BaseDevice.cpp 
  ${BaseDevice_SRC_DIR}/BaseDeviceStateMachine.cpp 
  ${BaseDevice_SRC_DIR}/ClassFactory.cpp
	${BaseDevice_SRC_DIR}/BaseDeviceDynAttrUtils.cpp
	${BaseDevice_SRC_DIR}/ConfigManager.cpp 
)

file(GLOB_RECURSE THIS_HEADERS ${BaseDevice_INC_DIR}/*.h)

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
list (APPEND BaseDevice_SRCS ${THIS_SRCS})
list (APPEND BaseDevice_HEADERS ${THIS_HEADERS})
list (APPEND BaseDevice_HEADERS_WPATH ${THIS_HEADERS_WPATH})
list (APPEND Main_BaseDevice_SRCS ${BaseDevice_SRC_DIR}/main.cpp)

set (BaseDevice_SRCS ${BaseDevice_SRCS} PARENT_SCOPE)
set (BaseDevice_HEADERS ${BaseDevice_HEADERS} PARENT_SCOPE)
set (BaseDevice_HEADERS_WPATH ${BaseDevice_HEADERS_WPATH} PARENT_SCOPE)
set (Main_BaseDevice_SRCS ${Main_BaseDevice_SRCS} PARENT_SCOPE)

list (APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

