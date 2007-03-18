// -*- C++ -*-

// ===================================================================
/**
 *  @file   Compressor_Factory.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_COMPRESSOR_FACTORY_H
#define TAO_COMPRESSOR_FACTORY_H

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
  class TAO_Compression_Export CompressorFactory :
    public virtual ::Compression::CompressorFactory,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    CompressorFactory (::Compression::CompressorId compressor_id);

    virtual ::Compression::CompressorId compressor_id (void);

    virtual ::CORBA::ULongLong compressed_bytes (void);

    virtual ::CORBA::ULongLong uncompressed_bytes (void);

    virtual ::CORBA::Double average_compression (void);

    virtual ::Compression::Compressor_ptr get_compressor
      (::Compression::CompressionLevel compression_level)
      = 0;

    virtual void add_sample (::CORBA::ULongLong compressed_bytes,
                             ::CORBA::ULongLong uncompressed_bytes);

  private:
    TAO_SYNCH_MUTEX mutex_;
    ::Compression::CompressorId const compressor_id_;
    ::CORBA::ULongLong compressed_bytes_;
    ::CORBA::ULongLong uncompressed_bytes_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_COMPRESSOR_FACTORY_H */

