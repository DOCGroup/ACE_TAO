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
    public virtual TAO_Local_RefCounted_Object
  {
    virtual void register_factory (
        ::Compression::CompressorFactory_ptr compressor_factory)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Compression::FactoryAlreadyRegistered));

    virtual void unregister_factory (
        ::Compression::CompressorId compressor_id)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Compression::UnknownCompressorId));

    virtual ::Compression::CompressorFactory_ptr get_factory (
        ::Compression::CompressorId compressor_id)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Compression::UnknownCompressorId));

    virtual ::Compression::Compressor_ptr get_compressor (
        ::Compression::CompressorId compressor_id,
        ::Compression::CompressionLevel compression_level)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Compression::UnknownCompressorId));

    virtual ::Compression::CompressorFactorySeq * get_factories (void)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
      TAO_SYNCH_MUTEX mutex_;
      ::Compression::CompressorFactorySeq factories_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_COMPRESSION_MANAGER_H */
