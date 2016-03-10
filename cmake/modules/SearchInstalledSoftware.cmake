
#================================
#==   Check for OMNIORB       ===
#================================
MESSAGE(STATUS "Looking for omniORB")
if(NOT DEFINED ENV{OMNI_ROOT})
	MESSAGE(SEND_ERROR "OMNI_ROOT variable not defined!")
endif()

SET (OMNI_ROOT $ENV{OMNI_ROOT})
MESSAGE(STATUS "OMNI_ROOT: ${OMNI_ROOT}")

FIND_PATH (OMNIORB_INCLUDE_DIR
	NAMES omniconfig.h
  HINTS
  ${OMNI_ROOT}/include
)

FIND_LIBRARY (OMNIORB_LIB1 NAMES omniORB4 HINTS ${OMNI_ROOT}/lib)
FIND_LIBRARY (OMNIORB_LIB2 NAMES COS4 HINTS ${OMNI_ROOT}/lib)
FIND_LIBRARY (OMNIORB_LIB3 NAMES omniDynamic4 HINTS ${OMNI_ROOT}/lib)
FIND_LIBRARY (OMNIORB_LIB4 NAMES omnithread HINTS ${OMNI_ROOT}/lib)
list(APPEND OMNIORB_LIBRARIES ${OMNIORB_LIB1} ${OMNIORB_LIB2} ${OMNIORB_LIB3} ${OMNIORB_LIB4})

MARK_AS_ADVANCED (OMNIORB_INCLUDE_DIR OMNIORB_LIBRARIES)
MESSAGE(STATUS "OMNIORB_INCLUDE_DIR: ${OMNIORB_INCLUDE_DIR}")
MESSAGE(STATUS "OMNIORB_LIBRARIES: ${OMNIORB_LIBRARIES}")


#================================
#==   Check for ZMQ           ===
#================================
MESSAGE(STATUS "Looking for ZMQ")
if(NOT DEFINED ENV{ZMQ_ROOT})
	MESSAGE(SEND_ERROR "ZMQ_ROOT variable not defined!")
endif()

SET (ZMQ_ROOT $ENV{ZMQ_ROOT})
MESSAGE(STATUS "ZMQ_ROOT: ${ZMQ_ROOT}")

FIND_PATH (ZMQ_INCLUDE_DIR
	NAMES zmq.h
  HINTS
  ${ZMQ_ROOT}/include
)

FIND_LIBRARY (ZMQ_LIBRARIES NAMES zmq HINTS ${ZMQ_ROOT}/lib)

MARK_AS_ADVANCED (ZMQ_INCLUDE_DIR ZMQ_LIBRARIES)
MESSAGE(STATUS "ZMQ_INCLUDE_DIR: ${ZMQ_INCLUDE_DIR}")
MESSAGE(STATUS "ZMQ_LIBRARIES: ${ZMQ_LIBRARIES}")


#==================================
#==   Check for TANGO           ===
#==================================
MESSAGE(STATUS "Looking for TANGO")
if(NOT DEFINED ENV{TANGO_ROOT})
	MESSAGE(SEND_ERROR "TANGO_ROOT variable not defined!")
endif()

SET (TANGO_ROOT $ENV{TANGO_ROOT})
MESSAGE(STATUS "TANGO_ROOT: ${TANGO_ROOT}")

FIND_PATH (TANGO_INCLUDE_DIR
	NAMES tango.h
  HINTS
  ${TANGO_ROOT}/include/tango
)

FIND_LIBRARY (TANGO_LIB1 NAMES tango HINTS ${TANGO_ROOT}/lib)
FIND_LIBRARY (TANGO_LIB2 NAMES log4tango HINTS ${TANGO_ROOT}/lib)
list(APPEND TANGO_LIBRARIES ${TANGO_LIB1} ${TANGO_LIB2})

MARK_AS_ADVANCED (TANGO_INCLUDE_DIR TANGO_LIBRARIES)
MESSAGE(STATUS "TANGO_INCLUDE_DIR: ${TANGO_INCLUDE_DIR}")
MESSAGE(STATUS "TANGO_LIBRARIES: ${TANGO_LIBRARIES}")


#==================================
#==   Check for JSONCPP         ===
#==================================
MESSAGE(STATUS "Looking for JSONCPP")
#FIND_PACKAGE(Jsoncpp REQUIRED)

if(NOT DEFINED ENV{JSONCPP_ROOT})
	MESSAGE(SEND_ERROR "JSONCPP_ROOT variable not defined!")
endif()

SET (JSONCPP_ROOT $ENV{JSONCPP_ROOT})
MESSAGE(STATUS "JSONCPP_ROOT: ${JSONCPP_ROOT}")

FIND_PATH (JSONCPP_INCLUDE_DIR
	NAMES json/json.h
  HINTS
 ${JSONCPP_ROOT}/include
)

FIND_LIBRARY (JSONCPP_LIBRARIES NAMES jsoncpp HINTS ${JSONCPP_ROOT}/lib)

MARK_AS_ADVANCED (JSONCPP_INCLUDE_DIR JSONCPP_LIBRARIES)
MESSAGE(STATUS "JSONCPP_INCLUDE_DIR: ${JSONCPP_INCLUDE_DIR}")
MESSAGE(STATUS "JSONCPP_LIBRARIES: ${JSONCPP_LIBRARIES}")

#==================================
#==   Check for PUGIXML         ===
#==================================
MESSAGE(STATUS "Looking for PUGIXML")
FIND_PACKAGE(PugiXML REQUIRED)


#if(NOT DEFINED ENV{PUGIXML_ROOT})
#	MESSAGE(SEND_ERROR "PUGIXML_ROOT variable not defined!")
#endif()

#SET (JSONCPP_ROOT $ENV{PUGIXML_ROOT})
#MESSAGE(STATUS "PUGIXML_ROOT: ${PUGIXML_ROOT}")

#FIND_PATH (PUGIXML_INCLUDE_DIR
#	NAMES json/json.h
#  HINTS
#  ${PUGIXML_ROOT}/include
#)

#FIND_LIBRARY (PUGIXML_LIBRARIES NAMES pugixml HINTS ${PUGIXML_ROOT}/lib)

#MARK_AS_ADVANCED (PUGIXML_INCLUDE_DIR PUGIXML_LIBRARIES)
MESSAGE(STATUS "PUGIXML_INCLUDE_DIR: ${PUGIXML_INCLUDE_DIR}")
MESSAGE(STATUS "PUGIXML_LIBRARIES: ${PUGIXML_LIBRARIES}")




