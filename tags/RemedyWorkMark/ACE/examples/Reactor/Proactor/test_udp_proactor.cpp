// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_udp_proactor.cpp
//
// = DESCRIPTION
//    This program illustrates how the <ACE_Proactor> can be used to
//    implement an application that does asynchronous operations using
//    datagrams.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu> and
//    Roger Tragin <r.tragin@computer.org>
//
// ============================================================================

#include "ace/OS_NS_string.h"
#include "ace/OS_main.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Message_Block.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"



#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on asynch I/O-capable platforms.

// Host that we're connecting to.
static ACE_TCHAR *host = 0;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Keep track of when we're done.
static int done = 0;

class Receiver : public ACE_Service_Handler
{
  // = TITLE
  //     This class will receive data from
  //     the network connection and dump it to a file.
public:
  // = Initialization and termination.
  Receiver (void);
  ~Receiver (void);

  int open_addr (const ACE_INET_Addr &localAddr);

protected:
  // These methods are called by the framework

  /// This method will be called when an asynchronous read completes on
  /// a UDP socket.
  virtual void handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result);

private:
  ACE_SOCK_Dgram sock_dgram_;

  ACE_Asynch_Read_Dgram rd_;
  // rd (read dgram): for reading from a UDP socket.
  const char* completion_key_;
  const char* act_;
};

Receiver::Receiver (void)
  : completion_key_ ("Receiver Completion Key"),
    act_ ("Receiver ACT")
{
}

Receiver::~Receiver (void)
{
  sock_dgram_.close ();
}

int
Receiver::open_addr (const ACE_INET_Addr &localAddr)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:Receiver::open_addr called\n"));

  // Create a local UDP socket to receive datagrams.
  if (this->sock_dgram_.open (localAddr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_SOCK_Dgram::open"), -1);

  // Initialize the asynchronous read.
  if (this->rd_.open (*this,
                      this->sock_dgram_.get_handle (),
                      this->completion_key_,
                      ACE_Proactor::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Read_Dgram::open"), -1);

  // Create a buffer to read into.  We are using scatter/gather to
  // read the message header and message body into 2 buffers

  // create a message block to read the message header
  ACE_Message_Block* msg = 0;
  ACE_NEW_RETURN (msg, ACE_Message_Block (1024), -1);

  // the next line sets the size of the header, even though we
  // allocated a the message block of 1k, by setting the size to 20
  // bytes then the first 20 bytes of the reveived datagram will be
  // put into this message block.
  msg->size (20); // size of header to read is 20 bytes

  // create a message block to read the message body
  ACE_Message_Block* body = 0;
  ACE_NEW_RETURN (body, ACE_Message_Block (1024), -1);
  // The message body will not exceed 1024 bytes, at least not in this test.

  // set body as the cont of msg.  This associates the 2 message
  // blocks so that a read will fill the first block (which is the
  // header) up to size (), and use the cont () block for the rest of
  // the data.  You can chain up to IOV_MAX message block using this
  // method.
  msg->cont (body);

  // ok lets do the asynch read
  size_t number_of_bytes_recvd = 0;

  int res = rd_.recv (msg,
                      number_of_bytes_recvd,
                      0,
                      PF_INET,
                      this->act_);
  switch (res)
    {
    case 0:
      // this is a good error.  The proactor will call our handler when the
      // read has completed.
      break;
    case 1:
      // actually read something, we will handle it in the handler callback
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG,
                  "%s = %d\n",
                  "bytes recieved immediately",
                  number_of_bytes_recvd));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      res = 0;
      break;
    case -1:
      // Something else went wrong.
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Read_Dgram::recv"));
      // the handler will not get called in this case so lets clean up our msg
      msg->release ();
      break;
    default:
      // Something undocumented really went wrong.
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Read_Dgram::recv"));
      msg->release ();
      break;
    }

  return res;
}

void
Receiver::handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_read_dgram called\n"));

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_INET_Addr peerAddr;
  result.remote_address (peerAddr);
  ACE_DEBUG ((LM_DEBUG, "%s = %s:%d\n", "peer_address", peerAddr.get_host_addr (), peerAddr.get_port_number ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "flags", result.flags ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "act", result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "completion_key", result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));

  if (result.success () && result.bytes_transferred () != 0)
    {
      // loop through our message block and print out the contents
      for (const ACE_Message_Block* msg = result.message_block (); msg != 0; msg = msg->cont ())
        { // use msg->length () to get the number of bytes written to the message
          // block.
          ACE_DEBUG ((LM_DEBUG, "Buf=[size=<%d>", msg->length ()));
          for (u_long i = 0; i < msg->length (); ++i)
            ACE_DEBUG ((LM_DEBUG,
                        "%c", (msg->rd_ptr ())[i]));
          ACE_DEBUG ((LM_DEBUG, "]\n"));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "Receiver completed\n"));

  // No need for this message block anymore.
  result.message_block ()->release ();

  // Note that we are done with the test.
  done++;
}

class Sender : public ACE_Handler
{
  // = TITLE
  //     The class will be created by <main>.
public:
  Sender (void);
  ~Sender (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (const ACE_TCHAR *host, u_short port);
  //FUZZ: enable check_for_lack_ACE_OS

protected:
  // These methods are called by the freamwork

  virtual void handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result);
  // This is called when asynchronous writes from the dgram socket
  // complete

private:

  ACE_SOCK_Dgram sock_dgram_;
  // Network I/O handle

  ACE_Asynch_Write_Dgram wd_;
  // wd (write dgram): for writing to the socket

  const char* completion_key_;
  const char* act_;
};

Sender::Sender (void)
  : completion_key_ ("Sender completion key"),
    act_ ("Sender ACT")
{
}

Sender::~Sender (void)
{
  this->sock_dgram_.close ();
}

int
Sender::open (const ACE_TCHAR *host,
              u_short port)
{
  // Initialize stuff

  if (this->sock_dgram_.open (ACE_INET_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_SOCK_Dgram::open"), -1);

  // Initialize the asynchronous read.
  if (this->wd_.open (*this,
                      this->sock_dgram_.get_handle (),
                      this->completion_key_,
                      ACE_Proactor::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Write_Dgram::open"), -1);

  // We are using scatter/gather to send the message header and
  // message body using 2 buffers

  // create a message block for the message header
  ACE_Message_Block* msg = 0;
  ACE_NEW_RETURN (msg, ACE_Message_Block (100), -1);
  const char raw_msg [] = "To be or not to be.";
  // Copy buf into the Message_Block and update the wr_ptr ().
  msg->copy (raw_msg, ACE_OS::strlen (raw_msg) + 1);

  // create a message block for the message body
  ACE_Message_Block* body = 0;
  ACE_NEW_RETURN (body, ACE_Message_Block (100), -1);
  ACE_OS::memset (body->wr_ptr (), 'X', 100);
  body->wr_ptr (100); // always remember to update the wr_ptr ()

  // set body as the cont of msg.  This associates the 2 message blocks so
  // that a send will send the first block (which is the header) up to
  // length (), and use the cont () to get the next block to send.  You can
  // chain up to IOV_MAX message block using this method.
  msg->cont (body);

  // do the asynch send
  size_t number_of_bytes_sent = 0;
  ACE_INET_Addr serverAddr (port, host);
  int res = this->wd_.send (msg, number_of_bytes_sent, 0, serverAddr, this->act_);

  switch (res)
    {
    case 0:
      // this is a good error.  The proactor will call our handler when the
      // send has completed.
      break;
    case 1:
      // actually sent something, we will handle it in the handler callback
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG,
                  "%s = %d\n",
                  "bytes sent immediately",
                  number_of_bytes_sent));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      res = 0;
      break;
    case -1:
      // Something else went wrong.
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Write_Dgram::recv"));
      // the handler will not get called in this case so lets clean up our msg
      msg->release ();
      break;
    default:
      // Something undocumented really went wrong.
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Write_Dgram::recv"));
      msg->release ();
      break;
    }
  return res;
}

void
Sender::handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_write_dgram called\n"));

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "flags", result.flags ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "act", result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "completion_key", result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));


  ACE_DEBUG ((LM_DEBUG,
              "Sender completed\n"));

  // No need for this message block anymore.
  result.message_block ()->release ();

  // Note that we are done with the test.
  done++;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("h:p:"));
  int c;

  while ((c = get_opt ()) != EOF)
    switch (c)
      {
      case 'h':
        host = get_opt.opt_arg ();
        break;
      case 'p':
        port = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                           "usage :\n"
                           "-h <host>\n"), -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  Sender sender;

  Receiver receiver;

  // If passive side
  if (host == 0)
    {
      if (receiver.open_addr (ACE_INET_Addr (port)) == -1)
        return -1;
    }
  // If active side
  else if (sender.open (host, port) == -1)
    return -1;

  for (int success = 1;
       success > 0  && !done;
       )
    // Dispatch events via Proactor singleton.
    success = ACE_Proactor::instance ()->handle_events ();

  return 0;
}

#else /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS*/

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example does not work on this platform.\n"));
  return 1;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

