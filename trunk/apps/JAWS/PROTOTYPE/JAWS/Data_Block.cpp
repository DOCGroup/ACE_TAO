// $Id$

#include "JAWS/Data_Block.h"
#include "JAWS/Policy.h"

JAWS_Pipeline_Handler *
JAWS_Data_Block::task (void)
{
  return this->task_;
}

JAWS_IO_Handler *
JAWS_Data_Block::io_handler (void)
{
  return this->io_handler_;
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
JAWS_Data_Block::io_handler (JAWS_IO_Handler *handlerp)
{
  this->io_handler_ = handlerp;
}

void
JAWS_Data_Block::policy (JAWS_Dispatch_Policy *policyp)
{
  this->policy_ = policyp;
}
