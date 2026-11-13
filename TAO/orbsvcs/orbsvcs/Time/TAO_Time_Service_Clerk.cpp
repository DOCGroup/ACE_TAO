// -*- C++ -*-
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Time/TAO_Time_Service_Clerk.h"
#include "orbsvcs/Time/TAO_TIO.h"
#include "orbsvcs/Time/TAO_UTO.h"

#include "tao/ORB_Core.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Time_Service_Clerk::TAO_Time_Service_Clerk (int timer_value,
                                                int timer_value_usecs,
                                                IORS const & servers)
  : server_ (servers),
    helper_ (this)
{
  // Schedule the helper to be invoked by the reactor
  // periodically.

  if (TAO_ORB_Core_instance ()->reactor ()->schedule_timer
      (&helper_,
       nullptr,
       ACE_Time_Value::zero,
       ACE_Time_Value (timer_value, timer_value_usecs)) == -1)
    ORBSVCS_ERROR ((LM_ERROR,
                "%p\n",
                "schedule_timer ()"));
}

/// Returns the global time and an estimate of inaccuracy in a UTO.
CosTime::UTO_ptr
TAO_Time_Service_Clerk::universal_time ()
{
  TAO_UTO * uto {};

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (this->get_time (),
                             this->inaccuracy (),
                             this->time_displacement_factor ()),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = uto;
  // Return the global time as a UTO.

  return uto->_this ();
}

/// Returns a UTO for global time only when it can be guaranteed to have
/// been obtained securely. This operation is not implemented.
CosTime::UTO_ptr
TAO_Time_Service_Clerk::secure_universal_time ()
{
  throw CORBA::NO_IMPLEMENT ();
}

/// Creates a new UTO from the given parameters.
CosTime::UTO_ptr
TAO_Time_Service_Clerk::new_universal_time (TimeBase::TimeT time,
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
TAO_Time_Service_Clerk::uto_from_utc (TimeBase::UtcT const & utc)
{
  TAO_UTO * uto {};

  // Use the low and high values of inaccuracy
  // to calculate the total inaccuracy.

  TimeBase::InaccuracyT inaccuracy = utc.inacchi;
  inaccuracy <<= 32;
  inaccuracy |= utc.inacclo;

  ACE_NEW_THROW_EX (uto,
                    TAO_UTO (utc.time,
                             inaccuracy,
                             utc.tdf),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var xfer = uto;
  return uto->_this ();
}

/// Creates a new TIO from the given interval bounds.
CosTime::TIO_ptr
TAO_Time_Service_Clerk::new_interval (TimeBase::TimeT lower,
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

CORBA::ULongLong
TAO_Time_Service_Clerk::get_time () const
{
  // Globally sync. time is the latest global time plus the time
  // elapsed since last updation was done.

  ACE_Time_Value const timeofday = ACE_OS::gettimeofday ();

  return static_cast<CORBA::ULongLong> (timeofday.sec ()) *
         static_cast<ACE_UINT32> (10000000) +
         static_cast<CORBA::ULongLong> (timeofday.usec () * 10)
    - this->update_timestamp_
    + this->time_;
}

/// Returns the time displacement factor in minutes from GMT.
CORBA::Short
TAO_Time_Service_Clerk::time_displacement_factor () const
{
  return time_displacement_factor_;
}

// Sets the TDF.
void
TAO_Time_Service_Clerk::time_displacement_factor (CORBA::Short tdf)
{
  this->time_displacement_factor_ = tdf;
}

/// Returns the time inaccuracy.
TimeBase::InaccuracyT
TAO_Time_Service_Clerk::inaccuracy () const
{
  return this->inaccuracy_;
}

/// Sets the time inaccuracy.
void
TAO_Time_Service_Clerk::inaccuracy (TimeBase::InaccuracyT inaccuracy)
{
  this->inaccuracy_ = inaccuracy;
}

TAO_END_VERSIONED_NAMESPACE_DECL
