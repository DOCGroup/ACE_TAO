// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_cancel.cpp
//
// = DESCRIPTION
//    This program tests cancelling an Asynchronous Operation in the
//    Proactor framework.
//
//    This tests accepts a connection and issues an Asynchronous Read
//    Stream. It reads <read_size> (option -s) number of bytes and
//    when this operation completes, it issues another Asynchronous
//    Read Stream to <read_size> and immediately calls <cancel> to
//    cancel the operation and so the program exits closing the
//    connection.
//
//    Works fine on NT. On Solaris platforms, the asynch read is
//    pending, but the cancel returns with the value <AIO_ALLDONE>
//    indicating all the operations in that handle are done.
//    But, LynxOS has a good <aio_cancel> implementation. It works
//    fine.
//
// = RUN
//   ./test_cancel -p <port_number>
//   Then telnet to this port and send <read_size> bytes and your
//   connection should get closed down.
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
//
// ============================================================================

#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_IO_Impl.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_socket.h"



#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms supporting
  // POSIX aio calls.

#include "test_cancel.h"

static u_short port = ACE_DEFAULT_SERVER_PORT;
static int done = 0;
static int read_size = 2;


Receiver::Receiver (void)
  : mb_ (read_size + 1),
    handle_ (ACE_INVALID_HANDLE)
{
}

Receiver::~Receiver (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Receiver: Closing down Remote connection:%d\n",
              this->handle_));

  ACE_OS::closesocket (this->handle_);
}

void
Receiver::open (ACE_HANDLE handle,
                ACE_Message_Block &)
{
  // New connection, initiate stuff

  ACE_DEBUG ((LM_DEBUG, "%N:%l:Receiver::open called\n"));

  // Cache the new connection
  this->handle_ = handle;

  // Initiate ACE_Asynch_Read_Stream
  if (this->rs_.open (*this, this->handle_) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Asynch_Read_Stream::open"));
      return;
    }

  // Try to read <n> bytes from the stream.

  ACE_DEBUG ((LM_DEBUG,
              "Receiver::open: Issuing Asynch Read of (%d) bytes from the stream\n",
              read_size));

  if (this->rs_.read (this->mb_,
                      read_size) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "Receiver::open: Failed to issue the read"));
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_read_stream called\n"));

  // Reset pointers
  result.message_block ().rd_ptr ()[result.bytes_transferred ()] = '\0';

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (uintptr_t) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (uintptr_t) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));

  if (result.success () && !result.error ())
    {
      // Successful read: No error.

      // Set the pointers back in the message block.
      result.message_block ().wr_ptr (result.message_block ().rd_ptr ());

      // Issue another read, but immediately cancel it.

      // Issue the read.

      ACE_DEBUG ((LM_DEBUG,
                  "Issuing Asynch Read of (%d) bytes from the stream\n",
                  read_size));

      if (this->rs_.read (this->mb_,
                          read_size) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "Receiver::handle_read_stream: Failed to issue the read"));

      // Cancel the read.

      ACE_DEBUG ((LM_DEBUG,
                  "Cacelling Asynch Read "));

      int ret_val = this->rs_.cancel ();
      if (ret_val == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "Receiver::handle_read_stream: Failed to cancel the read"));

      ACE_DEBUG ((LM_DEBUG, "Asynch IO : Cancel : Result = %d\n",
                  ret_val));
    }
  else
    {
      done = 1;

      ACE_DEBUG ((LM_DEBUG, "Receiver completed\n"));

      // Print the error message if any.
      if (result.error () != 0)
        {
          errno = result.error ();

          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "Asynch Read Stream Error: "));
        }
    }
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("p:s:"));
  int c;

  while ((c = get_opt ()) != EOF)
    switch (c)
      {
      case 'p':
        port = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 's':
        read_size = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        ACE_ERROR ((LM_ERROR, "%p.\n",
                    "usage :\n"
                    "-p <port>\n"
                    "-s <read_size>\n"));
        return -1;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  // Note: acceptor parameterized by the Receiver
  ACE_Asynch_Acceptor<Receiver> acceptor;

  // Listening passively.
  if (acceptor.open (ACE_INET_Addr (port),
                     read_size,
                     1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE:acceptor::open failed\n"),
                      1);

  int success = 1;

  while (success > 0  && !done)
    // dispatch events
    success = ACE_Proactor::instance ()->handle_events ();

  return 0;
}

#else /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example does not work on this platform.\n"));
  return 1;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
