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

#include "CounterS.h"

class Counter_Servant : public POA_Counter
{
public:

  // = Ctor-Dtor Declaration

  Counter_Servant (CORBA::ORB_ptr o);
  virtual ~Counter_Servant (void);

  // = Counter Interface Methods Overloading.

  virtual void increment (CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ());

  virtual CORBA::Long get_count (CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ());

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  CORBA::Long count_;
  CORBA::ORB_var orb_;
};

#endif /*COUNTER_I_H_*/
