add_subdirectory(MoniDataAggregator)
add_subdirectory(SelfMonitoring)

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


set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

