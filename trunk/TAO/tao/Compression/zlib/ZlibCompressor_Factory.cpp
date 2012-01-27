// $Id$

#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
#include "tao/Compression/zlib/ZlibCompressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

Zlib_CompressorFactory::Zlib_CompressorFactory (void) :
  ::TAO::CompressorFactory (::Compression::COMPRESSORID_ZLIB),
  compressor_ (::Compression::Compressor::_nil ())
{
}

::Compression::Compressor_ptr
Zlib_CompressorFactory::get_compressor (
    ::Compression::CompressionLevel compression_level)
{
  // @todo, make a array based on compression level
  if (CORBA::is_nil (compressor_.in ()))
    {
      compressor_ = new ZlibCompressor (this, compression_level);
    }

  return ::Compression::Compressor::_duplicate (compressor_.in ());
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
