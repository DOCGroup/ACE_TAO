// $Id$

#include "HTTP_10.h"

#include "JAWS/JAWS.h"

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"


// --------------- WRITE TASK ----------------------

JAWS_HTTP_10_Write_Task::JAWS_HTTP_10_Write_Task (void)
{
}

JAWS_HTTP_10_Write_Task::~JAWS_HTTP_10_Write_Task (void)
{
}

int
JAWS_HTTP_10_Write_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put");

  JAWS_Dispatch_Policy *policy = data->policy ();
  JAWS_IO_Handler *handler = data->io_handler ();
  //  JAWS_Pipeline_Handler *task = data->task ();

  JAWS_IO *io = policy->io ();

  JAWS_HTTP_10_Request *info = ACE_static_cast (JAWS_HTTP_10_Request *,
                                                data->payload ());

  ACE_DEBUG ((LM_DEBUG, " (%t) request %s %s %s parsed\n",
              (info->method () ? info->method () : "-"),
              (info->uri () ? info->uri () : "="),
              (info->version () ? info->version () : "HTTP/0.9")));

  char message[] = "<html><h1>This is a test</h1></html>\n";

  io->send_error_message (handler, message, sizeof (message));
  if (handler->status () == JAWS_IO_Handler::WRITE_OK)
    {
      return 0;
    }

  return -1;
}
