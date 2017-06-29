/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <cerrno>

#include "ByteCache.h"

using namespace serial_wiring;

void
callback (
    void * context_
) {
    (void)context_;
    return;
}

class ConcreteByteCache : public ByteCache {
  public:
    bool cacheByte_invoked;
    uint8_t cacheByte_byte_arg;
    int cacheByte_result;
    bool readCachedByte_invoked;
    int readCachedByte_result;
    bool registerCachedBytesCallback_invoked;
    serial_event_t registerCachedBytesCallback_uponCachedBytes_arg;
    void * registerCachedBytesCallback_context_arg;
    int registerCachedBytesCallback_result;
    static bool size_invoked;
    size_t size_result;

    ConcreteByteCache (
        void
    ) :
        cacheByte_invoked(false),
        cacheByte_byte_arg(255),
        cacheByte_result(0),
        readCachedByte_invoked(false),
        readCachedByte_result(0),
        registerCachedBytesCallback_invoked(false),
        registerCachedBytesCallback_uponCachedBytes_arg(nullptr),
        registerCachedBytesCallback_context_arg(nullptr),
        registerCachedBytesCallback_result(0),
        size_result(0)
    {}

  private:
    inline
    int
    _cacheByte (
        uint8_t byte_
    ) override {
        cacheByte_invoked = true;
        cacheByte_byte_arg = byte_;
        return cacheByte_result;
    }

    inline
    int
    _readCachedByte (
        void
    ) override {
        readCachedByte_invoked = true;
        return readCachedByte_result;
    }

    inline
    int
    _registerCachedBytesCallback (
        serial_event_t uponCachedBytes_,
        void * context_ = nullptr
    ) override {
        registerCachedBytesCallback_invoked = true;
        registerCachedBytesCallback_uponCachedBytes_arg = uponCachedBytes_;
        registerCachedBytesCallback_context_arg = context_;
        return registerCachedBytesCallback_result;
    }

    inline
    size_t
    _size (
        void
    ) const override {
        size_invoked = true;
        return size_result;
    }
};

bool ConcreteByteCache::size_invoked = false;

TEST_CASE("ByteCache::cacheByte - Invokes the underlying implementation", "[ByteCache::cacheByte]") {
    ConcreteByteCache ByteCache;
    ByteCache.cacheByte(0x00);
    REQUIRE( ByteCache.cacheByte_invoked );
}

TEST_CASE("ByteCache::cacheByte - Supplies `byte` argument to the underlying implementation", "[ByteCache::cacheByte]") {
    ConcreteByteCache ByteCache;
    uint8_t expected_result = 0x79;
    ByteCache.cacheByte(expected_result);
    REQUIRE( expected_result == ByteCache.cacheByte_byte_arg );
}

TEST_CASE("ByteCache::cacheByte - A `byte` argument not in the set {x: 0 <= x <= 255}, shall result in `EDOM` error", "[ByteCache::cacheByte]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteByteCache ByteCache;
     * typedef void(ByteCache::*cacheByte_func_t)(uint8_t);
     * cacheByte_func_t cacheByte_func = &ByteCache.cacheByte;
     */
}

TEST_CASE("ByteCache::cacheByte - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[ByteCache::cacheByte]") {
    errno = 0;
    ConcreteByteCache ByteCache;
    ByteCache.cacheByte_result = ENOTTY;
    ByteCache.cacheByte(0x00);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("ByteCache::cacheByte - If no errors occur during execution, any previous `errno` shall be preserved", "[ByteCache::cacheByte]") {
    errno = ENOTTY;
    ConcreteByteCache ByteCache;
    ByteCache.cacheByte(0x00);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("ByteCache::readCachedByte - Invokes the underlying implementation", "[ByteCache::readCachedByte]") {
    ConcreteByteCache ByteCache;
    ByteCache.readCachedByte();
    REQUIRE( ByteCache.readCachedByte_invoked );
}

TEST_CASE("ByteCache::readCachedByte - Returns the result of the underlying implementation", "[ByteCache::readCachedByte]") {
    ConcreteByteCache ByteCache;
    const int expected_result = 0x79;
    ByteCache.readCachedByte_result = expected_result;
    REQUIRE( expected_result == ByteCache.readCachedByte() );
}

TEST_CASE("ByteCache::readCachedByte - A result returned from the underlying implementation not in the set {x: -1 <= x <= 255}, shall result in `ERANGE` error", "[ByteCache::readCachedByte]") {
    errno = 0;
    ConcreteByteCache ByteCache;
    ByteCache.readCachedByte_result = 0x1979;
    ByteCache.readCachedByte();
    REQUIRE( ERANGE == errno );
}

TEST_CASE("ByteCache::registerCachedBytesCallback - Invokes the underlying implementation", "[ByteCache::registerCachedBytesCallback]") {
    ConcreteByteCache ByteCache;
    ByteCache.registerCachedBytesCallback(nullptr);
    REQUIRE( ByteCache.registerCachedBytesCallback_invoked );
}

TEST_CASE("ByteCache::registerCachedBytesCallback - A pointer adhering to the `serial_event_t` signature shall be passed to the underlying implementation", "[ByteCache::registerCachedBytesCallback]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteByteCache ByteCache;
     * typedef void(ByteCache::*registerCachedBytesCallback_func_t)(uint8_t);
     * registerCachedBytesCallback_func_t registerCachedBytesCallback_func = &ByteCache.registerCachedBytesCallback;
     */
}

TEST_CASE("ByteCache::registerCachedBytesCallback - Supplies `uponBytesAvailable` argument to the underlying implementation", "[ByteCache::registerCachedBytesCallback]") {
    ConcreteByteCache ByteCache;
    serial_event_t expected_result = callback;
    ByteCache.registerCachedBytesCallback(expected_result);
    REQUIRE( expected_result == ByteCache.registerCachedBytesCallback_uponCachedBytes_arg );
}

TEST_CASE("ByteCache::registerCachedBytesCallback - Supplies `context` argument to the underlying implementation", "[ByteCache::registerCachedBytesCallback]") {
    ConcreteByteCache ByteCache;
    void * const context_provided = &ByteCache;
    ByteCache.registerCachedBytesCallback(callback, context_provided);
    REQUIRE( context_provided == ByteCache.registerCachedBytesCallback_context_arg );
}

TEST_CASE("ByteCache::registerCachedBytesCallback - If the `context` argument is unspecified, the default value shall be `nullptr`", "[ByteCache::registerCachedBytesCallback]") {
    ConcreteByteCache ByteCache;
    ByteCache.registerCachedBytesCallback(nullptr);
    REQUIRE( nullptr == ByteCache.registerCachedBytesCallback_context_arg );
}

TEST_CASE("ByteCache::registerCachedBytesCallback - If `nullptr` is supplied to `uponBytesAvailable`, the `context` argument shall be discarded", "[ByteCache::registerCachedBytesCallback]") {
    ConcreteByteCache ByteCache;
    void * const context_provided = &ByteCache;
    ByteCache.registerCachedBytesCallback(nullptr, context_provided);
    REQUIRE(context_provided != ByteCache.registerCachedBytesCallback_context_arg);
}

TEST_CASE("ByteCache::registerCachedBytesCallback - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[ByteCache::registerCachedBytesCallback]") {
    errno = 0;
    ConcreteByteCache ByteCache;
    ByteCache.registerCachedBytesCallback_result = ENOTTY;
    ByteCache.registerCachedBytesCallback(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("ByteCache::registerCachedBytesCallback - If no errors occur during execution, any previous `errno` shall be preserved", "[ByteCache::registerCachedBytesCallback]") {
    errno = ENOTTY;
    ConcreteByteCache ByteCache;
    ByteCache.registerCachedBytesCallback(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("ByteCache::size - Invokes the underlying implementation", "[ByteCache::size]") {
    ConcreteByteCache ByteCache;
    ByteCache.size();
    REQUIRE( ByteCache.size_invoked );
}

TEST_CASE("ByteCache::size - Returns the result of the underlying implementation", "[ByteCache::size]") {
    ConcreteByteCache ByteCache;
    const size_t expected_result = 79;
    ByteCache.size_result = expected_result;
    REQUIRE( expected_result == ByteCache.size() );
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
