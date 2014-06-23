// -*- C++ -*-

// ===================================================================
/**
 *  @file   ZlibCompressor_Factory.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_ZLIBCOMPRESSOR_FACTORY_H
#define TAO_ZLIBCOMPRESSOR_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/Compression/zlib/ZlibCompressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Compressor_Factory.h"
#include <map>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class TAO_ZLIBCOMPRESSOR_Export Zlib_CompressorFactory :
    public ::TAO::CompressorFactory
  {
    typedef std::map< ::Compression::CompressionLevel,
        const ::Compression::Compressor_var> ZlibCompressorMap;

  public:
    Zlib_CompressorFactory (void);

    virtual ::Compression::Compressor_ptr get_compressor (
        ::Compression::CompressionLevel compression_level);

  private:
    ACE_UNIMPLEMENTED_FUNC (Zlib_CompressorFactory (const Zlib_CompressorFactory &))
    ACE_UNIMPLEMENTED_FUNC (Zlib_CompressorFactory &operator= (const Zlib_CompressorFactory &))

    // Ensure we can lock with imutability (i.e. const)
    mutable TAO_SYNCH_MUTEX mutex_;
    ZlibCompressorMap       compressors_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ZLIBCOMPRESSOR_FACTORY_H */
