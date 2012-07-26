// $Id$

#include "tao/Compression/rle/RLECompressor_Factory.h"
#include "tao/Compression/rle/RLECompressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
RLE_CompressorFactory::RLE_CompressorFactory(void)
    : ::TAO::CompressorFactory(Compression::COMPRESSORID_RLE)
    , compressor_(::Compression::Compressor::_nil())
{
}

Compression::Compressor_ptr
RLE_CompressorFactory::get_compressor(Compression::CompressionLevel)
{
    if (::CORBA::is_nil(this->compressor_.in())) {
         ::Compression::Compressor_ptr compressor;
         ACE_NEW_RETURN( compressor, ::TAO::RLECompressor(this), 0 );
         this->compressor_ = compressor;
    }
    return ::Compression::Compressor::_duplicate(this->compressor_.in());
}
}

TAO_END_VERSIONED_NAMESPACE_DECL

