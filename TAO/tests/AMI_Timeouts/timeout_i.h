// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI_Timeouts
//
// = FILENAME
//    timeout_i.h
//
// = DESCRIPTION
//    Implements the CORBA object.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================


#include "tao/corba.h"

#include "timeoutS.h"

class Timeout_i
: public POA_Timeout
{
public:
  Timeout_i (CORBA::ORB_ptr orb);

  ~Timeout_i ();

  virtual void sendTimeToWait (CORBA::Long msec,
                               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_var orb_;
};

class TimeoutHandler_i
: public POA_AMI_TimeoutHandler
{
public:
  TimeoutHandler_i ();

  ~TimeoutHandler_i ();

  virtual void sendTimeToWait (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void sendTimeToWait_excep (
      AMI_TimeoutExceptionHolder * excep_holder,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

};
