// $Id$

#include "ace/RMCast/RMCast_Reliable_Factory.h"
#include "ace/RMCast/RMCast_Receiver_Module.h"
#include "ace/RMCast/RMCast_Reassembly.h"
#include "ace/RMCast/RMCast_Reordering.h"

#if !defined (__ACE_INLINE__)
# include "ace/RMCast/RMCast_Reliable_Factory.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Reliable_Factory, "$Id$")

ACE_RMCast_Reliable_Factory::~ACE_RMCast_Reliable_Factory (void)
{
}

ACE_RMCast_Module*
ACE_RMCast_Reliable_Factory::create (void)
{
  ACE_RMCast_Module *receiver;
  ACE_NEW_RETURN (receiver, ACE_RMCast_Receiver_Module, 0);

  ACE_RMCast_Module *reassembly;
  ACE_NEW_RETURN (reassembly, ACE_RMCast_Reassembly, 0);

  ACE_RMCast_Module *reordering;
  ACE_NEW_RETURN (reordering, ACE_RMCast_Reordering, 0);

  ACE_RMCast_Module *user = this->factory_->create ();
  if (user == 0)
    {
      delete receiver;
      delete reordering;
      delete reassembly;
      return 0;
    }
  receiver->next (reassembly);
  reassembly->next (reordering);
  reordering->next (user);
  return receiver;
}

void
ACE_RMCast_Reliable_Factory::destroy (ACE_RMCast_Module *receiver)
{
  ACE_RMCast_Module *reassembly = receiver->next ();
  ACE_RMCast_Module *reordering = reassembly->next ();
  ACE_RMCast_Module *user = reordering->next ();
  this->factory_->destroy (user);
  delete reordering;
  delete reassembly;
  delete receiver;
}
