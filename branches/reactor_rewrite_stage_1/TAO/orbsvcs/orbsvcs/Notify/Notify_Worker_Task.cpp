// $Id$

#include "Notify_Worker_Task.h"
#include "Notify_Command.h"

ACE_RCSID(Notify, Notify_Worker_Task, "$Id$")

TAO_Notify_Worker_Task::TAO_Notify_Worker_Task (void)
{
}

TAO_Notify_Worker_Task::~TAO_Notify_Worker_Task ()
{
}

int
TAO_Notify_Worker_Task::init_task (TAO_Notify_AdminProperties* const)
{
  return 0;
}

void
TAO_Notify_Worker_Task::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
}

int
TAO_Notify_Worker_Task::process_event (TAO_Notify_Command *mb TAO_ENV_ARG_DECL, ACE_Time_Value * /*tv*/)
{
  int result = mb->execute (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_Message_Block::release (mb);
  return result;
}
