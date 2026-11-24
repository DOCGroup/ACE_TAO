#include "JAWS/JAWS.h"
#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

#include "HTTP_10.h"

#include "HTTP_10_Request.h"
#include <cstring>

// --------------- WRITE TASK ----------------------

JAWS_HTTP_10_Write_Task::JAWS_HTTP_10_Write_Task ()
{
}

JAWS_HTTP_10_Write_Task::~JAWS_HTTP_10_Write_Task () = default;

int
JAWS_HTTP_10_Write_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put");

  JAWS_IO_Handler *handler_ptr = data->io_handler ();

  JAWS_Dispatch_Policy *dispatch_policy = this->policy ();
  if (dispatch_policy == 0)
    {
      dispatch_policy = data->policy ();
    }

  JAWS_IO *io_ptr = dispatch_policy->io ();

  JAWS_HTTP_10_Request *info = static_cast<JAWS_HTTP_10_Request *> (data->payload ());

  if (info->status () == (int) JAWS_HTTP_10_Request::STATUS_QUIT)
    {
      data->payload (0);
      delete info;
      return -3;
    }

#if 0
  ACE_DEBUG ((LM_DEBUG, " (%t) request %s::%s::%s parsed\n",
              (info->method () ? info->method () : "-"),
              (info->uri () ? info->uri () : "="),
              (info->version () ? info->version () : "HTTP/0.9")));
#endif

  if (info->type () != (int) JAWS_HTTP_10_Request::GET)
    {
      info->set_status (JAWS_HTTP_10_Request::STATUS_NOT_IMPLEMENTED);
    }

  if (info->type () == (int) JAWS_HTTP_10_Request::QUIT)
    {
      info->set_status (JAWS_HTTP_10_Request::STATUS_QUIT);
    }

  if (info->status () != (int) JAWS_HTTP_10_Request::STATUS_OK)
    {
      JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, ! STATUS OK");

      char msg[] =
        "<html><head><title>HTTP/1.0 500 Internal Server Error</title>"
        "<body><h1>Server Error</h1>HTTP/1.0 500 Internal Server Error"
        "</body></html>";

      io_ptr->send_error_message (handler_ptr, msg, sizeof (msg));
      switch (handler_ptr->status ())
        {
        case JAWS_IO_Handler::WRITE_OK:
        case JAWS_IO_Handler::WRITE_OK_A:
          if (info->status () == (int) JAWS_HTTP_10_Request::STATUS_QUIT)
            {
              data->payload (0);
              delete info;
              return -3;
            }

          data->payload (0);
          delete info;
          return 0;

        case JAWS_IO_Handler::WRITE_ERROR:
        case JAWS_IO_Handler::WRITE_ERROR_A:
          data->payload (0);
          delete info;
          return -1;

        default:
          return 2;
        }

      return 2;
    }
  else
    {
      if (std::strcmp (info->version (), "HTTP/0.9") == 0)
        {
          io_ptr->transmit_file (handler_ptr,
                             info->path (),
                             "",
                             0,
                             "",
                             0);
        }
      else
        {
          io_ptr->transmit_file (handler_ptr,
                             info->path (),
                             "HTTP/1.0 200 Ok\r\n\r\n",
                             19,
                             "",
                             0);
        }

      switch (handler_ptr->status ())
        {
        case JAWS_IO_Handler::TRANSMIT_OK_A:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, OK_A");
            data->payload (0);
            delete info;
            return 1;
          }

        case JAWS_IO_Handler::TRANSMIT_OK:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, OK");
            data->payload (0);
            delete info;
            return 0;
          }
        case JAWS_IO_Handler::TRANSMIT_ERROR:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, ERROR");
            data->payload (0);
            delete info;
            return 0;
          }
        case JAWS_IO_Handler::TRANSMIT_ERROR_A:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, ERROR_A");
            data->payload (0);
            delete info;
            return 1;
          }
        default:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, DEFAULT");
            data->payload (0);
            delete info;
            return 1;
          }
        }

    }

  data->payload (0);
  delete info;
  return -1;
}
