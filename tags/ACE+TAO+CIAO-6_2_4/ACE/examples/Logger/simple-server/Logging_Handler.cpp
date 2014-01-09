// $Id$

#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include "ace/Log_Record.h"
#include "ace/OS_NS_string.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"

#include "Logging_Handler.h"
#include "Reactor_Singleton.h"

// Default constructor.

Logging_Handler::Logging_Handler (void)
{
}

Logging_Handler::~Logging_Handler (void)
{
  // Make sure there are no timers.
  REACTOR::instance ()->cancel_timer (this);

  this->cli_stream_.close ();
}

// Extract the underlying ACE_SOCK_Stream (e.g., for purposes of
// accept()).

ACE_SOCK_Stream &
Logging_Handler::peer (void)
{
  return this->cli_stream_;
}

int
Logging_Handler::handle_timeout (const ACE_Time_Value &,
                                 const void *arg)
{
#if defined (ACE_NDEBUG)
  ACE_UNUSED_ARG (arg);
#endif /* ACE_NDEBUG */

  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) handling timeout from this = %@\n"),
              this));
  return 0;
}

// Perform the logging record receive.

int
Logging_Handler::handle_input (ACE_HANDLE)
{
  ACE_Log_Record log_record;

  // We need to use the old two-read trick here since TCP sockets
  // don't support framing natively.  Allocate a message block for the
  // payload; initially at least large enough to hold the header, but
  // needs some room for alignment.
  ACE_Message_Block *payload_p = 0;
  ACE_Message_Block *header_p = 0;
  ACE_NEW_RETURN (header_p,
                  ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE),
                  -1);

  auto_ptr <ACE_Message_Block> header (header_p);

  // Align the Message Block for a CDR stream
  ACE_CDR::mb_align (header.get ());

  ACE_CDR::Boolean byte_order;
  ACE_CDR::ULong length;

  ssize_t count = ACE::recv_n (this->peer ().get_handle (),
                               header->wr_ptr (),
                               8);
  switch (count)
    {
      // Handle shutdown and error cases.
    default:
    case -1:
    case 0:

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("server logging daemon closing down\n")));

      return -1;
      /* NOTREACHED */

    case 8:
      // Just fall through in this case..
      break;
    }

  header->wr_ptr (8); // Reflect addition of 8 bytes.

  // Create a CDR stream to parse the 8-byte header.
  ACE_InputCDR header_cdr (header.get ());

  // Extract the byte-order and use helper methods to disambiguate
  // octet, booleans, and chars.
  header_cdr >> ACE_InputCDR::to_boolean (byte_order);

  // Set the byte-order on the stream...
  header_cdr.reset_byte_order (byte_order);

  // Extract the length
  header_cdr >> length;

  ACE_NEW_RETURN (payload_p,
                  ACE_Message_Block (length),
                  -1);
  auto_ptr <ACE_Message_Block> payload (payload_p);

  // Ensure there's sufficient room for log record payload.
  ACE_CDR::grow (payload.get (), 8 + ACE_CDR::MAX_ALIGNMENT + length);

  // Use <recv_n> to obtain the contents.
  if (ACE::recv_n (this->peer ().get_handle (),
                   payload->wr_ptr (),
                   length) <= 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("recv_n()")));
      return -1;
    }

  payload->wr_ptr (length);   // Reflect additional bytes

  ACE_InputCDR payload_cdr (payload.get ());
  payload_cdr.reset_byte_order (byte_order);
  payload_cdr >> log_record;  // Finally extract the <ACE_log_record>.

  log_record.length (length);

  log_record.print (ACE_TEXT_CHAR_TO_TCHAR (this->host_name_), 1, stderr);

  return 0;
}

// Extract underlying device descriptor.

ACE_HANDLE
Logging_Handler::get_handle (void) const
{
  return this->cli_stream_.get_handle ();
}

int
Logging_Handler::open (void)
{
  ACE_INET_Addr addr;

  if (this->cli_stream_.get_remote_addr (addr) == -1)
    return -1;
  else
    {
      ACE_OS::strncpy (this->host_name_,
                       addr.get_host_name (),
                       MAXHOSTNAMELEN + 1);

      if (REACTOR::instance ()->register_handler (this, READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                          -1);
      else if (REACTOR::instance ()->schedule_timer
               (this, (const void *) this,
                ACE_Time_Value (2),
                ACE_Time_Value (2)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) connected with %C\n"),
                    this->host_name_));
      return 0;
    }
}

// Perform termination activities when deregistered from the
// ACE_Reactor.

int
Logging_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  // Must have been allocated dynamically
  delete this;
  return 0;
}

// Perform termination activities when close fails.

int
Logging_Handler::close (void)
{
  return this->handle_close ();
}
