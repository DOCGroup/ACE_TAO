#include "tao/ZIOP/ZIOP.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
      class CompressionManager : public ::ZIOP::CompressionManager
      {
        virtual void register_factory (
            ::ZIOP::CompressorFactory_ptr compressor_factory
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::FactoryAlreadyRegistered
          ));

        virtual void unregister_factory (
            ::ZIOP::CompressorId compressor_id
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ));

        virtual ::ZIOP::CompressorFactory_ptr get_factory (
            ::ZIOP::CompressorId compressor_id
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ));

        virtual ::ZIOP::Compressor_ptr get_compressor (
            ::ZIOP::CompressorId compressor_id,
            ::ZIOP::CompressionLevel compression_level
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ));

        virtual ::ZIOP::CompressorFactorySeq * get_factories (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        private:
          ::ZIOP::CompressorFactorySeq factories_;
    };
  }
}
