//
// $Id$
//
#include "Memory_Growth.h"
#include "ace/Synch.h"

ACE_RCSID (IORSize,
           Memory_Growth,
           "$Id$")

Memory_Growth::Memory_Growth (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Memory_Growth::send_objref (const Test::PayLoad & /*p*/
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Manual_Event me;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Going to wait .. \n"));

  me.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Wokeup from wait .. \n"));
  return;
}

void
Memory_Growth::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
