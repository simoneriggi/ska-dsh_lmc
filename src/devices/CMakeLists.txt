add_subdirectory(Core)
add_subdirectory(ITM)
add_subdirectory(Emulators)
add_subdirectory(Monitoring)
add_subdirectory(Logging)
add_subdirectory(Pointing)

####### CORE #########
# BaseDevice
set (BaseDevice_SRCS ${BaseDevice_SRCS} PARENT_SCOPE)
set (BaseDevice_HEADERS ${BaseDevice_HEADERS} PARENT_SCOPE)
set (BaseDevice_HEADERS_WPATH ${BaseDevice_HEADERS_WPATH} PARENT_SCOPE)
set (Main_BaseDevice_SRCS ${Main_BaseDevice_SRCS} PARENT_SCOPE)
#####################


###### ITM ###########
# LMCInterface
set (LMCInterface_SRCS ${LMCInterface_SRCS} PARENT_SCOPE)
set (LMCInterface_HEADERS ${LMCInterface_HEADERS} PARENT_SCOPE)
set (LMCInterface_HEADERS_WPATH ${LMCInterface_HEADERS_WPATH} PARENT_SCOPE)
set (Main_LMCInterface_SRCS ${Main_LMCInterface_SRCS} PARENT_SCOPE)

# Scheduler
set (Scheduler_SRCS ${Scheduler_SRCS} PARENT_SCOPE)
set (Scheduler_HEADERS ${Scheduler_HEADERS} PARENT_SCOPE)
set (Scheduler_HEADERS_WPATH ${Scheduler_HEADERS_WPATH} PARENT_SCOPE)
set (Main_Scheduler_SRCS ${Main_Scheduler_SRCS} PARENT_SCOPE)
######################


##### SE EMULATORS #########
# SEEmulatorBase
set (SEEmulatorBase_SRCS ${SEEmulatorBase_SRCS} PARENT_SCOPE)
set (SEEmulatorBase_HEADERS ${SEEmulatorBase_HEADERS} PARENT_SCOPE)
set (SEEmulatorBase_HEADERS_WPATH ${SEEmulatorBase_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SEEmulatorBase_SRCS ${Main_SEEmulatorBase_SRCS} PARENT_SCOPE)

# RxEmulator
set (RxEmulator_SRCS ${RxEmulator_SRCS} PARENT_SCOPE)
set (RxEmulator_HEADERS ${RxEmulator_HEADERS} PARENT_SCOPE)
set (RxEmulator_HEADERS_WPATH ${RxEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_RxEmulator_SRCS ${Main_RxEmulator_SRCS} PARENT_SCOPE)

# DSEmulator
set (DSEmulator_SRCS ${DSEmulator_SRCS} PARENT_SCOPE)
set (DSEmulator_HEADERS ${DSEmulator_HEADERS} PARENT_SCOPE)
set (DSEmulator_HEADERS_WPATH ${DSEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_DSEmulator_SRCS ${Main_DSEmulator_SRCS} PARENT_SCOPE)

# SPFEmulator
set (SPFEmulator_SRCS ${SPFEmulator_SRCS} PARENT_SCOPE)
set (SPFEmulator_HEADERS ${SPFEmulator_HEADERS} PARENT_SCOPE)
set (SPFEmulator_HEADERS_WPATH ${SPFEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SPFEmulator_SRCS ${Main_SPFEmulator_SRCS} PARENT_SCOPE)

# TMEmulator
set (TMEmulator_SRCS ${TMEmulator_SRCS} PARENT_SCOPE)
set (TMEmulator_HEADERS ${TMEmulator_HEADERS} PARENT_SCOPE)
set (TMEmulator_HEADERS_WPATH ${TMEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_TMEmulator_SRCS ${Main_TMEmulator_SRCS} PARENT_SCOPE)
############################


##### MONITORING #########
# MoniDataAggregator
set (MoniDataAggregator_SRCS ${MoniDataAggregator_SRCS} PARENT_SCOPE)
set (MoniDataAggregator_HEADERS ${MoniDataAggregator_HEADERS} PARENT_SCOPE)
set (MoniDataAggregator_HEADERS_WPATH ${MoniDataAggregator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_MoniDataAggregator_SRCS ${Main_MoniDataAggregator_SRCS} PARENT_SCOPE)

# SelfMonitoring
set (SelfMonitoring_SRCS ${SelfMonitoring_SRCS} PARENT_SCOPE)
set (SelfMonitoring_HEADERS ${SelfMonitoring_HEADERS} PARENT_SCOPE)
set (SelfMonitoring_HEADERS_WPATH ${SelfMonitoring_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SelfMonitoring_SRCS ${Main_SelfMonitoring_SRCS} PARENT_SCOPE)
###############################################

###### LOGGING ##########
# LogConsumer
set (LogConsumer_SRCS ${LogConsumer_SRCS} PARENT_SCOPE)
set (LogConsumer_HEADERS ${LogConsumer_HEADERS} PARENT_SCOPE)
set (LogConsumer_HEADERS_WPATH ${LogConsumer_HEADERS_WPATH} PARENT_SCOPE)
set (Main_LogConsumer_SRCS ${Main_LogConsumer_SRCS} PARENT_SCOPE)

# Logger
set (Logger_SRCS ${Logger_SRCS} PARENT_SCOPE)
set (Logger_HEADERS ${Logger_HEADERS} PARENT_SCOPE)
set (Logger_HEADERS_WPATH ${Logger_HEADERS_WPATH} PARENT_SCOPE)
set (Main_Logger_SRCS ${Main_Logger_SRCS} PARENT_SCOPE)
############################

####### POINTING #########
# PointingMgr
set (PointingMgr_SRCS ${PointingMgr_SRCS} PARENT_SCOPE)
set (PointingMgr_HEADERS ${PointingMgr_HEADERS} PARENT_SCOPE)
set (PointingMgr_HEADERS_WPATH ${PointingMgr_HEADERS_WPATH} PARENT_SCOPE)
set (Main_PointingMgr_SRCS ${Main_PointingMgr_SRCS} PARENT_SCOPE)
#####################

set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

