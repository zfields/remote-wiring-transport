/*
UartSerial Echo Sample
======================

This sample demonstrates the serialEvent callback ability of serial wiring
as well as basic transmit and receive. It is meant to be run with an
Arduino attached to your machine via USB cable.

The Arduino is programmed with the SerialEvent sample sketch which reads
characters in on the serialport until a newline is encountered. Once the
newline is received it transmits the entire string back out over serial.
That transmission triggers the serial_event() callback in the code below.
*/

#include <serial_wiring>

#include <string.h>
#include <unistd.h>

#include <iostream>

void onSerialEvent (void *context_) {
    // Cast the context into a Stream object
    Stream * s = reinterpret_cast<Stream *>(context_);

    while(s->available())
    {
        std::cout << (char)s->read() << std::flush;
    }
}

int main (int argc, char * argv[]) {
    std::cout << "*****************************************************" << std::endl;
    std::cout << "** The \"Examples > 04.Communication > SerialEvent\" **" << std::endl;
    std::cout << "** sketch must be deployed to the Arduino in order **" << std::endl;
    std::cout << "** for the sample to work correctly.               **" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }

    serial_wiring::UartSerial usb(argv[1]);

    usb.begin(9600);  // The sample Arduino sketch is expecting a baud rate of 9600
    usb.registerSerialEventCallback(onSerialEvent, &usb);

    // By default Arduinos go through a hard reset when they receive a new
    // serial connection. The following sleep() is to allow it time to
    // boot up before sending serial data.
    ::sleep(2);

    // Get user input
    char * user_input_buffer = NULL;
    size_t user_input_buffer_length = 0;
    std::cout << "Type a message to send to the Arduino, then press <Enter>: ";
    getline(&user_input_buffer, &user_input_buffer_length, stdin);
    std::cout << std::endl;
    user_input_buffer_length = ::strnlen(user_input_buffer, user_input_buffer_length);

    // Send the user input to the Arduino sketch to echo back
    for (size_t i = 0 ; i < user_input_buffer_length ; ++i) {
        usb.write(user_input_buffer[i]);
    }
    free(user_input_buffer);

    std::cout << "Arduino says..." << std::endl;

    // Give the Arduino time to send the string back over the serial line
    sleep(1);

    usb.end();
    return 0;
}
