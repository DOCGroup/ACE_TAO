// $Id$

#include "tao/corba.h"
#include "eh_i.h"

ACE_RCSID(Reactor, eh_i, "$Id$")

// CTOR
EventHandler_i::EventHandler_i (int q)
  :  quiet_ (q)
{
}

// DTOR
EventHandler_i::~EventHandler_i (void)
{
}

// Return a long
CORBA::Long
EventHandler_i::peer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Doesn't matter what value we return!
  CORBA::Long val = 6;

  if (!this->quiet_)
    ACE_DEBUG ((LM_DEBUG,
                "EventHandler_i::peer() returning %d\n",
                val));

  return val;
}

CORBA::UShort
EventHandler_i::decrement (Reactor_ptr eh,
                           CORBA::UShort num,
                           CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->quiet_ == 0)
    {
      ACE_ASSERT (this->quiet_ == 0);
      ACE_DEBUG ((LM_DEBUG,
                  "%{%I(%P|%t) EventHandler::decrement (%d)%$",
                  num, this->quiet_));
    }

  CORBA::UShort ret;
  if (--num <= 0)
    ret = 0;
  else
    {
      if (!this->quiet_)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) EventHandler::decrement() "
                    "invoking Reactor::decrement(%d) %d %$",
                    num, this->quiet_));

      ret = eh->decrement (_this (env), num, env);
    }

  if (this-quiet_ == 0)
    {
      ACE_ASSERT (this->quiet_ == 0);
      ACE_DEBUG ((LM_DEBUG,
                  "%}(%P|%t) EventHandler::decrement() "
                  "returning %d%$", ret));
    }

  return ret;
}
