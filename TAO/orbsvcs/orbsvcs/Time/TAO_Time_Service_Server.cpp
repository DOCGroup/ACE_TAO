#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Time/TAO_UTO.h"
#include "orbsvcs/Time/TAO_TIO.h"
#include "orbsvcs/Time/TAO_Time_Service_Server.h"
#include "ace/OS_NS_sys_time.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Returns the current system time and an estimate of inaccuracy in a UTO.
CosTime::UTO_ptr
TAO_Time_Service_Server::universal_time ()
{
  TAO_UTO * uto {};

  TimeBase::TimeT timestamp {};
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

/// Returns a UTO for the current time only when it can be guaranteed to
/// have been obtained securely. This operation is not implemented.
CosTime::UTO_ptr
TAO_Time_Service_Server::secure_universal_time ()
{
  throw CORBA::NO_IMPLEMENT ();
}

/// Creates a new UTO from the given parameters.
CosTime::UTO_ptr
TAO_Time_Service_Server::new_universal_time (TimeBase::TimeT time,
                                             TimeBase::InaccuracyT inaccuracy,
                                             TimeBase::TdfT tdf)
{
  TAO_UTO * uto {};

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (time,
                             inaccuracy,
                             tdf),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = uto;

  return uto->_this ();
}

/// Creates a new UTO from a UtcT value.
CosTime::UTO_ptr
TAO_Time_Service_Server::uto_from_utc (TimeBase::UtcT const & utc)
{
  TAO_UTO * uto {};

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (utc.time,
                             utc.inacclo + utc.inacchi,
                             utc.tdf),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = uto;

  return uto->_this ();
}

/// Creates a new TIO from the given interval bounds.
CosTime::TIO_ptr
TAO_Time_Service_Server::new_interval (TimeBase::TimeT lower,
                                       TimeBase::TimeT upper)
{
  TAO_TIO * tio {};

  ACE_NEW_THROW_EX (tio,
                    TAO_TIO (lower,
                             upper),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = tio;
  return tio->_this ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
