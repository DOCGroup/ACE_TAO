// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Message_Queue_Notification_Test.cpp
//
// = DESCRIPTION
//      There are two tests that test 2 different notification
//      mechanisms in Message Queue.
//
//      The first test illustrates the notification mechanisms in
//      Message_Queue and its integration with Reactor.
//
//      Note the following things about this part of the test:
//
//      1. Multiple threads are not required.
//      2. You do not have to explicitly notify the Reactor
//      3. This code will work the same with any Reactor Implementation
//      4. handle_input, handle_exception, handle_output are the only
//         callbacks supported by this mechanism
//      5. The notification mechanism need not notify the Reactor. You can
//         write your own strategy classes that can do whatever application
//         specific behavior you want.
//
//      The second test also makes sure the high/low water mark
//      signaling mechanism works flawlessly.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu> and Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Task.h"
#include "ace/Reactor_Notification_Strategy.h"
#include "ace/Atomic_Op.h"
#include "ace/Barrier.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"



static int iterations = 10;

static const size_t worker_threads = 2;
static const char * default_message = "ACE RULES";
static const size_t default_high_water_mark = 20;
static const size_t default_low_water_mark = 10;
static const int watermark_iterations = 2 * default_high_water_mark;

class Message_Handler : public ACE_Task<ACE_NULL_SYNCH>
{
  // = TITLE
  //   This class implements a notification strategy for the Reactor.
public:
  // = Initialization and termination.
  Message_Handler (ACE_Reactor &reactor);
  // Constructor.

  // = Demuxing hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);

private:
  int process_message (void);
  void make_message (void);

  ACE_Reactor_Notification_Strategy notification_strategy_;
};

class Watermark_Test : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //     This class test the correct functioning of build-in flow
  //     control machanism in ACE_Task.
public:
  Watermark_Test (void);

  virtual int svc (void);

  int consumer (void);
  int producer (void);
  int put_message (ACE_Time_Value* timeout = 0);
  int get_message (void);
  void print_producer_debug_message (void);

private:
  const size_t len_;
  const size_t hwm_;
  const size_t lwm_;
  ACE_Atomic_Op <ACE_SYNCH_MUTEX, int> role_;
#if defined (ACE_HAS_THREADS)
  ACE_Barrier mq_full_;
  ACE_Barrier mq_low_water_mark_hit_;
#endif /* ACE_HAS_THREADS */
};

Message_Handler::Message_Handler (ACE_Reactor &reactor)
  // First time handle_input will be called
  : notification_strategy_ (&reactor,
                            this,
                            ACE_Event_Handler::READ_MASK)
{
  this->msg_queue ()->notification_strategy (&this->notification_strategy_);
  this->make_message ();
}

int
Message_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message_Handler::handle_input\n")));

  // Next time handle_output will be called.
  this->notification_strategy_.mask (ACE_Event_Handler::WRITE_MASK);

  return process_message ();
}

int
Message_Handler::handle_output (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message_Handler::handle_output\n")));
  ACE_UNUSED_ARG (fd);

  // Next time handle_exception will be called.
  this->notification_strategy_.mask (ACE_Event_Handler::EXCEPT_MASK);

  return process_message ();
}

int
Message_Handler::handle_exception (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message_Handler::handle_exception\n")));
  ACE_UNUSED_ARG (fd);

  // Next time handle_input will be called.
  this->notification_strategy_.mask (ACE_Event_Handler::READ_MASK);

  return this->process_message ();
}

int
Message_Handler::process_message (void)
{
  ACE_Message_Block *mb = 0;

  if (this->getq (mb,
                  (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("dequeue_head")),
                      -1);
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("message received = %s\n"),
                  mb->rd_ptr ()));
      mb->release ();
    }

  this->make_message ();
  return 0;
}

void
Message_Handler::make_message (void)
{
  if (--iterations > 0)
    {
      ACE_Message_Block *mb = 0;
      ACE_NEW (mb,
               ACE_Message_Block ((char *) ACE_TEXT ("hello")));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("sending message\n")));
      this->putq (mb);
    }
}

Watermark_Test::Watermark_Test (void)
  : len_ (ACE_OS::strlen (default_message) + 1),
    hwm_ (this->len_ * default_high_water_mark),
    lwm_ (this->len_ * default_low_water_mark),
    role_ (0)
#if defined (ACE_HAS_THREADS)
    , mq_full_ (worker_threads),
    mq_low_water_mark_hit_ (worker_threads)
#endif /* ACE_HAS_THREADS */
{
  this->water_marks (ACE_IO_Cntl_Msg::SET_LWM,
                     this->lwm_);
  this->water_marks (ACE_IO_Cntl_Msg::SET_HWM,
                     this->hwm_);
}

int
Watermark_Test::producer (void)
{
  int i = watermark_iterations;

  for (ssize_t hwm = this->hwm_;
       hwm >= 0 ;
       hwm -= this->len_)
    {
      this->put_message ();
      this->print_producer_debug_message ();
      i--;
      if (this->msg_queue ()->is_full ())
        break;

    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Producer: High water mark hit ----\n")));

  ACE_MT (this->mq_full_.wait ());

  // The following put_message should block until the message queue
  // has dropped under the lwm.
  this->put_message ();

  ACE_TEST_ASSERT (this->msg_queue ()-> message_bytes () <= this->lwm_ + this->len_);

  this->print_producer_debug_message ();

  for (i--; i >= 0 ; i--)
    {
      this->put_message ();
      this->print_producer_debug_message ();
    }

  return 0;
}

int
Watermark_Test::consumer (void)
{
  ACE_MT (this->mq_full_.wait ());

  ACE_OS::sleep (1);

  // Let producer proceed and block in putq.

  for (int i = watermark_iterations; i >= 0; i--)
    {
      this->get_message ();
      ACE_OS::sleep (0);
    }

  return 0;
}

int
Watermark_Test::get_message (void)
{
  ACE_Message_Block *mb = 0;

  if (this->getq (mb) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("dequeue_head")),
                      -1);
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Consumer: message size = %3d, ")
                  ACE_TEXT ("message count = %3d\n"),
                  this->msg_queue ()-> message_bytes (),
                  this->msg_queue ()-> message_count ()));
      mb->release ();
    }

  return 0;
}

int
Watermark_Test::put_message (ACE_Time_Value *timeout)
{
  ACE_Message_Block *mb = 0;

  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (default_message,
                                     this->len_),
                  -1);

  return this->putq (mb, timeout);
}

void
Watermark_Test::print_producer_debug_message (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Producer: message size = %3d, ")
              ACE_TEXT ("message count = %3d\n"),
              this->msg_queue ()-> message_bytes (),
              this->msg_queue ()-> message_count ()));
}

int
Watermark_Test::svc (void)
{
  // this->role_ is an Atomic_Op object.
  int role = this->role_++;

  switch (role)
    {
    case 0:
      this->producer ();
      break;
    case 1:
      this->consumer ();
      break;
    default:
      break;
    }
  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Message_Queue_Notifications_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Starting message queue reactive notification test...\n")));

  ACE_Reactor reactor;
  Message_Handler mh (reactor);

  while (iterations > 0)
    reactor.handle_events ();

#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Starting message queue watermark test...\n")));
  Watermark_Test watermark_test;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("High water mark is %d\n")
              ACE_TEXT ("Low water mark is %d\n"),
              default_high_water_mark,
              default_low_water_mark));

  watermark_test.activate (THR_NEW_LWP,
                           worker_threads);

  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Message queue watermark test not performed because threads are not supported\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}

