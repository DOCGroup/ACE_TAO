// -*- C++ -*-

// ============================================================================
/**
 *  @file Proactor_Test.cpp
 *
 *  $Id$
 *
 *  This program illustrates how the ACE_Proactor can be used to
 *  implement an application that does various asynchronous
 *  operations.
 *
 *  @author Alexander Libman <alibman@baltimore.com>
 */
// ============================================================================

#include "test_config.h"

ACE_RCSID (tests,
           Proactor_Test,
           "$Id$")

#if defined (ACE_HAS_THREADS) && ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

#include "ace/Signal.h"

#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Object_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Task.h"

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

#  include "ace/WIN32_Proactor.h"

#elif defined (ACE_HAS_AIO_CALLS)

#  include "ace/POSIX_Proactor.h"
#  include "ace/POSIX_CB_Proactor.h"
#  include "ace/SUN_Proactor.h"

#endif /* defined (ACE_WIN32) && !defined (ACE_HAS_WINCE) */

// Proactor Type (UNIX only, Win32 ignored)
typedef enum { DEFAULT = 0, AIOCB, SIG, SUN, CB } ProactorType;
static ProactorType proactor_type = DEFAULT;

// POSIX : > 0 max number aio operations  proactor,
static size_t max_aio_operations = 0;

// both: 0 run client or server / depends on host
//     != 0 run client and server
static int both = 0;

// Host that we're connecting to.
static const ACE_TCHAR *host = 0;

// number of Senders instances
static int senders = 1;
const  int MAX_SENDERS = 1000;
const  int MAX_RECEIVERS = 1000;

// duplex mode: == 0 half-duplex
//              != 0 full duplex
static int duplex = 0;

// number threads in the Proactor thread pool
static size_t threads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Log options
static int loglevel = 0; // 0 full , 1 only errors

static const size_t MIN_TIME = 1;    // min 1 sec
static const size_t MAX_TIME = 3600; // max 1 hour
static size_t seconds = 2;  // default time to run - 2 seconds

static ACE_TCHAR request_line[] =
  ACE_TEXT ("GET / HTTP/1.1\r\n");

static ACE_TCHAR headers[] =
  ACE_TEXT ("Accept: */*\r\n")
  ACE_TEXT ("Accept-Language: C++\r\n")
  ACE_TEXT ("Accept-Encoding: gzip, deflate\r\n")
  ACE_TEXT ("User-Agent: Proactor_Test/1.0 (non-compatible)\r\n")
  ACE_TEXT ("Connection: Keep-Alive\r\n");

static ACE_TCHAR end_of_request_header[] =
  ACE_TEXT ("\r\n");

class LogLocker 
{
public:

  LogLocker () { ACE_LOG_MSG->acquire (); }
  virtual ~LogLocker () { ACE_LOG_MSG->release (); }
};



// Function to remove signals from the signal mask.
static int
disable_signal (int sigmin, int sigmax)
{
#ifndef ACE_WIN32

  sigset_t signal_set;
  if (sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: (%P|%t):%p\n"),
                ACE_TEXT ("sigemptyset failed")));

  for (int i = sigmin; i <= sigmax; i++)
    sigaddset (&signal_set, i);

  //  Put the <signal_set>.
  if (ACE_OS::pthread_sigmask (SIG_BLOCK, &signal_set, 0) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: (%P|%t):%p\n"),
                ACE_TEXT ("pthread_sigmask failed")));
#else
  ACE_UNUSED_ARG (sigmin);
  ACE_UNUSED_ARG (sigmax);
#endif /* ACE_WIN32 */

  return 1;
}


// *************************************************************
//  MyTask is ACE_Task resposible for :
//  1. creation and deletion of
//     Proactor and Proactor thread pool
//  2. running Proactor event loop
// *************************************************************

/**
 * @class MyTask
 *
 * MyTask plays role for Proactor threads pool
 *
 * MyTask is ACE_Task resposible for:
 * 1. Creation and deletion of Proactor and Proactor thread pool
 * 2. Running Proactor event loop
 */
class MyTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  MyTask (void): lock_ (), sem_ (0), proactor_(0) {}

  virtual ~MyTask() { (void) this->stop (); }

  virtual int svc (void);

  int start (size_t num_threads,
             ProactorType type_proactor,
             size_t max_op );
  int stop  (void);

private:
  int  create_proactor (ProactorType type_proactor,
                        size_t max_op);
  int  delete_proactor (void);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  ACE_Thread_Semaphore sem_;
  ACE_Proactor * proactor_;

};

int
MyTask::create_proactor (ProactorType type_proactor, size_t max_op)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_ASSERT (this->proactor_ == 0);

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  ACE_UNUSED_ARG (type_proactor);
  ACE_UNUSED_ARG (max_op);

  ACE_WIN32_Proactor *proactor = 0;

  ACE_NEW_RETURN (proactor,
                  ACE_WIN32_Proactor,
                  -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%t) Create Proactor Type = WIN32\n")));

#elif defined (ACE_HAS_AIO_CALLS)

  ACE_POSIX_Proactor * proactor = 0;

  switch (type_proactor)
    {
    case AIOCB:
      ACE_NEW_RETURN (proactor,
                      ACE_POSIX_AIOCB_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = AIOCB\n")));
      break;

    case SIG:
      ACE_NEW_RETURN (proactor,
                      ACE_POSIX_SIG_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = SIG\n")));
      break;

#  if defined (sun)
    case SUN:
      ACE_NEW_RETURN (proactor,
                      ACE_SUN_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%t) Create Proactor Type = SUN\n")));
      break;
#  endif /* sun */

#  if defined (__sgi)
    case CB:
      ACE_NEW_RETURN (proactor,
                      ACE_POSIX_CB_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = CB\n")));
      break;
#  endif

    default:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = DEFAULT\n")));
      break;
  }

#endif // (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  ACE_NEW_RETURN (this->proactor_,
                  ACE_Proactor (proactor, !(proactor == 0)),
                  -1);

  ACE_Proactor::instance (this->proactor_);
  return 0;
}

int
MyTask::delete_proactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Delete Proactor\n")));

  ACE_Proactor::instance ((ACE_Proactor *) 0);
  delete this->proactor_;
  this->proactor_ = 0;

  return 0;
}

int
MyTask::start (size_t num_threads,
               ProactorType type_proactor,
               size_t max_op)
{
  if (this->create_proactor (type_proactor, max_op) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to create proactor")),
                      -1);

  if (this->activate (THR_NEW_LWP, num_threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to activate thread pool")),
                      -1);

  for (; num_threads > 0; num_threads--)
    {
      sem_.acquire ();
    }

  return 0;
}


int
MyTask::stop ()
{
  if (this->proactor_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("End Proactor event loop\n")));

      ACE_Proactor::end_event_loop ();
    }

  if (this->wait () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to stop thread pool")));

  if (this->delete_proactor () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to delete proactor")));

  return 0;
}

int
MyTask::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) MyTask started\n")));

  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);

  // signal that we are ready
  sem_.release (1);

  ACE_Proactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) MyTask finished\n")));
  return 0;
}

// *************************************************************
//   Receiver and Acceptor
// *************************************************************
// forward declaration
class Acceptor;

class Receiver : public ACE_Service_Handler
{
  friend class Acceptor;
public:
  Receiver  (Acceptor *acceptor = 0, int index = -1);
  ~Receiver (void);

  /// This is called after the new connection has been accepted.
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);

protected:
  /**
   * @name AIO callback handling
   *
   * These methods are called by the framework
   */
  /// This is called when asynchronous <read> operation from the
  /// socket completes.
  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);

  /// This is called when an asynchronous <write> to the socket
  /// completes.
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

private:
  int initiate_read_stream (void);
  int initiate_write_stream (ACE_Message_Block &mb, int nbytes);
  int check_destroy (void);
  void cancel ();

  Acceptor *acceptor_;
  int index_;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;
  ACE_SYNCH_RECURSIVE_MUTEX lock_;

  long io_count_;
  int flg_cancel_;
};

class Acceptor : public ACE_Asynch_Acceptor<Receiver>
{
  friend class Receiver;
public:
  int get_number_sessions (void) { return this->sessions_; }

  Acceptor (void);
  virtual ~Acceptor (void);

  void stop (void);
  void cancel_all (void);

  // Virtual from ACE_Asynch_Acceptor
  Receiver *make_handler (void);

private:
  void on_new_receiver (Receiver &rcvr);
  void on_delete_receiver (Receiver &rcvr);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  int sessions_;
  Receiver *list_receivers_[MAX_RECEIVERS];
};

// *************************************************************
Acceptor::Acceptor (void)
  : sessions_ (0)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  for (int i = 0; i < MAX_RECEIVERS; ++i)
     this->list_receivers_[i] = 0;
}

Acceptor::~Acceptor (void)
{
  this->stop ();
}


void
Acceptor::cancel_all (void)
{
  // This method can be called only after proactor event loop is done
  // in all threads.

  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->cancel ();

  for (int i = 0; i < MAX_RECEIVERS; ++i)
    {
      if (this->list_receivers_[i] != 0)
        this->list_receivers_[i]->cancel ();
    }
  return;
}


void
Acceptor::stop (void)
{
  // This method can be called only after proactor event loop is done
  // in all threads.

  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  for (int i = 0; i < MAX_RECEIVERS; ++i)
    {
      delete this->list_receivers_[i];
      this->list_receivers_[i] = 0;
    }
}

void
Acceptor::on_new_receiver (Receiver & rcvr)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);
  this->sessions_++;
  this->list_receivers_[rcvr.index_] = &rcvr;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::CTOR sessions_ = %d\n"),
              this->sessions_));
}

void
Acceptor::on_delete_receiver (Receiver & rcvr)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->sessions_--;
  if (rcvr.index_ >= 0
      && rcvr.index_ < MAX_RECEIVERS
      && this->list_receivers_[rcvr.index_] == &rcvr)
    this->list_receivers_[rcvr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::~DTOR sessions_ = %d\n"),
              this->sessions_));
}

Receiver *
Acceptor::make_handler (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, 0);

  if (this->sessions_ >= MAX_RECEIVERS)
    return 0;

  for (int i = 0; i < MAX_RECEIVERS; ++i)
    {
      if (this->list_receivers_[i] == 0)
        {
          ACE_NEW_RETURN (this->list_receivers_[i],
                          Receiver (this, i),
                          0);
          return this->list_receivers_[i];
        }
    }

  return 0;
}
// ***************************************************
Receiver::Receiver (Acceptor * acceptor, int index)
  : acceptor_ (acceptor),
    index_    (index),
    handle_   (ACE_INVALID_HANDLE),
    io_count_ (0),
    flg_cancel_ (0)
{
  if (this->acceptor_ != 0)
    this->acceptor_->on_new_receiver (*this);
}

Receiver::~Receiver (void)
{
  if (this->acceptor_ != 0)
    this->acceptor_->on_delete_receiver (*this);

  if (this->handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->handle_);

  this->index_ = -1;
  this->handle_= ACE_INVALID_HANDLE;
}

// return true if we alive, false if we commited suicide
int
Receiver::check_destroy (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, -1);

    if (this->io_count_ > 0)
      return 1;
  }

  delete this;
  return 0;
}


void
Receiver::cancel ()
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->flg_cancel_ = 1;
  this->ws_.cancel ();
  this->rs_.cancel ();
  return;
}


void
Receiver::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  this->handle_ = handle;

  if (this->ws_.open (*this, this->handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Receiver::ACE_Asynch_Write_Stream::open")));
  else if (this->rs_.open (*this, this->handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Receiver::ACE_Asynch_Read_Stream::open")));
  else
    this->initiate_read_stream ();

  this->check_destroy ();
}

int
Receiver::initiate_read_stream (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, -1);

  if (this->flg_cancel_ != 0)
    return 0;

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (1024), //BUFSIZ + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size () - 1) == -1)
    {
      mb->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Receiver::ACE_Asynch_Stream::read")),
                        -1);
    }

  this->io_count_++;
  return 0;
}

int
Receiver::initiate_write_stream (ACE_Message_Block &mb, int nbytes)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, -1);

  if (this->flg_cancel_ != 0)
   {
      mb.release ();
      return -1;
   }

  if (nbytes <= 0)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("Receiver::ACE_Asynch_Write_Stream::write nbytes <0 ")),
                       -1);
    }

  if (this->ws_.write (mb, nbytes) == -1)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Receiver::ACE_Asynch_Write_Stream::write")),
                       -1);
    }

  this->io_count_++;
  return 0;
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_Message_Block & mb = result.message_block ();

  // Reset pointers.
  mb.rd_ptr ()[result.bytes_transferred ()] = '\0';

  if (loglevel == 0
      || result.bytes_transferred () == 0
      || result.error () != 0)
    {
      LogLocker log_lock;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** Receiver::handle_read_stream() SessionId = %d ****\n"),
                  this->index_));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_to_read"),
                  result.bytes_to_read ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("handle"),
                  result.handle ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_transfered"),
                  result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("act"),
                  (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("success"),
                  result.success ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("completion_key"),
                  (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("error"),
                  result.error ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %s\n"),
                  ACE_TEXT ("message_block"),
                  mb.rd_ptr ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** end of message ****************\n")));
    }

  if (result.error () == 0 && result.bytes_transferred () != 0)
    {
      if(this->initiate_write_stream (mb,
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
    mb.release ();

  {
    ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);
    this->io_count_--;
  }

  this->check_destroy ();
}

void
Receiver::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_Message_Block & mb = result.message_block ();

  if (loglevel == 0 ||
      result.bytes_transferred () == 0 ||
      result.error () != 0)
    {
      LogLocker log_lock;

      //mb.rd_ptr () [0] = '\0';
      mb.rd_ptr (mb.rd_ptr () - result.bytes_transferred ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** Receiver::handle_write_stream() SessionId = %d ****\n"),
                  this->index_));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_to_write"),
                  result.bytes_to_write ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("handle"),
                  result.handle ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_transfered"),
                  result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("act"),
                  (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("success"),
                  result.success ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("completion_key"),
                  (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("error"),
                  result.error ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %s\n"),
                  ACE_TEXT ("message_block"),
                  mb.rd_ptr ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** end of message ****************\n")));
    }

  mb.release ();

  if (result.error () == 0 && result.bytes_transferred () != 0)
    {
      if (duplex == 0)
        this->initiate_read_stream ();
    }

  {
    ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);
    this->io_count_--;
  }

  this->check_destroy ();
}

// *******************************************
//   Sender
// *******************************************

class Connector;

class Sender : public ACE_Service_Handler
{
  friend class Connector;
public:

  /// This is called after the new connection has been established.
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);

  Sender  (Connector *connector = 0, int index = -1);
  ~Sender (void);

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous reads from the socket complete

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete

private:
  int check_destroy (void);
  int initiate_read_stream (void);
  int initiate_write_stream (void);
  void cancel ();

  int  index_;
  Connector * connector_;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;

  ACE_SYNCH_RECURSIVE_MUTEX lock_;

  long io_count_;
  int flg_cancel_;
};

class Connector : public ACE_Asynch_Connector<Sender>
{
  friend class Sender;
public:
  int get_number_sessions (void) { return this->sessions_; }

  Connector (void);
  virtual ~Connector (void);

  int  start (const ACE_INET_Addr &addr, int num);
  void stop (void);
  void cancel_all (void);

  // Virtual from ACE_Asynch_Connector
  Sender *make_handler (void);

private:
  void on_new_sender  (Sender &rcvr);
  void on_delete_sender (Sender &rcvr);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  int sessions_;
  Sender *list_senders_[MAX_SENDERS];
};

// *************************************************************

Connector::Connector (void)
  : sessions_ (0)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  for (int i = 0; i < MAX_SENDERS; ++i)
     this->list_senders_[i] = 0;
}

Connector::~Connector (void)
{
  this->stop ();
}


void
Connector::cancel_all(void)
{
  // This method can be called only after proactor event loop is done
  // in all threads.
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->cancel ();

  for (int i = 0; i < MAX_SENDERS; ++i)
    {
      if (this->list_senders_[i] != 0)
        this->list_senders_[i]->cancel ();
    }
  return;
}

void
Connector::stop (void)
{
  // This method can be called only after proactor event loop is done
  // in all threads.

  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  for (int i = 0; i < MAX_SENDERS; ++i)
    {
      delete this->list_senders_[i];
      this->list_senders_[i] = 0;
    }
}

void
Connector::on_new_sender (Sender &sndr)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);
  this->sessions_++;
  this->list_senders_[sndr.index_] = &sndr;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::CTOR sessions_ = %d\n"),
              this->sessions_));
}

void
Connector::on_delete_sender (Sender &sndr)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->sessions_--;
  if (sndr.index_ >= 0
      && sndr.index_ < MAX_SENDERS
      && this->list_senders_[sndr.index_] == &sndr)
    this->list_senders_[sndr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::~DTOR sessions_ = %d\n"),
              this->sessions_));
}

Sender *
Connector::make_handler (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, 0);

  if (this->sessions_ >= MAX_SENDERS)
    return 0;

  for (int i = 0; i < MAX_SENDERS; ++i)
    {
      if (this->list_senders_ [i] == 0)
        {
          ACE_NEW_RETURN (this->list_senders_[i],
                          Sender (this, i),
                          0);
          return this->list_senders_[i];
        }
    }

  return 0;
}


int
Connector::start (const ACE_INET_Addr& addr, int num)
{

  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, 0);

  if (num > MAX_SENDERS)
    num = MAX_SENDERS;

  if (num < 0)
    num = 1;

  int rc = 0;

  // int open  ( int pass_addresses = 0,
  //             ACE_Proactor *proactor = 0,
  //             int validate_new_connection = 0 );

  if (this->open (1, 0, 1) != 0)
  {
     ACE_ERROR ((LM_ERROR,
                 ACE_LIB_TEXT ("%p\n"),
                 ACE_LIB_TEXT ("Connector::open failed")));
     return rc;
  }

  for (; rc < num;  rc++)
    {
      if (this->connect (addr) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("Connector::connect failed")));
          break;
        }
    }
  return rc;
}


Sender::Sender (Connector * connector, int index)
  : index_     (index),
    connector_ (connector),
    handle_    (ACE_INVALID_HANDLE),
    io_count_  (0),
    flg_cancel_ (0)
{
  if (this->connector_ != 0)
    this->connector_->on_new_sender (*this);
}

Sender::~Sender (void)
{
  if (this->connector_ != 0)
    this->connector_->on_delete_sender (*this);

  if (this->handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::closesocket (this->handle_);
    }

  this->index_ = -1;
  this->handle_= ACE_INVALID_HANDLE;
}

//  return true if we alive, false  we commited suicide
int
Sender::check_destroy (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, -1);

    if (this->io_count_ > 0)
      return 1;
  }

  delete this;
  return 0;
}

void
Sender::cancel ()
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->flg_cancel_ = 1;
  this->ws_.cancel ();
  this->rs_.cancel ();
  return;
}


void
Sender::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  this->handle_ = handle;

  // Open ACE_Asynch_Write_Stream
  if (this->ws_.open (*this, this->handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Sender::ACE_Asynch_Write_Stream::open")));

  // Open ACE_Asynch_Read_Stream
  else if (this->rs_.open (*this, this->handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Sender::ACE_Asynch_Read_Stream::open")));

  else if (this->initiate_write_stream () == 0)
    {
      if (duplex != 0)
        // Start an asynchronous read
        this->initiate_read_stream ();
    }

  this->check_destroy ();
}

int
Sender::initiate_write_stream (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, -1);

  if (this->flg_cancel_ != 0)
    return -1;

  static const size_t request_line_length = ACE_OS::strlen (request_line);
  static const size_t headers_length = ACE_OS::strlen (headers);
  static const size_t end_of_request_header_length = ACE_OS::strlen (end_of_request_header);

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
  ACE_Message_Block *mb1 = 0, 
                    *mb2 = 0, 
                    *mb3 = 0;

  ACE_NEW_RETURN (mb1, ACE_Message_Block (request_line_length), -1);
  ACE_NEW_RETURN (mb2, ACE_Message_Block (headers_length), -1);
  ACE_NEW_RETURN (mb3, ACE_Message_Block (end_of_request_header_length), -1);

  mb1->init (request_line, request_line_length);
  mb1->wr_ptr (request_line_length);
  mb2->init (headers, headers_length);
  mb2->wr_ptr (headers_length);
  mb3->init (end_of_request_header, end_of_request_header_length);
  mb3->wr_ptr (end_of_request_header_length);

  // chain them together
  mb1->cont (mb2);
  mb2->cont (mb3);

  if (this->ws_.writev (*mb1, mb1->total_length ()) == -1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Sender::ACE_Asynch_Stream::writev")),
                       -1);
    }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
  ACE_Message_Block *mb = 0;

  ACE_NEW_RETURN (mb, 
                  ACE_Message_Block (request_line_length
                                     + headers_length
                                     + end_of_request_header_length)
                  , -1);

  mb->copy (request_line, request_line_length);
  mb->wr_ptr (request_line_length);
  mb->copy (headers, headers_length);
  mb->wr_ptr (headers_length);
  mb->copy (end_of_request_header, end_of_request_header_length);
  mb->wr_ptr (end_of_request_header_length);

  if (this->ws_.write (*mb, mb->length ()) == -1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Sender::ACE_Asynch_Stream::write")),
                       -1);
    }
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  this->io_count_++;
  return 0;
}

int
Sender::initiate_read_stream (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_, -1);

  if (this->flg_cancel_ != 0)
    return -1;

  static const size_t request_line_length = ACE_OS::strlen (request_line);
  static const size_t headers_length = ACE_OS::strlen (headers);
  static const size_t end_of_request_header_length = ACE_OS::strlen (end_of_request_header);

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
  ACE_Message_Block *mb1 = 0, 
                    *mb2 = 0, 
                    *mb3 = 0;

  ACE_NEW_RETURN (mb1, ACE_Message_Block (request_line_length), -1);
  ACE_NEW_RETURN (mb2, ACE_Message_Block (headers_length), -1);
  ACE_NEW_RETURN (mb3, ACE_Message_Block (end_of_request_header_length), -1);

  mb1->cont (mb2);
  mb2->cont (mb3);

  // Inititiate read
  if (this->rs_.readv (*mb1, mb1->total_size () - 1) == -1)
    {
      mb1->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Sender::ACE_Asynch_Read_Stream::read")),
                        -1);
    }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
  ACE_Message_Block *mb = 0;

  ACE_NEW_RETURN (mb, 
                  ACE_Message_Block (request_line_length
                                     + headers_length
                                     + end_of_request_header_length)
                  , -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->total_size () - 1) == -1)
    {
      mb->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Sender::ACE_Asynch_Read_Stream::read")),
                        -1);
    }
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  this->io_count_++;
  return 0;
}

void
Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_Message_Block & mb = result.message_block ();

  if (loglevel == 0
      || result.bytes_transferred () == 0
      || result.error () != 0)
    {
      LogLocker log_lock;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** Sender::handle_write_stream() SessionId = %d ****\n"),
                  index_));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_to_write"),
                  result.bytes_to_write ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("handle"),
                  result.handle ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_transfered"),
                  result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("act"),
                  (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("success"),
                  result.success ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("completion_key"),
                  (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("error"),
                  result.error ()));

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
      size_t bytes_transferred = result.bytes_transferred ();
      char index = 0;
      for (ACE_Message_Block* mb_i = &mb; 
           (mb_i != 0) && (bytes_transferred > 0); 
           mb_i = mb_i->cont ())
      {
        if (mb_i->rd_ptr () - mb_i->base () >= bytes_transferred)
        {
          mb_i->rd_ptr (- bytes_transferred);
          bytes_transferred = 0;
        }
        else
        {
          size_t len = mb_i->rd_ptr () - mb_i->base ();
          mb_i->rd_ptr (- len);
          bytes_transferred -= len;
        }

        ++index;
        char message[1024];
        ACE_OS::strncpy (message, mb_i->rd_ptr (), mb_i->length ());
        message[mb_i->length ()] = 0;
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s%d = %s\n"),
                    ACE_TEXT ("message_block, part "),
                    index,
                    message));
      }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
      char message[1024];
      ACE_OS::strncpy (message, mb.rd_ptr (), mb.length ());
      message[mb.length ()] = 0;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %s\n"),
                  ACE_TEXT ("message_block"),
                  message));
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** end of message ****************\n")));
    }

  mb.release ();

  if (result.error () == 0 && result.bytes_transferred () != 0)
    {
      if (duplex != 0)  // full duplex, continue write
        this->initiate_write_stream ();
      else  // half-duplex   read reply, after read we will start write
        this->initiate_read_stream ();
    }

  {
    ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);
    this->io_count_--;
  }

  this->check_destroy ();
}

void
Sender::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_Message_Block & mb = result.message_block ();

  if (loglevel == 0
      || result.bytes_transferred () == 0
      || result.error () != 0)
    {
      LogLocker log_lock;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** Sender::handle_read_stream() SessionId = %d ****\n"),
                  index_));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_to_read"),
                  result.bytes_to_read ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("handle"),
                  result.handle ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("bytes_transfered"),
                  result.bytes_transferred ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("act"),
                  (u_long) result.act ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("success"),
                  result.success ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %u\n"),
                  ACE_TEXT ("completion_key"),
                  (u_long) result.completion_key ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %d\n"),
                  ACE_TEXT ("error"),
                  result.error ()));

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
      char index = 0;
      for (ACE_Message_Block* mb_i = &mb; 
           mb_i != 0; 
           mb_i = mb_i->cont ())
      {
        ++index;
        char message[1024];
        ACE_OS::strncpy (message, mb_i->rd_ptr (), mb_i->length ());
        message[mb_i->length ()] = 0;
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s%d = %s\n"),
                    ACE_TEXT ("message_block, part "),
                    index,
                    message));
      }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
      char message[1024];
      ACE_OS::strncpy (message, mb.rd_ptr (), mb.length ());
      message[mb.length ()] = 0;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %s\n"),
                  ACE_TEXT ("message_block"),
                  message));
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**** end of message ****************\n")));
    }

  mb.release ();

  if (result.error () == 0 && result.bytes_transferred () != 0)
    {
      if (duplex != 0)  // full duplex, continue read
        this->initiate_read_stream ();
      else  // half-duplex  write, after write we will start read
        this->initiate_write_stream ();
    }

  {
    ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);
    this->io_count_--;
  }

  this->check_destroy ();
}

// *************************************************************
//   Configuration helpers
// *************************************************************
int
print_usage (int /* argc */, ACE_TCHAR *argv[])
{
  ACE_ERROR
    ((LM_ERROR,
      ACE_TEXT ("\nusage: %s")
      ACE_TEXT ("\n-o <max number of started aio operations for Proactor>")
      ACE_TEXT ("\n-t <Proactor type> UNIX-only, Win32-default always:")
      ACE_TEXT ("\n    a AIOCB")
      ACE_TEXT ("\n    i SIG")
      ACE_TEXT ("\n    c CB")
      ACE_TEXT ("\n    s SUN")
      ACE_TEXT ("\n    d default")
      ACE_TEXT ("\n-d <duplex mode 1-on/0-off>")
      ACE_TEXT ("\n-h <host> for Sender mode")
      ACE_TEXT ("\n-n <number threads for Proactor pool>")
      ACE_TEXT ("\n-p <port to listen/connect>")
      ACE_TEXT ("\n-s <number of sender's instances>")
      ACE_TEXT ("\n-b run client and server at the same time")
      ACE_TEXT ("\n    f file")
      ACE_TEXT ("\n    c console")
      ACE_TEXT ("\n-v log level")
      ACE_TEXT ("\n    0 - log all messages")
      ACE_TEXT ("\n    1 - log only errors and unusual cases")
      ACE_TEXT ("\n-i time to run in seconds")
      ACE_TEXT ("\n-u show this message")
      ACE_TEXT ("\n"),
      argv[0]
   ));
  return -1;
}

static int
set_proactor_type (const ACE_TCHAR *ptype)
{
  if (!ptype)
    return 0;

  switch (toupper (*ptype))
    {
    case 'D':
      proactor_type = DEFAULT;
      return 1;
    case 'A':
      proactor_type = AIOCB;
      return 1;
    case 'I':
      proactor_type = SIG;
      return 1;
#if defined (sun)
    case 'S':
      proactor_type = SUN;
      return 1;
#endif /* sun */
    case 'C':
      proactor_type = CB;
      return 1;
    default:
      break;
    }
  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1) // no arguments , so one button test
    {
      both = 1;                       // client and server simultaneosly
#if defined(ACE_WIN32) || defined(sun)
      duplex = 1;                     // full duplex is on
#else   // Linux,IRIX - weak AIO implementation
      duplex = 0;                     // full duplex is off
#endif
      host = ACE_LOCALHOST;           // server to connect
      port = ACE_DEFAULT_SERVER_PORT; // port to connect/listen
      max_aio_operations = 512;       // POSIX Proactor params
#if defined (sun)
      proactor_type = SUN;            // Proactor type for SunOS
#else
      proactor_type = DEFAULT;        // Proactor type = default
#endif
      threads = 3;                    // size of Proactor thread pool
      senders = 20;                   // number of senders
      loglevel = 0;                   // log level : 0 full/ 1 only errors
      seconds = 20;                   // time to run in seconds
      return 0;
    }

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:t:o:n:p:d:h:s:v:ub"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
    switch (c)
      {
      case 'i':  // time to run
        seconds = ACE_OS::atoi (get_opt.opt_arg ());
        if (seconds < MIN_TIME)
          seconds = MIN_TIME;
        if (seconds > MAX_TIME)
          seconds = MAX_TIME;
       	break;
      case 'b':  // both client and server
        both = 1;
        break;
      case 'v':  // log level
        loglevel = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'd':         // duplex
        duplex = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'h':         // host for sender
        host = get_opt.opt_arg ();
        break;
      case 'p':         // port number
        port = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'n':         // thread pool size
        threads = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 's':     // number of senders
        senders = ACE_OS::atoi (get_opt.opt_arg ());
        if (senders > MAX_SENDERS)
          senders = MAX_SENDERS;
        break;
      case 'o':     // max number of aio for proactor
        max_aio_operations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 't':    //  Proactor Type
        if (set_proactor_type (get_opt.opt_arg ()))
          break;
        return print_usage (argc, argv);
      case 'u':
      default:
        return print_usage (argc, argv);
      } // switch
    } // while

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Test"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR); // Edan

  if (::parse_args (argc, argv) == -1)
    return -1;

  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);
  disable_signal (SIGPIPE, SIGPIPE);

  MyTask    task1;
  Acceptor  acceptor;
  Connector connector;

  if (task1.start (threads,
                   proactor_type,
                   max_aio_operations) == 0)
    {
      int rc = 0;

      if (both != 0 || host == 0) // Acceptor
        {
          // Simplify, initial read with zero size
          if (acceptor.open (ACE_INET_Addr (port), 0, 1) == 0)
            rc = 1;
        }

      if (both != 0 || host != 0)
        {
          ACE_INET_Addr addr;
          if (host == 0)
            host = ACE_LOCALHOST;

          if (addr.set (port, host) == -1)
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), host));
          else
            rc += connector.start (addr, senders);
        }

      if (rc > 0)
        ACE_OS::sleep (seconds);
    }

  task1.stop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nNumber of Receivers objects = %d\n")
              ACE_TEXT ("\nNumber of Sender objects = %d\n"),
              acceptor.get_number_sessions (),
              connector.get_number_sessions ()));

  // As Proactor event loop now is inactive it is safe to destroy all
  // senders

  connector.stop ();
  acceptor.stop ();

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Asynch_Acceptor<Receiver>;
template class ACE_Asynch_Connector<Sender>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Asynch_Acceptor<Receiver>
#pragma instantiate ACE_Asynch_Connector<Sender>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Threads or Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Test will not be run.")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS */
