// $Id$

#include "tao/corba.h"
#include "eh_i.h"

// CTOR
EventHandler_i::EventHandler_i (void)
{
}

// DTOR
EventHandler_i::~EventHandler_i (void)
{
}

// Return a long
CORBA::Long
EventHandler_i::peer (CORBA::Environment &env)
{
  // Doesn't matter what value we return!
  CORBA::Long val = 6;
  
  ACE_DEBUG ((LM_DEBUG,
              "EventHandler_i::peer() returning %d\n",
              val));
  
  return val;
}

CORBA::UShort
EventHandler_i::decrement (Reactor_ptr eh,
                      CORBA::UShort num,
                      CORBA::Environment &env)
{
  CORBA::UShort ret;
  if (--num <= 0)
    ret = 0;
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) EventHandler::decrement() invoking Reactor::decrement(%d)\n", num));
      ret = eh->decrement (_this (env), num, env);
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) EventHandler::decrement() returning %d\n", ret));
  return ret;
}

