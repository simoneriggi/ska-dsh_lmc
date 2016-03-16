add_subdirectory(SEEmulatorBase)
add_subdirectory(RxEmulator)
add_subdirectory(SPFEmulator)
add_subdirectory(DSEmulator)
add_subdirectory(TMEmulator)

# SEEmulatorBase
set (SEEmulatorBase_SRCS ${SEEmulatorBase_SRCS} PARENT_SCOPE)
set (SEEmulatorBase_HEADERS ${SEEmulatorBase_HEADERS} PARENT_SCOPE)
set (SEEmulatorBase_HEADERS_WPATH ${SEEmulatorBase_HEADERS_WPATH} PARENT_SCOPE)
set (Main_SEEmulatorBase_SRCS ${Main_SEEmulatorBase_SRCS} PARENT_SCOPE)

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

# RxEmulator
set (RxEmulator_SRCS ${RxEmulator_SRCS} PARENT_SCOPE)
set (RxEmulator_HEADERS ${RxEmulator_HEADERS} PARENT_SCOPE)
set (RxEmulator_HEADERS_WPATH ${RxEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_RxEmulator_SRCS ${Main_RxEmulator_SRCS} PARENT_SCOPE)

# TMEmulator
set (TMEmulator_SRCS ${TMEmulator_SRCS} PARENT_SCOPE)
set (TMEmulator_HEADERS ${TMEmulator_HEADERS} PARENT_SCOPE)
set (TMEmulator_HEADERS_WPATH ${TMEmulator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_TMEmulator_SRCS ${Main_TMEmulator_SRCS} PARENT_SCOPE)


set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

