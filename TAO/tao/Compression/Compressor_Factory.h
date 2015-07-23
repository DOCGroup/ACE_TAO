// -*- C++ -*-

// ===================================================================
/**
 *  @file   Compressor_Factory.h
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
    public virtual ::CORBA::LocalObject
  {
  public:
    CompressorFactory (::Compression::CompressorId compressor_id);

    virtual ::Compression::CompressorId compressor_id (void);

    virtual ::Compression::Compressor_ptr get_compressor
      (::Compression::CompressionLevel compression_level) = 0;

  private:
    ::Compression::CompressorId const compressor_id_;

#if defined (ACE_WIN32_VC14)
    // Workaround for connect issue 1577211
    ACE_UNIMPLEMENTED_FUNC (CompressorFactory (const CompressorFactory &))
    ACE_UNIMPLEMENTED_FUNC (CompressorFactory &operator = (const CompressorFactory &))
#endif
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_COMPRESSOR_FACTORY_H */

