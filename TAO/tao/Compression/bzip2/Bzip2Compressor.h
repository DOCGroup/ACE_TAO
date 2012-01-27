// -*- C++ -*-

// ===================================================================
/**
 *  @file   Bzip2Compressor.h
 *
 *  $Id$
 *
 *  See http://www.zlib.net/manual.html for the zlib interface itself
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_BZIP2COMPRESSOR_H
#define TAO_BZIP2COMPRESSOR_H

#include /**/ "ace/pre.h"

#include "tao/Compression/bzip2/Bzip2Compressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Base_Compressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class TAO_BZIP2COMPRESSOR_Export Bzip2Compressor : public BaseCompressor
  {
    public:
      Bzip2Compressor (::Compression::CompressorFactory_ptr compressor_factory,
                       ::Compression::CompressionLevel compression_level);

      virtual void compress (
          const ::Compression::Buffer & source,
          ::Compression::Buffer & target);

      virtual void decompress (
          const ::Compression::Buffer & source,
          ::Compression::Buffer & target);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BZIP2COMPRESSOR_H */
