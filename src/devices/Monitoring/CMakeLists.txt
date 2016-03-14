add_subdirectory(MoniDataAggregator)
add_subdirectory(LogConsumer)

# MoniDataAggregator
set (MoniDataAggregator_SRCS ${MoniDataAggregator_SRCS} PARENT_SCOPE)
set (MoniDataAggregator_HEADERS ${MoniDataAggregator_HEADERS} PARENT_SCOPE)
set (MoniDataAggregator_HEADERS_WPATH ${MoniDataAggregator_HEADERS_WPATH} PARENT_SCOPE)
set (Main_MoniDataAggregator_SRCS ${Main_MoniDataAggregator_SRCS} PARENT_SCOPE)

# LogConsumer
set (LogConsumer_SRCS ${LogConsumer_SRCS} PARENT_SCOPE)
set (LogConsumer_HEADERS ${LogConsumer_HEADERS} PARENT_SCOPE)
set (LogConsumer_HEADERS_WPATH ${LogConsumer_HEADERS_WPATH} PARENT_SCOPE)
set (Main_LogConsumer_SRCS ${Main_LogConsumer_SRCS} PARENT_SCOPE)


set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

