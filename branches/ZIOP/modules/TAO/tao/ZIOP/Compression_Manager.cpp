#include "Compression_Manager.h"

namespace TAO
{
  namespace ZIOP
  {

void CompressionManager::register_factory (
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
              ACE_THROW (::ZIOP::FactoryAlreadyRegistered ());
            }

        }

      factories_.length (length + 1);
      factories_[length] = ::ZIOP::CompressorFactory::_duplicate (compressor_factory);
    }
  else
    {
    }
}

void
CompressionManager::unregister_factory (
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

::ZIOP::CompressorFactory_ptr
CompressionManager::get_factory (
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

  ACE_THROW (::ZIOP::UnknownCompressorId ());

  return ::ZIOP::CompressorFactory::_nil ();

}

::ZIOP::Compressor_ptr
CompressionManager::get_compressor (
            ::ZIOP::CompressorId compressor_id,
            ::ZIOP::CompressionLevel compression_level
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::ZIOP::UnknownCompressorId
          ))
{
	return 0;
//          return this->get_compressor (compressor_id);
}

::ZIOP::CompressorFactorySeq *
CompressionManager::get_factories (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
        {
          // todo
          return 0;
        }


  }
}

