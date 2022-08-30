// -*- C++ -*-

// ===================================================================
/**
 *  @file   Base_Compressor.h
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_BASE_COMPRESSOR_H
#define TAO_BASE_COMPRESSOR_H

#include /**/ "ace/pre.h"

#include "tao/Compression/compression_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * This class is a helper to implement real compressors
   */
  class TAO_Compression_Export BaseCompressor :
    public virtual ::Compression::Compressor,
    public virtual ::CORBA::LocalObject
  {
  public:
    BaseCompressor (::Compression::CompressorFactory_ptr compressor_factory,
                    ::Compression::CompressionLevel compression_level);

    void compress (const ::Compression::Buffer &source,
                           ::Compression::Buffer &target) override = 0;

    void decompress (const ::Compression::Buffer &source,
                             ::Compression::Buffer &target) override = 0;

    ::Compression::CompressorFactory_ptr compressor_factory (void) override;

    ::Compression::CompressionLevel compression_level (void) override;

    ::CORBA::ULongLong compressed_bytes (void) override;

    ::CORBA::ULongLong uncompressed_bytes (void) override;

    ::Compression::CompressionRatio compression_ratio (void) override;

  protected:
    void update_stats (::CORBA::ULongLong uncompressed_bytes,
                       ::CORBA::ULongLong compressed_bytes);

    TAO_SYNCH_MUTEX mutex_;
    ::Compression::CompressionLevel compression_level_;
    ::Compression::CompressorFactory *compressor_factory_;
    ::CORBA::ULongLong compressed_bytes_;
    ::CORBA::ULongLong uncompressed_bytes_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASE_COMPRESSOR_H */

