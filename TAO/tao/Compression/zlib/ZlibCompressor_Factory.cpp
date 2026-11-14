#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
#include "tao/Compression/zlib/ZlibCompressor.h"
#include "ace/Min_Max.h"
#include "zlib.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
Zlib_CompressorFactory::Zlib_CompressorFactory ()
  : ::TAO::CompressorFactory (::Compression::COMPRESSORID_ZLIB)
{
}

::Compression::Compressor_ptr
Zlib_CompressorFactory::get_compressor (
    ::Compression::CompressionLevel compression_level)
{
    // Ensure Compression range 0-9 and convert -1 (default) to 9 (maximum).
    ::Compression::CompressionLevel const selected_level = ace_range (
      static_cast<::Compression::CompressionLevel> (Z_NO_COMPRESSION),
      static_cast<::Compression::CompressionLevel> (Z_BEST_COMPRESSION),
      compression_level);

    ::Compression::Compressor_var compressor;

    {
        // Keep the map access locked until the selected compressor has its own reference.
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, nullptr);

        try
          {
            ZlibCompressorMap::iterator it =
              this->compressors_.find (selected_level);

            if (it == this->compressors_.end())
              {
                compressor = new ::TAO::ZlibCompressor (this, selected_level);
                it = this->compressors_.insert (
                  ZlibCompressorMap::value_type (selected_level, compressor)).first;
              }

            // Assignment from a raw pointer transfers ownership to the var.
            // Duplicate the map's reference so the local var owns its reference.
            compressor = ::Compression::Compressor::_duplicate ((*it).second.in ());
          }
        catch (...)
          {
            TAOLIB_ERROR ((LM_ERROR,
                           ACE_TEXT ("(%P | %t) ERROR: ZlibCompressor - Unable to create Zlib Compressor at level [%d].\n"),
                           int (compression_level)));
            return nullptr;
          }

    }

    return ::Compression::Compressor::_duplicate (compressor.in ());
}

}

TAO_END_VERSIONED_NAMESPACE_DECL
