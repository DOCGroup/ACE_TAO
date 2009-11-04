// -*- C++ -*-

// ===================================================================
/**
 *  @file   LzoCompressor_Factory.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_LZOCOMPRESSOR_FACTORY_H
#define TAO_LZOCOMPRESSOR_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/Compression/lzo/LzoCompressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Compressor_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
    class TAO_LZOCOMPRESSOR_Export Lzo_CompressorFactory :
      public ::TAO::CompressorFactory
    {
      public:
        Lzo_CompressorFactory (void);

        virtual ::Compression::Compressor_ptr get_compressor (
            ::Compression::CompressionLevel compression_level);
       private:
         ::Compression::Compressor_var compressor_;
    };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_LZOCOMPRESSOR_FACTORY_H */
