// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TPReactor_test.cpp
//
// = DESCRIPTION
//    This program illustrates how the <ACE_TP_Reactor> can be used to
//    implement an application that does various operations.
//
// = AUTHOR
//      Alexander Libman <alibman@baltimore.com>
//
// ============================================================================

#include "test_config.h"

#include "ace/Signal.h"
#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"

ACE_RCSID(TPReactor, TPReactor_Test, "TPReactor_Test.cpp,v 1.27 2000/03/07 17:15:56 schmidt Exp")

//  Some debug helper functions
static int disable_signal (int sigmin, int sigmax);

// both: 0 run client or server / depends on host
//     != 0 run client and server
static int both = 0;

// Host that we're connecting to.
static const ACE_TCHAR *host = 0;

// number of Senders instances
static size_t senders = 1;
const  size_t MAX_SENDERS = 100;
const  size_t MAX_RECEIVERS = 1000;

// duplex mode: == 0 half-duplex
//              != 0 full duplex
static int duplex = 0;

// number threads in the TP_Reactor thread pool
static size_t threads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Log options
static int logflag  = 0; // 0 STDERR, 1 FILE
static int loglevel = 0; // 0 full , 1 only errors

static const size_t MIN_TIME = 1;    // min 1 sec
static const size_t MAX_TIME = 3600; // max 1 hour
static size_t seconds = 2;  // default time to run - 2 seconds

static char data[] =
  "GET / HTTP/1.1\r\n"
  "Accept: */*\r\n"
  "Accept-Language: C++\r\n"
  "Accept-Encoding: gzip, deflate\r\n"
  "User-Agent: TPReactor_Test/1.0 (non-compatible)\r\n"
  "Connection: Keep-Alive\r\n"
  "\r\n" ;

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
  MyTask (void): sem_ (0),
                 my_reactor_ (0) {}

  virtual ~MyTask () { stop (); }

  virtual int svc (void);

  int start (size_t num_threads);
  int stop (void);

private:
  int  create_reactor (void);
  int  delete_reactor (void);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  ACE_Reactor * my_reactor_;
  ACE_Thread_Semaphore sem_;
};

int
MyTask::create_reactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_ASSERT (this->my_reactor_ == 0);

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

  ACE_Reactor::instance ((ACE_Reactor *) 0);
  delete this->my_reactor_;
  this->my_reactor_ = 0;
  this->reactor (0);

  return 0;
}

int
MyTask::start (size_t num_threads)
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

      ACE_Reactor::end_event_loop ();
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

  // signal that we are ready
  sem_.release (1);

  while (ACE_Reactor::event_loop_done () == 0)
    ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) MyTask finished\n")));
  return 0;
}

// *************************************************************
//   Receiver and Acceptor
// *************************************************************
// forward declaration
class Acceptor;

class Receiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
  friend class Acceptor;
public:

  Receiver (Acceptor * acceptor=0, int index=-1);

  ~Receiver (void);

  // virtual from ACE_Svc_Handler<>
  virtual int open (void * pVoid);

  // virtual from ACE_Event_Handler
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_output (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE h , ACE_Reactor_Mask mask);

private:
  int  terminate_io (ACE_Reactor_Mask mask);
  int  initiate_io (ACE_Reactor_Mask mask);
  int  check_destroy (void);

  Acceptor * acceptor_;
  int  index_;
  int  flg_mask_;

  ACE_Recursive_Thread_Mutex mutex_;
};

// *************************************************************

class Acceptor : public ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
{
 friend class Receiver;
public:
 long get_number_sessions (void) { return sessions_; }

 Acceptor (void);
 virtual ~Acceptor (void);

 void stop (void);
 int start (const ACE_INET_Addr & addr);

 //	virtual from ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
 virtual int make_svc_handler (Receiver * & sh);

private:

   ACE_Recursive_Thread_Mutex mutex_;
   long sessions_;
   Receiver *list_receivers_[MAX_RECEIVERS];

   void on_new_receiver (Receiver & rcvr);
   void on_delete_receiver (Receiver & rcvr);
};

// *************************************************************

Acceptor::Acceptor (void)
  : ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR> ((ACE_Reactor *) 0),
    sessions_ (0)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  for (int i = 0; i < MAX_RECEIVERS; ++i)
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

  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  for (int i = 0; i < MAX_RECEIVERS; ++i)
    {
      delete this->list_receivers_[i];
      this->list_receivers_[i] =0;
    }
}

void
Acceptor::on_new_receiver (Receiver &rcvr)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);
  this->sessions_++;
  this->list_receivers_[ rcvr.index_] = & rcvr;
  ACE_DEBUG ((LM_DEBUG,
              "Receiver::CTOR sessions_=%d\n",
              this->sessions_));
}

void
Acceptor::on_delete_receiver (Receiver &rcvr)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  this->sessions_--;
  if (rcvr.index_ >= 0
     && rcvr.index_ < MAX_RECEIVERS
     && this->list_receivers_[ rcvr.index_] == & rcvr)
    this->list_receivers_[ rcvr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              "Receiver::~DTOR sessions_=%d\n",
              this->sessions_));
}

int
Acceptor::start (const ACE_INET_Addr &addr)
{
  if (ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
      ::open (addr,
              ACE_Reactor::instance (),
              ACE_NONBLOCK) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "Acceptor::start () - open failed"),
                      0);
  return 1;
}

int
Acceptor::make_svc_handler (Receiver *&sh)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  if (sessions_ >= MAX_RECEIVERS)
    return -1;

  for (int i = 0; i < MAX_RECEIVERS; ++i)
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

Receiver::Receiver (Acceptor * acceptor, int index)
  : acceptor_ (acceptor),
    index_ (index),
    flg_mask_ (ACE_Event_Handler::NULL_MASK)
{
  if (acceptor_ != 0)
    acceptor_->on_new_receiver (*this);
}


Receiver::~Receiver (void)
{
  this->reactor (0);
  if (acceptor_ != 0)
    acceptor_->on_delete_receiver (*this);

  this->index_ = -1;

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
Receiver::open (void *pVoid)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (mutex_);

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
Receiver::handle_close (ACE_HANDLE h, ACE_Reactor_Mask mask)
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
Receiver::handle_input (ACE_HANDLE h)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (mutex_);

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ),
                  -1);

  int err = 0;
  ssize_t res = this->peer ().recv (mb->rd_ptr (), BUFSIZ-1);

  if (res >= 0)
    mb->wr_ptr (res);
  else
    err = errno ;

  mb->wr_ptr ()[0] = '\0';

  if (loglevel == 0 || res <= 0 || err!= 0)
    {
      ACE_DEBUG ((LM_DEBUG, "**** Receiver::handle_input () SessionId=%d****\n", index_));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", BUFSIZ));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", h));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transferred", res));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", err));
      ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", mb->rd_ptr ()));
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
      if (qcount >= 2)   // flow control, stop read
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
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (mutex_);

  ACE_Time_Value tv = ACE_Time_Value::zero;
  ACE_Message_Block *mb = 0;

  int     err = 0;
  ssize_t res = 0;
  int     bytes = 0;

  int     qcount = this->getq (mb, &tv);

  if (mb != 0)  // qcount >= 0)
    {
      bytes = mb->length ();
      res = this->peer ().send (mb->rd_ptr (), bytes);

      if (res < 0)
        err = errno ;

      if (loglevel == 0 || res <= 0 || err!= 0)
        {
          ACE_DEBUG ((LM_DEBUG, "**** Receiver::handle_output () SessionId=%d****\n", index_));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", bytes));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", h));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transferred", res));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", err));
          ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", mb->rd_ptr ()));
          ACE_DEBUG ((LM_DEBUG, "**** end of message ****************\n"));
        }
    }

  ACE_Message_Block::release (mb);

  if (err != 0  || res < 0)
    return -1;

  if (qcount < 0)  // no more message blocks in queue
    {
      if (this->terminate_io (ACE_Event_Handler::WRITE_MASK) != 0)
        return -1;

      if (this->initiate_io (ACE_Event_Handler::READ_MASK) != 0)
        return -1;
    }

  return check_destroy ();
}

// *******************************************
//   Sender
// *******************************************

class Connector;

class Sender : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
  friend class Connector;

public:
  Sender (Connector * connector=0, int index=-1);

  ~Sender (void);

  // virtual from ACE_Svc_Handler<>
  virtual int open (void * pVoid);

  // virtual from ACE_Event_Handler
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_output (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE h , ACE_Reactor_Mask mask);

private:
  int  terminate_io (ACE_Reactor_Mask mask);
  int  initiate_io (ACE_Reactor_Mask mask);
  int  initiate_write ();
  int  check_destroy (void);

  Connector * connector_;
  int  index_;
  int  flg_mask_;

  ACE_Recursive_Thread_Mutex mutex_;

  char send_buf_ [1024];
};

// *************************************************************

class Connector: public ACE_Connector<Sender,ACE_SOCK_CONNECTOR>
{
  friend class Sender;
public:
  long get_number_sessions (void) { return sessions_; }

  Connector ();
  virtual ~Connector ();

  void stop ();
  int  start (const ACE_INET_Addr & addr , int num);

  //	virtual from ACE_Connector<>
  virtual int make_svc_handler (Sender * & sh);

private:

  ACE_Recursive_Thread_Mutex mutex_;
  long  sessions_;
  Sender * list_senders_ [MAX_SENDERS];

  void on_new_sender (Sender & sndr);
  void on_delete_sender (Sender & sndr);
};

// *************************************************************

Connector::Connector ()
  : ACE_Connector<Sender,ACE_SOCK_CONNECTOR> ((ACE_Reactor *) 0),
    sessions_ (0)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  for (int i = 0; i < MAX_SENDERS; ++i)
     this->list_senders_[i] = 0;
}

Connector::~Connector ()
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

  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  for (int i = 0; i < MAX_SENDERS; ++i)
    {
      delete this->list_senders_[i];
      this->list_senders_[i] =0;
    }
}

void
Connector::on_new_sender (Sender & sndr)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);
  this->sessions_++;
  this->list_senders_[ sndr.index_] = &sndr;
  ACE_DEBUG ((LM_DEBUG,
              "Sender::CTOR sessions_=%d\n",
              this->sessions_));
}

void
Connector::on_delete_sender (Sender & sndr)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  this->sessions_--;
  if (sndr.index_ >= 0
     && sndr.index_ < MAX_SENDERS
     && this->list_senders_[ sndr.index_] == &sndr)
    this->list_senders_[ sndr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              "Sender::~DTOR sessions_=%d\n",
              this->sessions_));
}

int
Connector::start (const ACE_INET_Addr & addr, int num)
{

  if (ACE_Connector<Sender,ACE_SOCK_CONNECTOR>
      ::open (ACE_Reactor::instance (),
              ACE_NONBLOCK) < 0)
    ACE_ERROR_RETURN
      ((LM_ERROR,
        "%p\n",
        "Connector::start () - open failed"),
       0);

  int rc = 0;

  for (int i = 0 ; i < num ; i++)
    {
      Sender * sender = 0;

      if (ACE_Connector<Sender,ACE_SOCK_CONNECTOR>
          ::connect (sender, addr) < 0)
        ACE_ERROR_RETURN
          ((LM_ERROR,
            "%p\n",
            "Connector::start () - connect failed"),
           rc);
    }

  return rc;
}

int
Connector::make_svc_handler (Sender * & sh)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (this->mutex_);

  if (sessions_ >= MAX_SENDERS)
    return -1;

  for (int i = 0; i < MAX_SENDERS; ++i)
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

Sender::Sender (Connector* connector, int index)
  : connector_ (connector),
    index_ (index),
    flg_mask_ (ACE_Event_Handler::NULL_MASK)
{
  if (connector_ != 0)
    connector_->on_new_sender (*this);

  ACE_OS::sprintf (send_buf_ ,data);
}


Sender::~Sender (void)
{
  this->reactor (0);
  if (connector_ != 0)
    connector_->on_delete_sender (*this);

  this->index_ = -1;

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

int Sender::open (void * pVoid)
{
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (mutex_);

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
  if (this->msg_queue ()->message_count () < 2) // flow control
    {
      int nbytes = ACE_OS::strlen (send_buf_);

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
Sender::handle_close (ACE_HANDLE h , ACE_Reactor_Mask mask)
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
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (mutex_);

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (BUFSIZ),
                  -1);

  int     err = 0;
  ssize_t res = this->peer ().recv (mb->rd_ptr (),
                                    BUFSIZ-1);

  if (res >= 0)
    mb->wr_ptr (res);
  else
    err = errno ;

  mb->wr_ptr ()[0] = '\0';

  if (loglevel == 0 || res <= 0 || err!= 0)
    {
      ACE_DEBUG ((LM_DEBUG, "**** Sender::handle_input () SessionId=%d****\n", index_));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", BUFSIZ));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", h));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transferred", res));
      ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", err));
      ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", mb->rd_ptr ()));
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
  ACE_Guard<ACE_Recursive_Thread_Mutex> locker (mutex_);

  ACE_Time_Value tv = ACE_Time_Value::zero;
  ACE_Message_Block *mb = 0;

  int     err=0;
  ssize_t res=0;
  int     bytes=0;

  int     qcount = this->getq (mb , & tv);

  if (mb != 0)  // qcount >= 0
    {
      bytes = mb->length ();
      res = this->peer ().send (mb->rd_ptr (), bytes);

      if (res < 0)
        err = errno ;

      if (loglevel == 0 || res <= 0 || err!= 0)
        {
          ACE_DEBUG ((LM_DEBUG, "**** Sender::handle_output () SessionId=%d****\n", index_));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", bytes));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", h));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transferred", res));
          ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", err));
          ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", mb->rd_ptr ()));
          ACE_DEBUG ((LM_DEBUG, "**** end of message ****************\n"));
        }
    }

  ACE_Message_Block::release (mb);

  if (err != 0  || res < 0)
    return -1;

  int rc = 0;

  if (qcount < 0)  // no more message blocks in queue
    {
      if (duplex != 0)  // full duplex, continue write
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
      logflag = 1 ;           // log to : 0 STDERR / 1 FILE
      loglevel = 0;           // log level : 0 full/ 1 only errors
      seconds = 2;            // time to run in seconds
      return 0;
    }

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:n:p:d:h:s:v:ub"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'i':  // time to run
          seconds = ACE_OS::atoi (get_opt.optarg);
          if (seconds < MIN_TIME)
            seconds = MIN_TIME;
          if (seconds > MAX_TIME)
            seconds = MAX_TIME;
          break;
        case 'b':  // both client and server
          both = 1;
          break;
        case 'v':  // log level
          loglevel = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'd':         // duplex
          duplex = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'h':         // host for sender
          host = get_opt.optarg;
          break;
        case 'p':         // port number
          port = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'n':         // thread pool size
          threads = ACE_OS::atoi (get_opt.optarg);
          break;
        case 's':     // number of senders
          senders = ACE_OS::atoi (get_opt.optarg);
          if (senders > MAX_SENDERS)
            senders = MAX_SENDERS;
          break;
        case 'u':
        default:
          return print_usage (argc,argv);
        } // switch
    } // while

  return 0;
}

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("TP_Reactor_Test"));

  if (::parse_args (argc, argv) == -1)
    return -1;

  ::disable_signal (SIGPIPE, SIGPIPE);

  MyTask    task1;
  Acceptor  acceptor;
  Connector connector;

  if (task1.start (threads) == 0)
    {
      int rc = 0;

      if (both != 0 || host == 0) // Acceptor
        // Simplify, initial read with  zero size
        if (acceptor.start (ACE_INET_Addr (port)) == 0)
          rc = 1;

      if (both != 0 || host != 0)
        {
          if (host == 0)
            host = ACE_TEXT ("localhost");

          rc += connector.start (ACE_INET_Addr (port, host),
                                 senders);

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

  ACE_END_TEST;

  return 0;
}

static int
disable_signal (int sigmin, int sigmax)
{
#ifndef ACE_WIN32
  sigset_t signal_set;
  if (sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                "Error: (%P | %t):%p\n",
                "sigemptyset failed"));

  for (int i = sigmin; i <= sigmax; i++)
    sigaddset (&signal_set, i);

  //  Put the <signal_set>.
  if (ACE_OS::pthread_sigmask (SIG_BLOCK, &signal_set, 0) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error: (%P | %t):%p\n",
                "pthread_sigmask failed"));
#endif /* ACE_WIN32 */

  return 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Tuple<Sender>;
template class ACE_Map_Manager<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >;
template class ACE_Map_Entry<int, ACE_Svc_Tuple<Sender> * >;
template class ACE_Map_Iterator<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >;
template class ACE_Map_Reverse_Iterator<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >;
template class ACE_Connector<Sender,ACE_SOCK_CONNECTOR>;
template class ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Tuple<Sender>
#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >
#pragma instantiate ACE_Map_Entry<int, ACE_Svc_Tuple<Sender> * >
#pragma instantiate ACE_Map_Iterator<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >
#pragma instantiate ACE_Map_Reverse_Iterator<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<Sender> *, ACE_RW_Thread_Mutex >
#pragma instantiate ACE_Connector<Sender,ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

