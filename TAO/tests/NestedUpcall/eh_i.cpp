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
