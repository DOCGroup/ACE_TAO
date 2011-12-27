// $Id$

#include "TestCompressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
TestCompressor::TestCompressor (
  ::Compression::CompressionLevel compression_level,
  ::Compression::CompressorFactory_ptr compressor_factory) :
    BaseCompressor (compression_level, compressor_factory)
{
}

void
TestCompressor::compress (
    const ::Compression::Buffer &,
    ::Compression::Buffer &)
{
  throw ::Compression::CompressionException (0, "");
}

void
TestCompressor::decompress (
  const ::Compression::Buffer &,
  ::Compression::Buffer &)
{
  throw ::Compression::CompressionException (0, "");
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
