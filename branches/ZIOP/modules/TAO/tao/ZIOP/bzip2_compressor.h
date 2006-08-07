#include "tao/ZIOP/Compressor_Factory.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
    class Bzip2Compressor : public BaseCompressor
    {
      Bzip2Compressor (::ZIOP::CompressionLevel compression_level,
                       ::ZIOP::CompressorFactory_ptr compressor_factory) :
        BaseCompressor (compression_level, compressor_factory)
      {
      }

      Bzip2Compressor
      virtual void compress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
        // See http://www.bzip.org/1.0.3/html/util-fns.html
      }

      virtual void decompress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
      }
    };

    class Bzip2_CompressorFactory : public ::TAO::ZIOP::CompressorFactory
    {
      public:
        virtual ::ZIOP::Compressor_ptr get_compressor (
            ::ZIOP::CompressionLevel compression_level
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
          return 0;
        }
    };
  }
}
