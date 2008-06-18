// $Id$

#ifndef SIMPLE_I_H
#define SIMPLE_I_H

#include "tao/corba.h"
#include "ace/Event_Handler.h"

#include "simpleS.h"
#include "calleeC.h"

class Simple_i : public POA_Simple, ACE_Event_Handler
{
public:
  Simple_i(CORBA::ORB_ptr orb);
  void registerCallee(Callee_ptr value);
  int handle_timeout(const ACE_Time_Value & currentTime, const void * data);

private:

  CORBA::ORB_ptr m_orb;
  Callee_var     m_callee;
};

#endif
