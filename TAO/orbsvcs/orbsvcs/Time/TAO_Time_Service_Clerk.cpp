// -*- C++ -*-
// $Id$

#include "TAO_Time_Service_Clerk.h"
#include "TAO_TIO.h"
#include "TAO_UTO.h"

// Constructor.
TAO_Time_Service_Clerk::TAO_Time_Service_Clerk (int timer_value,
						const IORS& servers)
  : server_ (servers),
    helper_ (this)
{
  // Schedule the helper to be invoked by the reactor
  // periodically.

  if (TAO_ORB_Core_instance ()->reactor ()->schedule_timer
      (&helper_,
       0,
       ACE_Time_Value::zero,
       ACE_Time_Value (timer_value)) == -1)
    ACE_ERROR ((LM_ERROR,
		"%p\n",
		"schedule_timer ()"));
}

// Destructor.

TAO_Time_Service_Clerk::~TAO_Time_Service_Clerk (void)
{
}

// This method returns the global time and an estimate of inaccuracy
// in a UTO.

CosTime::UTO_ptr
TAO_Time_Service_Clerk::universal_time (CORBA::Environment &TAO_IN_ENV)
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_RETURN (uto,
			TAO_UTO (this->get_time (),
				 0,
				 0),
			CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			CosTime::UTO::_nil ());
  // Return the global time as a UTO.

  return uto->_this ();
}

// This method returns the global time in a UTO only if the time can
// be guaranteed to have been obtained securely.  This method is not
// implemented currently.

CosTime::UTO_ptr
TAO_Time_Service_Clerk::secure_universal_time (CORBA::Environment &env)
{
  env.exception (new CORBA::NO_IMPLEMENT (CORBA::COMPLETED_NO));
  return 0;
}

// This creates a new UTO based on the given parameters.

CosTime::UTO_ptr
TAO_Time_Service_Clerk::new_universal_time (TimeBase::TimeT time,
					    TimeBase::InaccuracyT inaccuracy,
					    TimeBase::TdfT tdf,
					    CORBA::Environment &TAO_IN_ENV)
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_RETURN (uto,
			TAO_UTO (time,
				 inaccuracy,
				 tdf),
			CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			CosTime::UTO::_nil ());
  return uto->_this ();
}

// This creates a new UTO given a time in the UtcT form.

CosTime::UTO_ptr
TAO_Time_Service_Clerk::uto_from_utc (const TimeBase::UtcT &utc,
				      CORBA::Environment &TAO_IN_ENV)
{
  TAO_UTO *uto = 0;

  ACE_NEW_THROW_RETURN (uto,
			TAO_UTO (utc.time,
				 utc.inacclo + utc.inacchi,
				 utc.tdf),
			CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			CosTime::UTO::_nil ());
  return uto->_this ();
}

// This creates a new TIO with the given parameters.

CosTime::TIO_ptr
TAO_Time_Service_Clerk::new_interval (TimeBase::TimeT lower,
				      TimeBase::TimeT upper,
				      CORBA::Environment &TAO_IN_ENV)
{
  TAO_TIO *tio = 0;

  ACE_NEW_THROW_RETURN (tio,
			TAO_TIO (lower,
				 upper),
			CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			CosTime::TIO::_nil ());
  return tio->_this ();
}

CORBA::ULongLong
TAO_Time_Service_Clerk::get_time (void)
{
  // Globally sync. time is the latest global time plus the time
  // elapsed since last updation was done.
  CORBA::ULongLong time;

  time = (CORBA::ULongLong) (ACE_OS::gettimeofday ().sec () * 10000000
                             + ACE_OS::gettimeofday ().usec () * 10)
    - this->update_timestamp_ + this->time_;

  return time;
}
