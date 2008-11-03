#include "LzoCompressor.h"

ACE_RCSID (LZO,
           LzoCompressor,
           "$Id$")

#include <lzo/lzo1x.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
LzoCompressor::LzoCompressor (
  ::Compression::CompressionLevel compression_level,
  ::Compression::CompressorFactory_ptr compressor_factory) :
    BaseCompressor (compression_level, compressor_factory)
{
}

void
LzoCompressor::compress (
    const ::Compression::Buffer & source,
    ::Compression::Buffer & target
  )
{
  wrkmem = (lzo_bytep) lzo_malloc(LZO1X_1_MEM_COMPRESS);
  unsigned int max_length = static_cast <unsigned int> (source.length () * 1.1) + 12;
  target.length (static_cast <CORBA::ULong> (max_length));

  int const retval = ::lzo1x_1_compress (
            reinterpret_cast <char*>(const_cast<CORBA::Octet*>(source.get_buffer ())),
            source.length (),
            reinterpret_cast <char*>(target.get_buffer ()),
            max_length,
            wrkmem);

  lzo_free(wrkmem);

  if (retval != LZO_E_OK)
    {
      throw ::Compression::CompressionException (retval);
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

  unsigned int max_length = static_cast <unsigned int> (target.length ());

  int const retval = ::lzo1x_decompress (
                                 reinterpret_cast <char*>(const_cast<CORBA::Octet*>(source.get_buffer ())),
                                 source.length (),
                                 reinterpret_cast <char*>(target.get_buffer ()),
                                 &max_length,
                                 0)

  if (retval != LZO_E_OK)
    {
      throw ::Compression::CompressionException (retval);
    }
  else
    {
      target.length (static_cast  <CORBA::ULong> (max_length));
    }
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
