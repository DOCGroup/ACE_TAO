// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_proactor.cpp
//
// = DESCRIPTION
//    This program illustrates how the <ACE_Proactor> can be used to
//    implement an application that does various asynchronous
//    operations.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "ace/OS_NS_string.h"
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
#include "ace/Truncate.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"



#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms supporting
  // POSIX aio calls.

#include "test_proactor.h"


// Host that we're connecting to.
static ACE_TCHAR *host = 0;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// File that we're sending.
static const ACE_TCHAR *file = ACE_TEXT("test_proactor.cpp");

// Name of the output file.
static const ACE_TCHAR *dump_file = ACE_TEXT("output");

// Keep track of when we're done.
static int done = 0;

// Size of each initial asynchronous <read> operation.
static int initial_read_size = BUFSIZ;


Receiver::Receiver (void)
  : dump_file_ (ACE_INVALID_HANDLE),
    handle_ (ACE_INVALID_HANDLE)
{
}

Receiver::~Receiver (void)
{
  ACE_OS::close (this->dump_file_);
  ACE_OS::closesocket (this->handle_);
}

void
Receiver::open (ACE_HANDLE handle,
                ACE_Message_Block &message_block)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:Receiver::open called\n"));

  // New connection, so initiate stuff.

  // Cache the new connection
  this->handle_ = handle;

  // File offset starts at zero
  this->file_offset_ = 0;

  // Open dump file (in OVERLAPPED mode)
  this->dump_file_ = ACE_OS::open (dump_file,
                                   O_CREAT | O_RDWR | O_TRUNC | \
                                   FILE_FLAG_OVERLAPPED);
  if (this->dump_file_ == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_OS::open"));
      return;
    }

  // Initiate <ACE_Asynch_Write_File>.
  if (this->wf_.open (*this,
                      this->dump_file_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Write_File::open"));
      return;
    }

  // Initiate <ACE_Asynch_Read_Stream>.
  if (this->rs_.open (*this, this->handle_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Read_Stream::open"));
      return;
    }

  // Fake the result and make the <handle_read_stream> get
  // called. But, not, if there is '0' is transferred.
  if (message_block.length () != 0)
    {
      // Duplicate the message block so that we can keep it around.
      ACE_Message_Block &duplicate =
        *message_block.duplicate ();

      // Fake the result so that we will get called back.
      ACE_Asynch_Read_Stream_Result_Impl *fake_result =
        ACE_Proactor::instance ()->create_asynch_read_stream_result (this->proxy (),
                                                                     this->handle_,
                                                                     duplicate,
                                                                     initial_read_size,
                                                                     0,
                                                                     ACE_INVALID_HANDLE,
                                                                     0,
                                                                     0);

      size_t bytes_transferred = message_block.length ();

      // <complete> for Accept would have already moved the <wr_ptr>
      // forward. Update it to the beginning position.
      duplicate.wr_ptr (duplicate.wr_ptr () - bytes_transferred);

      // This will call the callback.
      fake_result->complete (message_block.length (),
                             1,
                             0);

      // Zap the fake result.
      delete fake_result;
    }
  else
    // Otherwise, make sure we proceed. Initiate reading the socket
    // stream.
    if (this->initiate_read_stream () == -1)
      return;
}

int
Receiver::initiate_read_stream (void)
{
  // Create a new <Message_Block>.  Note that this message block will
  // be used both to <read> data asynchronously from the socket and to
  // <write> data asynchronously to the file.
  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb,
                      mb->size () - 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Read_Stream::read"),
                      -1);
  return 0;
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_read_stream called\n"));

  // Reset pointers.
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
#if 0
  // This can overrun the ACE_Log_Msg buffer and do bad things.
  // Re-enable it at your risk.
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));
#endif /* 0 */

  if (result.success () && result.bytes_transferred () != 0)
    {
      // Successful read: write the data to the file asynchronously.
      // Note how we reuse the <ACE_Message_Block> for the writing.
      // Therefore, we do not delete this buffer because it is handled
      // in <handle_write_stream>.
      if (this->wf_.write (result.message_block (),
                           result.bytes_transferred (),
                           this->file_offset_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "ACE_Asynch_Write_File::write"));
          return;
        }

      // Initiate new read from the stream.
      if (this->initiate_read_stream () == -1)
        return;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Receiver completed\n"));

      // No need for this message block anymore.
      result.message_block ().release ();

      // Note that we are done with the test.
      done = 1;

      // We are done: commit suicide.
      delete this;
    }
}

void
Receiver::handle_write_file (const ACE_Asynch_Write_File::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_write_file called\n"));

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (uintptr_t) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (uintptr_t) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));

  result.message_block ().release ();

  if (result.success ())
    // Write successful:  Increment file offset
    this->file_offset_ +=
      ACE_Utils::truncate_cast<u_long> (result.bytes_transferred ());

  // This code is not robust enough to deal with short file writes
  // (which hardly ever happen) ;-)
  ACE_ASSERT (result.bytes_to_write () == result.bytes_transferred ());
}

class Sender : public ACE_Handler
{
  // = TITLE
  //     The class will be created by <main>.  After connecting to the
  //     host, this class will then read data from a file and send it
  //     to the network connection.
public:
  Sender (void);
  ~Sender (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (const ACE_TCHAR *host,
            u_short port);
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_HANDLE handle (void) const;
  void handle (ACE_HANDLE);

protected:
  // These methods are called by the freamwork

  virtual void handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result);
  // This is called when asynchronous transmit files complete
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete
  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This is called when asynchronous reads from the socket complete

private:
  int transmit_file (void);
  // Transmit the entire file in one fell swoop.

  int initiate_read_file (void);
  // Initiate an asynchronous file read.

  ACE_SOCK_Stream stream_;
  // Network I/O handle

  ACE_Asynch_Write_Stream ws_;
  // ws (write stream): for writing to the socket

  ACE_Asynch_Read_File rf_;
  // rf (read file): for writing from the file

  ACE_Asynch_Transmit_File tf_;
  // Transmit file.

  ACE_HANDLE input_file_;
  // File to read from

  u_long file_offset_;
  // Current file offset

  u_long file_size_;
  // File size

  ACE_Message_Block welcome_message_;
  // Welcome message

  ACE_Asynch_Transmit_File::Header_And_Trailer header_and_trailer_;
  // Header and trailer which goes with transmit_file

  int stream_write_done_;
  int transmit_file_done_;
  // These flags help to determine when to close down the event loop
};

Sender::Sender (void)
  : input_file_ (ACE_INVALID_HANDLE),
    file_offset_ (0),
    file_size_ (0),
    stream_write_done_ (0),
    transmit_file_done_ (0)
{
  // Moment of inspiration... :-)
  static const char *data = "Welcome to Irfan World! Irfan RULES here !!\n";
  this->welcome_message_.init (data,
                               ACE_OS::strlen (data));
  this->welcome_message_.wr_ptr (ACE_OS::strlen (data));
}

Sender::~Sender (void)
{
  this->stream_.close ();
}

ACE_HANDLE
Sender::handle (void) const
{
  return this->stream_.get_handle ();
}

void
Sender::handle (ACE_HANDLE handle)
{
  this->stream_.set_handle (handle);
}

int
Sender::open (const ACE_TCHAR *host,
               u_short port)
{
  // Initialize stuff

  // Open input file (in OVERLAPPED mode)
  this->input_file_ =
    ACE_OS::open (file, GENERIC_READ | FILE_FLAG_OVERLAPPED);
  if (this->input_file_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::open"), -1);

  // Find file size
  this->file_size_ =
    ACE_OS::filesize (this->input_file_);

  // Connect to remote host
  ACE_INET_Addr address (port, host);
  ACE_SOCK_Connector connector;
  if (connector.connect (this->stream_,
                         address) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_SOCK_Connector::connect"),
                      -1);

  // Open ACE_Asynch_Write_Stream
  if (this->ws_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Write_Stream::open"),
                      -1);

  // Open ACE_Asynch_Read_File
  if (this->rf_.open (*this, this->input_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Read_File::open"),
                      -1);

  // Start an asynchronous transmit file
  if (this->transmit_file () == -1)
    return -1;

  // Start an asynchronous read file
  if (this->initiate_read_file () == -1)
    return -1;

  return 0;
}

int
Sender::transmit_file (void)
{
  // Open file (in SEQUENTIAL_SCAN mode)
  ACE_HANDLE file_handle =
    ACE_OS::open (file, GENERIC_READ | FILE_FLAG_SEQUENTIAL_SCAN);
  if (file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::open"),
                      -1);

  // Open ACE_Asynch_Transmit_File
  if (this->tf_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Transmit_File::open"),
                      -1);

  // Header and trailer data for the file.
  // @@ What happens if header and trailer are the same?
  this->header_and_trailer_.header_and_trailer (&this->welcome_message_,
                                                this->welcome_message_.length (),
                                                &this->welcome_message_,
                                                this->welcome_message_.length ());

  // Send the entire file in one fell swoop!
  if (this->tf_.transmit_file (file_handle,
                               &this->header_and_trailer_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Transmit_File::transmit_file"),
                      -1);

  return 0;
}

void
Sender::handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_transmit_file called\n"));

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "socket", result.socket ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "file", result.file ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_per_send", result.bytes_per_send ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "flags", result.flags ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (uintptr_t) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (uintptr_t) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));

  // Done with file
  ACE_OS::close (result.file ());

  this->transmit_file_done_ = 1;
  if (this->stream_write_done_)
    done = 1;
}

int
Sender::initiate_read_file (void)
{
  // Create a new <Message_Block>.  Note that this message block will
  // be used both to <read> data asynchronously from the file and to
  // <write> data asynchronously to the socket.
  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ + 1),
                  -1);

  // Inititiate an asynchronous read from the file
  if (this->rf_.read (*mb,
                      mb->size () - 1,
                      this->file_offset_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Read_File::read"),
                      -1);
  return 0;
}

void
Sender::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_read_file called\n"));

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
  //ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));

  if (result.success ())
    {
      // Read successful: increment offset and write data to network.
      // Note how we reuse the <ACE_Message_Block> for the writing.
      // Therefore, we do not delete this buffer because it is handled
      // in <handle_write_stream>.

      this->file_offset_ +=
        ACE_Utils::truncate_cast<u_long> (result.bytes_transferred ());

      if (this->ws_.write (result.message_block (),
                           result.bytes_transferred ()) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "ACE_Asynch_Write_Stream::write"));
          return;
        }

      if (this->file_size_ > this->file_offset_)
        {
          // Start an asynchronous read file.
          if (initiate_read_file () == -1)
            return;
        }
    }
}

void
Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_write_stream called\n"));

  // Reset pointers.
  result.message_block ().rd_ptr (result.message_block ().rd_ptr () - result.bytes_transferred ());

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (uintptr_t) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (uintptr_t) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
#if 0
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));
#endif

  if (result.success ())
    {
      // Partial write to socket
      int unsent_data =
        ACE_Utils::truncate_cast<int> (
          result.bytes_to_write () - result.bytes_transferred ());

      if (unsent_data != 0)
        {
          // Reset pointers
          result.message_block ().rd_ptr (result.bytes_transferred ());

          // Duplicate the message block and retry remaining data
          if (this->ws_.write (*result.message_block ().duplicate (),
                               unsent_data) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "%p\n",
                          "ACE_Asynch_Write_Stream::write"));
              return;
            }
        }
      else if (!(this->file_size_ > this->file_offset_))
        {
          this->stream_write_done_ = 1;
          if (this->transmit_file_done_)
            done = 1;
        }
    }

  // Release message block.
  result.message_block ().release ();
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("h:p:f:d:"));
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
      case 'f':
        file = get_opt.opt_arg ();
        break;
      case 'd':
        dump_file = get_opt.opt_arg ();
        break;
      default:
        ACE_ERROR ((LM_ERROR, "%p.\n",
                    "usage :\n"
                    "-h <host>\n"
                    "-p <port>\n"
                    "-f <file>\n"));
        return -1;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  Sender sender;

  // Note: acceptor parameterized by the Receiver.
  ACE_Asynch_Acceptor<Receiver> acceptor;

  // If passive side
  if (host == 0)
   {
     if (acceptor.open (ACE_INET_Addr (port),
                        initial_read_size,
                        1) == -1)
       return -1;
   }
  // If active side
  else if (sender.open (host, port) == -1)
    return -1;

  int success = 1;

  while (success > 0  && !done)
    // Dispatch events via Proactor singleton.
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
