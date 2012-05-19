// -*- C++ -*-

// ===================================================================
/**
 *  @file   Compression_Manager.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_COMPRESSION_MANAGER_H
#define TAO_COMPRESSION_MANAGER_H

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
  class TAO_Compression_Export CompressionManager :
    public virtual ::Compression::CompressionManager,
    public virtual ::CORBA::LocalObject
  {
  public:
    virtual ~CompressionManager ();

    virtual void register_factory (::Compression::CompressorFactory_ptr
                                   compressor_factory);

    virtual void unregister_factory (::Compression::CompressorId
                                     compressor_id);

    virtual ::Compression::CompressorFactory_ptr get_factory
      (::Compression::CompressorId compressor_id);

    virtual ::Compression::Compressor_ptr get_compressor
      (::Compression::CompressorId compressor_id,
       ::Compression::CompressionLevel compression_level);

    virtual ::Compression::CompressorFactorySeq *get_factories (void);

  private:
    TAO_SYNCH_MUTEX mutex_;
    ::Compression::CompressorFactorySeq factories_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_COMPRESSION_MANAGER_H */
