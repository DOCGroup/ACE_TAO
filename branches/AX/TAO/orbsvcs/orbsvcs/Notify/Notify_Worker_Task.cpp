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
TAO_Notify_Worker_Task::shutdown (CORBA::Environment&)
{
}

int
TAO_Notify_Worker_Task::process_event (TAO_Notify_Command *mb, CORBA::Environment& ACE_TRY_ENV, ACE_Time_Value * /*tv*/)
{
  int result = mb->execute (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_Message_Block::release (mb);
  return result;
}
