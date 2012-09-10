// $Id$
// server_loggerd.cpp,v 4.29 2003/12/30 23:18:59 shuston Exp

// This server daemon collects, formats, and displays logging
// information forwarded from client daemons running on other hosts in
// the network.  In addition, this example illustrates how to use the
// ACE_Reactor, ACE_Acceptor, ACE_Singleton, and the ACE_Test_and_Set
// components.

#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/Null_Mutex.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Singleton.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Test_and_Set.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Log_Record.h"
#include "ace/Test_and_Set.h"

#include "server_loggerd.h"

// ----------------------------------------

// Return the port number.

u_short
Options::port (void)
{
  return this->port_;
}

// Parse the command-line options.

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'p':
        this->port_ = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        break;
      }
}

// ----------------------------------------

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
REACTOR;

// Our Options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex>
OPTIONS;

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
QUIT_HANDLER;

// ----------------------------------------

// Specialize a Logging Acceptor.
typedef ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR>
Logging_Acceptor;

// Default constructor.

Logging_Handler::Logging_Handler (void)
{
}

int
Logging_Handler::handle_timeout (const ACE_Time_Value &,
                                 const void *arg)
{
#if defined (ACE_NDEBUG)
  ACE_UNUSED_ARG (arg);
#endif /* ACE_NDEBUG */

  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) handling timeout from this = %@\n"), this));
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

  log_record.print (ACE_TEXT_CHAR_TO_TCHAR (this->peer_name_), 1, stderr);

  return 0;
}

int
Logging_Handler::open (void *)
{
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;
  else
    {
      ACE_OS::strncpy (this->peer_name_,
                       addr.get_host_name (),
                       MAXHOSTNAMELEN + 1);

      if (REACTOR::instance ()->register_handler (this, READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                          -1);
      else if (REACTOR::instance ()->schedule_timer
               (this,
                (const void *) this,
                ACE_Time_Value (2),
                ACE_Time_Value (2)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) connected with %C\n"),
                    this->peer_name_));
      return 0;
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Acceptor factory.
  Logging_Acceptor peer_acceptor;

  OPTIONS::instance ()->parse_args (argc, argv);

  // We need to pass in REACTOR::instance () here so that we don't use
  // the default ACE_Reactor::instance ().

  if (peer_acceptor.open
      (ACE_INET_Addr (OPTIONS::instance ()->port ()),
       REACTOR::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);

  // Register QUIT_HANDLER to receive SIGINT commands.  When received,
  // QUIT_HANDLER becomes "set" and thus, the event loop below will
  // exit.
  else if (REACTOR::instance ()->register_handler
           (SIGINT, QUIT_HANDLER::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("registering service with ACE_Reactor\n")),
                      -1);

  // Run forever, performing logging service.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting up server logging daemon\n")));

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance ()->is_set () == 0)
    REACTOR::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) shutting down server logging daemon\n")));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> *
  ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>::singleton_;
template ACE_Singleton<Options, ACE_Null_Mutex> *
  ACE_Singleton<Options, ACE_Null_Mutex>::singleton_;
template ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex> *
  ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
