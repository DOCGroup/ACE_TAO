// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TP_Reactor_Test.h
//
// = DESCRIPTION
//    Define class needed for generating templates. IBM C++ requires this to
//    be in its own file for auto template instantiation.
//
// = AUTHOR
//    Alexander Libman <alibman@ihug.com.au>,<alexl@rumblgroup.com>
//
// ============================================================================

#ifndef ACE_TESTS_TP_REACTOR_TEST_H
#define ACE_TESTS_TP_REACTOR_TEST_H

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

const  size_t MAX_SENDERS = 100;
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
  size_t index_;
  int  flg_mask_;

  ACE_Recursive_Thread_Mutex mutex_;
};

// *************************************************************

class Acceptor : public ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
{
 friend class Receiver;
public:
 size_t get_number_sessions (void) { return sessions_; }

 Acceptor (void);
 virtual ~Acceptor (void);

 void stop (void);
 int start (const ACE_INET_Addr & addr);

 //	virtual from ACE_Acceptor<Receiver,ACE_SOCK_ACCEPTOR>
 virtual int make_svc_handler (Receiver * & sh);

private:

   ACE_Recursive_Thread_Mutex mutex_;
   size_t sessions_;
   Receiver *list_receivers_[MAX_RECEIVERS];

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
  size_t  index_;
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
  size_t  sessions_;
  Sender * list_senders_ [MAX_SENDERS];

  void on_new_sender (Sender & sndr);
  void on_delete_sender (Sender & sndr);
};

#endif /* ACE_TESTS_TP_REACTOR_TEST_H */
