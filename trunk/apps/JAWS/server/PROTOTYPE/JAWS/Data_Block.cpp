// $Id$

#include "JAWS/Data_Block.h"
#include "JAWS/Policy.h"

JAWS_Pipeline_Task *
JAWS_Data_Block::task (void)
{
  return this->task_;
}

ACE_INET_Addr *
JAWS_Data_Block::addr (void)
{
  return this->addr_;
}

JAWS_IO_Handler *
JAWS_Data_Block::handler (void)
{
  return this->handler_;
}

JAWS_Dispatch_Policy *
JAWS_Data_Block::policy (void)
{
  return this->policy_;
}

void
JAWS_Data_Block::task (JAWS_Pipeline_Handler *taskp)
{
  this->task_ = taskp;
}

void
JAWS_Data_Block::addr (ACE_INET_Addr *addrp)
{
  this->addr_ = addrp;
}

void
JAWS_Data_Block::handler (JAWS_IO_Handler *handlerp)
{
  this->handler_ = handlerp;
}

void
JAWS_Data_Block::policy (JAWS_Dispatch_Policy *policyp)
{
  this->policy_ = policyp;
}

int
JAWS_Pipeline_Accept_Task::handle_put (JAWS_Data_Block *data,
                                       ACE_Time_Value *)
{
  /* JAWS_Data_Block should contain an INET_Addr and an IO */
  // JAWS_IO_Handler *handler = data->handler ();
  JAWS_Dispatch_Policy *policy = data->policy ();

  // data->policy ()->update (handler);

  JAWS_IO *io = policy->io ();
  io->accept (data->addr ());
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
