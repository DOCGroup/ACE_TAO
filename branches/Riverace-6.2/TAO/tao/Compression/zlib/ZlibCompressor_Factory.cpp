// $Id$

#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
#include "tao/Compression/zlib/ZlibCompressor.h"
#include "ace/Min_Max.h"
#include "zlib.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

Zlib_CompressorFactory::Zlib_CompressorFactory (void) :
  ::TAO::CompressorFactory (::Compression::COMPRESSORID_ZLIB)
{
}

::Compression::Compressor_ptr
Zlib_CompressorFactory::get_compressor (
    ::Compression::CompressionLevel compression_level)
{
    // Ensure Compression range 0-9 and will also convert -1(default) to 9(max).
    compression_level = ace_range(  ::Compression::CompressionLevel(Z_NO_COMPRESSION),   // Min value
                                    ::Compression::CompressionLevel(Z_BEST_COMPRESSION), // Max value
                                    compression_level   // Argument value
                                  );

    ::Compression::Compressor_ptr compressor = 0;

    {   // Ensure scoped lock for compressor Map container

        ACE_GUARD_RETURN( TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0 );

        try {
            // Try and locate the compressor (we may already have it)
            ZlibCompressorMap::iterator it = this->compressors_.find(compression_level);

            if (it == this->compressors_.end())
            {  // Does not yet exist so create it
                ACE_NEW_RETURN(compressor, ::TAO::ZlibCompressor(this, compression_level), 0);
                it = this->compressors_.insert(ZlibCompressorMap::value_type(compression_level, compressor)).first;
            }

            compressor = (*it).second.in();

        } catch (...) {
            TAOLIB_ERROR_RETURN((LM_ERROR,
                ACE_TEXT("(%P | %t) ERROR: ZlibCompressor - Unable to create Zlib Compressor at level [%d].\n"),
                int(compression_level)),0);
        }

    }   // End of scoped container locking

    return ::Compression::Compressor::_duplicate(compressor);
}

}

TAO_END_VERSIONED_NAMESPACE_DECL
