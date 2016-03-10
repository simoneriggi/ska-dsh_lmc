#----------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
# ---------------------------------------------------------------
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "DSH_LMC requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

# -------------------------------------------
# CMAKE VERSIONS
# -------------------------------------------
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
cmake_policy(SET CMP0005 NEW)

#---------------------------------------------
# - Project definition
# --------------------------------------------
project(DSH_LMC)

#---Set pathes where to put the libraries, executables and headers------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
set(BINARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)

message (STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message (STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

set(DSHLMC ${CMAKE_BINARY_DIR})
set(HEADER_OUTPUT_PATH ${CMAKE_BINARY_DIR}/include)
set(DSHLMC_INCLUDE_DIR ${HEADER_OUTPUT_PATH})


# ----------------------------------------------------------------------------------------------
#---Where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked-------------
# ----------------------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

# - Versioning. We do this here for now
set(${PROJECT_NAME}_VERSION "1.1.0")
set(${PROJECT_NAME}_VERSION_MAJOR "1")
set(${PROJECT_NAME}_VERSION_MINOR "1")
set(${PROJECT_NAME}_VERSION_PATCH "0")

# ----------------------------------------------------------------------------------------------
#---Load some basic macros which are needed later for the configuration and build----------------
#-----------------------------------------------------------------------------------------------
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT -Dlinux -std=c++11 -O2 -Wall -Wunused -Wuninitialized -fPIC -pthread -m64")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT -Dlinux -std=c++0x -O2 -Wall -Wunused -Wuninitialized -fPIC -pthread -m64")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#-----------------------------------------------------------------------------------------------
#---Here we look for installed software and switch on and of the different build options--------
include(SearchInstalledSoftware)
#-----------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------
#---  SET INCLUDES & LIBS
#-----------------------------------------------------------------------------------------------
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(LINKED_LIBS "")

# Append subdirs (headers & sources files are appended in subdirs)
add_subdirectory(src)

# Append external includes found 
list (APPEND INCLUDE_DIRS ${OMNIORB_INCLUDE_DIR})
list (APPEND INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})
list (APPEND INCLUDE_DIRS ${TANGO_INCLUDE_DIR})
list (APPEND INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR})
list (APPEND INCLUDE_DIRS ${PUGIXML_INCLUDE_DIR})
include_directories(${INCLUDE_DIRS})

# Append external libs found 
list (APPEND LINKED_LIBS ${OMNIORB_LIBRARIES})
list (APPEND LINKED_LIBS ${ZMQ_LIBRARIES})
list (APPEND LINKED_LIBS ${TANGO_LIBRARIES})
list (APPEND LINKED_LIBS ${JSONCPP_LIBRARIES})
list (APPEND LINKED_LIBS ${PUGIXML_LIBRARIES})
message (STATUS "LINKED_LIBS: ${LINKED_LIBS}")


set(DEVICE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/devices)
set(DEVICE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/devices)

#-- Base Device
set(BaseDevice_DIR ${DEVICE_SRC_DIR}/Core/BaseDevice)
set(BaseDevice_DOWNLOAD_DIR ${BaseDevice_DIR})
set(BaseDevice_DOWNLOAD_DEST_DIR ${DEVICE_BUILD_DIR}/Core)
set(BaseDevice_BUILD_DIR ${DEVICE_BUILD_DIR}/Core/BaseDevice)
set(BaseDevice_EXE ${BaseDevice_BUILD_DIR}/bin/BaseDevice)
#message(STATUS "BaseDevice_DIR: ${BaseDevice_DIR}")
#message(STATUS "BaseDevice_DOWNLOAD_DIR: ${BaseDevice_DOWNLOAD_DIR}")
#message(STATUS "BaseDevice_DOWNLOAD_DEST_DIR: ${BaseDevice_DOWNLOAD_DEST_DIR}")
#message(STATUS "BaseDevice_BUILD_DIR: ${BaseDevice_BUILD_DIR}")
#message(STATUS "BaseDevice_EXE: ${BaseDevice_EXE}")

#-- LMCInterface
set(LMCInterface_DIR ${DEVICE_SRC_DIR}/ITM/LMCInterface)
set(LMCInterface_DOWNLOAD_DIR ${LMCInterface_DIR})
set(LMCInterface_DOWNLOAD_DEST_DIR ${DEVICE_BUILD_DIR}/ITM)
set(LMCInterface_BUILD_DIR ${DEVICE_BUILD_DIR}/ITM/LMCInterface)
set(LMCInterface_EXE ${LMCInterface_BUILD_DIR}/bin/LMCInterface)


#-----------------------------------------------------------------------------------------------
#---  CLEAN STUFF
#-----------------------------------------------------------------------------------------------
#--- Set external WCTOOLS lib to be removed by clean command
#--- Set copied headers to be removed by clean command
set(ADDITIONAL_CLEAN_TARGETS ${BINARY_INSTALL_DIR}/BaseDevice)
list(APPEND ADDITIONAL_CLEAN_TARGETS ${BINARY_INSTALL_DIR}/LMCInterface)

set(CLEANED_HEADERS "")
foreach (_headerFile ${UTILS_HEADERS_WPATH} ${MSGPARSER_HEADERS_WPATH} ${SDD_HEADERS_WPATH})
	set(_cleanedHeader ${CMAKE_INSTALL_INCLUDEDIR}/${_headerFile})
	list(APPEND CLEANED_HEADERS ${_cleanedHeader})
endforeach()

list(REMOVE_DUPLICATES CLEANED_HEADERS)
list(APPEND ADDITIONAL_CLEAN_TARGETS ${CLEANED_HEADERS})
#list(APPEND ADDITIONAL_CLEAN_TARGETS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/CaesarDict_rdict.pcm)

message(STATUS "ADDITIONAL_CLEAN_TARGETS: ${ADDITIONAL_CLEAN_TARGETS}")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ADDITIONAL_CLEAN_TARGETS}") ## do not forget to put "" in final vars otherwise you'll get a parsing error


#-----------------------------------------------------------------------------------------------
#---  BUILD PROJECT SHARED LIBS
#-----------------------------------------------------------------------------------------------
#--- Create Utils shared lib
add_library(Utils SHARED ${UTILS_SRCS})
target_link_libraries(Utils PUBLIC ${LINKED_LIBS})

#--- Create MessageParser shared lib
add_library(MessageParser SHARED ${MSGPARSER_SRCS})
add_dependencies(MessageParser Utils)
target_link_libraries(MessageParser PUBLIC ${LINKED_LIBS} Utils)

#--- Create SDD shared lib
add_library(SDD SHARED ${SDD_SRCS})
#add_dependencies(SDD Utils)
target_link_libraries(SDD PUBLIC ${LINKED_LIBS})

#--- Create BaseDevice shared lib
add_library(BaseDevice SHARED ${BaseDevice_SRCS})
target_link_libraries(BaseDevice PUBLIC ${LINKED_LIBS})

#--- Create LMCInterface shared lib
add_library(LMCInterface SHARED ${LMCInterface_SRCS})
add_dependencies(LMCInterface MessageParser Utils BaseDevice)
target_link_libraries(LMCInterface PUBLIC ${LINKED_LIBS} MessageParser Utils BaseDevice)


#-----------------------------------------------------------------------------------------------


#-----------------------------------------------------------------------------------------------
#---  BUILD DEVICE (EXTERNAL PROJS)
#-----------------------------------------------------------------------------------------------

#include(ExternalProject)

#ExternalProject_Add(BaseDevice
#	DOWNLOAD_COMMAND cp -rp ${BaseDevice_DOWNLOAD_DIR} ${BaseDevice_DOWNLOAD_DEST_DIR}
#	BINARY_DIR ${BaseDevice_BUILD_DIR}
#	PREFIX ${BaseDevice_BUILD_DIR}
#	BUILD_IN_SOURCE 0
#	CONFIGURE_COMMAND ""
#	BUILD_COMMAND make
#	INSTALL_COMMAND mkdir -p ${BINARY_INSTALL_DIR} COMMAND cp ${BaseDevice_EXE} ${BINARY_INSTALL_DIR}
#)

#ExternalProject_Add(LMCInterface
#	DOWNLOAD_COMMAND cp -rp ${LMCInterface_DOWNLOAD_DIR} ${LMCInterface_DOWNLOAD_DEST_DIR}
#	BINARY_DIR ${LMCInterface_BUILD_DIR}
#	PREFIX ${LMCInterface_BUILD_DIR}
#	BUILD_IN_SOURCE 0
#	CONFIGURE_COMMAND ""
#	BUILD_COMMAND make
#	INSTALL_COMMAND mkdir -p ${BINARY_INSTALL_DIR} COMMAND cp ${LMCInterface_EXE} ${BINARY_INSTALL_DIR}
#)

#-----------------------------------------------------------------------------------------------
#---  BUILD PROJECT TASKS EXEs
#-----------------------------------------------------------------------------------------------
## BaseDevice
add_executable(BaseDeviceExe ${Main_BaseDevice_SRCS} ${BaseDevice_SRCS})
target_link_libraries(BaseDeviceExe ${LINKED_LIBS})
set_target_properties(BaseDeviceExe PROPERTIES OUTPUT_NAME BaseDevice)

## LMCInterface 
add_executable(LMCInterfaceExe ${Main_LMCInterface_SRCS} ${LMCInterface_SRCS})
target_link_libraries(LMCInterfaceExe MessageParser Utils BaseDevice LMCInterface ${LINKED_LIBS})
set_target_properties(LMCInterfaceExe PROPERTIES OUTPUT_NAME LMCInterface)


#-----------------------------------------------------------------------------------------------
#---  INSTALL STUFF
#-----------------------------------------------------------------------------------------------
#-- INstall targets
#install(TARGETS FindSource FindBkg DESTINATION ${BINARY_INSTALL_DIR} COMPONENT binaries)
#install(TARGETS BaseDeviceExe DESTINATION ${BINARY_INSTALL_DIR} COMPONENT binaries)

# -- Install library dict map
install(FILES DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#-- Install project headers
install(FILES ${UTILS_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${MSGPARSER_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${SDD_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#-----------------------------------------------------------------------------------------------



###############################################################################
## packaging ##################################################################
###############################################################################

# all install commands get the same destination. this allows us to use paths
# relative to the executable.
#install(TARGETS example DESTINATION example_destination)
# this is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package
#install(DIRECTORY resources DESTINATION example_destination)

# now comse everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here
set(CPACK_PACKAGE_NAME "DSH_LMC")
set(CPACK_PACKAGE_VERSION "1.0.0")

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
