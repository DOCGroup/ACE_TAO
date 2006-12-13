#include "tao/ZIOP/ZIOP.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
    class BaseCompressor : public ::ZIOP::Compressor
    {
      public:
      BaseCompressor (::ZIOP::CompressionLevel compression_level,
                      ::ZIOP::CompressorFactory_ptr compressor_factory);

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

      virtual ::ZIOP::CompressorFactory_ptr compressor_factory (void)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ));

      virtual ::ZIOP::CompressionLevel compression_level (void)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ));

      private:
        ::ZIOP::CompressionLevel compression_level_;
        ::ZIOP::CompressorFactory_var compressor_factory_;

    };
  }
}
