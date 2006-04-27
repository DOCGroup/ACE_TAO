#include "orbsvcs/Time/TAO_UTO.h"
#include "orbsvcs/Time/TAO_TIO.h"
#include "orbsvcs/Time/TAO_Time_Service_Server.h"
#include "ace/OS_NS_sys_time.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"


ACE_RCSID (Time,
           TAO_Time_Service_Server,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  TimeBase::TimeT timestamp;
  ORBSVCS_Time::Absolute_Time_Value_to_TimeT(timestamp, ACE_OS::gettimeofday());

  // Return the local time of the system as a UTO.
  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (timestamp,
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

TAO_END_VERSIONED_NAMESPACE_DECL
