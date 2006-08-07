#include "tao/ZIOP/ZIOP.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
    class BaseCompressor : public ::ZIOP::Compressor
    {
      BaseCompressor (::ZIOP::CompressionLevel compression_level,
                      ::ZIOP::CompressorFactory_ptr compressor_factory) :
        compression_level_ (compression_level),
        compressor_factory_ (::ZIOP::CompressorFactory::_duplicate (compressor_factory))
      {
      }

      virtual void compress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        )) = 0;

      virtual void decompress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        )) = 0;

      virtual ::ZIOP::CompressorFactory_ptr compressor_factory (
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ))
      {
        return ::ZIOP::CompressorFactory::_duplicate (compressor_factory_.in ());
      }

      virtual ::ZIOP::CompressionLevel compression_level (
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ))
      {
        return compression_level_;
      }

      private:
        ::ZIOP::CompressionLevel compression_level_;
        ::ZIOP::CompressorFactory_var compressor_factory_;

    };
  }
}
