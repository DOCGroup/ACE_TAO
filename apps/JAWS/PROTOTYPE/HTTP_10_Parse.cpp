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
        = JAWS_Data_Block::JAWS_DATA_BLOCK_SIZE - data->length ();

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
          return 2;
        }
    }

  // request completely parsed

  return 0;
}

int
JAWS_HTTP_10_Parse_Task::parse_request (JAWS_HTTP_10_Request *info,
                                        JAWS_Data_Block *data)
{
  JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_request");

  // Algorithm --
  //
  // -- if we have not parsed a request line
  //      scan for a newline
  //        if no newline present, return
  //      parse the request line
  //      set "parsed request line" flag
  //      set "end of line" flag
  //      adjust the read pointer
  //
  // -- parse the header line input
  //    loop:
  //      if no more input break
  //      if "end of line" flag is set and
  //        line does not begin with leading white space
  //          if this char is a newline
  //            set "end of headers" flag
  //            return
  //          get the header type (i.e, up to colon)
  //            if there is no colon
  //              ignore input up to next newline or no more input
  //              adjust the read pointer
  //              goto loop:
  //          set header value to something empty
  //          set last header to point to current header
  //          unset "end of line" flag
  //      if "end of line" flag is not set or
  //        if the line begins with leading white space,
  //          while not a newline character present
  //            if "end of input" end while
  //            otherwise append char to last header value
  //            adjust the read pointer
  //          if a newline, set "end of line" flag
  //          goto loop:
  //
  // -- adjust read and write pointer
  //      if "end of input" occurs but there is still

  ACE_Message_Block &mb = *data;

  if (!info->got_request_line ())
    {
      char *p;
      p = this->skipset ("\n", mb.rd_ptr (), mb.wr_ptr ());
      if (p == mb.wr_ptr ())
        // can't do anything yet
        return 0;

      info->parse_request_line (mb.rd_ptr ());

      mb.rd_ptr (p+1);
      info->end_of_line (1);
    }

  return this->parse_headers (info, mb);
}

int
JAWS_HTTP_10_Parse_Task::parse_headers (JAWS_HTTP_10_Request *info,
                                        ACE_Message_Block &mb)
{
  JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_headers");

  for (;;)
    {
      if (mb.rd_ptr () == mb.wr_ptr ())
          break;

      char *p = mb.rd_ptr ();

      if (info->end_of_line ()
          && (*p != ' ' && *p != '\t'))
        {
          int r = this->parse_header_type (info, mb);
          if (r == 1)
            return info->end_of_headers ();
          continue;
        }
      else
        {
          int r = this->parse_header_value (info, mb);
          if (r == 1)
            break;
          continue;
        }
    }

  if (mb.rd_ptr () != mb.base ())
    {
      size_t len = mb.length ();
      if (len > 0)
        ACE_OS::memmove (mb.base (), mb.rd_ptr (), len);
      mb.rd_ptr (mb.base ());
      mb.wr_ptr (mb.base () + len);
      return 0;
    }
  else if (mb.length () == mb.size ())
    {
      // This is one of those cases that should rarely ever happen.
      // If we get here, it means the initial HTTP request line was
      // over 8K long or that the header type name is over 8K long.
      // In either case, we flag this as a bad request.

      // In HTTP/1.1, I have to remember that a bad request means the
      // connection needs to be closed and the client has to
      // reinitiate the connection.

      info->status (JAWS_HTTP_10_Request::STATUS_BAD_REQUEST);
      return 1;
    }
}

char *
JAWS_HTTP_10_Parse_Task::skipset (const char *set, char *start, char *end)
{
  char *p = start;
  while (p < end)
    {
      if (ACE_OS::strchr (set, *p) != NULL)
        break;
      p++;
    }
  return p;
}

int
JAWS_HTTP_10_Parse_Task::parse_header_type (JAWS_HTTP_10_Request *info,
                                            ACE_Message_Block &mb)
{
  JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_header_type");

  char *p = mb.rd_ptr ();
  char *q;

  q = this->skipset (":\n", p, mb.wr_ptr ());
  if (q == mb.wr_ptr ())
    // no more progress can be made until we find a ':'
    return 1;
  if (q == p)
    {
      // Ignore empty header type names
      info->create_next_header_value (0, 0);
      info->end_of_line (0);
      mb.rd_ptr (q+1);
      return 0;
    }
  if (*q == '\n')
    {
      // ignore this line
      mb.rd_ptr (q+1);
      if (q == p || ((q-1) == p && q[-1] == '\r'))
        {
          // blank line means end of headers
          info->create_next_header_value (0, 0);
          info->end_of_headers (1);
          if (mb.rd_ptr () == mb.wr_ptr ())
            {
              mb.rd_ptr (mb.base ());
              mb.wr_ptr (mb.base ());
            }
          return 1;
        }

      // not a blank line, but no ':', so ignore it
      info->create_next_header_value (0, 0);
      return 0;
    }

  // otherwise, we have a header type name!
  *q = '\0';
  char *header_type = ACE_OS::strdup (p);
  char *header_value = info->header_buf ();

  if (header_type == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "JAWS_HTTP_10_Parse_Task::parse_request"));
      ACE_OS::free (header_type);
      header_type = 0;
      header_value = 0;
      info->create_next_header_value (header_type, header_value);
    }
  else
    {
      JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_header_type got one!");

      info->create_next_header_value (header_type, header_value);
      header_value[0] = '\0';
      info->table ()->insert (header_type, header_value);
    }

  info->end_of_line (0);

  mb.rd_ptr (q+1);
  return 0;
}

int
JAWS_HTTP_10_Parse_Task::parse_header_value (JAWS_HTTP_10_Request *info,
                                             ACE_Message_Block &mb)
{
  JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_header_value");

  // break --> return 1;
  // continue --> return 0;

  char *q = mb.rd_ptr ();

  if (info->last_header_value () == 0)
    {
      JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_header_value ignore");

      q = this->skipset ("\n", mb.rd_ptr (), mb.wr_ptr ());
      if (q == mb.wr_ptr ())
        {
          info->end_of_line (0);
          mb.rd_ptr (q);
          if (q[-1] == '\r')
            mb.rd_ptr (q-1);
          return 1;
        }
      if (*q == '\n')
        {
          info->end_of_line (1);
          q++;
        }
      mb.rd_ptr (q);
      return 0;
    }
  else
    {
      JAWS_TRACE ("JAWS_HTTP_10_Parse_Task::parse_header_value skip");

      // Skip over leading linear white space
      q = this->skipset (" \t", mb.rd_ptr (), mb.wr_ptr ());
      if (q == mb.wr_ptr ())
        {
          // need more input
          info->end_of_line (0);
          mb.rd_ptr (q);
          return 1;
        }

      // If not the first header value character,
      // insert a single space
      if (info->last_header_length () != 0)
        info->append_last_header_value (' ');

      while (q < mb.wr_ptr ())
        {
          if (*q == '\n')
            break;
          info->append_last_header_value (*q);
          q++;
        }

      if (q == mb.wr_ptr ())
        {
          info->end_of_line (0);
          mb.rd_ptr (q);
          return 1;
        }

      if (*q == '\n')
        {
          if (q[-1] == '\r')
            info->reduce_last_header_value ();
          info->end_of_line (1);
          mb.rd_ptr (q+1);
          return 0;
        }
    }

  // NOT REACHED
  return 1;
}
