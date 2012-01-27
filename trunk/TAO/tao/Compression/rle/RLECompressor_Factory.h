// -*- C++ -*-

// ===================================================================
/**
 *  @file   RLECompressor_Factory.h
 *
 *  $Id$
 *
 *  @author Derek Dominish  <derek.dominish@dsto.defence.gov.au>
 */
// ===================================================================

#ifndef TAO_RLECOMPRESSOR_FACTORY_H
#define TAO_RLECOMPRESSOR_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/Compression/rle/RLECompressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Compressor_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
    class TAO_RLECOMPRESSOR_Export RLE_CompressorFactory :
        public ::TAO::CompressorFactory
    {
    public:
        RLE_CompressorFactory(void);

        virtual ::Compression::Compressor_ptr	get_compressor (
            ::Compression::CompressionLevel = 0);

    private:
        ::Compression::Compressor_var compressor_;
    };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_RLECOMPRESSOR_FACTORY_H */
