// -*- C++ -*-

// ===================================================================
/**
 *  @file   RLECompressor.h
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

        void compress (
            ::Compression::Buffer const &source,
            ::Compression::Buffer &target) override;

        void decompress (
            ::Compression::Buffer const &source,
            ::Compression::Buffer &target) override;

    private:
        RLECompressor (RLECompressor const &) = delete;
        RLECompressor &operator= (RLECompressor const &) = delete;
        RLECompressor (RLECompressor &&) = delete;
        RLECompressor &operator= (RLECompressor &&) = delete;
    };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_RLECOMPRESSOR_H */
