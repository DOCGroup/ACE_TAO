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
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    BaseCompressor (::Compression::CompressionLevel compression_level,
                    ::Compression::CompressorFactory_ptr compressor_factory);

    virtual void compress (const ::CORBA::OctetSeq &source,
                           ::CORBA::OctetSeq &target) = 0;

    virtual void decompress (const ::CORBA::OctetSeq &source,
                             ::CORBA::OctetSeq &target) = 0;

    virtual ::Compression::CompressorFactory_ptr compressor_factory (void);

    virtual ::Compression::CompressionLevel compression_level (void);

  protected:
    ::Compression::CompressionLevel compression_level_;
    ::Compression::CompressorFactory_var compressor_factory_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASE_COMPRESSOR_H */

