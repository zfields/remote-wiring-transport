/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef BYTE_CACHE_H
#define BYTE_CACHE_H

#include <cstddef>
#include <cstdint>

#include "Stream.h"

namespace serial_wiring {

/*!
 * \brief Interface for accessing character and binary based caches
 */
class ByteCache {
  public:
    /*!
     * \brief Writes a byte to the cache
     *
     * \param [in] byte_ The byte of data to write into the cache
     */
    inline
    void
    cacheByte (
        uint8_t byte_
    ) {
        _cacheByte(byte_);
    }

    /*!
     * \brief Fetches the next byte from the cache
     *
     * \return The first byte of cached data available
     *
     * \note Returns -1 if there is no byte, although this should
     *       be avoided by first checking `available()` to see if
     *       data is available.
     */
    inline
    int
    readCachedByte (
        void
    ) {
        return _readCachedByte();
    }

    /*!
     * \brief Register a cached bytes ready callback
     *
     * The supplied callback shall be invoked when bytes are
     * available in the cache
     *
     * \param [in] uponCachedBytes_ Callback invoked when byte(s) are
     *                              available in the cache
     *
     * \param [in] context_ Optional context provided to uponCachedBytes_
     *                      via the `context_` parameter
     */
    inline
    void
    registerCachedBytesCallback (
        serial_event_t uponCachedBytes_,
        void * context_ = nullptr
    ) {
        _registerCachedBytesCallback(uponCachedBytes_, context_);
    }

    /*!
     * \brief Checks the number of bytes available in the cache
     *
     * \return The number of bytes available
     */
    inline
    size_t
    size (
        void
    ) const {
        return _size();
    }

  protected:
    ~ByteCache (void) {}

    virtual
    void
    _cacheByte (
        uint8_t byte_
    ) = 0;

    virtual
    int
    _readCachedByte (
        void
    ) = 0;

    virtual
    void
    _registerCachedBytesCallback (
        serial_event_t uponCachedBytes_,
        void * context_ = nullptr
    ) = 0;

    virtual
    size_t
    _size (
        void
    ) const = 0;
};

}  // namespace serial_wiring

#endif // BYTE_CACHE_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
