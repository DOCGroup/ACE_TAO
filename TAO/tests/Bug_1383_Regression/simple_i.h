#ifndef SIMPLE_I_H
#define SIMPLE_I_H

#include "tao/corba.h"
#include "ace/Event_Handler.h"

#include "simpleS.h"
#include "calleeC.h"

class Simple_i : public POA_Simple, public ACE_Event_Handler
{
public:
  explicit Simple_i (CORBA::ORB_ptr orb);
  Simple_i (Simple_i const &) = delete;
  Simple_i &operator= (Simple_i const &) = delete;
  Simple_i (Simple_i &&) = delete;
  Simple_i &operator= (Simple_i &&) = delete;

  void registerCallee (Callee_ptr value) override;
  int handle_timeout (ACE_Time_Value const &current_time, void const *data) override;

private:
  CORBA::ORB_ptr m_orb;
  Callee_var     m_callee;
};

#endif
