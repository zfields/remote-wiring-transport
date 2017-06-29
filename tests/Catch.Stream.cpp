/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <cerrno>

#include "Stream.h"

using namespace serial_wiring;

void
callback (
    void * context_
) {
    (void)context_;
    return;
}

class ConcreteStream : public Stream {
  public:
    static bool available_invoked;
    size_t available_result;
    bool begin_invoked;
    int begin_result;
    bool end_invoked;
    int end_result;
    bool flush_invoked;
    int flush_result;
    bool read_invoked;
    int read_result;
    bool registerSerialEventCallback_invoked;
    serial_event_t registerSerialEventCallback_uponBytesAvailable_arg;
    void * registerSerialEventCallback_context_arg;
    int registerSerialEventCallback_result;
    bool write_invoked;
    uint8_t write_byte_arg;
    int write_result;

    ConcreteStream (
        void
    ) :
        available_result(0),
        begin_invoked(false),
        begin_result(0),
        end_invoked(false),
        end_result(0),
        flush_invoked(false),
        flush_result(0),
        read_invoked(false),
        read_result(0),
        registerSerialEventCallback_invoked(false),
        registerSerialEventCallback_uponBytesAvailable_arg(nullptr),
        registerSerialEventCallback_context_arg(nullptr),
        registerSerialEventCallback_result(0),
        write_invoked(false),
        write_byte_arg(255),
        write_result(0)
    {}

  private:
    inline
    size_t
    _available (
        void
    ) const override {
        available_invoked = true;
        return available_result;
    }

    inline
    int
    _begin (
        void
    ) override {
        begin_invoked = true;
        return begin_result;
    }

    inline
    int
    _end (
        void
    ) override {
        end_invoked = true;
        return end_result;
    }

    inline
    int
    _flush (
        void
    ) override {
        flush_invoked = true;
        return flush_result;
    }

    inline
    int
    _read (
        void
    ) override {
        read_invoked = true;
        return read_result;
    }

    inline
    int
    _registerSerialEventCallback (
        serial_event_t uponBytesAvailable_,
        void * context_ = nullptr
    ) override {
        registerSerialEventCallback_invoked = true;
        registerSerialEventCallback_uponBytesAvailable_arg = uponBytesAvailable_;
        registerSerialEventCallback_context_arg = context_;
        return registerSerialEventCallback_result;
    }

    inline
    int
    _write (
        uint8_t byte_
    ) override {
        write_invoked = true;
        write_byte_arg = byte_;
        return write_result;
    }
};

bool ConcreteStream::available_invoked = false;

TEST_CASE("Stream::available - Invokes the underlying implementation", "[Stream::available]") {
    ConcreteStream stream;
    stream.available();
    REQUIRE( stream.available_invoked );
}

TEST_CASE("Stream::available - Returns the result of the underlying implementation", "[Stream::available]") {
    ConcreteStream stream;
    const size_t expected_result = 79;
    stream.available_result = expected_result;
    REQUIRE( expected_result == stream.available() );
}

TEST_CASE("Stream::begin - Invokes the underlying implementation", "[Stream::begin]") {
    ConcreteStream stream;
    stream.begin();
    REQUIRE( stream.begin_invoked );
}

TEST_CASE("Stream::begin - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Stream::begin]") {
    errno = 0;
    ConcreteStream stream;
    stream.begin_result = ENOTTY;
    stream.begin();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::begin - If no errors occur during execution, any previous `errno` shall be preserved", "[Stream::begin]") {
    errno = ENOTTY;
    ConcreteStream stream;
    stream.begin();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::end - Invokes the underlying implementation", "[Stream::end]") {
    ConcreteStream stream;
    stream.end();
    REQUIRE( stream.end_invoked );
}

TEST_CASE("Stream::end - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Stream::end]") {
    errno = 0;
    ConcreteStream stream;
    stream.end_result = ENOTTY;
    stream.end();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::end - If no errors occur during execution, any previous `errno` shall be preserved", "[Stream::end]") {
    errno = ENOTTY;
    ConcreteStream stream;
    stream.end();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::flush - Invokes the underlying implementation", "[Stream::flush]") {
    ConcreteStream stream;
    stream.flush();
    REQUIRE( stream.flush_invoked );
}

TEST_CASE("Stream::flush - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Stream::flush]") {
    errno = 0;
    ConcreteStream stream;
    stream.flush_result = ENOTTY;
    stream.flush();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::flush - If no errors occur during execution, any previous `errno` shall be preserved", "[Stream::flush]") {
    errno = ENOTTY;
    ConcreteStream stream;
    stream.flush();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::read - Invokes the underlying implementation", "[Stream::read]") {
    ConcreteStream stream;
    stream.read();
    REQUIRE( stream.read_invoked );
}

TEST_CASE("Stream::read - Returns the result of the underlying implementation", "[Stream::read]") {
    ConcreteStream stream;
    const int expected_result = 0x79;
    stream.read_result = expected_result;
    REQUIRE( expected_result == stream.read() );
}

TEST_CASE("Stream::read - A result returned from the underlying implementation not in the set {x: -1 <= x <= 255}, shall result in `ERANGE` error", "[Stream::read]") {
    errno = 0;
    ConcreteStream stream;
    stream.read_result = 0x1979;
    stream.read();
    REQUIRE( ERANGE == errno );
}

TEST_CASE("Stream::registerSerialEventCallback - Invokes the underlying implementation", "[Stream::registerSerialEventCallback]") {
    ConcreteStream stream;
    stream.registerSerialEventCallback(nullptr);
    REQUIRE( stream.registerSerialEventCallback_invoked );
}

TEST_CASE("Stream::registerSerialEventCallback - A pointer adhering to the `serial_event_t` signature shall be passed to the underlying implementation", "[Stream::registerSerialEventCallback]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteStream stream;
     * typedef void(Stream::*write_func_t)(uint8_t);
     * write_func_t write_func = &stream.write;
     */
}

TEST_CASE("Stream::registerSerialEventCallback - Supplies `uponBytesAvailable` argument to the underlying implementation", "[Stream::registerSerialEventCallback]") {
    ConcreteStream stream;
    serial_event_t expected_result = callback;
    stream.registerSerialEventCallback(expected_result);
    REQUIRE( expected_result == stream.registerSerialEventCallback_uponBytesAvailable_arg );
}

TEST_CASE("Stream::registerSerialEventCallback - Supplies `context` argument to the underlying implementation", "[Stream::registerSerialEventCallback]") {
    ConcreteStream stream;
    void * const context_provided = &stream;
    stream.registerSerialEventCallback(callback, context_provided);
    REQUIRE( context_provided == stream.registerSerialEventCallback_context_arg );
}

TEST_CASE("Stream::registerSerialEventCallback - If the `context` argument is unspecified, the default value shall be `nullptr`", "[Stream::registerSerialEventCallback]") {
    ConcreteStream stream;
    stream.registerSerialEventCallback(nullptr);
    REQUIRE( nullptr == stream.registerSerialEventCallback_context_arg );
}

TEST_CASE("Stream::registerSerialEventCallback - If `nullptr` is supplied to `uponBytesAvailable`, the `context` argument shall be discarded", "[Stream::registerSerialEventCallback]") {
    ConcreteStream stream;
    void * const context_provided = &stream;
    stream.registerSerialEventCallback(nullptr, context_provided);
    REQUIRE(context_provided != stream.registerSerialEventCallback_context_arg);
}

TEST_CASE("Stream::registerSerialEventCallback - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Stream::registerSerialEventCallback]") {
    errno = 0;
    ConcreteStream stream;
    stream.registerSerialEventCallback_result = ENOTTY;
    stream.registerSerialEventCallback(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::registerSerialEventCallback - If no errors occur during execution, any previous `errno` shall be preserved", "[Stream::registerSerialEventCallback]") {
    errno = ENOTTY;
    ConcreteStream stream;
    stream.registerSerialEventCallback(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::write - Invokes the underlying implementation", "[Stream::write]") {
    ConcreteStream stream;
    stream.write(0x00);
    REQUIRE( stream.write_invoked );
}

TEST_CASE("Stream::write - Supplies `byte` argument to the underlying implementation", "[Stream::write]") {
    ConcreteStream stream;
    uint8_t expected_result = 0x79;
    stream.write(expected_result);
    REQUIRE( expected_result == stream.write_byte_arg );
}

TEST_CASE("Stream::write - A `byte` argument not in the set {x: 0 <= x <= 255}, shall result in `EDOM` error", "[Stream::write]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteStream stream;
     * typedef void(Stream::*write_func_t)(uint8_t);
     * write_func_t write_func = &stream.write;
     */
}

TEST_CASE("Stream::write - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Stream::write]") {
    errno = 0;
    ConcreteStream stream;
    stream.write_result = ENOTTY;
    stream.write(0x00);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Stream::write - If no errors occur during execution, any previous `errno` shall be preserved", "[Stream::write]") {
    errno = ENOTTY;
    ConcreteStream stream;
    stream.write(0x00);
    REQUIRE( ENOTTY == errno );
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
