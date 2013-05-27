// $Id$

#include "TestCompressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
TestCompressor::TestCompressor (
  ::Compression::CompressorFactory_ptr compressor_factory,
  ::Compression::CompressionLevel compression_level) :
    BaseCompressor (compressor_factory, compression_level)
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
