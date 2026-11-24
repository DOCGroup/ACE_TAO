#include "HTTP_10.h"

#include "JAWS/JAWS.h"
#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

// --------------- READ TASK ----------------------

JAWS_HTTP_10_Read_Task::JAWS_HTTP_10_Read_Task ()
{
}

JAWS_HTTP_10_Read_Task::~JAWS_HTTP_10_Read_Task () = default;

int
JAWS_HTTP_10_Read_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put");

  JAWS_IO_Handler *handler_ptr = data->io_handler ();
  JAWS_Dispatch_Policy *dispatch_policy = this->policy ();
  if (dispatch_policy == 0)
    {
      dispatch_policy = data->policy ();
    }

  JAWS_IO *io_ptr = dispatch_policy->io ();

  if (data->length () > 0)
    {
      if (data->rd_ptr ()[0] != '\0')
        {
          JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put, have data");
          return 0;
        }
    }
  data->rd_ptr (data->wr_ptr ());
  data->crunch ();

  io_ptr->read (handler_ptr, data, data->size ());
  switch (handler_ptr->status ())
    {
    case JAWS_IO_Handler::READ_OK:
      {
        JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put, READ_OK");
        return 0;
      }
      break;
    case JAWS_IO_Handler::READ_ERROR:
    case JAWS_IO_Handler::READ_ERROR_A:
      {
        JAWS_TRACE ("JAWS_HTTP_10_Read_Task::handle_put, READ_ERROR");
        return -1;
      }
    default:
      break;
    }

  return 1;
}
