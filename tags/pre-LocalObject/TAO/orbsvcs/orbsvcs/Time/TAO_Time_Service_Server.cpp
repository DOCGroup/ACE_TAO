// -*- C++ -*-
// $Id$

#include "TAO_UTO.h"
#include "TAO_TIO.h"
#include "TAO_Time_Service_Server.h"

// Constructor.
TAO_Time_Service_Server::TAO_Time_Service_Server (void)
{
}

// Destructor.
TAO_Time_Service_Server::~TAO_Time_Service_Server (void)
{
}

// This method returns the current system time and an estimate of
// inaccuracy in a UTO.

CosTime::UTO_ptr
TAO_Time_Service_Server::universal_time (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable))
{
  TAO_UTO *uto = 0;

  // This is the difference in seconds between
  // 15th October 1582 and 1st Jan 1970. It needs to be added to the system
  // time to make the base time as 15th October 1582. The decimal value of
  // the offset is 1.221929 * 10^10.

#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::ULongLong TAO_Time_Base_Offset (0xD8539190, 2);
  // (Lower 32 bits of the offset in hex, Upper 32 bits of the offset in hex)
#else
  CORBA::ULongLong TAO_Time_Base_Offset = ACE_UINT64_LITERAL(0x2D8539190);
#endif

  // Return the local time of the system as a UTO.
  ACE_NEW_THROW_EX (uto,
                    TAO_UTO ((TAO_Time_Base_Offset +
                              ACE_static_cast(CORBA::ULongLong,
                                              ACE_OS::gettimeofday ().sec ())) *
                             ACE_static_cast(ACE_UINT32,
                                             10000000) +
                             ACE_static_cast(CORBA::ULongLong,
                                             ACE_OS::gettimeofday ().usec () * 10),
                             0,
                             0),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CosTime::UTO::_nil ());

  ACE_DEBUG ((LM_DEBUG,
              "Returning a UTO\n"));

  return uto->_this ();

}

// This method returns the current time in a UTO only if the time can
// be guaranteed to have been obtained securely.  This method is not
// implemented currently.

CosTime::UTO_ptr
TAO_Time_Service_Server::secure_universal_time (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// This creates a new UTO based on the given parameters.

CosTime::UTO_ptr
TAO_Time_Service_Server::new_universal_time (TimeBase::TimeT time,
                                             TimeBase::InaccuracyT inaccuracy,
                                             TimeBase::TdfT tdf,
                                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (time,
                             inaccuracy,
                             tdf),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosTime::UTO::_nil ());

  return uto->_this ();

}

// This creates a new UTO given a time in the UtcT form.

CosTime::UTO_ptr
TAO_Time_Service_Server::uto_from_utc (const TimeBase::UtcT &utc,
                                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (utc.time,
                             utc.inacclo + utc.inacchi,
                             utc.tdf),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosTime::UTO::_nil ());
  return uto->_this ();
}

// This creates a new TIO with the given parameters.

CosTime::TIO_ptr
TAO_Time_Service_Server::new_interval (TimeBase::TimeT lower,
                                       TimeBase::TimeT upper,
                                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TIO *tio = 0;

  ACE_NEW_THROW_EX (tio,
                    TAO_TIO (lower,
                             upper),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosTime::TIO::_nil ());
  return tio->_this ();
}
