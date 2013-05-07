
//=============================================================================
/**
 *  @file    TP_Reactor_Test.cpp
 *
 *  $Id$
 *
 *  This program illustrates how the <ACE_TP_Reactor> can be used to
 *  implement an application that does various  operations.
 *  usage: TP_Reactor_Test
 *         -n number threads in the TP_Reactor thread pool
 *         -d duplex mode 1 (full-duplex) vs. 0 (half-duplex)
 *         -p port to listen(Server)/connect(Client)
 *         -h host to connect (Client mode)
 *         -s number of sender's instances ( Client mode)
 *         -b run client and server (both modes ) at the same time
 *         -v log level
 *            0 - log all messages
 *            1 - log only errors and unusual cases
 *         -i time to run in seconds
 *         -u show this message
 *
 *   The main differences between Thread_Pool_Reactor_Test.cpp and
 *   this test are:
 *
 *   1. Thread_Pool_Reactor_Test.cpp tests only handle_input()
 *   events on the server, whereas this one tests both handle_input() and
 *   handle_output() on both server and client, i.e., the receiver
 *   and sender are completely event-driven.
 *
 *   2. The receiver and sender in this test can work in full duplex
 *   mode, i.e., input and ouput events are processed independently.
 *   Half-duplex mode (request-reply) is also supported.
 *
 *   This test is therefore a bit more stressful than the
 *   Thread_Pool_Reactor.cpp for the ACE_TP_Reactor since same
 *   thread pool is shared between client and server.
 *
 *   This test is a "twin" of the Proactor_Test.cpp, so it can help for
 *   developers to provide independent of Reactor/Proactor solutions.
 *
 *
 *  @author   Alexander Libman <alibman@ihug.com.au>
 *  @author <alexl@rumblgroup.com>
 */
//=============================================================================


#include "test_config.h"

#if defined(ACE_HAS_THREADS)

#include "TP_Reactor_Test.h"

#include "ace/Signal.h"
#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"

#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Semaphore.h"

//  Some debug helper functions
static int disable_signal (int sigmin, int sigmax);

// both: 0 run client or server / depends on host
//     != 0 run client and server
static int both = 0;

// Host that we're connecting to.
static const ACE_TCHAR *host = 0;

// number of Senders instances
static int senders = 1;

// duplex mode: == 0 half-duplex
//              != 0 full duplex
static int duplex = 0;

// number threads in the TP_Reactor thread pool
static int threads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Log options
static int loglevel = 1; // 0 full , 1 only errors

static const size_t MIN_TIME = 1;    // min 1 sec
static const size_t MAX_TIME = 3600; // max 1 hour
static u_int seconds = 2;  // default time to run - 2 seconds

static char data[] =
  "GET / HTTP/1.1\r\n"
  "Accept: */*\r\n"
  "Accept-Language: C++\r\n"
  "Accept-Encoding: gzip, deflate\r\n"
  "User-Agent: TPReactor_Test/1.0 (non-compatible)\r\n"
  "Connection: Keep-Alive\r\n"
  "\r\n" ;

// *************************************************************

class LogLocker
{
public:

  LogLocker () { ACE_LOG_MSG->acquire (); }
  virtual ~LogLocker () { ACE_LOG_MSG->release (); }
};
// *************************************************************

/**
 * @class MyTask
 *
 * MyTask plays role for TP_Reactor threads pool
 *
 * MyTask is ACE_Task resposible for:
 * 1. Creation and deletion of TP_Reactor and TP_Reactor thread pool
 * 2. Running TP_Reactor event loop
 */
class MyTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  MyTask (void): sem_ ((unsigned int) 0),
                 my_reactor_ (0) {}

  virtual ~MyTask () { stop (); }

  virtual int svc (void);

  int start (int num_threads);
  int stop (void);

private:
  int  create_reactor (void);
  int  delete_reactor (void);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  ACE_Thread_Semaphore sem_;
  ACE_Reactor *my_reactor_;
};

int
MyTask::create_reactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_TEST_ASSERT (this->my_reactor_ == 0);

  ACE_TP_Reactor * pImpl = 0;

  ACE_NEW_RETURN (pImpl,ACE_TP_Reactor, -1);

  ACE_NEW_RETURN (my_reactor_,
                   ACE_Reactor (pImpl ,1),
                   -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) Create TP_Reactor\n")));

  ACE_Reactor::instance (this->my_reactor_);

  this->reactor (my_reactor_);

  return 0;
}

int
MyTask::delete_reactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) Delete TP_Reactor\n")));

  delete this->my_reactor_;
  ACE_Reactor::instance ((ACE_Reactor *) 0);
  this->my_reactor_ = 0;
  this->reactor (0);

  return 0;
}

int
MyTask::start (int num_threads)
{
  if (this->create_reactor () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to create reactor")),
                      -1);

  if (this->activate (THR_NEW_LWP, num_threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to activate thread pool")),
                      -1);

  for (; num_threads > 0 ; num_threads--)
    sem_.acquire ();

  return 0;
}


int
MyTask::stop (void)
{
  if (this->my_reactor_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("End TP_Reactor event loop\n")));

      ACE_Reactor::instance()->end_reactor_event_loop ();
    }

  if (this->wait () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to stop thread pool")));

  if (this->delete_reactor () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to delete reactor")));

  return 0;
}

int
MyTask::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) MyTask started\n")));

  disable_signal (SIGPIPE, SIGPIPE);

  // signal that we are ready
  sem_.release (1);

  while (ACE_Reactor::instance()->reactor_event_loop_done () == 0)
    ACE_Reactor::instance()->run_reactor_event_loop ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) MyTask finished\n")));
  return 0;
}

// *************************************************************

Acceptor::Acceptor (void)
  : ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR> ((ACE_Reactor *) 0),
    sessions_ (0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);

  for (size_t i = 0; i < MAX_RECEIVERS; ++i)
     this->list_receivers_[i] =0;
}

Acceptor::~Acceptor (void)
{
  this->reactor (0);
  stop ();
}

void
Acceptor::stop (void)
{
  // this method can be called only after reactor event loop id done
  // in all threads

  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);

  for (size_t i = 0; i < MAX_RECEIVERS; ++i)
    {
      delete this->list_receivers_[i];
      this->list_receivers_[i] =0;
    }
}

void
Acceptor::on_new_receiver (Receiver &rcvr)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);
  this->sessions_++;
  this->list_receivers_[rcvr.index_] = & rcvr;
  ACE_DEBUG ((LM_DEBUG,
              "Receiver::CTOR sessions_=%d\n",
              this->sessions_));
}

void
Acceptor::on_delete_receiver (Receiver &rcvr)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);

  this->sessions_--;

  this->total_snd_ += rcvr.get_total_snd ();
  this->total_rcv_ += rcvr.get_total_rcv ();
  this->total_w_   += rcvr.get_total_w ();
  this->total_r_   += rcvr.get_total_r ();

  if (rcvr.index_ < MAX_RECEIVERS
      && this->list_receivers_[rcvr.index_] == &rcvr)
    this->list_receivers_[rcvr.index_] = 0;

  ACE_TCHAR bufs [256];
  ACE_TCHAR bufr [256];

  ACE_OS::sprintf ( bufs , ACE_TEXT ("%ld(%ld)"),
                    rcvr.get_total_snd (),
                    rcvr.get_total_w () );

  ACE_OS::sprintf ( bufr , ACE_TEXT ("%ld(%ld)"),
                    rcvr.get_total_rcv (),
                    rcvr.get_total_r ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::~DTOR index=%d snd=%s rcv=%s sessions_=%d\n"),
              rcvr.index_,
              bufs,
              bufr,
              this->sessions_));
}

int
Acceptor::start (const ACE_INET_Addr &addr)
{
  if (ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>::open (addr,
                                                      ACE_Reactor::instance (),
                                                      ACE_NONBLOCK) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("Acceptor::start () - open failed")),
                       0);
  return 1;
}

int
Acceptor::make_svc_handler (Receiver *&sh)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  if (sessions_ >= MAX_RECEIVERS)
    return -1;

  for (size_t i = 0; i < MAX_RECEIVERS; ++i)
    if (this->list_receivers_ [i] == 0)
      {
        ACE_NEW_RETURN (sh,
                        Receiver (this , i),
                        -1);
        return 0;
      }
  return -1;
}

// *************************************************************

Receiver::Receiver (Acceptor * acceptor, size_t index)
  : acceptor_ (acceptor),
    index_ (index),
    flg_mask_ (ACE_Event_Handler::NULL_MASK),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{
  if (acceptor_ != 0)
    acceptor_->on_new_receiver (*this);
}


Receiver::~Receiver (void)
{
  this->reactor (0);
  if (acceptor_ != 0)
    acceptor_->on_delete_receiver (*this);

  this->index_ = 0;

  for (; ;)
    {
      ACE_Time_Value tv = ACE_Time_Value::zero;
      ACE_Message_Block *mb = 0;

      if (this->getq (mb, &tv) < 0)
        break;

      ACE_Message_Block::release (mb);
    }
}

int
Receiver::check_destroy (void)
{
  if (flg_mask_ == ACE_Event_Handler::NULL_MASK)
    return -1;

  return 0;
}

int
Receiver::open (void *)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  ACE_Reactor *TPReactor = ACE_Reactor::instance ();

  this->reactor (TPReactor);

  flg_mask_ = ACE_Event_Handler::NULL_MASK ;

  if (TPReactor->register_handler (this, flg_mask_) == -1)
    return -1;

  initiate_io (ACE_Event_Handler::READ_MASK);

  return check_destroy ();
}

int
Receiver::initiate_io (ACE_Reactor_Mask mask)
{
  if (ACE_BIT_ENABLED (flg_mask_, mask))
    return 0;

  if (ACE_Reactor::instance ()->schedule_wakeup  (this, mask) == -1)
    return -1;

  ACE_SET_BITS (flg_mask_, mask);
  return 0;
}

int
Receiver::terminate_io (ACE_Reactor_Mask mask)
{
  if (ACE_BIT_DISABLED (flg_mask_, mask))
    return 0;

  if (ACE_Reactor::instance ()->cancel_wakeup (this, mask) == -1)
    return -1;

  ACE_CLR_BITS (flg_mask_, mask);
  return 0;
}

int
Receiver::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_Reactor *TPReactor = ACE_Reactor::instance ();

  TPReactor->remove_handler (this,
                             ACE_Event_Handler::ALL_EVENTS_MASK |
                             ACE_Event_Handler::DONT_CALL);  // Don't call handle_close
  this->reactor (0);
  this->destroy ();
  return 0;
}

int
Receiver::handle_input (ACE_HANDLE h)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ),
                  -1);

  int err = 0;
  ssize_t res = this->peer ().recv (mb->rd_ptr (), BUFSIZ-1);

  this->total_r_++;

  if (res >= 0)
    {
      mb->wr_ptr (res);
      this->total_rcv_ += res;
    }
  else
    err = errno ;

  mb->wr_ptr ()[0] = '\0';

  if (loglevel == 0 || res <= 0 || err!= 0)
    {
      LogLocker log_lock;

      ACE_DEBUG ((LM_DEBUG, "**** Receiver::handle_input () SessionId=%d****\n", index_));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_to_read", BUFSIZ));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "handle", h));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_transferred", res));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "error", err));
      ACE_DEBUG ((LM_DEBUG, "%C = %s\n", "message_block", mb->rd_ptr ()));
      ACE_DEBUG ((LM_DEBUG, "**** end of message ****************\n"));
    }

  if (err == EWOULDBLOCK)
    {
      err=0;
      res=0;
      return check_destroy ();
    }

  if (err !=0  || res <= 0)
    {
      ACE_Message_Block::release (mb);
      return -1;
    }

  ACE_Time_Value tv = ACE_Time_Value::zero;

  int qcount = this->putq (mb, & tv);

  if (qcount <= 0)  // failed to putq
    {
      ACE_Message_Block::release (mb);
      return -1 ;
    }

  int rc = 0;

  if (duplex == 0)      // half-duplex , stop read
    rc = this->terminate_io (ACE_Event_Handler::READ_MASK);
  else                     // full duplex
    {
      if (qcount >= 20 )   // flow control, stop read
        rc = this->terminate_io (ACE_Event_Handler::READ_MASK);
      else
        rc = this->initiate_io (ACE_Event_Handler::READ_MASK);
    }

  if (rc == -1)
    return -1;

  //initiate write
  if (this->initiate_io (ACE_Event_Handler::WRITE_MASK) != 0)
    return -1;

  return check_destroy ();
}

int
Receiver::handle_output (ACE_HANDLE h)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  ACE_Time_Value tv = ACE_Time_Value::zero;
  ACE_Message_Block *mb = 0;

  int     err = 0;
  ssize_t res = 0;
  size_t  bytes = 0;

  int     qcount = this->getq (mb, &tv);

  if (mb != 0)  // qcount >= 0)
    {
      bytes = mb->length ();
      res = this->peer ().send (mb->rd_ptr (), bytes);

      this->total_w_++;

      if (res < 0)
        err = errno ;
      else
        this->total_snd_ += res;


      if (loglevel == 0 || res <= 0 || err!= 0)
        {
          LogLocker log_lock;

          ACE_DEBUG ((LM_DEBUG, "**** Receiver::handle_output () SessionId=%d****\n", index_));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_to_write", bytes));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "handle", h));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_transferred", res));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "error", err));
          ACE_DEBUG ((LM_DEBUG, "%C = %s\n", "message_block", mb->rd_ptr ()));
          ACE_DEBUG ((LM_DEBUG, "**** end of message ****************\n"));
        }
    }

  ACE_Message_Block::release (mb);

  if (err != 0  || res < 0)
    return -1;

  if (qcount <= 0)  // no more message blocks in queue
    {
      if (this->terminate_io (ACE_Event_Handler::WRITE_MASK) != 0)
        return -1;

      if (this->initiate_io (ACE_Event_Handler::READ_MASK) != 0)
        return -1;
    }

  return check_destroy ();
}

// *************************************************************

Connector::Connector (void)
  : ACE_Connector<Sender,ACE_SOCK_CONNECTOR> ((ACE_Reactor *) 0),
    sessions_ (0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);

  for (size_t i = 0; i < MAX_SENDERS; ++i)
     this->list_senders_[i] = 0;
}

Connector::~Connector (void)
{
  this->reactor (0);
  stop ();
}

void
Connector::stop ()
{
  // this method can be called only
  // after reactor event loop id done
  // in all threads

  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);

  for (size_t i = 0; i < MAX_SENDERS; ++i)
    {
      delete this->list_senders_[i];
      this->list_senders_[i] =0;
    }
}

void
Connector::on_new_sender (Sender & sndr)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);
  this->sessions_++;
  this->list_senders_[sndr.index_] = &sndr;
  ACE_DEBUG ((LM_DEBUG,
              "Sender::CTOR sessions_=%d\n",
              this->sessions_));
}

void
Connector::on_delete_sender (Sender & sndr)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, locker, this->mutex_);

  this->sessions_--;
  this->total_snd_ += sndr.get_total_snd();
  this->total_rcv_ += sndr.get_total_rcv();
  this->total_w_   += sndr.get_total_w();
  this->total_r_   += sndr.get_total_r();

  if (sndr.index_ < MAX_SENDERS
      && this->list_senders_[sndr.index_] == &sndr)
    this->list_senders_[sndr.index_] = 0;

  ACE_TCHAR bufs [256];
  ACE_TCHAR bufr [256];

  ACE_OS::sprintf ( bufs , ACE_TEXT ("%ld(%ld)"),
                    sndr.get_total_snd(),
                    sndr.get_total_w() );

  ACE_OS::sprintf ( bufr , ACE_TEXT ("%ld(%ld)"),
                    sndr.get_total_rcv(),
                    sndr.get_total_r() );

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::~DTOR index=%d snd=%s rcv=%s sessions_=%d\n"),
              sndr.index_,
              bufs,
              bufr,
              this->sessions_));

}

int
Connector::start (const ACE_INET_Addr & addr, int num)
{

  if (ACE_Connector<Sender,ACE_SOCK_CONNECTOR>::open (ACE_Reactor::instance (),
                                                      ACE_NONBLOCK) < 0)
    ACE_ERROR_RETURN
      ((LM_ERROR,
        ACE_TEXT("%p\n"),
        ACE_TEXT("Connector::start () - open failed")),
       0);

  int rc = 0;

  for (int i = 0 ; i < num ; i++)
    {
      Sender * sender = 0;

      if (ACE_Connector<Sender,ACE_SOCK_CONNECTOR>::connect (sender, addr) < 0)
        ACE_ERROR_RETURN
          ((LM_ERROR,
            ACE_TEXT("%p\n"),
            ACE_TEXT("Connector::start () - connect failed")),
           rc);
    }

  return rc;
}

int
Connector::make_svc_handler (Sender * & sh)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  if (sessions_ >= MAX_SENDERS)
    return -1;

  for (size_t i = 0; i < MAX_SENDERS; ++i)
    if (this->list_senders_ [i] == 0)
      {
        ACE_NEW_RETURN (sh,
                        Sender (this , i),
                        -1);
        return 0;
      }

  return -1;
}

// *************************************************************

Sender::Sender (Connector* connector, size_t index)
  : connector_ (connector),
    index_ (index),
    flg_mask_ (ACE_Event_Handler::NULL_MASK),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{
  if (connector_ != 0)
    connector_->on_new_sender (*this);

  ACE_OS::sprintf (send_buf_, "%s", data);
}


Sender::~Sender (void)
{
  this->reactor (0);
  if (connector_ != 0)
    connector_->on_delete_sender (*this);

  this->index_ = 0;

  for (; ;)
    {
      ACE_Time_Value tv = ACE_Time_Value::zero;
      ACE_Message_Block *mb = 0;

      if (this->getq (mb, &tv) < 0)
        break;

      ACE_Message_Block::release (mb);
    }
}

int
Sender::check_destroy (void)
{
  if (flg_mask_ == ACE_Event_Handler::NULL_MASK)
    return -1;

  return 0;
}

int Sender::open (void *)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  ACE_Reactor * TPReactor = ACE_Reactor::instance ();

  this->reactor (TPReactor);

  flg_mask_ = ACE_Event_Handler::NULL_MASK ;

  if (TPReactor->register_handler (this,flg_mask_) == -1)
    return -1;

  if (this->initiate_write () == -1)
    return -1;

  if (duplex != 0)
    initiate_io (ACE_Event_Handler::READ_MASK);

  return check_destroy ();
}

int
Sender::initiate_write (void)
{
  if ( this->msg_queue ()->message_count () < 20) // flow control
    {
      size_t nbytes = ACE_OS::strlen (send_buf_);

      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (nbytes+8),
                      -1);

      mb->init (send_buf_, nbytes);
      mb->rd_ptr (mb->base ());
      mb->wr_ptr (mb->base ());
      mb->wr_ptr (nbytes);

      ACE_Time_Value tv = ACE_Time_Value::zero;

      int qcount =this->putq (mb, & tv);

      if (qcount <= 0)
        {
          ACE_Message_Block::release (mb);
          return -1;
        }
    }

  return initiate_io (ACE_Event_Handler::WRITE_MASK);
}

int
Sender::initiate_io (ACE_Reactor_Mask mask)
{
  if (ACE_BIT_ENABLED (flg_mask_, mask))
    return 0;

  if (ACE_Reactor::instance ()->schedule_wakeup (this, mask) == -1)
    return -1;

  ACE_SET_BITS (flg_mask_, mask);
  return 0;
}

int
Sender::terminate_io (ACE_Reactor_Mask mask)
{
  if (ACE_BIT_DISABLED (flg_mask_, mask))
    return 0;

  if (ACE_Reactor::instance ()->cancel_wakeup (this, mask) == -1)
    return -1;

  ACE_CLR_BITS (flg_mask_, mask);
  return 0;
}

int
Sender::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_Reactor * TPReactor = ACE_Reactor::instance ();

  TPReactor->remove_handler (this,
                             ACE_Event_Handler::ALL_EVENTS_MASK |
                             ACE_Event_Handler::DONT_CALL);  // Don't call handle_close
  this->reactor (0);
  this->destroy ();
  return 0;
}

int
Sender::handle_input (ACE_HANDLE h)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ),
                  -1);

  int     err = 0;
  ssize_t res = this->peer ().recv (mb->rd_ptr (),
                                    BUFSIZ-1);
  this->total_r_++;

  if (res >= 0)
    {
      mb->wr_ptr (res);
      this->total_rcv_ += res;
    }
  else
    err = errno ;

  mb->wr_ptr ()[0] = '\0';

  if (loglevel == 0 || res <= 0 || err!= 0)
    {
      LogLocker log_lock;

      ACE_DEBUG ((LM_DEBUG, "**** Sender::handle_input () SessionId=%d****\n", index_));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_to_read", BUFSIZ));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "handle", h));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_transferred", res));
      ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "error", err));
      ACE_DEBUG ((LM_DEBUG, "%C = %s\n", "message_block", mb->rd_ptr ()));
      ACE_DEBUG ((LM_DEBUG, "**** end of message ****************\n"));
    }

  ACE_Message_Block::release (mb);

  if (err == EWOULDBLOCK)
    {
      err=0;
      res=0;
      return check_destroy ();
    }

  if (err !=0  || res <= 0)
    return -1;

  int rc = 0;

  if (duplex != 0)  // full duplex, continue read
    rc = initiate_io (ACE_Event_Handler::READ_MASK);
  else
    rc = terminate_io (ACE_Event_Handler::READ_MASK);

  if (rc != 0)
    return -1 ;

  rc = initiate_write ();
  if (rc != 0)
    return -1;

  return check_destroy ();
}

int
Sender::handle_output (ACE_HANDLE h)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, locker, this->mutex_, -1);

  ACE_Time_Value tv = ACE_Time_Value::zero;
  ACE_Message_Block *mb = 0;

  int     err=0;
  ssize_t res=0;
  size_t  bytes=0;

  int     qcount = this->getq (mb , & tv);

  if (mb != 0)  // qcount >= 0
    {
      bytes = mb->length ();
      res = this->peer ().send (mb->rd_ptr (), bytes);

      this->total_w_++;

      if (res < 0)
        err = errno ;
      else
        this->total_snd_ += res;

      if (loglevel == 0 || res <= 0 || err!= 0)
        {
          LogLocker log_lock;

          ACE_DEBUG ((LM_DEBUG, "**** Sender::handle_output () SessionId=%d****\n", index_));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_to_write", bytes));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "handle", h));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "bytes_transferred", res));
          ACE_DEBUG ((LM_DEBUG, "%C = %d\n", "error", err));
          ACE_DEBUG ((LM_DEBUG, "%C = %s\n", "message_block", mb->rd_ptr ()));
          ACE_DEBUG ((LM_DEBUG, "**** end of message ****************\n"));
        }
    }

  ACE_Message_Block::release (mb);

  if (err != 0  || res < 0)
    return -1;

  int rc = 0;

  if (qcount <= 0)  // no more message blocks in queue
    {
      if (duplex != 0 &&   // full duplex, continue write
          (this->total_snd_ - this->total_rcv_ ) < 1024*32 )  // flow control
        rc = initiate_write ();
      else
        rc = terminate_io (ACE_Event_Handler::WRITE_MASK);

      if (rc == -1)
        return -1;
    }

  rc = initiate_io (ACE_Event_Handler::READ_MASK);
  if (rc == -1)
    return -1;

  return check_destroy ();
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
      ACE_TEXT ("\n-n <number threads in the thread pool>")
      ACE_TEXT ("\n-d <duplex mode 1-on/0-off>")
      ACE_TEXT ("\n-p <port to listen/connect>")
      ACE_TEXT ("\n-h <host> for Sender mode")
      ACE_TEXT ("\n-s <number of sender's instances>")
      ACE_TEXT ("\n-b run client and server at the same time")
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
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1) // no arguments , so one button test
    {
      both = 1;               // client and server simultaneosly
      duplex = 1;             // full duplex is on
      host = ACE_TEXT ("localhost");      // server to connect
      port = ACE_DEFAULT_SERVER_PORT; // port to connect/listen
      threads = 3;            // size of Proactor thread pool
      senders = 20;           // number of senders
      loglevel = 1;           // log level : 0 full/ 1 only errors
      seconds = 20;           // time to run in seconds
#if defined(SOMAXCONN) // The test is invalid if senders > SOMAXCONN
      if(SOMAXCONN < senders)
        senders = SOMAXCONN;
#endif
      return 0;
    }

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:n:p:d:h:s:v:ub"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'i':  // time to run
          seconds = ACE_OS::atoi (get_opt.opt_arg());
          if (seconds < MIN_TIME)
            seconds = MIN_TIME;
          if (seconds > MAX_TIME)
            seconds = MAX_TIME;
          break;
        case 'b':  // both client and server
          both = 1;
          break;
        case 'v':  // log level
          loglevel = ACE_OS::atoi (get_opt.opt_arg());
          break;
        case 'd':         // duplex
          duplex = ACE_OS::atoi (get_opt.opt_arg());
          break;
        case 'h':         // host for sender
          host = get_opt.opt_arg();
          break;
        case 'p':         // port number
          port = ACE_OS::atoi (get_opt.opt_arg());
          break;
        case 'n':         // thread pool size
          threads = ACE_OS::atoi (get_opt.opt_arg());
          break;
        case 's':     // number of senders
          senders = ACE_OS::atoi (get_opt.opt_arg());
          if (size_t (senders) > MAX_SENDERS)
            senders = MAX_SENDERS;
          break;
        case 'u':
        default:
          return print_usage (argc,argv);
        } // switch
    } // while

  return 0;
}

static int
disable_signal (int sigmin, int sigmax)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  sigset_t signal_set;
  if (ACE_OS::sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("Error: (%P | %t):%p\n"),
                ACE_TEXT("sigemptyset failed")));

  for (int i = sigmin; i <= sigmax; i++)
    ACE_OS::sigaddset (&signal_set, i);

  // Put the <signal_set>.
# if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  // In multi-threaded application this is not POSIX compliant
  // but let's leave it just in case.
  if (ACE_OS::sigprocmask (SIG_BLOCK, &signal_set, 0) != 0)
# else
  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, &signal_set, 0) != 0)
# endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: (%P|%t): %p\n"),
                       ACE_TEXT ("SIG_BLOCK failed")),
                      -1);
#else
  ACE_UNUSED_ARG(sigmin);
  ACE_UNUSED_ARG(sigmax);
#endif /* ACE_LACKS_UNIX_SIGNALS */

  return 0;
}

#endif /* ACE_HAS_THREADS */

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("TP_Reactor_Test"));

#if defined(ACE_HAS_THREADS)
  if (::parse_args (argc, argv) == -1)
    return -1;

  disable_signal (SIGPIPE, SIGPIPE);

  MyTask    task1;
  Acceptor  acceptor;
  Connector connector;

  if (task1.start (threads) == 0)
    {
      int rc = 0;

      ACE_INET_Addr addr (port);
      if (both != 0 || host == 0) // Acceptor
        rc += acceptor.start (addr);

      if (both != 0 || host != 0)
        {
          if (host == 0)
            host = ACE_LOCALHOST;

          if (addr.set (port, host, 1, addr.get_type ()) == -1)
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), host));
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

  // As Reactor event loop now is inactive it is safe to destroy all
  // senders

  connector.stop ();
  acceptor.stop ();

  //Print statistic
  ACE_TCHAR bufs [256];
  ACE_TCHAR bufr [256];

  ACE_OS::sprintf ( bufs , ACE_TEXT ("%ld(%ld)"),
                    connector.get_total_snd(),
                    connector.get_total_w() );

  ACE_OS::sprintf ( bufr , ACE_TEXT ("%ld(%ld)"),
                    connector.get_total_rcv(),
                    connector.get_total_r() );

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Connector/Senders  total bytes: snd=%s rcv=%s\n"),
              bufs,
              bufr
            ));

  ACE_OS::sprintf ( bufs , ACE_TEXT ("%ld(%ld)"),
                    acceptor.get_total_snd(),
                    acceptor.get_total_w() );

  ACE_OS::sprintf ( bufr , ACE_TEXT ("%ld(%ld)"),
                    acceptor.get_total_rcv(),
                    acceptor.get_total_r() );

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Acceptor/Receivers total bytes: snd=%s rcv=%s\n"),
              bufs,
              bufr
            ));

#else /* ACE_HAS_THREADS */
   ACE_UNUSED_ARG( argc );
   ACE_UNUSED_ARG( argv );
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;

  return 0;
}
