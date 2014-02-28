// -*- C++ -*-

// ===================================================================
/**
 *  @file   RLECompressor.h
 *
 *  $Id$
 *
 *  @author Derek Dominish  <derek.dominish@dsto.defence.gov.au>
 */
// ===================================================================

#ifndef TAO_RLECOMPRESSOR_H
#define TAO_RLECOMPRESSOR_H

#include /**/ "ace/pre.h"

#include "tao/Compression/rle/RLECompressor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Compression/Compression.h"
#include "tao/Compression/Base_Compressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
    class TAO_RLECOMPRESSOR_Export RLECompressor : public ::TAO::BaseCompressor
    {
    public:

        RLECompressor(::Compression::CompressorFactory_ptr);

        virtual void compress (
            const ::Compression::Buffer &source,
            ::Compression::Buffer &target);

        virtual void decompress (
            const ::Compression::Buffer &source,
            ::Compression::Buffer &target);
    };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_RLECOMPRESSOR_H */
