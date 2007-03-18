#include "tao/Compression/Base_Compressor.h"

ACE_RCSID (Compression,
           Base_Compressor,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  BaseCompressor::BaseCompressor (::Compression::CompressionLevel
                                  compression_level,
                                  ::Compression::CompressorFactory_ptr
                                  compressor_factory) :
    compression_level_ (compression_level),
    compressor_factory_ (::Compression::CompressorFactory::_duplicate (
                         compressor_factory))
  {
  }

  ::Compression::CompressorFactory_ptr
  BaseCompressor::compressor_factory (void)
  {
    return ::Compression::CompressorFactory::_duplicate (
      compressor_factory_.in ());
  }

  ::Compression::CompressionLevel
  BaseCompressor::compression_level (void)
  {
    return compression_level_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
