#include "tao/ZIOP/ZIOP.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
    class CompressorFactory : public ::ZIOP::CompressorFactory
    {
      public:
        virtual ::ZIOP::CompressorId compressor_id (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
            return compressor_id_;
        }

        virtual ::CORBA::ULongLong compressed_bytes (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
          ::CORBA::ULongLong return_value;
          {
            ACE_Guard <ACE_SYNCH_MUTEX> guard (mutex_);
            return_value = compressed_bytes_;
          }
          return return_value;
        }

        virtual ::CORBA::ULongLong uncompressed_bytes (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
          ::CORBA::ULongLong return_value;
          {
            ACE_Guard <ACE_SYNCH_MUTEX> guard (mutex_);
            return_value = uncompressed_bytes_;
          }
          return return_value;
        }

        virtual ::CORBA::Double average_compression (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
          ::CORBA::Double return_value;
          {
            ACE_Guard <ACE_SYNCH_MUTEX> guard (mutex_);
            // @tido
          }
          return return_value;
        }

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
          ))
        {
          ACE_Guard <ACE_SYNCH_MUTEX> guard (mutex_);
          compressed_bytes_ += compressed_bytes;
          uncompressed_bytes_ += uncompressed_bytes;
        }
      private:
        ACE_SYNCH_MUTEX mutex_;
        ::ZIOP::CompressorId compressor_id_;
        ::CORBA::ULongLong compressed_bytes_;
        ::CORBA::ULongLong uncompressed_bytes_;
      };
  }
}
