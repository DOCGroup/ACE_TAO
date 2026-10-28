#ifndef TAO_BUG_2494_REGRESSION_TEST_I_H
#define TAO_BUG_2494_REGRESSION_TEST_I_H

#include "testS.h"
#include "ace/Synch.h"

/**
 * Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server, public ACE_Event_Handler
{
public:
  Simple_Server_i (CORBA::ORB_ptr orb);
  // ctor

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  char *test_method (Simple_Server_ptr objref) override;

  void client_done () override;

  void shutdown () override;

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *act = nullptr) override;

private:
  CORBA::ORB_var orb_;
  // The ORB
  TAO_SYNCH_MUTEX     lock_;
  TAO_SYNCH_CONDITION cond_;
  bool                timer_registed_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BUG_2494_REGRESSION_TEST_I_H */
