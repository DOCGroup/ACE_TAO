// $Id$

// Exercise more tests for the <ACE_Task>s.  This also shows off some
// Interesting uses of the <ACE_Log_Msg>'s ability to print to
// ostreams.  BTW, make sure that you set the out_stream in *every*
// thread that you want to have write to the output file, i.e.:
//
//  if (out_stream)
//    {
//      ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
//      ACE_LOG_MSG->msg_ostream (out_stream);
//    }

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_main.h"
#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Signal.h"



#if defined (ACE_HAS_THREADS)

static ACE_OSTREAM_TYPE *out_stream = 0;
static sig_atomic_t done = 0;
static const size_t NUM_INVOCATIONS = 100;
static const size_t TASK_COUNT = 130;

class Test_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Test_Task (void);
  ~Test_Task (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *args = 0);
  virtual int close (u_long flags = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int svc (void);

  virtual int handle_input (ACE_HANDLE fd);

  ACE_Reactor *r_;
  size_t handled_;
  static size_t current_count_;
  static size_t done_cnt_;
};

size_t Test_Task::current_count_ = 0;
size_t Test_Task::done_cnt_ = 0;

static ACE_Thread_Mutex Lock;

Test_Task::Test_Task (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, Lock);

  this->handled_ = 0;
  Test_Task::current_count_++;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test_Task constructed, current_count_ = %d\n"),
              Test_Task::current_count_));
}

Test_Task::~Test_Task (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, Lock);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test_Task destroyed, current_count_ = %d\n"),
              Test_Task::current_count_));
}

int
Test_Task::open (void *args)
{
  r_ = reinterpret_cast <ACE_Reactor *> (args);
  return ACE_Task<ACE_MT_SYNCH>::activate (THR_NEW_LWP);
}

int
Test_Task::close (u_long)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Lock, -1);

  Test_Task::current_count_--;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test_Task::close () current_count_ = %d.\n"),
              Test_Task::current_count_));
  return 0;
}

int
Test_Task::svc (void)
{
  // Every thread must register the same stream to write to file.
  if (out_stream)
    {
      ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (out_stream);
    }

  for (size_t index = 0; index < NUM_INVOCATIONS; index++)
    {
      ACE_OS::thr_yield ();

      if (r_->notify (this, ACE_Event_Handler::READ_MASK) == -1)
        {
          ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Lock, -1);

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Test_Task: error %p!\n"),
                             ACE_TEXT ("notifying reactor")),
                             0);
        }
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) returning from svc ()\n")));
  return 0;
}

int
Test_Task::handle_input (ACE_HANDLE)
{
  this->handled_++;

  if (this->handled_ == NUM_INVOCATIONS)
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Lock, -1);
      Test_Task::done_cnt_++;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT (" (%t) Test_Task: handle_input done_cnt_ = %d.\n"),
                  Test_Task::done_cnt_));
    }

  ACE_OS::thr_yield ();
  return -1;
}

static void *
dispatch (void *arg)
{
  // every thread must register the same stream to write to file
  if (out_stream)
    {
      ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (out_stream);
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) Dispatcher Thread started!\n")));
  ACE_Reactor *r = reinterpret_cast <ACE_Reactor *> (arg);
  int result;

  r->owner (ACE_OS::thr_self ());

  while (1)
    {
      result = r->handle_events ();

      if (result <= 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Dispatch: handle_events (): %d"),
                    result));
    }

  ACE_NOTREACHED (return 0);
}

extern "C" void
handler (int)
{
  done = 1;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *[])
{
  if (argc > 1)
    {
      // Send output to file.
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
      ACE_NEW_RETURN (out_stream,
                      ofstream ("test_task_three.out",
                                ios::trunc|ios::out),
                      -1);
#else
      if ((out_stream = ACE_OS::fopen ("test_task_three.out", "w")) == 0)
        return -1;
#endif
      ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (out_stream);
    }

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  ACE_Reactor *reactor1 = ACE_Reactor::instance ();
  ACE_Reactor *reactor2 = new ACE_Reactor ();

  Test_Task t1[TASK_COUNT];
  Test_Task t2[TASK_COUNT];

  ACE_Thread::spawn (ACE_THR_FUNC (dispatch),
                     reactor2);

  reactor1->owner (ACE_OS::thr_self ());

  for (size_t index = 0; index < TASK_COUNT; index++)
    {
      t1[index].open (reactor1);
      t2[index].open (reactor2);
    }

  ACE_OS::sleep (3);

  while (done == 0)
    {
      ACE_Time_Value timeout (2);

      if (reactor1->handle_events (timeout) <= 0)
        {
          if (errno == ETIME)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("no activity within 2 seconds, shutting down\n")));
              break;
            }
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p error handling events\n"),
                        ACE_TEXT ("main")));
        }
    }

  if (argc > 1)
    {
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
      *out_stream << flush;
      delete out_stream;
#else
      ACE_OS::fflush(out_stream);
      ACE_OS::fclose(out_stream);
#endif
      ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (0);
    }

  // Bail out here so that we don't call the destructors for the tasks..
  ACE_OS::exit (0);
  /* NOTREACHED */

  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("threads not supported on this platform\n")));
  return 0;
}
#endif /* ACE_HAS_THREADS */
