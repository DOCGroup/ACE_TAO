// $Id$

#include "HTTP_10.h"

#include "JAWS/JAWS.h"

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

// --------------- PARSE TASK ----------------------

JAWS_HTTP_10_Parse_Task::JAWS_HTTP_10_Parse_Task (void)
{
}

JAWS_HTTP_10_Parse_Task::~JAWS_HTTP_10_Parse_Task (void)
{
}

int
JAWS_HTTP_10_Parse_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::handle_put");

  JAWS_Dispatch_Policy *policy = data->policy ();
  JAWS_IO_Handler *handler = data->io_handler ();
  JAWS_IO *io = policy->io ();

  JAWS_HTTP_10_Request *info = new JAWS_HTTP_10_Request;
  if (info == 0)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_HTTP_10_Parse_Task::handle_put"));
      return -1;
    }
  data->payload (ACE_static_cast (void *, info));

  while (this->parse_request (info, data) == 0)
    {
      int next_read_size
        = JAWS_Data_Block::JAWS_DATA_BLOCK_SIZE - data->length () - 1;

      if (next_read_size == 0)
        {
          // Set payload to reflect "request too long"
          break;
        }

      io->read (handler, data, next_read_size);
      if (handler->status () == JAWS_IO_Handler::READ_OK)
        {
          // Behaved synchronously, reiterate
          continue;
        }
      else
        {
          // This needs to be a value that tells the framework that
          // the call is asynchronous, but that we should remain in
          // the current task state.
          return 1;
        }
    }

  // request completely parsed

  return 0;
}

int
JAWS_HTTP_10_Parse_Task::parse_request (JAWS_HTTP_10_Request *info,
                                        JAWS_Data_Block *data)
{
  ACE_Message_Block &mb = *data;

  mb.wr_ptr ()[0] = '\0';

  // Note that RFC 822 does not mention the maximum length of a header
  // line.  So in theory, there is no maximum length.
  // In Apache, they assume that each header line should not exceed
  // 8K.

  int result = info->complete_header_line (mb.rd_ptr ());

  if (result != 0)
    {
      if (!info->got_request_line ())
        {
          this->parse_request_line (info, mb.rd_ptr ());
          while (info->complete_header_line (mb.rd_ptr ()) > 0)
            this->parse_header_line (info, mb.rd_ptr ());
        }
      else if (result > 0)
        do
          this->parse_header_line (info, mb.rd_ptr ());
        while (info->complete_header_line (mb.rd_ptr ()) > 0);
    }

  mb.wr_ptr (strlen(mb.rd_ptr ()) - mb.length ());
  if (info->end_of_headers ()
      || (info->got_request_line () && info->version () == 0))
    return info->reset (mb.rd_ptr (), mb.length ());
  else
    return 0;
}

void
JAWS_HTTP_10_Parse_Task::parse_request_line (JAWS_HTTP_10_Request *info,
                                             char *request_line)
{
  info->parse_request_line (request_line);
}

void
JAWS_HTTP_10_Parse_Task::parse_header_line (JAWS_HTTP_10_Request *info,
                                             char *header_line)
{
  info->parse_header_line (header_line);
}
