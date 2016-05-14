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
set(CMAKE_TEST_COMMAND ctest -VV)

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


# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(ENABLE_TEST "Build all tests." OFF) # Makes boolean 'test' available.

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
list (APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list (APPEND INCLUDE_DIRS ${LOG4CXX_INCLUDE_DIRS})
include_directories(${INCLUDE_DIRS})

# Append external libs found 
list (APPEND LINKED_LIBS ${OMNIORB_LIBRARIES})
list (APPEND LINKED_LIBS ${ZMQ_LIBRARIES})
list (APPEND LINKED_LIBS ${TANGO_LIBRARIES})
list (APPEND LINKED_LIBS ${JSONCPP_LIBRARIES})
list (APPEND LINKED_LIBS ${PUGIXML_LIBRARIES})
list (APPEND LINKED_LIBS ${Boost_LIBRARIES})
list (APPEND LINKED_LIBS ${LOG4CXX_LIBRARIES})
message (STATUS "LINKED_LIBS: ${LINKED_LIBS}")


set(DEVICE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/devices)
set(DEVICE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/devices)

#-- Base Device
#set(BaseDevice_DIR ${DEVICE_SRC_DIR}/Core/BaseDevice)
#set(BaseDevice_DOWNLOAD_DIR ${BaseDevice_DIR})
#set(BaseDevice_DOWNLOAD_DEST_DIR ${DEVICE_BUILD_DIR}/Core)
#set(BaseDevice_BUILD_DIR ${DEVICE_BUILD_DIR}/Core/BaseDevice)
#set(BaseDevice_EXE ${BaseDevice_BUILD_DIR}/bin/BaseDevice)

#-- LMCInterface
#set(LMCInterface_DIR ${DEVICE_SRC_DIR}/ITM/LMCInterface)
#set(LMCInterface_DOWNLOAD_DIR ${LMCInterface_DIR})
#set(LMCInterface_DOWNLOAD_DEST_DIR ${DEVICE_BUILD_DIR}/ITM)
#set(LMCInterface_BUILD_DIR ${DEVICE_BUILD_DIR}/ITM/LMCInterface)
#set(LMCInterface_EXE ${LMCInterface_BUILD_DIR}/bin/LMCInterface)


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
#--- UTILS LIBS ----
add_library(Utils SHARED ${UTILS_SRCS})
target_link_libraries(Utils PUBLIC ${LINKED_LIBS})

add_library(MessageParser SHARED ${MSGPARSER_SRCS})
add_dependencies(MessageParser Utils)
target_link_libraries(MessageParser PUBLIC ${LINKED_LIBS} Utils)

add_library(SDD SHARED ${SDD_SRCS})
add_dependencies(SDD Utils)
target_link_libraries(SDD PUBLIC ${LINKED_LIBS} Utils)
#---------------------


#--- CORE LIBS -----
add_library(BaseDevice SHARED ${BaseDevice_SRCS})
add_dependencies(BaseDevice Utils MessageParser SDD)
target_link_libraries(BaseDevice PUBLIC ${LINKED_LIBS} Utils MessageParser SDD)
#-------------------

#--- ITM LIBS -------
add_library(LMCInterface SHARED ${LMCInterface_SRCS})
add_dependencies(LMCInterface Utils MessageParser SDD BaseDevice)
target_link_libraries(LMCInterface PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)

add_library(Scheduler SHARED ${Scheduler_SRCS})
add_dependencies(Scheduler Utils MessageParser SDD BaseDevice)
target_link_libraries(Scheduler PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)
#--------------------

#--- SEEmulators LIBS --------
add_library(SEEmulatorBase SHARED ${SEEmulatorBase_SRCS})
add_dependencies(SEEmulatorBase Utils MessageParser SDD BaseDevice)
target_link_libraries(SEEmulatorBase PUBLIC ${LINKED_LIBS})

add_library(RxEmulator SHARED ${RxEmulator_SRCS})
add_dependencies(RxEmulator Utils MessageParser SDD SEEmulatorBase)
target_link_libraries(RxEmulator PUBLIC ${LINKED_LIBS} Utils MessageParser SDD SEEmulatorBase)

add_library(SPFEmulator SHARED ${SPFEmulator_SRCS})
add_dependencies(SPFEmulator Utils MessageParser SDD SEEmulatorBase)
target_link_libraries(SPFEmulator PUBLIC ${LINKED_LIBS} Utils MessageParser SDD SEEmulatorBase)

add_library(DSEmulator SHARED ${DSEmulator_SRCS})
add_dependencies(DSEmulator Utils MessageParser SDD SEEmulatorBase)
target_link_libraries(DSEmulator PUBLIC ${LINKED_LIBS} Utils MessageParser SDD SEEmulatorBase)

add_library(TMEmulator SHARED ${TMEmulator_SRCS})
add_dependencies(TMEmulator Utils MessageParser SDD BaseDevice)
target_link_libraries(TMEmulator PUBLIC ${LINKED_LIBS})
#-----------------------------

#--- MONITORING LIBS --------
add_library(MoniDataAggregator SHARED ${MoniDataAggregator_SRCS})
add_dependencies(MoniDataAggregator Utils MessageParser SDD BaseDevice)
target_link_libraries(MoniDataAggregator PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)

add_library(LogConsumer SHARED ${LogConsumer_SRCS})
##add_dependencies(LogConsumer Utils MessageParser SDD BaseDevice)
##target_link_libraries(LogConsumer PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)
add_dependencies(LogConsumer Utils MessageParser SDD BaseDevice Scheduler)
target_link_libraries(LogConsumer PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice Scheduler)

add_library(LMCLogger SHARED ${Logger_SRCS})
add_dependencies(LMCLogger Utils MessageParser SDD BaseDevice)
target_link_libraries(LMCLogger PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)


add_library(SelfMonitoring SHARED ${SelfMonitoring_SRCS})
add_dependencies(SelfMonitoring Utils MessageParser SDD BaseDevice)
target_link_libraries(SelfMonitoring PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)
#-----------------------------------------------------------------------------------------------


#--- POINTING LIBS --------
add_library(PointingMgr SHARED ${PointingMgr_SRCS})
add_dependencies(PointingMgr Utils MessageParser SDD BaseDevice)
target_link_libraries(PointingMgr PUBLIC ${LINKED_LIBS} Utils MessageParser SDD BaseDevice)


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

###### CORE #########
## BaseDevice
add_executable(BaseDeviceExe ${Main_BaseDevice_SRCS} ${BaseDevice_SRCS})
target_link_libraries(BaseDeviceExe Utils MessageParser SDD ${LINKED_LIBS})
set_target_properties(BaseDeviceExe PROPERTIES OUTPUT_NAME BaseDevice)
#####################

###### ITM ##########
## LMCInterface 
add_executable(LMCInterfaceExe ${Main_LMCInterface_SRCS} ${LMCInterface_SRCS})
target_link_libraries(LMCInterfaceExe Utils MessageParser SDD BaseDevice LMCInterface ${LINKED_LIBS})
set_target_properties(LMCInterfaceExe PROPERTIES OUTPUT_NAME LMCInterface)

## Scheduler
add_executable(SchedulerExe ${Main_Scheduler_SRCS} ${Scheduler_SRCS})
target_link_libraries(SchedulerExe Utils MessageParser SDD BaseDevice Scheduler ${LINKED_LIBS})
set_target_properties(SchedulerExe PROPERTIES OUTPUT_NAME Scheduler)
#####################

###### SEEmulators ######
## SEEmulatorBase 
add_executable(SEEmulatorBaseExe ${Main_SEEmulatorBase_SRCS} ${SEEmulatorBase_SRCS})
target_link_libraries(SEEmulatorBaseExe ${LINKED_LIBS})
set_target_properties(SEEmulatorBaseExe PROPERTIES OUTPUT_NAME SEEmulatorBase)

## RxEmulator
add_executable(RxEmulatorExe ${Main_RxEmulator_SRCS} ${RxEmulator_SRCS})
target_link_libraries(RxEmulatorExe Utils MessageParser SDD SEEmulatorBase RxEmulator ${LINKED_LIBS})
set_target_properties(RxEmulatorExe PROPERTIES OUTPUT_NAME RxEmulator)

## SPFEmulator
add_executable(SPFEmulatorExe ${Main_SPFEmulator_SRCS} ${SPFEmulator_SRCS})
target_link_libraries(SPFEmulatorExe Utils MessageParser SDD SEEmulatorBase SPFEmulator ${LINKED_LIBS})
set_target_properties(SPFEmulatorExe PROPERTIES OUTPUT_NAME SPFEmulator)

## DSEmulator
add_executable(DSEmulatorExe ${Main_DSEmulator_SRCS} ${DSEmulator_SRCS})
target_link_libraries(DSEmulatorExe Utils MessageParser SDD SEEmulatorBase DSEmulator ${LINKED_LIBS})
set_target_properties(DSEmulatorExe PROPERTIES OUTPUT_NAME DSEmulator)

## TMEmulator
add_executable(TMEmulatorExe ${Main_TMEmulator_SRCS} ${TMEmulator_SRCS})
target_link_libraries(TMEmulatorExe ${LINKED_LIBS})
set_target_properties(TMEmulatorExe PROPERTIES OUTPUT_NAME TMEmulator)
#########################


###### MONITORING ######
## MoniDataAggregator
add_executable(MoniDataAggregatorExe ${Main_MoniDataAggregator_SRCS} ${MoniDataAggregator_SRCS})
target_link_libraries(MoniDataAggregatorExe Utils MessageParser SDD BaseDevice MoniDataAggregator ${LINKED_LIBS})
set_target_properties(MoniDataAggregatorExe PROPERTIES OUTPUT_NAME MoniDataAggregator)

## LogConsumer
add_executable(LogConsumerExe ${Main_LogConsumer_SRCS} ${LogConsumer_SRCS})
##target_link_libraries(LogConsumerExe Utils MessageParser SDD BaseDevice LogConsumer ${LINKED_LIBS})
target_link_libraries(LogConsumerExe Utils MessageParser SDD BaseDevice Scheduler LogConsumer ${LINKED_LIBS})
set_target_properties(LogConsumerExe PROPERTIES OUTPUT_NAME LogConsumer)

## LMCLogger
add_executable(LMCLoggerExe ${Main_Logger_SRCS} ${Logger_SRCS})
set_target_properties(LMCLoggerExe PROPERTIES COMPILE_DEFINITIONS "APPENDERS_HAVE_LEVEL_THRESHOLD")
target_compile_definitions(LMCLoggerExe PRIVATE APPENDERS_HAVE_LEVEL_THRESHOLD=1)
target_link_libraries(LMCLoggerExe Utils MessageParser SDD BaseDevice LMCLogger ${LINKED_LIBS})
set_target_properties(LMCLoggerExe PROPERTIES OUTPUT_NAME LMCLogger)

## SelfMonitoring
add_executable(SelfMonitoringExe ${Main_SelfMonitoring_SRCS} ${SelfMonitoring_SRCS})
target_link_libraries(SelfMonitoringExe Utils MessageParser SDD BaseDevice SelfMonitoring ${LINKED_LIBS})
set_target_properties(SelfMonitoringExe PROPERTIES OUTPUT_NAME SelfMonitoring)

#########################

###### POINTING ##########
## PointingMgr
add_executable(PointingMgrExe ${Main_PointingMgr_SRCS} ${PointingMgr_SRCS})
target_link_libraries(PointingMgrExe Utils MessageParser SDD BaseDevice PointingMgr ${LINKED_LIBS})
set_target_properties(PointingMgrExe PROPERTIES OUTPUT_NAME PointingMgr)
#####################


################################
# Testing
################################
if (ENABLE_TEST)
	# Append subdirs (headers & sources files are appended in subdirs)
	set(TEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})
	add_subdirectory(tests)

	enable_testing()

  # Include test & Google Test headers
  include_directories(${TEST_INCLUDE_DIRS})

	# Append external libs found 
	list (APPEND LINKED_LIBS ${GTEST_BOTH_LIBRARIES})
	message (STATUS "LINKED_LIBS: ${LINKED_LIBS}")
	

  ##############
  # Unit Tests
  ##############	
	message (STATUS "SDDTEST_SRCS: ${SDDTEST_SRCS}")

	add_executable(runSDDTests ${SDDTEST_SRCS})
	add_dependencies(runSDDTests SDD)

  # Standard linking to gtest stuff.
	#target_link_libraries(runSDDTests ${LINKED_LIBS})
	target_link_libraries(runSDDTests PUBLIC ${LINKED_LIBS} SDD)


  # Extra linking for the project.
	#  target_link_libraries(runUnitTests project1_lib)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
	##add_test(NAME TestSDD COMMAND runSDDTests)
	add_test(TestSDD ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/runSDDTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
#  add_test(that-other-test-I-made runUnitTests)
endif()






#-----------------------------------------------------------------------------------------------
#---  INSTALL STUFF
#-----------------------------------------------------------------------------------------------
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
