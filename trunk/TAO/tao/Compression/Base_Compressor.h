// -*- C++ -*-

// ===================================================================
/**
 *  @file   Base_Compressor.h
 *
 *  $Id$
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

    virtual void compress (const ::Compression::Buffer &source,
                           ::Compression::Buffer &target) = 0;

    virtual void decompress (const ::Compression::Buffer &source,
                             ::Compression::Buffer &target) = 0;

    virtual ::Compression::CompressorFactory_ptr compressor_factory (void);

    virtual ::Compression::CompressionLevel compression_level (void);

    virtual ::CORBA::ULongLong compressed_bytes (void);

    virtual ::CORBA::ULongLong uncompressed_bytes (void);

    virtual ::Compression::CompressionRatio compression_ratio (void);

  protected:
    void update_stats (::CORBA::ULongLong compressed_bytes,
                       ::CORBA::ULongLong uncompressed_bytes);

    TAO_SYNCH_MUTEX mutex_;
    ::Compression::CompressionLevel compression_level_;
    ::Compression::CompressorFactory_var compressor_factory_;
    ::CORBA::ULongLong compressed_bytes_;
    ::CORBA::ULongLong uncompressed_bytes_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASE_COMPRESSOR_H */

