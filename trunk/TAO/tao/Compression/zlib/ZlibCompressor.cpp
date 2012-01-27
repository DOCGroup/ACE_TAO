// $Id$

#include "ZlibCompressor.h"
#include "zlib.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
ZlibCompressor::ZlibCompressor (
  ::Compression::CompressorFactory_ptr compressor_factory,
  ::Compression::CompressionLevel compression_level) :
    BaseCompressor (compressor_factory, compression_level)
{
}

void
ZlibCompressor::compress (
    const ::Compression::Buffer & source,
    ::Compression::Buffer & target
  )
{
  uLongf max_length =
    static_cast <uLongf> (source.length () * 1.001) + 12;
  target.length (static_cast <CORBA::ULong> (max_length));

  int const retval = ::compress2 (reinterpret_cast <Bytef*>(target.get_buffer ()),
                                  &max_length,
                                  reinterpret_cast <const Bytef*>(source.get_buffer ()),
                                  source.length (),
                                  this->compression_level ());

  if (retval != Z_OK)
    {
      throw ::Compression::CompressionException (retval, ::zError (retval));
    }
  else
    {
      target.length (static_cast <CORBA::ULong> (max_length));
    }

  // Update statistics for this compressor
  this->update_stats (source.length (), target.length ());
}

void
ZlibCompressor::decompress (
  const ::Compression::Buffer & source,
  ::Compression::Buffer & target)
{
  uLongf max_length = static_cast <uLongf> (target.length ());
  int const retval = uncompress (reinterpret_cast <Bytef*>(target.get_buffer ()),
                                 &max_length,
                                 reinterpret_cast <const Bytef*>(source.get_buffer ()),
                                 source.length ());

  if (retval != Z_OK)
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
