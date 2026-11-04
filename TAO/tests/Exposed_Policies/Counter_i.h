
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
  explicit Counter_Servant (Policy_Tester *policy_tester);
  ~Counter_Servant () override = default;
  Counter_Servant (Counter_Servant const &) = delete;
  Counter_Servant (Counter_Servant &&) = delete;
  Counter_Servant &operator= (Counter_Servant const &) = delete;
  Counter_Servant &operator= (Counter_Servant &&) = delete;

  // = Counter Interface Methods Overloading.

  void increment () override;

  CORBA::Long get_count () override;

  void reset () override;

  void shutdown () override;

protected:
  CORBA::Long count_ {};
  // Borrowed; Policy_Tester outlives the servant.
  Policy_Tester *policy_tester_;
};

#endif /*COUNTER_I_H_*/
