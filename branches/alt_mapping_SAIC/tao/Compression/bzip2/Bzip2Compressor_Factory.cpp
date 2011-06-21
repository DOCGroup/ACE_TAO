// $Id$

#include "tao/Compression/bzip2/Bzip2Compressor_Factory.h"
#include "tao/Compression/bzip2/Bzip2Compressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

Bzip2_CompressorFactory::Bzip2_CompressorFactory (void) :
  ::TAO::CompressorFactory (::Compression::COMPRESSORID_BZIP2),
  compressor_ (::Compression::Compressor::_nil ())
{
}

::Compression::Compressor_ptr
Bzip2_CompressorFactory::get_compressor (
    ::Compression::CompressionLevel compression_level)
{
  // @todo, make a array based on compression level
  if (CORBA::is_nil (compressor_.in ()))
    {
      compressor_ = new Bzip2Compressor (compression_level, this);
    }

  return ::Compression::Compressor::_duplicate (compressor_.in ());
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
