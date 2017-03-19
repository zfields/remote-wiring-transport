add_executable(UartEcho UartEcho.cpp)
target_link_libraries(UartEcho LINK_PUBLIC serial_wiring pthread)
