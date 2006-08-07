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
          ))
        {
          if (!CORBA::is_nil (compressor_factory))
            {
              CORBA::ULong const length = this->factories_.length ();

              for (CORBA::ULong i = 0; i < length; ++i)
                {
                  ::ZIOP::CompressorId const current =
                    this->factories_[i]->compressor_id ();

                  if (compressor_factory->compressor_id () == current)
                    {
                      // exception
                    }

                }

              factories_.length (length + 1);
              factories_[length] = ::ZIOP::CompressorFactory::_duplicate (compressor_factory);
            }
          else
            {
            }
        }

        virtual void unregister_factory (
            ::ZIOP::CompressorId compressor_id
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ))
        {
          CORBA::ULong const length = this->factories_.length ();

          for (CORBA::ULong i = 0; i < length; ++i)
            {
              ::ZIOP::CompressorId const current =
                this->factories_[i]->compressor_id ();

              if (current != compressor_id)
                {
                  continue;
                }

              this->factories_[i] = ::ZIOP::CompressorFactory::_nil ();
              // make sequence smaller
            }

          // todo exception
//          return ::ZIOP::CompressorFactory::_nil ();

          // search for id, if not there, raise exception, if there,
          // remove it
        }

        virtual ::ZIOP::CompressorFactory_ptr get_factory (
            ::ZIOP::CompressorId compressor_id
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ))
        {
          CORBA::ULong const length = this->factories_.length ();

          for (CORBA::ULong i = 0; i < length; ++i)
            {
              ::ZIOP::CompressorId const current =
                this->factories_[i]->compressor_id (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK_RETURN (0);

              if (current != compressor_id)
                {
                  continue;
                }

              return ::ZIOP::CompressorFactory::_duplicate (this->factories_[i]);
            }

          // todo exception
          return ::ZIOP::CompressorFactory::_nil ();

        }

        virtual ::ZIOP::Compressor_ptr get_compressor (
            ::ZIOP::CompressorId compressor_id,
            ::ZIOP::CompressionLevel compression_level
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ))
        {
//          return this->get_compressor (compressor_id);
        }

        virtual ::ZIOP::CompressorFactorySeq * get_factories (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
          // todo
          return 0;
        }

        private:
          ::ZIOP::CompressorFactorySeq factories_;
    };
  }
}
