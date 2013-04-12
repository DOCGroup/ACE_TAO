// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Time/TAO_UTO.h"
#include "orbsvcs/Time/TAO_TIO.h"
#include "orbsvcs/Time/TAO_Time_Service_Server.h"
#include "ace/OS_NS_sys_time.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"

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
TAO_Time_Service_Server::universal_time (void)
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
  PortableServer::ServantBase_var xfer = uto;

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "Returning a UTO\n"));

  return uto->_this ();
}

// This method returns the current time in a UTO only if the time can
// be guaranteed to have been obtained securely.  This method is not
// implemented currently.

CosTime::UTO_ptr
TAO_Time_Service_Server::secure_universal_time (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

// This creates a new UTO based on the given parameters.

CosTime::UTO_ptr
TAO_Time_Service_Server::new_universal_time (TimeBase::TimeT time,
                                             TimeBase::InaccuracyT inaccuracy,
                                             TimeBase::TdfT tdf)
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (time,
                             inaccuracy,
                             tdf),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = uto;

  return uto->_this ();
}

// This creates a new UTO given a time in the UtcT form.

CosTime::UTO_ptr
TAO_Time_Service_Server::uto_from_utc (const TimeBase::UtcT &utc)
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (utc.time,
                             utc.inacclo + utc.inacchi,
                             utc.tdf),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = uto;

  return uto->_this ();
}

// This creates a new TIO with the given parameters.

CosTime::TIO_ptr
TAO_Time_Service_Server::new_interval (TimeBase::TimeT lower,
                                       TimeBase::TimeT upper)
{
  TAO_TIO *tio = 0;

  ACE_NEW_THROW_EX (tio,
                    TAO_TIO (lower,
                             upper),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = tio;
  return tio->_this ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
