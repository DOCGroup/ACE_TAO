// $Id$

#include "tao/corba.h"
#include "reactor_i.h"

ACE_RCSID(Reactor, reactor_i, "$Id$")

// CTOR
Reactor_i::Reactor_i (void)
{
}

// DTOR
Reactor_i::~Reactor_i (void)
{
}

// register...with nothing
CORBA::Long
Reactor_i::register_handler (EventHandler_ptr eh,
                             CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Reactor_i::register_handler ()\n"));

  CORBA::Long r = 0;

  TAO_TRY
    {
      r = eh->peer (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("getting peer");
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) got this value from peer: %d\n",
              r));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Reactor_i::register_handler ()\n"));
  return 0;
}

void
Reactor_i::set_value (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) doing Reactor_i::set_value()\n"));
}

CORBA::UShort
Reactor_i::decrement (EventHandler_ptr eh,
                      CORBA::UShort num,
                      CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "%{%I(%P|%t) Reactor::decrement (%d)%$", num));

  CORBA::UShort ret;
  if (--num <= 0)
    ret = 0;
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reactor::decrement() invoking EventHandler::decrement(%d)%$", num));
      Reactor_var me = _this (env);
      ret = eh->decrement (me.in (), num, env);
    }
  ACE_DEBUG ((LM_DEBUG, "%}(%P|%t) Reactor::decrement() returning %d\n", ret));
  return ret;
}

void
Reactor_i::stop (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) stopping.\n"));
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
