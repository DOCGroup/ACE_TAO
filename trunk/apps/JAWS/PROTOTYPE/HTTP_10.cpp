// $Id$

#include "JAWS/HTTP_10.h"

JAWS_HTTP_10_Read_Task::JAWS_HTTP_10_Read_Task (void)
{
}

JAWS_HTTP_10_Read_Task::~JAWS_HTTP_10_Read_Task (void)
{
}

int
JAWS_HTTP_10_Read_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_Dispatch_Policy *policy = data->policy ();
  JAWS_IO_Handler *handler = data->handler ();
  JAWS_Pipeline_Handler *task = data->task ();

  JAWS_IO *io = policy->io ();
}
