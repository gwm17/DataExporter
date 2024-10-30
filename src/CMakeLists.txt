add_executable(DataExporter)

target_include_directories(DataExporter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/.
    ${Ice_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/asio/asio/include
    ${DEX_GET_DIR}/include
    ${DEX_GET_DIR}
)

target_sources(DataExporter PRIVATE
    BadMacros.h
    DataRouter.h
    DataRouter.cpp
    Exporter.h
    Exporter.cpp
    server/ThreadSafeQueue.h
    server/ServerMessage.h
    server/TCPServer.h
    server/TCPServer.cpp
    main.cpp
)

target_link_directories(DataExporter PRIVATE
    ${DEX_GET_DIR}/lib
)

target_link_libraries(DataExporter PRIVATE
    ${Ice_LIBRARIES}
    ${Boost_LIBRARIES}
    getbench-daq
    mdaq-daq
    MultiFrame
    mdaq-utl
    Utilities
)

set_target_properties(DataExporter PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEX_BINARY_DIR})