#include "TAO_UTO.h"
#include "TAO_TIO.h"
#include "TAO_Time_Service_Server.h"

#include "tao/debug.h"


ACE_RCSID (Time,
           TAO_Time_Service_Server,
           "$Id$")


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
TAO_Time_Service_Server::universal_time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable))
{
  TAO_UTO *uto = 0;

  // UNIX systems use 1st Jan. 1970 as the Base Time. The CORBA Time
  // Service uses the Universal Time Coordinated (UTC) representation
  // of time from the X/Open DCE Time Service. The UTC time signals
  // broadcast by the WWV radio station of the National Bureau of
  // Standards deliver time that is easier to handle in this
  // representation. UTC time is defined as :
  //
  //  Time Units : 100 nanosecs
  //  Base Time  : 15th October 1582 00:00:00
  //  Approximate range : AD 30,000
  //
  //  To construct the UTC time from UNIX time we need to add the
  //  difference of days between 15th October 1582 and 1st Jan
  //  1970. This difference is 141427 days or 0x2D8539C80 secs.

#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::ULongLong TAO_Time_Base_Offset (0xD8539C80, 2);
  // (Lower 32 bits of the offset in hex, Upper 32 bits of the offset in hex)
#else
  CORBA::ULongLong TAO_Time_Base_Offset = ACE_UINT64_LITERAL (0x2D8539C80);
#endif

  const ACE_Time_Value timeofday = ACE_OS::gettimeofday ();

  // Return the local time of the system as a UTO.
  ACE_NEW_THROW_EX (uto,
                    TAO_UTO ((TAO_Time_Base_Offset +
                              ACE_static_cast (CORBA::ULongLong,
                                               timeofday.sec ())) *
                             ACE_static_cast (ACE_UINT32,
                                              10000000) +
                             ACE_static_cast  (CORBA::ULongLong,
                                               timeofday.usec () * 10),
                             0,
                             0),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CosTime::UTO::_nil ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Returning a UTO\n"));

  return uto->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// This method returns the current time in a UTO only if the time can
// be guaranteed to have been obtained securely.  This method is not
// implemented currently.

CosTime::UTO_ptr
TAO_Time_Service_Server::secure_universal_time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosTime::UTO::_nil ());
}

// This creates a new UTO based on the given parameters.

CosTime::UTO_ptr
TAO_Time_Service_Server::new_universal_time (TimeBase::TimeT time,
                                             TimeBase::InaccuracyT inaccuracy,
                                             TimeBase::TdfT tdf
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (time,
                             inaccuracy,
                             tdf),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosTime::UTO::_nil ());

  return uto->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// This creates a new UTO given a time in the UtcT form.

CosTime::UTO_ptr
TAO_Time_Service_Server::uto_from_utc (const TimeBase::UtcT &utc
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (utc.time,
                             utc.inacclo + utc.inacchi,
                             utc.tdf),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosTime::UTO::_nil ());

  return uto->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// This creates a new TIO with the given parameters.

CosTime::TIO_ptr
TAO_Time_Service_Server::new_interval (TimeBase::TimeT lower,
                                       TimeBase::TimeT upper
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TIO *tio = 0;

  ACE_NEW_THROW_EX (tio,
                    TAO_TIO (lower,
                             upper),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosTime::TIO::_nil ());

  return tio->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}









