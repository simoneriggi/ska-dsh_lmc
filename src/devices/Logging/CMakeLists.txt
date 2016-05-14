add_subdirectory(LogConsumer)
add_subdirectory(Logger)

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


set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

