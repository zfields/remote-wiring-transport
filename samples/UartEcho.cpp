/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <serial_wiring>

void onSerialEvent (void * context_) {
    // Cast the context into a Stream object
    Stream * s = reinterpret_cast<Stream *>(context_);

    while (s->available()) {
        ::printf("%c", (char)s->read());
        ::fflush(stdout);
    }
}

int main (int argc, char * argv[]) {
    ::printf("*****************************************************\n");
    ::printf("** The \"Examples > 04.Communication > SerialEvent\" **\n");
    ::printf("** sketch must be deployed to the Arduino in order **\n");
    ::printf("** for the sample to work correctly.               **\n");
    ::printf("*****************************************************\n");

    if ( argc < 2 ) { ::printf("Usage: %s <serial device descriptor>\n", argv[0]); return -1; }

    serial_wiring::UartSerial usb(argv[1]);

    usb.begin(9600); // The sample Arduino sketch is expecting a baud rate of 9600
    usb.registerSerialEventCallback(::onSerialEvent, &usb);

    // Allow the Arduino time to boot-up before sending serial data.
    ::sleep(1);

    // Prompt for user input
    ::printf("Type a message to send to the Arduino, then press <Enter>: ");
    ::fflush(stdout);

    // Capture user input
    char * user_input_buffer = NULL;
    size_t user_input_buffer_length = 0;
    ::getline(&user_input_buffer, &user_input_buffer_length, stdin);
    user_input_buffer_length = ::strnlen(user_input_buffer, user_input_buffer_length);

    // Send the user input to the Arduino sketch to echo back
    for (size_t i = 0 ; i < user_input_buffer_length ; ++i) {
        usb.write(user_input_buffer[i]);
    }
    ::free(user_input_buffer);

    // Give the Arduino time to send the string back over the serial line
    ::printf("\nArduino says...\n");
    ::sleep(1);

    usb.end();
    return 0;
}

/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */

