// $Id$

#include "tao/Compression/Compression_Manager.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  void
  CompressionManager::register_factory (::Compression::CompressorFactory_ptr
                                        compressor_factory)
  {
    if (!::CORBA::is_nil (compressor_factory))
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
        CORBA::ULong const length = this->factories_.length ();

        for (CORBA::ULong i = 0; i < length; ++i)
          {
            ::Compression::CompressorId const current =
              this->factories_[i]->compressor_id ();

            if (compressor_factory->compressor_id () == current)
              {
                throw ::Compression::FactoryAlreadyRegistered ();
              }
          }
        factories_.length (length + 1);
        factories_[length] = ::Compression::CompressorFactory::_duplicate (
          compressor_factory);
      }
    else
      {
        // @todo Standardize this minor code
        throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 44, CORBA::COMPLETED_NO);
      }
  }

  void
  CompressionManager::unregister_factory (::Compression::CompressorId
                                          compressor_id)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    CORBA::ULong const length = this->factories_.length ();

    for (CORBA::ULong i = 0; i < length; ++i)
      {
        ::Compression::CompressorId const current =
          this->factories_[i]->compressor_id ();

        if (current != compressor_id)
          {
            continue;
          }

        this->factories_[i] = ::Compression::CompressorFactory::_nil ();
        // make sequence smaller
        return;
      }

    // todo exception
    throw ::Compression::UnknownCompressorId ();
  }

  ::Compression::CompressorFactory_ptr
  CompressionManager::get_factory (::Compression::CompressorId compressor_id)
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_,
            ::Compression::CompressorFactory::_nil ());

    CORBA::ULong const length = this->factories_.length ();

    for (CORBA::ULong i = 0; i < length; ++i)
      {
        ::Compression::CompressorId const current =
          this->factories_[i]->compressor_id ();

        if (current != compressor_id)
          {
            continue;
          }

        return ::Compression::CompressorFactory::_duplicate (
          this->factories_[i]);
      }

    throw ::Compression::UnknownCompressorId ();
  }

  ::Compression::Compressor_ptr
  CompressionManager::get_compressor (::Compression::CompressorId
                                      compressor_id,
                                      ::Compression::CompressionLevel
                                      compression_level)
  {
    ::Compression::CompressorFactory_var factory = this->get_factory (
      compressor_id);

    return factory->get_compressor (compression_level);
  }

  ::Compression::CompressorFactorySeq *
  CompressionManager::get_factories (void)
  {
    // todo
    return 0;
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
