
//=============================================================================
/**
 *  @file    Counter_i.h
 *
 *   This class implements the Counter IDL interface.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


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
  virtual ~Counter_Servant () = default;

  // = Counter Interface Methods Overloading.

  virtual void increment ();

  virtual CORBA::Long get_count ();

  virtual void reset ();

  virtual void shutdown ();

protected:
  CORBA::Long count_;
  Policy_Tester *policy_tester_;
};

#endif /*COUNTER_I_H_*/
