// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI_Timeouts
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Implements the CORBA object
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "timeout_i.h"


Timeout_i::Timeout_i (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
};

Timeout_i::~Timeout_i ()
{

};

void
Timeout_i::sendTimeToWait (CORBA::Long msec,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG, 
              "Timeout_i::sendTimeToWait: invoked with msec = %d\n\n",
              msec));

  if (msec != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Timeout_i::sendTimeToWait: sleeping\n\n"));

      ACE_Time_Value tv (0, (msec * 2) * 1000);
	    ACE_OS::sleep (tv);
    }
};

void
Timeout_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  orb_->shutdown (false);
  ACE_DEBUG ((LM_DEBUG,
              "Timeout_i::shutdown: shut down ORB\n\n"));
}



// Reply Handler implementation

TimeoutHandler_i::TimeoutHandler_i ()
{

};

TimeoutHandler_i::~TimeoutHandler_i ()
{

};

void
TimeoutHandler_i::sendTimeToWait (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeoutHandler_i::sendTimeToWait: invoked\n\n"));

};

void
TimeoutHandler_i::sendTimeToWait_excep (AMI_TimeoutExceptionHolder * excep_holder,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeoutHandler_i::sendTimeToWait_excep: invoked\n\n"));

};
