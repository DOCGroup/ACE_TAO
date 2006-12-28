#include "tao/Compression/Compressor_Factory.h"

ACE_RCSID (Compression,
           Compressor_Factory,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
CompressorFactory::CompressorFactory (::Compression::CompressorId compressor_id) :
  compressor_id_ (compressor_id),
  compressed_bytes_ (0),
  uncompressed_bytes_ (0)
{
}

::Compression::CompressorId
CompressorFactory::compressor_id (void)
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
  ::CORBA::Double return_value = 0.0;
  {
    ACE_Guard <ACE_SYNCH_MUTEX> guard (mutex_);
    if (this->uncompressed_bytes_ > 0)
      {
        return_value = (this->uncompressed_bytes_ - this->compressed_bytes_) / this->uncompressed_bytes_;
      }
  }
  return return_value;
}

void
CompressorFactory::add_sample (
  ::CORBA::ULongLong compressed_bytes,
  ::CORBA::ULongLong uncompressed_bytes)
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ))
{
  ACE_Guard <ACE_SYNCH_MUTEX> guard (mutex_);
  this->compressed_bytes_ += compressed_bytes;
  this->uncompressed_bytes_ += uncompressed_bytes;
}
}

TAO_END_VERSIONED_NAMESPACE_DECL
