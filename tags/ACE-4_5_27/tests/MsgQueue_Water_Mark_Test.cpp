// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MsgQueue_Water_Mark_Test.cpp
//
// = DESCRIPTION
//      This is a test makes sure the high/low water mark signaling mechanisms
//      work flawlessly.
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

// @@ This test still needs to be clean up.....

#include "test_config.h"
#include "ace/Message_Queue.h"
#include "ace/Task.h"

ACE_RCSID(tests, MsgQueue_Water_Mark_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static const size_t worker_threads = 2;
static const char * default_message = "ACE RULES";
static const size_t default_high_water_mark = 20;
static const size_t default_low_water_mark = 10;
static const int iterations = 2 * default_high_water_mark;

class Message_Handler : public ACE_Task<ACE_SYNCH>
{
public:
  Message_Handler ();

  virtual int svc ();

  int consumer ();
  int producer ();
  int put_message (ACE_Time_Value* timeout = 0);
  int get_message (void);
  void print_producer_debug_message (void);

private:
  const len_, hwm_, lwm_;
  ACE_Atomic_Op <ACE_Thread_Mutex, int> role_;
  ACE_Barrier mq_full_, mq_low_water_mark_hit_;
};

Message_Handler::Message_Handler ()
  : len_ (ACE_OS::strlen (default_message) + 1),
    hwm_ (this->len_ * default_high_water_mark),
    lwm_ (this->len_ * default_low_water_mark),
    role_ (0),
    mq_full_ (worker_threads),
    mq_low_water_mark_hit_ (worker_threads)
{
  this->water_marks (ACE_IO_Cntl_Msg::SET_LWM,
                     this->lwm_);
  this->water_marks (ACE_IO_Cntl_Msg::SET_HWM,
                     this->hwm_);
}

int
Message_Handler::producer (void)
{
  int result = 0;
  int i, hwm;

  for (hwm = this->hwm_, i = iterations; hwm >= 0 ; hwm -= this->len_, i--)
    {
      result = this->put_message ();
      this->print_producer_debug_message ();
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Producer: High water mark hit - "));
  this->mq_full_.wait ();

  this->put_message ();
  ACE_ASSERT (this->msg_queue ()-> message_bytes () <= this->lwm_ + this->len_);
  this->print_producer_debug_message ();

  for (i--; i >= 0 ; i--)
    {
      this->put_message ();
      this->print_producer_debug_message ();
    }
  return 0;
}

int
Message_Handler::consumer (void)
{
  this->mq_full_.wait ();
  ACE_OS::sleep (1);
  // Let producer proceed and block in putq.

  for (int i = iterations; i >= 0; i--)
    {
      this->get_message ();
      ACE_OS::sleep (0);
    }
  return 0;
}

int
Message_Handler::get_message (void)
{
  ACE_Message_Block *mb;

  if (this->getq (mb) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("dequeue_head")), -1);
  else
    {
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) Consumer: message size = %3d, ")
              ASYS_TEXT ("message count = %3d\n"),
              this->msg_queue ()-> message_bytes (),
              this->msg_queue ()-> message_count ()));
      delete mb;
    }
  return 0;
}

int
Message_Handler::put_message (ACE_Time_Value *timeout)
{
  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (default_message, this->len_),
                  -1);

  return this->putq (mb, timeout);
}

void
Message_Handler::print_producer_debug_message (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) Producer: message size = %3d, ")
              ASYS_TEXT ("message count = %3d\n"),
              this->msg_queue ()-> message_bytes (),
              this->msg_queue ()-> message_count ()));
}

int
Message_Handler::svc (void)
{
  int role = this->role_.value ();
  this->role_++;

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
main (int, ASYS_TCHAR *[])
{
  //  ACE_START_TEST (ASYS_TEXT ("MsgQueue_Water_Mark_Test"));

  Message_Handler msg_handler;
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("High water mark is %d\n")
              ASYS_TEXT ("Low water mark is %d\n"),
              default_high_water_mark,
              default_low_water_mark));
              
  msg_handler.activate (THR_NEW_LWP, worker_threads);

  ACE_Thread_Manager::instance ()->wait ();
  //  ACE_END_TEST;
  return 0;
}
