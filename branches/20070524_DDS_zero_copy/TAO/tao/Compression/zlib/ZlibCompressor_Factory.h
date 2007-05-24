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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
    class TAO_ZLIBCOMPRESSOR_Export Zlib_CompressorFactory :
      public ::TAO::CompressorFactory
    {
      public:
        Zlib_CompressorFactory (void);

        virtual ::Compression::Compressor_ptr get_compressor (
            ::Compression::CompressionLevel compression_level);
       private:
         ::Compression::Compressor_var compressor_;
    };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ZLIBCOMPRESSOR_FACTORY_H */
