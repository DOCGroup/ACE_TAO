#include "tao/ZIOP/ZIOP.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
    class CompressorFactory : public ::ZIOP::CompressorFactory
    {
      public:
        virtual ::ZIOP::CompressorId compressor_id (void)
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual ::CORBA::ULongLong compressed_bytes (void)
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual ::CORBA::ULongLong uncompressed_bytes (void)
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual ::CORBA::Double average_compression (void)
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual ::ZIOP::Compressor_ptr get_compressor (
            ::ZIOP::CompressionLevel compression_level
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          )) = 0;

        virtual void add_sample (
            ::CORBA::ULongLong compressed_bytes,
            ::CORBA::ULongLong uncompressed_bytes
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

      private:
        ACE_SYNCH_MUTEX mutex_;
        ::ZIOP::CompressorId compressor_id_;
        ::CORBA::ULongLong compressed_bytes_;
        ::CORBA::ULongLong uncompressed_bytes_;
      };
  }
}
