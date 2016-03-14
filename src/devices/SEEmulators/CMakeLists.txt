add_subdirectory(SEEmulatorBase)
add_subdirectory(RxEmulator)

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


set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

