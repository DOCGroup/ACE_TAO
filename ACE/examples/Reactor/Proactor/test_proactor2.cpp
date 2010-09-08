// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_proactor2.cpp
//
// = DESCRIPTION
//    Alexander Libman <Alibman@baltimore.com> modified
//    <test_proactor> and made this test. Instead of writing received
//    data to the file, the receiver sends them back to the
//    sender,i.e. ACE_Asynch_Write_File wf_  has been changed to
//    ACE_Asynch_Write_Stream wf_.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu> and Alexander Libman
//    <Alibman@baltimore.com>.
// ============================================================================

#include "ace/Signal.h"

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

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Task.h"
#include "ace/OS_main.h"



#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms supporting
  // POSIX aio calls.

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)

#include "ace/WIN32_Proactor.h"

#elif defined (ACE_HAS_AIO_CALLS)

#include "ace/POSIX_Proactor.h"

#endif

  //  Some debug helper functions
  int DisableSignal ( int SigNum );
int PrintSigMask ();

#define  COUT(X)  cout << X ; cout.flush ();

// Host that we're connecting to.
static ACE_TCHAR *host = 0;

// duplex mode: ==0 half-duplex
//              !=0 full duplex
static int duplex = 0 ;

// number threads in the Proactor thread pool
static int nThreads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Size of each initial asynchronous <read> operation.
static int initial_read_size = BUFSIZ;


#define MyMutex ACE_Recursive_Thread_Mutex
//#define MyMutex ACE_Thread_Mutex
//#define MyMutex ACE_Null_Mutex

//--------------------------------------------------------------------------
// MyTask plays role for Proactor threads pool
//--------------------------------------------------------------------------
class MyTask: public ACE_Task<ACE_MT_SYNCH>
{

public:

  int svc (void) ;
};


int MyTask::svc (void )
{
  ACE_DEBUG ((LM_DEBUG, "(%t) MyTask started\n"));

  while ( ACE_Proactor::event_loop_done () == 0 )
    {
      ACE_Proactor::run_event_loop ();
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) MyTask finished\n"));
  return 0 ;
}

//-----------------------------------------------------------
//  Receiver
//-----------------------------------------------------------
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

protected:
  // These methods are called by the framework

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result
                                   &result);
  // This is called when asynchronous <read> operation from the socket
  // complete.

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result
                                    &result);
  // This is called when an asynchronous <write> to the file
  // completes.

private:
  int  initiate_read_stream  (void);
  int  initiate_write_stream (ACE_Message_Block & mb, int nBytes );
  bool check_destroy () ;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;
  MyMutex m_Mtx ;
  long    nIOCount ;
  static long nSessions ;
};


long Receiver::nSessions = 0 ;

Receiver::Receiver (void)
  : handle_ (ACE_INVALID_HANDLE),
    nIOCount ( 0 )
{
  ACE_GUARD (MyMutex, locker, m_Mtx);
  nSessions ++ ;
  ACE_DEBUG ((LM_DEBUG, "Receiver Ctor nSessions=%d\n", nSessions ));
}

Receiver::~Receiver (void)
{
  ACE_GUARD (MyMutex, locker, m_Mtx);
  nSessions -- ;
  ACE_OS::closesocket (this->handle_);
  ACE_DEBUG ((LM_DEBUG, "~Receiver Dtor nSessions=%d\n", nSessions ));
}

//---------------------------------------------------------------------
//  return true if we alive, false  we commited suicide
//
//---------------------------------------------------------------------
bool Receiver::check_destroy ()
{
  {
    ACE_GUARD_RETURN (MyMutex, locker, m_Mtx, false);

    if ( nIOCount > 0 )
      {
        return true ;
      }
  }

  delete this ;
  return false ;
}


void Receiver::open (ACE_HANDLE handle,
                     ACE_Message_Block &)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:Receiver::open called\n"));


  this->handle_ = handle;

  if (this->ws_.open (*this, this->handle_ ) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Write_Stream::open"));

    }
  else if (this->rs_.open (*this, this->handle_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "ACE_Asynch_Read_Stream::open"));
    }
  else
    {
      initiate_read_stream ();
    }


  check_destroy ();
}

int Receiver::initiate_read_stream (void)
{
  ACE_GUARD_RETURN (MyMutex, locker, m_Mtx, -1);

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
      mb->release () ;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ACE_Asynch_Read_Stream::read"),
                        -1);
    }

  nIOCount++ ;
  return 0;
}

int Receiver::initiate_write_stream (ACE_Message_Block & mb, int nBytes )
{
  ACE_GUARD_RETURN (MyMutex, locker, m_Mtx, -1);
  if (this->ws_.write (mb , nBytes ) == -1)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "ACE_Asynch_Write_File::write"),
                       -1);
    }

  nIOCount++ ;
  return 0;
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_read_stream called\n"));

  // Reset pointers.
  result.message_block ().rd_ptr ()[result.bytes_transferred ()] =
    '\0';

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read
              ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered",
              result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long)
              result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block",
              result.message_block ().rd_ptr ()));

  if ( result.success () && result.bytes_transferred () != 0)
    {
      // Successful read: write the data to the file asynchronously.
      // Note how we reuse the <ACE_Message_Block> for the writing.
      // Therefore, we do not delete this buffer because it is handled
      // in <handle_write_stream>.

      if(this->initiate_write_stream (result.message_block (),

                                      result.bytes_transferred () ) == 0 )
        {
          if ( duplex != 0 )
            {
              // Initiate new read from the stream.
              this->initiate_read_stream () ;
            }
        }
    }
  else
    {
      result.message_block ().release ();
      ACE_DEBUG ((LM_DEBUG,  "Receiver completed\n"));
    }

  {
    ACE_GUARD (MyMutex, locker, m_Mtx);
    --nIOCount;
  }
  check_destroy () ;
}

void
Receiver::handle_write_stream (const ACE_Asynch_Write_Stream::Result
                               &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_write_stream called\n"));

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write",
              result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered",
              result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long)
              result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));

  result.message_block ().release ();

  if (result.success ())
    {
      // This code is not robust enough to deal with short file writes
      // (which hardly ever happen) ;-)
      //ACE_ASSERT (result.bytes_to_write () == result.bytes_transferred ());

      if ( duplex == 0 )
        {
          initiate_read_stream () ;
        }
    }

  {
    ACE_GUARD (MyMutex, locker, m_Mtx);
    --nIOCount;
  }
  check_destroy () ;
}

//-------------------------------------------------------------------------
// Sender:  sends indefinetely welcome message
//  and  recieves it back
//------------------------------------------------------------------------
class Sender : public ACE_Handler
{
public:
  Sender (void);
  ~Sender (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (const ACE_TCHAR *host, u_short port);
  void close ();
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_HANDLE handle (void) const;
  void handle (ACE_HANDLE);

protected:
// These methods are called by the freamwork

virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result
&result);
// This is called when asynchronous reads from the socket complete

virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result
&result);
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

MyMutex m_Mtx ;
long    nIOCount ;
};

static const char *data = "Welcome to Irfan World! Irfan RULES here !!\n";

Sender::Sender (void)
  :nIOCount ( 0 )
{
  // Moment of inspiration... :-)
  this->welcome_message_.init (data, ACE_OS::strlen (data));
}

Sender::~Sender (void)
{
  this->close ();
}

void Sender::close ()
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

  if (connector.connect (this->stream_,
                         address) == -1)
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
                       "ACE_Asynch_Read_File::open"),
                      -1);

  // Start an asynchronous transmit file
  if ( this->initiate_write_stream () == -1)
    return -1;

  if ( duplex != 0 )
    {
      // Start an asynchronous read file
      if (this->initiate_read_stream () == -1)
        return -1;
    }

  return 0;
}

int Sender::initiate_write_stream (void)
{
  ACE_GUARD_RETURN (MyMutex, locker, m_Mtx, -1);

  welcome_message_.rd_ptr( welcome_message_.base ());
  welcome_message_.wr_ptr( welcome_message_.base ());
  welcome_message_.wr_ptr (ACE_OS::strlen (data));

  if (this->ws_.write (welcome_message_,
                       welcome_message_.length ()
                       ) == -1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "ACE_Asynch_Write_File::write"),
                       -1);
    }

  nIOCount++ ;
  return 0;
}

int Sender::initiate_read_stream (void)
{
  ACE_GUARD_RETURN (MyMutex, locker, m_Mtx, -1);

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
      mb->release () ;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ACE_Asynch_Read_Stream::read"),
                        -1);
    }

  nIOCount++ ;
  return 0;
}


void Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result
                                  &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_write_stream called\n"));

  // Reset pointers.
  result.message_block ().rd_ptr (result.message_block ().rd_ptr () -
                                  result.bytes_transferred ());


  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write",
              result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered",
              result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long)
              result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block",
              result.message_block ().rd_ptr ()));

  // Simplify just for Test
  if (result.success () && result.bytes_transferred () != 0)
    {
      if ( duplex != 0 )  // full duplex, continue write
        {
          initiate_write_stream () ;
        }
      else  // half-duplex   read reply, after read we will start
        // write
        {
          initiate_read_stream () ;
        }
    }

  {
    ACE_GUARD_RETURN (MyMutex, locker, m_Mtx);
    --nIOCount;
  }
}

void
Sender::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG,
              "handle_read_stream called\n"));

  // Reset pointers.
  result.message_block ().rd_ptr ()[result.bytes_transferred ()] =
    '\0';

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read
              ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered",
              result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long)
              result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block",
              result.message_block ().rd_ptr ()));

  result.message_block().release ();

  if ( result.success () && result.bytes_transferred () != 0)
    {
      // Successful read: write the data to the file asynchronously.
      // Note how we reuse the <ACE_Message_Block> for the writing.
      // Therefore, we do not delete this buffer because it is handled
      // in <handle_write_stream>.

      if ( duplex != 0 )  // full duplex, continue read
        {
          initiate_read_stream () ;
        }
      else  // half-duplex  writey, after write we will start read
        {
          initiate_write_stream () ;
        }
    }

  {
    ACE_GUARD (MyMutex, locker, m_Mtx);
    --nIOCount;
  }
}

//--------------------------------------------------------------------------

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:p:d:h:"));
  int c;

  while ((c = get_opt ()) != EOF)
    switch (c)
      {
      case 'h':
        host = get_opt.opt_arg ();
        break;
      case 'n':
        nThreads = ACE_OS::atoi (get_opt.opt_arg ()) ;
        break;
      case 'p':
        port = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'd':
        duplex = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        ACE_ERROR ((LM_ERROR, "%p.\n",
                    "usage :\n"
                    "-h <host> for Sender mode\n"
                    "-d <duplex mode 1-on/0-off>\n"
                    "-p <port to listen/connect>\n"
                    "-n <number threads for Proactor pool>\n"));
        return -1;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (initial_read_size);

  if (parse_args (argc, argv) == -1)
    return -1;

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)

  ACE_WIN32_Proactor *      pImpl = new ACE_WIN32_Proactor;

#elif defined (ACE_HAS_AIO_CALLS)

  //  ACE_POSIX_AIOCB_Proactor *  pImpl = new ACE_POSIX_AIOCB_Proactor;
  ACE_POSIX_SIG_Proactor *  pImpl = new ACE_POSIX_SIG_Proactor;
#endif

  ACE_Proactor Proactor ( pImpl ,1 );

  ACE_Proactor::instance( & Proactor );


  MyTask  Task1 ;

  if (Task1.activate (THR_NEW_LWP, nThreads ) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);
    }

  Sender sender;
  ACE_Asynch_Acceptor<Receiver> acceptor;

  int Rc = -1 ;

  if ( host == 0 ) // Acceptor
    {
      // Simplify , initial read with  zero size
      Rc = acceptor.open (ACE_INET_Addr (port),0,1);

    }
  else
    {
      Rc = sender.open (host, port);
    }

  if ( Rc == 0 )
    {
      char c ;
      cout << "Press any key to stop and exit=>\n" << flush ;
      cin.clear ();
      cin >> c ;
    }

  ACE_Proactor::end_event_loop () ;

  if ( host != 0 ) // we are sender
    {
      sender.close () ; // disconnect to get reciever error !!!
    }


  ACE_Thread_Manager * pTM = ACE_Thread_Manager::instance();

  pTM->wait_task ( & Task1 ) ;

  ACE_Proactor::instance( ( ACE_Proactor* )0 );

  return 0;
}
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int DisableSignal ( int SigNum )
{

#ifndef ACE_WIN32
  sigset_t signal_set;
  if ( ACE_OS::sigemptyset (&signal_set) == - 1 )
    {
      ACE_ERROR ((LM_ERROR,
                  "Error:(%P | %t):%p\n",
                  "sigemptyset failed"));
    }

  ACE_OS::sigaddset (&signal_set, SigNum);

  //  Put the <signal_set>.
  if (ACE_OS::pthread_sigmask (SIG_BLOCK, &signal_set, 0) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error:(%P | %t):%p\n",
                  "pthread_sigmask failed"));
    }
#else
  ACE_UNUSED_ARG(SigNum);
#endif

  return  1;
}
//--------------------------------------------------------------------
// Get the <signal_set> back from the OS.
//--------------------------------------------------------------------

int PrintSigMask ()
{
#ifndef ACE_WIN32

  sigset_t  mask ;
  int member = 0;

  COUT ( "\n=============Signal Mask==========" )

    if (ACE_OS::pthread_sigmask (SIG_SETMASK, 0, & mask ) != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "Error:(%P | %t):%p\n",
                    "ACE_OS::pthread_sigmask failed"));
      }
    else  for (int i = 1 ; i < 1000; i++)
      {
        member = ACE_OS::sigismember (&mask,i);

        COUT ( "\nSig " )
          COUT ( i )
          COUT ( " is " )
          COUT (member )

          if (member == -1)
            {
              break ;
            }
      }
#endif
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
