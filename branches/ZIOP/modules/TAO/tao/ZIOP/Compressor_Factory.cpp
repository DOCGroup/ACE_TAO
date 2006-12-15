#include "Compressor_Factory.h"

namespace TAO
{
  namespace ZIOP
  {

::ZIOP::CompressorId
CompressorFactory::compressor_id (
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
{
    return compressor_id_;
}

::CORBA::ULongLong
CompressorFactory::compressed_bytes (void)
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

::CORBA::ULongLong
CompressorFactory::uncompressed_bytes (
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

::CORBA::Double
CompressorFactory::average_compression (void)
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

void
CompressorFactory::add_sample (
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


  }
}

