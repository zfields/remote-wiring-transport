add_library(serial_wiring UartSerial.cpp)
install(
  TARGETS serial_wiring
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
