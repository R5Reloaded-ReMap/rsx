#pragma once

#include <cstddef>

namespace Compression
{
    struct ZstdDecodeResult
    {
        size_t bytesWritten;
        const char* error;

        explicit operator bool() const { return error == nullptr; }
    };

    ZstdDecodeResult DecodeZstdFrame(
        const void* source,
        size_t sourceSize,
        void* destination,
        size_t destinationCapacity);
}
