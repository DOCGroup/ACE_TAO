// $Id$

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
static int threads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Log options
static int loglevel;       // 0 full , 1 only errors

static size_t xfer_limit;  // Number of bytes for Sender to send.

static ACE_TCHAR complete_message[] =
  ACE_TEXT ("GET / HTTP/1.1\r\n")
  ACE_TEXT ("Accept: */*\r\n")
  ACE_TEXT ("Accept-Language: C++\r\n")
  ACE_TEXT ("Accept-Encoding: gzip, deflate\r\n")
  ACE_TEXT ("User-Agent: Proactor_Test/1.0 (non-compatible)\r\n")
  ACE_TEXT ("Connection: Keep-Alive\r\n")
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

  virtual ~MyTask()
    {
      (void) this->stop ();
      (void) this->delete_proactor();
    }

  virtual int svc (void);

  int start (int num_threads,
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

  ACE_WIN32_Proactor *proactor_impl = 0;

  ACE_NEW_RETURN (proactor_impl,
                  ACE_WIN32_Proactor,
                  -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%t) Create Proactor Type = WIN32\n")));

#elif defined (ACE_HAS_AIO_CALLS)

  ACE_POSIX_Proactor * proactor_impl = 0;

  switch (type_proactor)
    {
    case AIOCB:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_POSIX_AIOCB_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = AIOCB\n")));
      break;

#if defined(ACE_HAS_POSIX_REALTIME_SIGNALS)
    case SIG:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_POSIX_SIG_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = SIG\n")));
      break;
#endif /* ACE_HAS_POSIX_REALTIME_SIGNALS */

#  if defined (sun)
    case SUN:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_SUN_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%t) Create Proactor Type = SUN\n")));
      break;
#  endif /* sun */

#  if defined (__sgi)
    case CB:
      ACE_NEW_RETURN (proactor_impl,
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

  // always delete implementation  1 , not  !(proactor_impl == 0)
  ACE_NEW_RETURN (this->proactor_,
                  ACE_Proactor (proactor_impl, 1 ),
                  -1);
  // Set new singleton and delete it in close_singleton()
  ACE_Proactor::instance (this->proactor_, 1);
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

  ACE_Proactor::close_singleton ();
  this->proactor_ = 0;

  return 0;
}

int
MyTask::start (int num_threads,
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
                  ACE_TEXT ("(%t) Calling End Proactor event loop\n")));

      ACE_Proactor::end_event_loop ();
    }

  if (this->wait () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to stop thread pool")));

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

  size_t get_total_snd (void) { return this->total_snd_; }
  size_t get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

  // This is called to pass the new connection's addresses.
  virtual void addresses (const ACE_INET_Addr& peer,
                          const ACE_INET_Addr& local);

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
  int initiate_write_stream (ACE_Message_Block &mb, size_t nbytes);
  void cancel ();

  Acceptor *acceptor_;
  int index_;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;
  ACE_SYNCH_MUTEX lock_;

  long io_count_;            // Number of currently outstanding I/O requests
  int flg_cancel_;
  size_t total_snd_;         // Number of bytes successfully sent
  size_t total_rcv_;         // Number of bytes successfully received
  long total_w_;             // Number of write operations
  long total_r_;             // Number of read operations
};

class Acceptor : public ACE_Asynch_Acceptor<Receiver>
{
  friend class Receiver;
public:
  int get_number_sessions (void) { return this->sessions_; }
  size_t get_total_snd (void) { return this->total_snd_; }
  size_t get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

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
  size_t total_snd_;
  size_t total_rcv_;
  long total_w_;
  long total_r_;
};

// *************************************************************
Acceptor::Acceptor (void)
  : sessions_ (0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
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
              ACE_TEXT ("(%t) Acceptor: receiver %d up; now have %d.\n"),
              rcvr.index_,
              this->sessions_));
}

void
Acceptor::on_delete_receiver (Receiver & rcvr)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->sessions_--;

  this->total_snd_ += rcvr.get_total_snd();
  this->total_rcv_ += rcvr.get_total_rcv();
  this->total_w_   += rcvr.get_total_w();
  this->total_r_   += rcvr.get_total_r();

  if (rcvr.index_ >= 0
      && rcvr.index_ < MAX_RECEIVERS
      && this->list_receivers_[rcvr.index_] == &rcvr)
    this->list_receivers_[rcvr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Acceptor: receiver %d gone; %d remain\n"),
              rcvr.index_,
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
    flg_cancel_(0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{
  if (this->acceptor_ != 0)
    this->acceptor_->on_new_receiver (*this);
}

Receiver::~Receiver (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Receiver %d dtor; %d sends (%d bytes); ")
              ACE_TEXT ("%d recvs (%d bytes)\n"),
              this->index_,
              this->total_w_, this->total_snd_,
              this->total_r_, this->total_rcv_));
  if (this->io_count_ != 0)
    ACE_ERROR ((LM_WARNING,
                ACE_TEXT ("(%t) Receiver %d deleted with ")
                ACE_TEXT ("%d I/O outstanding\n"),
                this->index_,
                this->io_count_));

  // This test bounces data back and forth between Senders and Receivers.
  // Therefore, if there was significantly more data in one direction, that's
  // a problem. Remember, the byte counts are unsigned values.
  int issue_data_warning = 0;
  if (this->total_snd_ > this->total_rcv_)
    {
      if (this->total_rcv_ == 0)
        issue_data_warning = 1;
      else if (this->total_snd_ / this->total_rcv_ > 2)
        issue_data_warning = 1;
    }
  else
    {
      if (this->total_snd_ == 0)
        issue_data_warning = 1;
      else if (this->total_rcv_ / this->total_snd_ > 2)
        issue_data_warning = 1;
    }
  if (issue_data_warning)
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("(%t) Above byte counts look odd; need review\n")));

  if (this->acceptor_ != 0)
    this->acceptor_->on_delete_receiver (*this);

  if (this->handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->handle_);

  this->index_ = -1;
  this->handle_= ACE_INVALID_HANDLE;
}

void
Receiver::cancel ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

  this->flg_cancel_ = 1;
  this->ws_.cancel ();
  this->rs_.cancel ();
  return;
}


void
Receiver::addresses (const ACE_INET_Addr& peer, const ACE_INET_Addr&)
{
  ACE_TCHAR str[256];
  if (0 == peer.addr_to_string (str, sizeof (str) / sizeof (ACE_TCHAR)))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Receiver %d connection from %s\n"),
                this->index_,
                str));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) Receiver %d %p\n"),
                this->index_,
                ACE_TEXT ("addr_to_string")));
  return;
}


void
Receiver::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    // Don't buffer serial sends.
    this->handle_ = handle;
    int nodelay = 1;
    ACE_SOCK_Stream option_setter (handle);
    if (-1 == option_setter.set_option (IPPROTO_TCP,
                                        TCP_NODELAY,
                                        &nodelay,
                                        sizeof (nodelay)))
      ACE_ERROR ((LM_ERROR, "%p\n", "set_option"));

    if (this->ws_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Receiver::ACE_Asynch_Write_Stream::open")));
    else if (this->rs_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Receiver::ACE_Asynch_Read_Stream::open")));
    else
      this->initiate_read_stream ();

    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

int
Receiver::initiate_read_stream (void)
{
  if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (1024), //BUFSIZ + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size () - 1) == -1)
    {
      mb->release ();
#if defined (ACE_WIN32)
      // On peer close, ReadFile will yield ERROR_NETNAME_DELETED; won't get
      // a 0-byte read as we would if underlying calls used WSARecv.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Receiver %d, peer closed\n"),
                           this->index_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) Receiver %d, %p\n"),
                         this->index_,
                         ACE_TEXT ("read")),
                        -1);
    }

  this->io_count_++;
  this->total_r_++;
  return 0;
}

int
Receiver::initiate_write_stream (ACE_Message_Block &mb, size_t nbytes)
{
  if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    {
      mb.release ();
      return -1;
    }

  if (nbytes == 0)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) Receiver::ACE_Asynch_Write_Stream::write nbytes <0 ")),
                       -1);
    }

  if (this->ws_.write (mb, nbytes) == -1)
    {
      mb.release ();
#if defined (ACE_WIN32)
      // On peer close, WriteFile will yield ERROR_NETNAME_DELETED.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Receiver %d, peer gone\n"),
                           this->index_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) Receiver %d, %p\n"),
                        this->index_,
                        ACE_TEXT ("write")),
                       -1);
    }

  this->io_count_++;
  this->total_w_++;
  return 0;
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_ );

    ACE_Message_Block & mb = result.message_block ();

    // Reset pointers.
    mb.rd_ptr ()[result.bytes_transferred ()] = '\0';

    if (loglevel > 1)
      {
        LogLocker log_lock;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Receiver %d: handle_read_stream() ****\n"),
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
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
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
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Receiver %d; %p\n"),
                                       this->index_,
                                       ACE_TEXT ("read"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Receiver %d: read %d bytes\n"),
                    this->index_,
                    result.bytes_transferred ()));
      }

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_rcv_ += result.bytes_transferred ();

        if (this->initiate_write_stream (mb,
                                         result.bytes_transferred ()) == 0)
          {
            if (duplex != 0)  // Initiate new read from the stream.
              this->initiate_read_stream ();
          }
      }
    else
      mb.release ();

    this->io_count_--;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

void
Receiver::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    ACE_Message_Block & mb = result.message_block ();

    if (loglevel > 1)
      {
        LogLocker log_lock;

        //mb.rd_ptr () [0] = '\0';
        mb.rd_ptr (mb.rd_ptr () - result.bytes_transferred ());

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Receiver %d: handle_write_stream() ****\n"),
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
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
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
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Receiver %d; %p\n"),
                                       this->index_,
                                       ACE_TEXT ("write"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Receiver %d: wrote %d bytes ok\n"),
                    this->index_,
                    result.bytes_transferred ()));
      }

    mb.release ();

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_snd_ += result.bytes_transferred ();

        if (duplex == 0)
          this->initiate_read_stream ();
      }

    this->io_count_--;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
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

  size_t get_total_snd (void) { return this->total_snd_; }
  size_t get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

  // This is called to pass the new connection's addresses.
  virtual void addresses (const ACE_INET_Addr& peer,
                          const ACE_INET_Addr& local);

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous reads from the socket complete

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete

private:
  int initiate_read_stream (void);
  int initiate_write_stream (void);
  void cancel (void);
  void close (void);

  int  index_;
  Connector * connector_;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;

  ACE_SYNCH_MUTEX lock_;

  long io_count_;
  int stop_writing_;           // Writes are shut down; just read.
  int flg_cancel_;
  size_t total_snd_;
  size_t total_rcv_;
  long total_w_;
  long total_r_;
};

class Connector : public ACE_Asynch_Connector<Sender>
{
  friend class Sender;
public:
  int get_number_sessions (void) { return this->sessions_; }
  size_t get_total_snd (void) { return this->total_snd_; }
  size_t get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

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
  size_t total_snd_;
  size_t total_rcv_;
  long total_w_;
  long total_r_;
};

// *************************************************************

Connector::Connector (void)
  : sessions_ (0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
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
              ACE_TEXT ("(%t) Connector: sender %d up; now have %d.\n"),
              sndr.index_,
              this->sessions_));
}

void
Connector::on_delete_sender (Sender &sndr)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, monitor, this->lock_);

  this->sessions_--;
  this->total_snd_ += sndr.get_total_snd();
  this->total_rcv_ += sndr.get_total_rcv();
  this->total_w_   += sndr.get_total_w();
  this->total_r_   += sndr.get_total_r();

  if (sndr.index_ >= 0
      && sndr.index_ < MAX_SENDERS
      && this->list_senders_[sndr.index_] == &sndr)
    this->list_senders_[sndr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Connector: sender %d gone; %d remain\n"),
              sndr.index_,
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
                 ACE_LIB_TEXT ("(%t) %p\n"),
                 ACE_LIB_TEXT ("Connector::open failed")));
     return rc;
  }

  for (; rc < num;  rc++)
    {
      if (this->connect (addr) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p\n"),
                      ACE_TEXT ("Connector::connect failed")));
          break;
        }
    }
  return rc;
}


Sender::Sender (Connector * connector, int index)
  : index_        (index),
    connector_    (connector),
    handle_       (ACE_INVALID_HANDLE),
    io_count_     (0),
    stop_writing_ (0),
    flg_cancel_   (0),
    total_snd_    (0),
    total_rcv_    (0),
    total_w_      (0),
    total_r_      (0)
{
  if (this->connector_ != 0)
    this->connector_->on_new_sender (*this);
}

Sender::~Sender (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Sender %d dtor; %d sends (%d bytes); ")
              ACE_TEXT ("%d recvs (%d bytes)\n"),
              this->index_,
              this->total_w_, this->total_snd_,
              this->total_r_, this->total_rcv_));
  if (this->io_count_ != 0)
    ACE_ERROR ((LM_WARNING,
                ACE_TEXT ("(%t) Sender %d deleted with %d I/O outstanding\n"),
                this->index_,
                this->io_count_));

  // This test bounces data back and forth between Senders and Receivers.
  // Therefore, if there was significantly more data in one direction, that's
  // a problem. Remember, the byte counts are unsigned values.
  int issue_data_warning = 0;
  if (this->total_snd_ > this->total_rcv_)
    {
      if (this->total_rcv_ == 0)
        issue_data_warning = 1;
      else if (this->total_snd_ / this->total_rcv_ > 2)
        issue_data_warning = 1;
    }
  else
    {
      if (this->total_snd_ == 0)
        issue_data_warning = 1;
      else if (this->total_rcv_ / this->total_snd_ > 2)
        issue_data_warning = 1;
    }
  if (issue_data_warning)
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("(%t) Above byte counts look odd; need review\n")));

  if (this->connector_ != 0)
    this->connector_->on_delete_sender (*this);

  if (this->handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::closesocket (this->handle_);
    }

  this->index_ = -1;
  this->handle_= ACE_INVALID_HANDLE;
}

void
Sender::cancel ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

  this->flg_cancel_ = 1;
  this->ws_.cancel ();
  this->rs_.cancel ();
  return;
}

void
Sender::close ()
{
  // This must be called with the lock_ held.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Closing Sender %d writes; %d I/O outstanding\n"),
              this->index_, this->io_count_));
  ACE_OS::shutdown (this->handle_, ACE_SHUTDOWN_WRITE);
  this->stop_writing_ = 1;
  return;
}


void
Sender::addresses (const ACE_INET_Addr& /* peer */, const ACE_INET_Addr& local)
{
  ACE_TCHAR str[256];
  if (0 == local.addr_to_string (str, sizeof (str) / sizeof (ACE_TCHAR)))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Sender %d connected on %s\n"),
                this->index_,
                str));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) Receiver %d %p\n"),
                this->index_,
                ACE_TEXT ("addr_to_string")));
  return;
}


void
Sender::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    // Don't buffer serial sends.
    this->handle_ = handle;
    int nodelay = 1;
    ACE_SOCK_Stream option_setter (handle);
    if (option_setter.set_option (IPPROTO_TCP,
                                  TCP_NODELAY,
                                  &nodelay,
                                  sizeof (nodelay)))
      ACE_ERROR ((LM_ERROR, "%p\n", "set_option"));

    // Open ACE_Asynch_Write_Stream
    if (this->ws_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Sender::ACE_Asynch_Write_Stream::open")));

    // Open ACE_Asynch_Read_Stream
    else if (this->rs_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Sender::ACE_Asynch_Read_Stream::open")));

    else if (this->initiate_write_stream () == 0)
      {
        if (duplex != 0)    // Start an asynchronous read
          this->initiate_read_stream ();
      }

    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

int
Sender::initiate_write_stream (void)
{
  if (this->flg_cancel_ != 0 ||
      this->stop_writing_ ||
      this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  static const size_t complete_message_length = ACE_OS::strlen (complete_message);

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))

  ACE_Message_Block *mb1 = 0,
                    *mb2 = 0,
                    *mb3 = 0;

  // No need to allocate +1 for proper printing - the memory includes it already
  ACE_NEW_RETURN (mb1,
                  ACE_Message_Block ((char *)complete_message,
                                     complete_message_length),
                  -1);

  ACE_NEW_RETURN (mb2,
                  ACE_Message_Block ((char *)complete_message,
                                     complete_message_length),
                  -1);

  ACE_NEW_RETURN (mb3,
                  ACE_Message_Block ((char *)complete_message,
                                     complete_message_length),
                  -1);

  mb1->wr_ptr (complete_message_length);
  mb2->wr_ptr (complete_message_length);
  mb3->wr_ptr (complete_message_length);

  // chain them together
  mb1->cont (mb2);
  mb2->cont (mb3);

  if (this->ws_.writev (*mb1, mb1->total_length ()) == -1)
    {
      mb1->release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) %p\n"),
                        ACE_TEXT ("Sender::ACE_Asynch_Stream::writev")),
                       -1);
    }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  ACE_Message_Block *mb = 0;

  // No need to allocate +1 for proper printing - the memory includes it already
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (complete_message, complete_message_length),
                  -1);
  mb->wr_ptr (complete_message_length);

  if (this->ws_.write (*mb, mb->length ()) == -1)
    {
      mb->release ();
#if defined (ACE_WIN32)
      // On peer close, WriteFile will yield ERROR_NETNAME_DELETED.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Sender %d, peer gone\n"),
                           this->index_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) Sender %d, %p\n"),
                        this->index_,
                        ACE_TEXT ("write")),
                       -1);
    }
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  this->io_count_++;
  this->total_w_++;
  return 0;
}

int
Sender::initiate_read_stream (void)
{
  if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  static const size_t complete_message_length = ACE_OS::strlen (complete_message);

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
  ACE_Message_Block *mb1 = 0,
                    *mb2 = 0,
                    *mb3 = 0,
                    *mb4 = 0,
                    *mb5 = 0,
                    *mb6 = 0;

  // We allocate +1 only for proper printing - we can just set the last byte
  // to '\0' before printing out
  ACE_NEW_RETURN (mb1, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb2, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb3, ACE_Message_Block (complete_message_length + 1), -1);

  // Let allocate memory for one more triplet,
  // This improves performance
  // as we can receive more the than one block at once
  // Generally, we can receive more triplets ....
  ACE_NEW_RETURN (mb4, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb5, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb6, ACE_Message_Block (complete_message_length + 1), -1);

  mb1->cont (mb2);
  mb2->cont (mb3);

  mb3->cont (mb4);
  mb4->cont (mb5);
  mb5->cont (mb6);


  // hide last byte in each message block, reserving it for later to set '\0'
  // for proper printouts
  mb1->size (mb1->size () - 1);
  mb2->size (mb2->size () - 1);
  mb3->size (mb3->size () - 1);

  mb4->size (mb4->size () - 1);
  mb5->size (mb5->size () - 1);
  mb6->size (mb6->size () - 1);

  // Inititiate read
  if (this->rs_.readv (*mb1, mb1->total_size () - 1) == -1)
    {
      mb1->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Sender::ACE_Asynch_Read_Stream::readv")),
                        -1);
    }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  // Try to read more chunks
  size_t blksize = ( complete_message_length > BUFSIZ ) ?
                     complete_message_length : BUFSIZ;

  ACE_Message_Block *mb = 0;

  // We allocate +1 only for proper printing - we can just set the last byte
  // to '\0' before printing out
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (blksize + 1)
                  , -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size () - 1) == -1)
    {
      mb->release ();
#if defined (ACE_WIN32)
      // On peer close, ReadFile will yield ERROR_NETNAME_DELETED; won't get
      // a 0-byte read as we would if underlying calls used WSARecv.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Receiver %d, peer closed\n"),
                           this->index_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) Sender %d, %p\n"),
                         this->index_,
                         ACE_TEXT ("read")),
                        -1);
    }
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  this->io_count_++;
  this->total_r_++;
  return 0;
}

void
Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    ACE_Message_Block & mb = result.message_block ();

    if (loglevel > 1)
      {
        LogLocker log_lock;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Sender %d: handle_write_stream() ****\n"),
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
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
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
            // write 0 at string end for proper printout (if end of mb, it's 0 already)
            mb_i->rd_ptr()[0]  = '\0';

            size_t len = mb_i->rd_ptr () - mb_i->base ();

            // move rd_ptr backwards as required for printout
            if (len >= bytes_transferred)
              {
                mb_i->rd_ptr (0 - bytes_transferred);
                bytes_transferred = 0;
              }
            else
              {
                mb_i->rd_ptr (0 - len);
                bytes_transferred -= len;
              }

            ++index;
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%s%d = %s\n"),
                        ACE_TEXT ("message_block, part "),
                        index,
                        mb_i->rd_ptr ()));
          }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
        // write 0 at string end for proper printout (if end of mb, it's 0 already)
        mb.rd_ptr()[0]  = '\0';
        // move rd_ptr backwards as required for printout
        mb.rd_ptr (- result.bytes_transferred ());
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),
                    mb.rd_ptr ()));
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("**** end of message ****************\n")));
      }
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Sender %d; %p\n"),
                                       this->index_,
                                       ACE_TEXT ("write"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Sender %d: wrote %d bytes ok\n"),
                    this->index_,
                    result.bytes_transferred ()));
      }

    mb.release ();

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_snd_ += result.bytes_transferred ();
	if (this->total_snd_ >= xfer_limit)
	  {
	    ACE_DEBUG ((LM_DEBUG,
			ACE_TEXT ("(%t) Sender %d sent %d, limit %d\n"),
			this->index_, this->total_snd_, xfer_limit));
	    this->close ();
	  }
        if (duplex != 0)   // full duplex, continue write
          {
            if ((this->total_snd_- this->total_rcv_) < 1024*32 ) //flow control
              this->initiate_write_stream ();
          }
        else  // half-duplex   read reply, after read we will start write
          this->initiate_read_stream ();
      }

    this->io_count_--;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

void
Sender::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    ACE_Message_Block & mb = result.message_block ();

    if (loglevel > 1)
      {
        LogLocker log_lock;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Sender %d: handle_read_stream() ****\n"),
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
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
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
            // write 0 at string end for proper printout
            mb_i->wr_ptr()[0]  = '\0';

            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%s%d = %s\n"),
                        ACE_TEXT ("message_block, part "),
                        index,
                        mb_i->rd_ptr ()));
          }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
        // write 0 at string end for proper printout
        mb.rd_ptr()[result.bytes_transferred ()]  = '\0'; // for proper printout
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),
                    mb.rd_ptr ()));
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("**** end of message ****************\n")));
      }
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Sender %d; %p\n"),
                                       this->index_,
                                       ACE_TEXT ("read"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Sender %d: read %d bytes ok\n"),
                    this->index_,
                    result.bytes_transferred ()));
      }

    mb.release ();

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_rcv_ += result.bytes_transferred ();

        if (duplex != 0 || this->stop_writing_)  // full duplex, continue read
          this->initiate_read_stream ();
        else  // half-duplex  write, after write we will start read
          this->initiate_write_stream ();
      }

    this->io_count_--;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
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
      ACE_TEXT ("\n    0 - log errors and highlights")
      ACE_TEXT ("\n    1 - log level 0 plus progress information")
      ACE_TEXT ("\n    2 - log level 1 plus operation parameters and results")
      ACE_TEXT ("\n-x max transfer byte count per Sender")
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
#if defined (__sgi)
     case 'C':
       proactor_type = CB;
       return 1;
#endif /* __sgi */
    default:
      break;
    }
  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  // First, set up all the defaults then let any args change them.
  both = 1;                       // client and server simultaneosly
  duplex = 1;                     // full duplex is on
  host = ACE_LOCALHOST;           // server to connect
  port = ACE_DEFAULT_SERVER_PORT; // port to connect/listen
  max_aio_operations = 512;       // POSIX Proactor params
  proactor_type = DEFAULT;        // Proactor type = default
  threads = 3;                    // size of Proactor thread pool
  senders = 10;                   // number of senders
  loglevel = 0;                   // log level : only errors and highlights
  // Default transfer limit 50 messages per Sender
  xfer_limit = 50 * ACE_OS::strlen (complete_message);

  if (argc == 1) // no arguments , so one button test
    return 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("x:t:o:n:p:d:h:s:v:ub"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
    switch (c)
      {
      case 'x':  // xfer limit
        xfer_limit = ACE_static_cast (size_t,
				      ACE_OS::atoi (get_opt.opt_arg ()));
        if (xfer_limit == 0)
	  xfer_limit = 1;          // Bare minimum.
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

  if (proactor_type == SUN && threads > 1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sun aiowait is not thread-safe; ")
                  ACE_TEXT ("changing to 1 thread\n")));
      threads = 1;
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Test"));

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
    }

  // Wait a couple of seconds to let things get going, then poll til
  // all sessions are done.
  ACE_OS::sleep (2);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Sleeping til sessions run down.\n")));
  while (acceptor.get_number_sessions () > 0 ||
	 connector.get_number_sessions () > 0   )
    ACE_OS::sleep (1);

#if 0
  // Cancel all pending AIO on Connector and Senders
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Cancel Connector/Senders: sessions_=%d\n"),
              connector.get_number_sessions ()
            ));
  connector.cancel_all ();
#endif

  //Cancel all pending AIO on Acceptor And Receivers
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Cancel Acceptor/Receivers:sessions_=%d\n"),
              acceptor.get_number_sessions ()
            ));
  acceptor.cancel_all ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Stop Thread Pool Task\n")
            ));
  task1.stop ();

  // As Proactor event loop now is inactive it is safe to destroy all
  // Senders
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Stop Connector/Senders: sessions_=%d\n"),
              connector.get_number_sessions ()
            ));
  connector.stop ();

  // As Proactor event loop now is inactive it is safe to destroy all
  // Receivers
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Stop Acceptor/Receivers:sessions_=%d\n"),
              acceptor.get_number_sessions ()
            ));
  acceptor.stop ();

  //Print statistic
  ACE_TCHAR bufs [256];
  ACE_TCHAR bufr [256];

  ACE_OS::sprintf (bufs,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(%ld)"),
                   connector.get_total_snd (),
                   connector.get_total_w ());

  ACE_OS::sprintf (bufr,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(%ld)"),
                   connector.get_total_rcv (),
                   connector.get_total_r ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Connector/Senders  total bytes: snd=%s rcv=%s\n"),
              bufs,
              bufr));

  ACE_OS::sprintf (bufs,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(%ld)"),
                   acceptor.get_total_snd (),
                   acceptor.get_total_w ());

  ACE_OS::sprintf (bufr,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(%ld)"),
                   acceptor.get_total_rcv (),
                   acceptor.get_total_r ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Acceptor/Receivers total bytes: snd=%s rcv=%s\n"),
              bufs,
              bufr));

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
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Threads or Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Test will not be run.")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS */
