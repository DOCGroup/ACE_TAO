// $Id$

#include "tao/Compression/Base_Compressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  BaseCompressor::BaseCompressor (
    ::Compression::CompressorFactory_ptr compressor_factory,
    ::Compression::CompressionLevel compression_level)
    : compression_level_ (compression_level),
      compressor_factory_ (::Compression::CompressorFactory::_duplicate (
                           compressor_factory)),
      compressed_bytes_ (0),
      uncompressed_bytes_ (0)
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

  ::CORBA::ULongLong
  BaseCompressor::compressed_bytes (void)
  {
    ::CORBA::ULongLong return_value = 0;
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return_value = compressed_bytes_;
    }
    return return_value;
  }

  ::CORBA::ULongLong
  BaseCompressor::uncompressed_bytes (void)
  {
    ::CORBA::ULongLong return_value = 0;
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return_value = uncompressed_bytes_;
    }
    return return_value;
  }

  ::Compression::CompressionRatio
  BaseCompressor::compression_ratio (void)
  {
    ::Compression::CompressionRatio return_value = 0;
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      if (this->uncompressed_bytes_ > 0)
        {

          return_value =
            static_cast< ::Compression::CompressionRatio>
              ((::Compression::CompressionRatio) this->compressed_bytes_ / this->uncompressed_bytes_);
        }
    }
    return return_value;
  }

  void
  BaseCompressor::update_stats (
    ::CORBA::ULongLong compressed_bytes,
    ::CORBA::ULongLong uncompressed_bytes)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->compressed_bytes_ += compressed_bytes;
    this->uncompressed_bytes_ += uncompressed_bytes;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
