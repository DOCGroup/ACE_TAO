#include "Compressor.h"

namespace TAO
{
  namespace ZIOP
  {

::ZIOP::CompressorFactory_ptr
BaseCompressor::compressor_factory (void)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ))
{
  return ::ZIOP::CompressorFactory::_duplicate (compressor_factory_.in ());
}

::ZIOP::CompressionLevel
BaseCompressor::compression_level (void)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ))
{
  return compression_level_;
}

BaseCompressor::BaseCompressor (
  ::ZIOP::CompressionLevel compression_level,
  ::ZIOP::CompressorFactory_ptr compressor_factory) :
    compression_level_ (compression_level),
    compressor_factory_ (::ZIOP::CompressorFactory::_duplicate (compressor_factory))
{
}



  }
}
