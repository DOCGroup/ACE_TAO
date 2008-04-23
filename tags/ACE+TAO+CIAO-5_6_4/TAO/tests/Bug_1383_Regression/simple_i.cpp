// $Id$

#include "simple_i.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

Simple_i::Simple_i(CORBA::ORB_ptr orb)
	: m_orb(orb->_duplicate(orb))
{
}

void Simple_i::registerCallee(Callee_ptr value)
{
  ACE_DEBUG ((LM_DEBUG, "registerCallee invoked\n"));
	// retain a reference so we can callback later
	m_callee = value->_duplicate(value);

	// register a callback with the reactor so we can invoke the
	// callback method
  m_orb->orb_core()->reactor()->schedule_timer(this, 0, ACE_Time_Value(5));
}

int Simple_i::handle_timeout(
  const ACE_Time_Value &,
  const void *
	)
{
  ACE_DEBUG ((LM_DEBUG, "timer expired\n"));
	m_callee->callback("got timeout");
	return 0;
}
