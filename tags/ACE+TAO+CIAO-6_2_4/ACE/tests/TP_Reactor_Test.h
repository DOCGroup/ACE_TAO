
//=============================================================================
/**
 *  @file    TP_Reactor_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Alexander Libman <alibman@ihug.com.au>
 *  @author <alexl@rumblgroup.com>
 */
//=============================================================================


#ifndef ACE_TESTS_TP_REACTOR_TEST_H
#define ACE_TESTS_TP_REACTOR_TEST_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Recursive_Thread_Mutex.h"

const  size_t MAX_SENDERS = 1000;
const  size_t MAX_RECEIVERS = 1000;


// *************************************************************
//   Receiver and Acceptor
// *************************************************************
// forward declaration
class Acceptor;

class Receiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
  friend class Acceptor;
public:

  Receiver (Acceptor * acceptor=0, size_t index=MAX_RECEIVERS+1);

  ~Receiver (void);

  long get_total_snd (void) { return this->total_snd_; }
  long get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

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
  size_t index_;
  int  flg_mask_;

  ACE_Recursive_Thread_Mutex mutex_;
  long total_snd_;
  long total_rcv_;
  long total_w_;
  long total_r_;
};

// *************************************************************

class Acceptor : public ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
{
 friend class Receiver;
public:
  size_t get_number_sessions (void) { return sessions_; }
  long get_total_snd (void) { return this->total_snd_; }
  long get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

  Acceptor (void);
  virtual ~Acceptor (void);

  void stop (void);
  int start (const ACE_INET_Addr & addr);

  // virtual from ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
  virtual int make_svc_handler (Receiver * & sh);

private:

  ACE_Recursive_Thread_Mutex mutex_;
  size_t sessions_;
  Receiver *list_receivers_[MAX_RECEIVERS];
  long total_snd_;
  long total_rcv_;
  long total_w_;
  long total_r_;

  void on_new_receiver (Receiver & rcvr);
  void on_delete_receiver (Receiver & rcvr);
};


// *******************************************
//   Sender
// *******************************************

class Connector;

class Sender : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
  friend class Connector;

public:
  Sender (Connector * connector=0, size_t index=MAX_SENDERS+1);

  ~Sender (void);

  long get_total_snd (void) { return this->total_snd_; }
  long get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

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
  size_t  index_;
  int  flg_mask_;

  ACE_Recursive_Thread_Mutex mutex_;

  char send_buf_ [1024];
  long total_snd_;
  long total_rcv_;
  long total_w_;
  long total_r_;
};

// *************************************************************

class Connector: public ACE_Connector<Sender,ACE_SOCK_CONNECTOR>
{
  friend class Sender;
public:
  size_t get_number_sessions (void) { return sessions_; }
  long get_total_snd (void) { return this->total_snd_; }
  long get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }


  Connector ();
  virtual ~Connector ();

  void stop ();
  int  start (const ACE_INET_Addr & addr , int num);

  // virtual from ACE_Connector<>
  virtual int make_svc_handler (Sender * & sh);

private:

  ACE_Recursive_Thread_Mutex mutex_;
  size_t  sessions_;
  Sender * list_senders_ [MAX_SENDERS];
  long total_snd_;
  long total_rcv_;
  long total_w_;
  long total_r_;

  void on_new_sender (Sender & sndr);
  void on_delete_sender (Sender & sndr);
};


#endif /* ACE_TESTS_TP_REACTOR_TEST_H */
