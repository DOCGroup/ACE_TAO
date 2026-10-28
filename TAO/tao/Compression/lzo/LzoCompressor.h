// -*- C++ -*-

// ===================================================================
/**
 *  @file   LzoCompressor.h
 *
 *  See http://www.oberhumer.com/ for the lzo interface itself
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_LZOCOMPRESSOR_H
#define TAO_LZOCOMPRESSOR_H

#include /**/ "ace/pre.h"

#include "tao/Compression/lzo/LzoCompressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Base_Compressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class TAO_LZOCOMPRESSOR_Export LzoCompressor : public BaseCompressor
  {
    public:
      LzoCompressor (::Compression::CompressorFactory_ptr compressor_factory,
                     ::Compression::CompressionLevel compression_level);

      void compress (
          ::Compression::Buffer const & source,
          ::Compression::Buffer & target) override;

      void decompress (
          ::Compression::Buffer const & source,
          ::Compression::Buffer & target) override;

    private:
      LzoCompressor (LzoCompressor const &) = delete;
      LzoCompressor &operator= (LzoCompressor const &) = delete;
      LzoCompressor (LzoCompressor &&) = delete;
      LzoCompressor &operator= (LzoCompressor &&) = delete;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_LZOCOMPRESSOR_H */
