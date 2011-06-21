// $Id$

#include "tao/Compression/lzo/LzoCompressor_Factory.h"
#include "tao/Compression/lzo/LzoCompressor.h"
#include <lzo1x.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

Lzo_CompressorFactory::Lzo_CompressorFactory (void) :
  ::TAO::CompressorFactory (::Compression::COMPRESSORID_LZO),
  compressor_ (::Compression::Compressor::_nil ())
{
  ::lzo_init();
}

::Compression::Compressor_ptr
Lzo_CompressorFactory::get_compressor (
    ::Compression::CompressionLevel compression_level)
{
  // @todo, make a array based on compression level
  if (CORBA::is_nil (compressor_.in ()))
    {
      compressor_ = new LzoCompressor (compression_level, this);
    }

  return ::Compression::Compressor::_duplicate (compressor_.in ());
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
