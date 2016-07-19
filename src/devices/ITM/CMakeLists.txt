add_subdirectory(LMCInterface)
add_subdirectory(Scheduler)

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

set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

