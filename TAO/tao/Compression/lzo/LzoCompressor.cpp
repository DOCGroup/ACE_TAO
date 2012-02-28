// $Id$

#include "LzoCompressor.h"
#include <lzo1x.h>
#include <lzoutil.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
LzoCompressor::LzoCompressor (
  ::Compression::CompressorFactory_ptr compressor_factory,
  ::Compression::CompressionLevel compression_level) :
    BaseCompressor (compressor_factory, compression_level)
{
}

void
LzoCompressor::compress (
    const ::Compression::Buffer & source,
    ::Compression::Buffer & target)
{
  void* wrkmem = (lzo_bytep) lzo_malloc(LZO1X_1_MEM_COMPRESS);
  lzo_uint max_length = static_cast <lzo_uint> (source.length () * 1.1) + 12;
  target.length (static_cast <CORBA::ULong> (max_length));

  int const retval = ::lzo1x_1_compress (
            reinterpret_cast <const unsigned char*>(source.get_buffer ()),
            source.length (),
            reinterpret_cast <unsigned char*>(target.get_buffer ()),
            &max_length,
            wrkmem);

  lzo_free(wrkmem);

  if (retval != LZO_E_OK)
    {
      throw ::Compression::CompressionException (retval, "");
    }
  else
    {
      target.length (static_cast <CORBA::ULong> (max_length));
    }

  // Update statistics for this compressor
  this->update_stats (source.length (), target.length ());
}

void
LzoCompressor::decompress (
  const ::Compression::Buffer & source,
  ::Compression::Buffer & target)
{
  lzo_uint max_length = static_cast <lzo_uint> (target.length ());

  int const retval = ::lzo1x_decompress (
                                 reinterpret_cast <const unsigned char*>(source.get_buffer ()),
                                 source.length (),
                                 reinterpret_cast <unsigned char*>(target.get_buffer ()),
                                 &max_length,
                                 0);

  if (retval != LZO_E_OK)
    {
      throw ::Compression::CompressionException (retval, "");
    }
  else
    {
      target.length (static_cast  <CORBA::ULong> (max_length));
    }
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
