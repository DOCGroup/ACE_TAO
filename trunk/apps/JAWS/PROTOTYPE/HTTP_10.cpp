// $Id$

#include "HTTP_10.h"

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

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
  JAWS_IO_Handler *handler = data->io_handler ();
  // JAWS_Pipeline_Handler *task = data->task ();

  JAWS_IO *io = policy->io ();

  ACE_Message_Block mb (data);

  io->read (handler, mb, data->size ());
  if (handler->status () == JAWS_IO_Handler::READ_OK)
    {
      return this->put_next (&mb);
    }

  return 1;
}

JAWS_HTTP_10_Write_Task::JAWS_HTTP_10_Write_Task (void)
{
}

JAWS_HTTP_10_Write_Task::~JAWS_HTTP_10_Write_Task (void)
{
}

int
JAWS_HTTP_10_Write_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_Dispatch_Policy *policy = data->policy ();
  JAWS_IO_Handler *handler = data->io_handler ();
  //  JAWS_Pipeline_Handler *task = data->task ();

  JAWS_IO *io = policy->io ();

  char message[] = "<html><h1>This is a test</h1></html>\n";

  ACE_Message_Block mb (data);

  io->send_error_message (handler, message, sizeof (message));
  if (handler->status () == JAWS_IO_Handler::WRITE_OK)
    {
      return 0;
    }

  return -1;
}
