#include <pch.h>

#include <core/compression/zstdcodec.h>

#include <zstd.h>

namespace Compression
{
    ZstdDecodeResult DecodeZstdFrame(
        const void* source,
        const size_t sourceSize,
        void* destination,
        const size_t destinationCapacity)
    {
        if (!source || sourceSize == 0)
            return { 0, "empty ZSTD input" };

        if (!destination || destinationCapacity == 0)
            return { 0, "empty ZSTD output buffer" };

        const size_t frameSize = ZSTD_findFrameCompressedSize(source, sourceSize);
        if (ZSTD_isError(frameSize))
            return { 0, ZSTD_getErrorName(frameSize) };

        if (frameSize == 0 || frameSize > sourceSize)
            return { 0, "invalid ZSTD frame size" };

        const unsigned long long advertisedSize = ZSTD_getFrameContentSize(source, frameSize);
        if (advertisedSize != ZSTD_CONTENTSIZE_UNKNOWN &&
            advertisedSize != ZSTD_CONTENTSIZE_ERROR &&
            advertisedSize > destinationCapacity)
        {
            return { 0, "ZSTD frame is larger than the destination buffer" };
        }

        const size_t decodedSize = ZSTD_decompress(destination, destinationCapacity, source, frameSize);
        if (ZSTD_isError(decodedSize))
            return { 0, ZSTD_getErrorName(decodedSize) };

        return { decodedSize, nullptr };
    }
}
