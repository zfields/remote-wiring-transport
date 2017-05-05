# Serial Wiring

Serial Wiring is designed to support the Remote Wiring library, and defines the `Stream` interface. The `Stream` interface is a compatible subset of the [Arduino Stream](https://www.arduino.cc/en/Reference/Stream) interface. Serial Wiring also provides the serial implementations for USB, Bluetooth\*, Bluetooth LE\* and Network\* serial streams under the `serial_wiring` namespace.

_\*Denotes serial channels not yet available._

### Serial Wiring API \[v0.2.0\]:

###### Data types

```cpp
typedef void(*serial_event_t)(void * context_);
```

###### Stream Interface

```cpp
size_t Stream::available (void);
  void Stream::begin (void);
  void Stream::end (void);
  void Stream::flush (void);
   int Stream::read (void);
  void Stream::registerSerialEventCallback (serialEvent upon_bytes_available, void * context = nullptr);
  void Stream::write (uint8_t byte);
```

> See also: [Arduino Stream](https://www.arduino.cc/en/Reference/Stream)

## Quick Start Guide

### Prerequisites:

* **CMake ** `v2.8.2`_ _\(_minimum version_\)
* **Git ** `v1.9.1`_ _\(_minimum version_\)

### Install Instructions:

```bash
$ git clone https://github.com/remote-wiring/serial-wiring.git
$ cd serial-wiring/
$ mkdir build
$ cd build/
$ cmake ..
$ make
```

### Run the sample:

1. Run `UartEcho` sample with no parameters to print out usage instructions.
   ```bash
   $ ./samples/UartEcho
   *****************************************************
   ** The "Examples > 04.Communication > SerialEvent" **
   ** sketch must be deployed to the Arduino in order **
   ** for the sample to work correctly.               **
   *****************************************************
   Usage: ./samples/UartEcho <serial device descriptor>
   ```
2. Install SerialEvent.ino on an Arduino clone.
3. Take note of the_** Tools &gt; Port**_ setting, because this is the `<serial device descriptor>` required by `UartEcho`.
4. Run `UartEcho` \(_linux sample shown below_\).
   ```bash
   $ ./samples/UartEcho /dev/ttyACM0
   ```

###### Expected Behavior

You will be prompted with a cursor, and whatever you type will be echoed back to you by the Arduino clone.

### Example Code:

```cpp
/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <serial_wiring>

void onSerialEvent (void * context_) {
    // Cast the context into a Stream object
    Stream * s = reinterpret_cast<Stream *>(context_);

    while(s->available())
    {
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

    usb.begin(9600);  // The sample Arduino sketch is expecting a baud rate of 9600
    usb.registerSerialEventCallback(::onSerialEvent, &usb);

    // Allow the Arduino time to boot-up before sending serial data.
    ::sleep(1);

    // Prompt for user input
    ::printf("Type a message to send to the Arduino, then press <Enter>: ");

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
```

### Code Breakdown:

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
```

These lines are good ol'  standard C libraries, \(with the exception of the Unix only `unistd.h`\). They give us the ability to pause the program, so our slow human reflexes can witness the programs execution on the hardware as well as through console logging.

```cpp
#include <serial_wiring>
```

These lines pull in the Serial Wiring libraries. As you can see in the example, Serial Wiring allows us to communicate with the remote device over a serial connection via an Arduino `Stream` compatible interface.

```cpp
void onSerialEvent (void * context_) {
    // Cast the context into a Stream object
    Stream * s = reinterpret_cast<Stream *>(context_);
```

The signature of `onSerialEvent` adheres to the serial event callback type specified by the Serial Wiring library. The `context` variable is provided by the user during the call to `registerSerialEventCallback`, and is a way for state to be persisted through stateless callbacks. Here a `Stream` object is passed through the `context`, enabling bytes to be consumed by a specific `Stream` instantiation.

```cpp
    while(s->available())
    {
        ::printf("%c", (char)s->read());
        ::fflush(stdout);
    }
}
```

The `while` loop simply iterates through the characters available in the buffer and prints them on the terminal.

```cpp
int main (int argc, char * argv[]) {
    ::printf("*****************************************************\n");
    ::printf("** The \"Examples > 04.Communication > SerialEvent\" **\n");
    ::printf("** sketch must be deployed to the Arduino in order **\n");
    ::printf("** for the sample to work correctly.               **\n");
    ::printf("*****************************************************\n");

    if ( argc < 2 ) { ::printf("Usage: %s <serial device descriptor>\n", argv[0]); return -1; }
```

`main()` is the default entrypoint for any C/C++ program. The parameters `argc` and `argv` describe an array of user specified command line parameters. The next lines generate a banner reminding the user to deploy `SerialEvent.ino` on the target device, which is necessary to echo the bytes sent by `Stream` object.

```cpp
    serial_wiring::UartSerial usb(argv[1]);
```

`UartSerial` is the physical communication channel with the remote device. It adheres to the `Stream` interface and provides a well-defined serial connection. Here `argv[1]` is passed as the construction parameter to the `UartSerial` object. It is expected the user provided the path to the serial device descriptor as a parameter on the command line \(i.e. `/dev/cu.usbmodem1411` on macOS\).

```cpp
    usb.begin(9600);  // The sample Arduino sketch is expecting a baud rate of 9600
```

`Stream::begin` starts the UART connection at 9600 baud, which is the baud rate specified in `SerialEvent.ino`.

```cpp
    usb.registerSerialEventCallback(::onSerialEvent, &usb);
```

`registerSerialEventCallback` schedules a callback to be invoked when serial data becomes available in the hardware buffer. We provide the static callback `onSerialEvent` to be called when serial data becomes available, and we pass the context `&usb` to be available to the callback.

```cpp
    // Allow the Arduino time to boot-up before sending serial data.
    ::sleep(1);
```

By default Arduinos go through a hard reset when they receive a new serial connection. Therefore we must allow time for the boot-up sequence to occur before attempting to send serial data to the device.

```cpp
    // Prompt for user input
    ::printf("Type a message to send to the Arduino, then press <Enter>: ");
```

A message is displayed on the terminal to prompt the user to enter an input string.

```cpp
    // Capture user input
    char * user_input_buffer = NULL;
    size_t user_input_buffer_length = 0;
    ::getline(&user_input_buffer, &user_input_buffer_length, stdin);
    user_input_buffer_length = ::strnlen(user_input_buffer, user_input_buffer_length);
```

`getline` will capture a user input string, allocate the memory for that string and store its pointer in `user_input_buffer`. `getline` will alsostore the size of the buffer \(not the string\) in `user_input_buffer_length`. Then `strnlen` is used to determine the size of the actual string sitting in the buffer.

```cpp
    // Send the user input to the Arduino sketch to echo back
    for (size_t i = 0 ; i < user_input_buffer_length ; ++i) {
        usb.write(user_input_buffer[i]);
    }
    ::free(user_input_buffer);
```

The `for` loop will iterate through each character of the user input string and send them on the `Stream`. The following line frees the user specified string allocated previously by `getline`.

```cpp
    // Give the Arduino time to send the string back over the serial line
    ::printf("\nArduino says...\n");
    ::sleep(1);
```

A header is printed for the Arduino's response. The `sleep` allows time for the asychronous response to comeback via the `onSerialEvent` callback.

```cpp
    usb.end();
    return 0;
}
```

`Stream::end` will close the communication channel and return those resources to the operating system. Finally, `return 0;` exits the program cleanly and returns control to the terminal.

### References:

* [Serial - Wiring.org.co](http://wiring.org.co/reference/Serial.html)
* [Serial - Arduino.cc](https://www.arduino.cc/en/Reference/Serial)
* [Serial - Particle.io](https://docs.particle.io/reference/firmware/photon/#serial)

### Project Organization:

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

### Software License:

This project uses the _**MIT license**_, please consult the license file for details.

