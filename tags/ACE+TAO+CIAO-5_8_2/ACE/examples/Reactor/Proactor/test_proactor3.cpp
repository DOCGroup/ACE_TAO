// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_proactor3.cpp
//
// = DESCRIPTION
//    This program illustrates how the <ACE_Proactor> can be used to
//    implement an application that does various asynchronous
//    operations.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//    modified by  Alexander Libman <alibman@baltimore.com>
//    from original test_proactor.cpp
// ============================================================================

#include "ace/Signal.h"

#include "ace/Service_Config.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_IO_Impl.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Manual_Event.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Get_Opt.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Task.h"

ACE_RCSID(Proactor, test_proactor, "test_proactor.cpp,v 1.27 2000/03/07 17:15:56 schmidt Exp")

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)

#include "ace/WIN32_Proactor.h"

#elif defined (ACE_HAS_AIO_CALLS)

#include "ace/POSIX_Proactor.h"
#include "ace/SUN_Proactor.h"

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO */

//  Some debug helper functions
static int disable_signal (int sigmin, int sigmax);
#if 0
static int print_sigmask (void);
#endif

#define  COUT(X)  cout << X; cout.flush ();

// Proactor Type (UNIX only, Win32 ignored) 0-default, 1 -AIOCB,
// 2-SIG, 3-SUN
static int proactor_type = 0;

// POSIX : > 0 max number aio operations  proactor,
static int max_aio_operations = 0;

// Host that we're connecting to.
static ACE_TCHAR *host = 0;

// number of Senders instances
static int senders = 1;
static const int MaxSenders = 100;

// duplex mode: ==0 half-duplex
//              !=0 full duplex
static int duplex = 0;

// number threads in the Proactor thread pool
static int threads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

class MyTask: public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //   MyTask plays role for Proactor threads pool
public:
  MyTask (void) : threads_ (0), proactor_ (0) {}

  int svc (void);
  void waitready (void) { event_.wait (); }

private:
  ACE_Recursive_Thread_Mutex mutex_;
  int threads_;
  ACE_Proactor *proactor_;
  ACE_Manual_Event event_;

  void create_proactor (void);
  void delete_proactor (void);
};

void
MyTask::create_proactor (void)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);

  if (threads_ == 0)
    {
#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)
      ACE_WIN32_Proactor *proactor = new ACE_WIN32_Proactor;
      ACE_DEBUG ((LM_DEBUG,"(%t) Create Proactor Type=WIN32"));

#elif defined (ACE_HAS_AIO_CALLS)

      ACE_POSIX_Proactor *proactor = 0;

      switch (proactor_type)
        {
        case 1:
          proactor = new ACE_POSIX_AIOCB_Proactor (max_aio_operations);
          ACE_DEBUG ((LM_DEBUG,"(%t) Create Proactor Type=AIOCB\n"));
          break;
        case 2:
          proactor = new ACE_POSIX_SIG_Proactor;
          ACE_DEBUG ((LM_DEBUG,"(%t) Create Proactor Type=SIG\n"));
          break;
#  if defined (sun)
        case 3:
          proactor = new ACE_SUN_Proactor (max_aio_operations);
          ACE_DEBUG ((LM_DEBUG,"(%t) Create Proactor Type=SUN\n"));
          break;
#  endif /* sun */
        default:
          proactor = new ACE_POSIX_SIG_Proactor;
          ACE_DEBUG ((LM_DEBUG,"(%t) Create Proactor Type=SIG\n"));
          break;
        }
#endif

      proactor_ = new ACE_Proactor (proactor, 1);

      ACE_Proactor::instance(proactor_);
      event_.signal ();
    }

  threads_++;
}

void
MyTask::delete_proactor (void)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
  if (--threads_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) Delete Proactor\n"));
      ACE_Proactor::instance ((ACE_Proactor *) 0);
      delete proactor_;
      proactor_ = 0;
    }
}

int
MyTask::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) MyTask started\n"));

  create_proactor ();
  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);

  while (ACE_Proactor::event_loop_done () == 0)
    ACE_Proactor::run_event_loop ();

  delete_proactor ();

  ACE_DEBUG ((LM_DEBUG, "(%t) MyTask finished\n"));
  return 0;
}

class Receiver : public ACE_Service_Handler
{
public:

  Receiver (void);
  ~Receiver (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);
  // This is called after the new connection has been accepted.
  //FUZZ: enable check_for_lack_ACE_OS

  static long get_number_sessions (void) { return sessions_; }

protected:
  // These methods are called by the framework

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous <read> operation from the socket
  // complete.

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when an asynchronous <write> to the file
  // completes.

private:
  int  initiate_read_stream (void);
  int  initiate_write_stream (ACE_Message_Block & mb, int nBytes);
  int check_destroy (void);

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;
  ACE_Recursive_Thread_Mutex mutex_;
  long io_count_;
  static long sessions_;
};

long Receiver::sessions_ = 0;

Receiver::Receiver (void)
  : handle_ (ACE_INVALID_HANDLE),
    io_count_ (0)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
  sessions_++;
  ACE_DEBUG ((LM_DEBUG, "Receiver Ctor sessions_=%d\n", sessions_));
}

Receiver::~Receiver (void)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
  sessions_--;
  ACE_OS::closesocket (this->handle_);
  ACE_DEBUG ((LM_DEBUG, "~Receiver Dtor sessions_=%d\n", sessions_));
}

//  return true if we alive, false  we commited suicide
int
Receiver::check_destroy (void)
{
  {
    ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, mutex_, -1);

    if (io_count_ > 0)
      return 1;
  }

  delete this;
  return 0;
}

void
Receiver::open (ACE_HANDLE handle,
                ACE_Message_Block &)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:Receiver::open called\n"));

  this->handle_ = handle;

  if (this->ws_.open (*this, this->handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "ACE_Asynch_Write_Stream::open"));
  else if (this->rs_.open (*this, this->handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "ACE_Asynch_Read_Stream::open"));
  else
    initiate_read_stream ();

  check_destroy ();
}

int
Receiver::initiate_read_stream (void)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, mutex_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size ()- 1) == -1)
    {
      mb->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ACE_Asynch_Read_Stream::read"),
                        -1);
    }

  io_count_++;
  return 0;
}

int
Receiver::initiate_write_stream (ACE_Message_Block &mb, int nbytes)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, mutex_, -1);
  if (nbytes <= 0)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        "ACE_Asynch_Write_Stream::write nbytes <0 "),
                       -1);
    }

  if (this->ws_.write (mb, nbytes) == -1)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "ACE_Asynch_Write_Stream::write"),
                       -1);
    }

  io_count_++;
  return 0;
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  // Reset pointers.
  result.message_block ().rd_ptr ()[result.bytes_transferred ()] = '\0';

  if (result.bytes_transferred () == 0 || result.error () != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "handle_read_stream called\n"));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));
    }

  if (result.success () && result.bytes_transferred () != 0)
    {
      // Successful read: write the data to the file asynchronously.
      // Note how we reuse the <ACE_Message_Block> for the writing.
      // Therefore, we do not delete this buffer because it is handled
      // in <handle_write_stream>.

      if(this->initiate_write_stream (result.message_block (),
                                      result.bytes_transferred ()) == 0)
        {
          if (duplex != 0)
            {
              // Initiate new read from the stream.
              this->initiate_read_stream ();
            }
        }
    }
  else
    {
      result.message_block ().release ();
      ACE_DEBUG ((LM_DEBUG,  "Receiver completed\n"));
    }

  {
    ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
    --io_count_;
  }
  check_destroy ();
}

void
Receiver::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  if (result.bytes_transferred () == 0 || result.error () != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "handle_write_stream called\n"));

      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
    }

  result.message_block ().release ();

  if (result.success () && result.bytes_transferred () != 0)
    {
      // This code is not robust enough to deal with short file writes
      // (which hardly ever happen);-)
      // ACE_ASSERT (result.bytes_to_write () == result.bytes_transferred ());

      if (duplex == 0)
        initiate_read_stream ();
    }

  {
    ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
    --io_count_;
  }
  check_destroy ();
}

class Sender : public ACE_Handler
{
  // = TITLE
  //   Sends welcome messages receives them back.
public:
  Sender (void);
  ~Sender (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (const ACE_TCHAR *host, u_short port);
  void close (void);
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_HANDLE handle (void) const;
  virtual void handle (ACE_HANDLE);

protected:
  // These methods are called by the freamwork

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous reads from the socket complete

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete

private:

  int initiate_read_stream (void);
  int initiate_write_stream (void);

  ACE_SOCK_Stream stream_;
  // Network I/O handle

  ACE_Asynch_Write_Stream ws_;
  // ws (write stream): for writing to the socket

  ACE_Asynch_Read_Stream rs_;
  // rs (read file): for reading from the socket

  ACE_Message_Block welcome_message_;
  // Welcome message

  ACE_Recursive_Thread_Mutex mutex_;
  long io_count_;
};

static const char *data = "Welcome to Irfan World! Irfan RULES here !!\n";

Sender::Sender (void)
  : io_count_ (0)
{
  // Moment of inspiration... :-)
  this->welcome_message_.init (data, ACE_OS::strlen (data));
}

Sender::~Sender (void)
{
  this->close ();
}

void Sender::close (void)
{
  this->stream_.close ();
}

ACE_HANDLE Sender::handle (void) const
{
  return this->stream_.get_handle ();
}

void Sender::handle (ACE_HANDLE handle)
{
  this->stream_.set_handle (handle);
}

int Sender::open (const ACE_TCHAR *host, u_short port)
{
  // Initialize stuff
  // Connect to remote host
  ACE_INET_Addr address (port, host);
  ACE_SOCK_Connector connector;

  if (connector.connect (this->stream_, address) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ACE_SOCK_Connector::connect"),
                        -1);
    }

  // Open ACE_Asynch_Write_Stream
  if (this->ws_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Write_Stream::open"),
                      -1);

  // Open ACE_Asynch_Read_Stream
  if (this->rs_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Asynch_Read_Stream::open"),
                      -1);

  // Start an asynchronous transmit file
  if (this->initiate_write_stream () == -1)
    return -1;

  if (duplex != 0)
    // Start an asynchronous read file
    if (this->initiate_read_stream () == -1)
      return -1;

  return 0;
}

int
Sender::initiate_write_stream (void)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, mutex_, -1);

  welcome_message_.rd_ptr(welcome_message_.base ());
  welcome_message_.wr_ptr(welcome_message_.base ());
  welcome_message_.wr_ptr (ACE_OS::strlen (data));

  if (this->ws_.write (welcome_message_,
                       welcome_message_.length ()) == -1)
    ACE_ERROR_RETURN((LM_ERROR,
                      "%p\n",
                      "ACE_Asynch_Write_Stream::write"),
                     -1);
  io_count_++;
  return 0;
}

int
Sender::initiate_read_stream (void)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, mutex_, -1);

  // Create a new <Message_Block>.  Note that this message block will
  // be used both to <read> data asynchronously from the socket and to
  // <write> data asynchronously to the file.
  ACE_DEBUG ((LM_DEBUG,
              "initiate_read_stream called\n"));

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size ()- 1) == -1)
    {
      mb->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ACE_Asynch_Read_Stream::read"),
                        -1);
    }

  io_count_++;
  return 0;
}

void
Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  if (result.bytes_transferred () == 0 || result.error () != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "handle_write_stream called\n"));

      // Reset pointers.
      result.message_block ().rd_ptr (result.message_block ().rd_ptr () - result.bytes_transferred ());

      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));
    }

  // Simplify just for Test
  if (result.success () && result.bytes_transferred () != 0)
    {
      if (duplex != 0)  // full duplex, continue write
        initiate_write_stream ();
      else  // half-duplex   read reply, after read we will start write
        initiate_read_stream ();
    }

  {
    ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
    --io_count_;
  }
}

void
Sender::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  if (result.bytes_transferred () == 0 || result.error () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "handle_read_stream called\n"));

      // Reset pointers.
      result.message_block ().rd_ptr ()[result.bytes_transferred ()] = '\0';

      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
      ACE_DEBUG ((LM_DEBUG, "********************\n"));
      ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));
    }

  result.message_block().release ();

  if (result.success () && result.bytes_transferred () != 0)
    {
      // Successful read: write the data to the file asynchronously.
      // Note how we reuse the <ACE_Message_Block> for the writing.
      // Therefore, we do not delete this buffer because it is handled
      // in <handle_write_stream>.

      if (duplex != 0)  // full duplex, continue read
        initiate_read_stream ();
      else  // half-duplex  writey, after write we will start read
        initiate_write_stream ();
    }

  {
    ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, mutex_);
    --io_count_;
  }
}

static int
set_proactor_type (const char *ptype)
{
  if (!ptype)
    return false;

  switch (ACE_OS::ace_toupper (*ptype))
    {
    case 'D' :  proactor_type = 0; return true;
    case 'A' :  proactor_type = 1; return true;
    case 'I' :  proactor_type = 2; return true;
#if defined (sun)
    case 'S' :  proactor_type = 3; return true;
#endif /* sun */
    }
  return false;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:o:n:p:d:h:s:u"));
  int c;

  while ((c = get_opt ()) != EOF)
    switch (c)
      {
      case 'd': // duplex
        duplex = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'h': // host for sender
        host = get_opt.opt_arg ();
        break;
      case 'p': // port number
        port = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'n': // thread pool size
        threads = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 's': // number of senders
        senders = ACE_OS::atoi (get_opt.opt_arg ());
        if (senders > MaxSenders)
          senders = MaxSenders;
        break;
      case 'o': // max number of aio for proactor
        max_aio_operations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 't': //  Proactor Type
        if (set_proactor_type (get_opt.opt_arg ()))
          break;
      case 'u':
      default:
        ACE_ERROR ((LM_ERROR, "%p.",
                    "\nusage:"
                    "\n-o <max number of started aio operations for Proactor>"
                    "\n-t <Proactor type> UNIX-only, Win32-default always:"
                    "\n    a AIOCB"
                    "\n    i SIG"
                    "\n    s SUN"
                    "\n    d default"
                    "\n-d <duplex mode 1-on/0-off>"
                    "\n-h <host> for Sender mode"
                    "\n-n <number threads for Proactor pool>"
                    "\n-p <port to listen/connect>"
                    "\n-s <number of sender's instances>"
                    "\n-u show this message"
                    "\n"));

        return -1;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
#if defined (sun)
  ACE_DEBUG ((LM_DEBUG, "\nSUN defined!\n"));
#endif
  if (parse_args (argc, argv) == -1)
    return -1;

  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);

  MyTask task1;

  if (task1.activate (THR_NEW_LWP, threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p.\n",
                       "main"),
                      -1);

  // wait for creation of Proactor
  task1.waitready ();

  Sender * send_list[MaxSenders];

  ACE_Asynch_Acceptor<Receiver> acceptor;

  int rc = -1;
  int i;
  char c;

  if (host == 0) // Acceptor
    {
      // Simplify, initial read with  zero size
      if (acceptor.open (ACE_INET_Addr (port),0,1) == 0)
        rc = 1;
    }
  else
    {
      for (i = 0; i < senders; ++i)
        send_list[i] = new Sender;

      for (i = 0; i < senders; ++i)
        if (send_list[i]->open (host, port) == 0)
          rc++;
    }

  if (rc > 0)
    {
      cout << "Press any key to stop=>" << flush;
      cin.clear ();
      cin >> c;
    }

  ACE_Proactor::end_event_loop ();

  if (host != 0) // we are sender
    {
      for (i = 0; i < senders; ++i)
        send_list[i]->close ();
    }


  ACE_Thread_Manager *tm =
    ACE_Thread_Manager::instance();

  tm->wait_task (&task1);

  cout << "\nNumber of Receivers objects="
       << Receiver::get_number_sessions ()
       << flush;

  for (i = 0; i < senders; ++i)
    {
      delete (send_list[i]);
      send_list[i] = 0;
    }

  return 0;
}

static int
disable_signal (int sigmin, int sigmax)
{
#ifndef ACE_WIN32

  sigset_t signal_set;
  if (ACE_OS::sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                "Error:(%P | %t):%p\n",
                "sigemptyset failed"));

  for (int i = sigmin; i <= sigmax; i++)
    ACE_OS::sigaddset (&signal_set, i);

  //  Put the <signal_set>.
  if (ACE_OS::pthread_sigmask (SIG_BLOCK, &signal_set, 0) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error:(%P | %t):%p\n",
                "pthread_sigmask failed"));
#else
  ACE_UNUSED_ARG (sigmin);
  ACE_UNUSED_ARG (sigmax);
#endif /* ACE_WIN32 */

  return 1;
}

// Get the <signal_set> back from the OS.

#if 0
static int
print_sigmask (void)
{
#ifndef ACE_WIN32
  sigset_t  mask;
  int member = 0;

  COUT ("\n=============Signal Mask==========")

  if (ACE_OS::pthread_sigmask (SIG_SETMASK, 0, & mask) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error:(%P | %t):%p\n",
                "ACE_OS::pthread_sigmask failed"));
  else
    for (int i = 1; i < 1000; i++)
      {
        member = ACE_OS::sigismember (&mask,i);

        COUT ("\nSig ")
        COUT (i)
        COUT (" is ")
        COUT (member)

        if (member == -1)
          break;
    }

#endif /* ACE_WIN32 */
  return 0;
}
#endif /* 0 */

#else /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example does not work on this platform.\n"));
  return 1;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
