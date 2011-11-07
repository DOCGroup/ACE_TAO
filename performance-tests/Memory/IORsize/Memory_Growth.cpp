//
// $Id$
//
#include "Memory_Growth.h"
#include "ace/Manual_Event.h"

Memory_Growth::Memory_Growth (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Memory_Growth::send_objref (const Test::PayLoad & /*p*/
                            )
{
  ACE_Manual_Event me;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Going to wait ..\n"));

  me.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Wokeup from wait ..\n"));
  return;
}

void
Memory_Growth::shutdown (void)
{
  this->orb_->shutdown (0);
}
