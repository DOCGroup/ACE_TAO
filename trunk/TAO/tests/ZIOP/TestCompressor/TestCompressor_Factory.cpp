// $Id$

#include "TestCompressor_Factory.h"
#include "TestCompressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

Test_CompressorFactory::Test_CompressorFactory (void) :
  ::TAO::CompressorFactory (COMPRESSORID_FOR_TESTING),
  compressor_ (::Compression::Compressor::_nil ())
{
}

::Compression::Compressor_ptr
Test_CompressorFactory::get_compressor (
    ::Compression::CompressionLevel compression_level)
{
  // @todo, make a array based on compression level
  if (CORBA::is_nil (compressor_.in ()))
    {
      compressor_ = new TestCompressor (this, compression_level);
    }

  return ::Compression::Compressor::_duplicate (compressor_.in ());
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
