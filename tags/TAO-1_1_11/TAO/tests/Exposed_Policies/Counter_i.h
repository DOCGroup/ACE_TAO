//$Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Exposed_Policies
//
// = FILENAME
//    Counter_i.h
//
// = DESCRIPTION
//     This class implements the Counter IDL interface.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef COUNTER_I_H_
#define COUNTER_I_H_

// -- App. Specific Include --
#include "CounterS.h"
#include "Policy_Tester.h"


class Counter_Servant : public POA_Counter
{
public:

  // = Ctor-Dtor Declaration

  Counter_Servant (Policy_Tester *policy_tester);
  virtual ~Counter_Servant (void);

  // = Counter Interface Methods Overloading.

  virtual void increment (CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Long get_count (CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void reset (CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  CORBA::Long count_;
  Policy_Tester *policy_tester_;
};

#endif /*COUNTER_I_H_*/
