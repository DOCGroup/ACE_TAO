// -*- C++ -*-

/**
 *  @file   TestCompressor.h
 *
 *  See http://www.zlib.net/manual.html for the zlib interface itself
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#ifndef TAO_TESTCOMPRESSOR_H
#define TAO_TESTCOMPRESSOR_H

#include /**/ "ace/pre.h"

#include "TestCompressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Base_Compressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class TAO_TESTCOMPRESSOR_Export TestCompressor : public BaseCompressor
  {
  public:
    TestCompressor (::Compression::CompressorFactory_ptr compressor_factory,
                    ::Compression::CompressionLevel compression_level);

    void compress (::Compression::Buffer const &,
                   ::Compression::Buffer &) override;

    void decompress (::Compression::Buffer const &,
                     ::Compression::Buffer &) override;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TESTCOMPRESSOR_H */
