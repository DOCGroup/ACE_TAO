// $Id$

#include "HTTP_10.h"

#include "JAWS/JAWS.h"
#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

ACE_RCSID(PROTOTYPE, HTTP_10_Read, "$Id$")

// --------------- READ TASK ----------------------

JAWS_HTTP_10_Read_Task::JAWS_HTTP_10_Read_Task (void)
{
}

JAWS_HTTP_10_Read_Task::~JAWS_HTTP_10_Read_Task (void)
{
}

int
JAWS_HTTP_10_Read_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put");

  JAWS_Dispatch_Policy *policy = data->policy ();
  JAWS_IO_Handler *handler = data->io_handler ();
  // JAWS_Pipeline_Handler *task = data->task ();

  JAWS_IO *io = policy->io ();

  if (data->length () > 0)
    {
      JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put, have data");
      return 0;
    }

  io->read (handler, data, data->size ());
  if (handler->status () == JAWS_IO_Handler::READ_OK)
    {
      JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put, READ_OK");
      return 0;
    }

  return 1;
}
