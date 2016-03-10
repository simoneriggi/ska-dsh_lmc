add_subdirectory(Core)
add_subdirectory(ITM)

# BaseDevice
set (BaseDevice_SRCS ${BaseDevice_SRCS} PARENT_SCOPE)
set (BaseDevice_HEADERS ${BaseDevice_HEADERS} PARENT_SCOPE)
set (BaseDevice_HEADERS_WPATH ${BaseDevice_HEADERS_WPATH} PARENT_SCOPE)
set (Main_BaseDevice_SRCS ${Main_BaseDevice_SRCS} PARENT_SCOPE)

# LMCInterface
set (LMCInterface_SRCS ${LMCInterface_SRCS} PARENT_SCOPE)
set (LMCInterface_HEADERS ${LMCInterface_HEADERS} PARENT_SCOPE)
set (LMCInterface_HEADERS_WPATH ${LMCInterface_HEADERS_WPATH} PARENT_SCOPE)
set (Main_LMCInterface_SRCS ${Main_LMCInterface_SRCS} PARENT_SCOPE)


set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

