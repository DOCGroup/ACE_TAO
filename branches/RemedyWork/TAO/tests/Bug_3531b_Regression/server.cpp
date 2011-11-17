// $Id$

#include "ace/Get_Opt.h"
#include "ace/Global_Macros.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/default_resource.h"
#include "tao/Leader_Follower.h"
#include "tao/LF_Event_Loop_Thread_Helper.h"
#include "tao/LF_Event.h"
#include "tao/Transport.h"
#include "ace/Task_T.h"
#include "tao/TAO_Export.h"
#include "ace/TP_Reactor.h"

int nthreads = 4;
bool debug = false;

// TEST_ASSERT is exactly the same as ACE_ASSERT except it is active
// for both debug and *release* builds.
#define TEST_ASSERT(X)                        \
  do { if(!(X)) { \
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("TEST_ASSERT: (%P|%t) file %N, line %l assertion failed for '%C'.%a\n"), \
                  #X, -1));                       \
    } } while (0)

#define TSS_ASSERT(TSS, LF, ELT, CLT, LA) \
    TEST_ASSERT ((TSS->event_loop_thread_ == ELT)); \
    TEST_ASSERT ((TSS->client_leader_thread_ == CLT)); \
    TEST_ASSERT ((LF.leader_available () == LA));

class Worker;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("d"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Command: public ACE_Message_Block
{
public:
  virtual int execute (Worker*) = 0;
};


//////////////////////////////////////////////////////////////////////
// NOTE: Do *NOT* put the same msg into the msg queue more than once.
// This will confuse the msg queue and result it in dropping messages
//////////////////////////////////////////////////////////////////////
class Worker: public ACE_Task<ACE_SYNCH>
{
public:
  Worker (void)
    : shutdown_ (false)
  {}

  virtual int svc (void);
  virtual int close (u_long = 0);
  virtual int put (ACE_Message_Block * mblk, ACE_Time_Value * tv = 0);
  int process_cmd (void);
  void shutdown (bool do_shutdown);
  bool shutdown (void);

private:
  bool shutdown_;
};

ACE_TSS<Worker> *workers_p = 0;
#define workers (*workers_p)

int Worker::svc (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Worker thread starting up.\n")));
  // Register this worker
  workers.ts_object (const_cast<Worker*> (this));
  int retval = 0;
  while (!shutdown_ && retval != -1)
    {
      retval = this->process_cmd ();
    }
  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Worker thread shutting down.\n")));
  return retval;
}

int Worker::close (u_long)
{
  // de-register this worker, otherwise the ACE_TSS will try to destroy it
  workers.ts_object (0);
  return 0;
}

int Worker::put (ACE_Message_Block * mblk, ACE_Time_Value * tv)
{
  return this->putq (mblk, tv);
}

int Worker::process_cmd (void)
{
  ACE_Message_Block *mb = 0;
  if (this->getq (mb, 0) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error calling getq: $!\n")));
      // Strangely, message queues return this instead of ETIME
      if (errno == EWOULDBLOCK || errno == ESHUTDOWN)
        return 0;
      return -1;
    }
  Command* cmd = dynamic_cast <Command*> (mb);
  ACE_ASSERT (cmd != 0);
  cmd->execute (this);
  cmd->release ();
  return 0;
}

void Worker::shutdown (bool do_shutdown)
{
  shutdown_ = do_shutdown;
}

bool Worker::shutdown (void)
{
  return shutdown_;
}

class Test_Reactor: public ACE_TP_Reactor
{
public:
  Test_Reactor (size_t max_number_of_handles,
                  bool restart = false,
                  ACE_Sig_Handler *sh = 0,
                  ACE_Timer_Queue *tq = 0,
                  bool mask_signals = true,
                  int s_queue = ACE_Select_Reactor_Token::FIFO)
  : ACE_TP_Reactor(max_number_of_handles, restart, sh, tq, mask_signals, s_queue) {}


  // This is the method that the Leader_Follower object calls.
  virtual int handle_events (ACE_Time_Value * = 0)
  {
    if (TAO_debug_level > 10)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Executing Test_Reactor::handle_events\n")));
    // This is called by client leader threads.  Note, the loop here
    // glosses over the fact that the Leader_Follower code does not
    // work quite the way we want it to.  Namely, this logic:
    //  1 - client leader thread detects when there are event loop
    //  threads waiting
    //  2 - client leader wakes up event loop threads via broadcast
    //  3 - client leader temporarily gives up lock to allow event loop
    //  threads to take over leadership
    //  4 - client leader thread takes lock again and loops around to
    //  become follower
    // The problem is that the gap between 3 & 4 is often not long
    // enough for the event loop threads to get switched in and take
    // ovwnership of the lock, even though the client leader thread
    // does a thr_yield!
    // Thus this code, once shutdown, will continuely return and thus
    // give the leader follower multiple chances to hand off to an
    // event loop thread.  This is not ideal but it will have to do
    // until the leader follower code is fixed (if possible)
    while (!workers->shutdown())
      // call this thread's (worker's) process_cmd method
      workers->process_cmd ();
    return 0;
  };

  virtual int handle_events (ACE_Time_Value &)
  {
    return this->handle_events ();
  }
};

// Our own Resource_Factory for testing purposes.  This just returns
// our Test_Reactor to the Leader_Follower object via the ORB_Core.
class Test_Resource_Factory: public TAO_Default_Resource_Factory
{
public:
  Test_Resource_Factory ()
  {}

  virtual ACE_Reactor_Impl* allocate_reactor_impl (void) const
  {
    ACE_Reactor_Impl *impl = 0;
    /*
     * Hook to specialize TAO's reactor implementation.
     */
  //@@ TAO_REACTOR_SPL_COMMENT_HOOK_START
    ACE_NEW_RETURN (impl,
                    Test_Reactor (ACE::max_handles (),
                                    1,
                                    (ACE_Sig_Handler*)0,
                                    (ACE_Timer_Queue*)0,
                                    this->reactor_mask_signals_,
                                    ACE_Select_Reactor_Token::LIFO),
                    0);
  //@@ TAO_REACTOR_SPL_COMMENT_HOOK_END
    return impl;
  }

private:
};

// force export flag otherwise Windoze will complain
#define TAO_Test_Export ACE_Proper_Export_Flag

ACE_FACTORY_DEFINE (TAO_Test, Test_Resource_Factory)
ACE_STATIC_SVC_DEFINE (Test_Resource_Factory,
                       ACE_TEXT ("Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Test_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_STATIC_SVC_REQUIRE (Test_Resource_Factory);

int load_test_resources =
ACE_Service_Config::process_directive (ace_svc_desc_Test_Resource_Factory);

class Test_LF_Event: public TAO_LF_Event
{
public:
  Test_LF_Event()
  {}

  void complete_event (TAO_Leader_Follower &lf)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Completing event\n")));
    this->state_changed (TAO_LF_Event::LFS_SUCCESS, lf);
  }

  virtual int successful (void) const
  {
    return this->state_ == TAO_LF_Event::LFS_SUCCESS;
  }

  virtual int error_detected (void) const
  {
    return (this->state_ == TAO_LF_Event::LFS_FAILURE
            || this->state_ == TAO_LF_Event::LFS_TIMEOUT
            || this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED);
  }
protected:
  virtual void state_changed_i (int new_state)
  {
    this->state_ = new_state;
    return;
  }

  virtual int is_state_final (void)
  {
    if (this->state_ == TAO_LF_Event::LFS_TIMEOUT ||
        this->state_ == TAO_LF_Event::LFS_FAILURE)
      return 1;
    return 0;
  }
};

class Test_Transport : public TAO_Transport
{
public:
  Test_Transport (CORBA::ULong tag,
                  TAO_ORB_Core *orb_core)
    : TAO_Transport (tag, orb_core)
  {}

  virtual int send_message (TAO_OutputCDR &,
                            TAO_Stub * = 0,
                            TAO_Message_Semantics  = TAO_Message_Semantics (),
                            ACE_Time_Value * = 0)
  {
    return 0;
  }

  virtual ssize_t send (iovec *, int ,
                        size_t &,
                        const ACE_Time_Value * = 0)
  {
    return 0;
  }

  virtual ssize_t recv (char *,
                        size_t,
                        const ACE_Time_Value * = 0)
  {
    return 0;
  }

  virtual int messaging_init (CORBA::Octet,
                              CORBA::Octet)
  {
    return 0;
  }

  virtual ACE_Event_Handler * event_handler_i (void)
  {
    return 0;
  }

protected:
  virtual TAO_Connection_Handler * connection_handler_i (void)
  {
    return 0;
  }

  virtual int send_request (TAO_Stub *,
                            TAO_ORB_Core *,
                            TAO_OutputCDR &,
                            TAO_Message_Semantics,
                            ACE_Time_Value *)
  {
    return 0;
  }

};

class Shutdown: public Command
{
public:
  virtual int execute (Worker* worker)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Shutdown cmd\n")));
    worker->shutdown (true);
    return 0;
  }

};

class TSS_Assert: public Command
{
public:
  TSS_Assert (TAO_ORB_Core* orb_core,
              int elt_count,
              int clt_count,
              bool leader_available)
    : orb_core_ (orb_core),
      elt_count_ (elt_count),
      clt_count_ (clt_count),
      leader_available_ (leader_available)
  {}

  virtual int execute (Worker*)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Executing TSS_Assert(%d,%d,%d) cmd\n"),
                  elt_count_, clt_count_, leader_available_));
    TAO_Leader_Follower &leader_follower = orb_core_->leader_follower ();
    TAO_ORB_Core_TSS_Resources* tss = orb_core_->get_tss_resources ();
    TSS_ASSERT (tss, leader_follower,
                elt_count_, clt_count_, leader_available_);
    return 0;
  }
private:
  TAO_ORB_Core*        orb_core_;
  const int            elt_count_;
  const int            clt_count_;
  const bool           leader_available_;
};

class Wait_For_Event: public Command
{
public:
  Wait_For_Event (Test_LF_Event& event,
                  Test_Transport& transport,
                  TAO_Leader_Follower& lf)
    : event_ (event),
      transport_ (transport),
      lf_ (lf)
  {}
  virtual int execute (Worker*)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Executing Wait_For_Event cmd\n")));
    int retval = lf_.wait_for_event (&event_, &transport_, 0);
    // The worker has probably been shutdown in order for the client
    // leader event loop to exit - reactivate the worker so it from
    // process msgs once we return
    workers->shutdown (false);
    return retval;
  }
private:
  Test_LF_Event&       event_;
  Test_Transport&      transport_;
  TAO_Leader_Follower& lf_;
};

class Cond_Signal: public Command
{
public:
  Cond_Signal ()
    : lock_ (),
      cond_ (lock_),
      ref_count_ (2)
  {}

  virtual int execute (Worker*)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Executing Cond_Signal cmd\n")));
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard ,this->cond_.mutex (), 0);
    return this->cond_.signal ();
  }
  TAO_SYNCH_MUTEX& lock ()
  {
    return lock_;
  }
  ACE_Condition_Thread_Mutex& cond ()
  {
    return cond_;
  }
  virtual ACE_Message_Block *release (void)
  {
    // we need to only release once both the main and worker thread
    // are done with this object - each signals this by calling this
    // method
    --this->ref_count_;
    if (this->ref_count_ == 0)
      return ACE_Message_Block::release ();
    return this;
  }

private:
  TAO_SYNCH_MUTEX lock_;
  ACE_Condition_Thread_Mutex cond_;
  int ref_count_;
};

class Event_Loop_Thread: public Command
{
public:
  Event_Loop_Thread(TAO_Leader_Follower& lf,
                    TAO_LF_Strategy& lf_strategy)
    : lf_ (lf), lf_strategy_ (lf_strategy)
  {}

  virtual int execute (Worker* worker)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Executing Event_Loop_Thread cmd\n")));
    TAO_LF_Event_Loop_Thread_Helper elt (lf_, lf_strategy_, 0);
    while (!worker->shutdown())
      worker->process_cmd ();
    // The worker has been shutdown in order for this event loop
    // thread to exit - reactivate the worker so it from process msgs
    // once we return
    worker->shutdown (false);
    return 0;
  }
private:
  TAO_Leader_Follower& lf_;
  TAO_LF_Strategy&     lf_strategy_;
};

class Set_Upcall_Thread: public Command
{
public:
  Set_Upcall_Thread (TAO_Leader_Follower& lf)
    : lf_ (lf)
  {}

  virtual int execute (Worker*)
  {
    if (debug)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Executing Set_Upcall_Thread cmd\n")));
    lf_.set_upcall_thread ();
    return 0;
  }

private:
  TAO_Leader_Follower& lf_;
};

void synch_with_worker (Worker& worker)
{
  // This object is released by the worker thread after it has
  // executed the cmd
  Cond_Signal* cond = new Cond_Signal;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, cond->lock ());
    worker.put (cond);
    ACE_Time_Value tv (1, 0);
    tv += ACE_OS::gettimeofday ();
    TEST_ASSERT ((cond->cond ().wait (&tv) == 0));
  }
}

// 1 - Simple event loop thread test
void Test_1 (TAO_ORB_Core* orb_core)
{
  TAO_LF_Strategy &lf_strategy = orb_core->lf_strategy ();
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  TAO_ORB_Core_TSS_Resources* tss = orb_core->get_tss_resources ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #1 - Simple Event Loop call\n")));

  TSS_ASSERT (tss, leader_follower, 0, 0, false);

  std::auto_ptr<TAO_LF_Event_Loop_Thread_Helper>
    elt (new TAO_LF_Event_Loop_Thread_Helper(leader_follower,
                                             lf_strategy,
                                             0));
  TSS_ASSERT (tss, leader_follower, 1, 0, true);

  elt.reset (0);
  TSS_ASSERT (tss, leader_follower, 0, 0, false);
}

// 2 - Nested event loop threads - no set_upcall_thread call
void Test_2 (TAO_ORB_Core* orb_core)
{
  TAO_LF_Strategy &lf_strategy = orb_core->lf_strategy ();
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  TAO_ORB_Core_TSS_Resources* tss = orb_core->get_tss_resources ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #2 - 2 nested Event Loop calls\n")));

  TSS_ASSERT (tss, leader_follower, 0, 0, false);

  std::auto_ptr<TAO_LF_Event_Loop_Thread_Helper>
    elt1 (new TAO_LF_Event_Loop_Thread_Helper(leader_follower,
                                              lf_strategy, 0));
  TSS_ASSERT (tss, leader_follower, 1, 0, true);

  std::auto_ptr<TAO_LF_Event_Loop_Thread_Helper>
    elt2 (new TAO_LF_Event_Loop_Thread_Helper(leader_follower,
                                              lf_strategy, 0));
  TSS_ASSERT (tss, leader_follower, 2, 0, true);

  elt2.reset (0);
  TSS_ASSERT (tss, leader_follower, 1, 0, true);

  elt1.reset (0);
  TSS_ASSERT (tss, leader_follower, 0, 0, false);
}

// 3 - Nested event loop threads - with set_upcall_thread call
void Test_3 (TAO_ORB_Core* orb_core)
{
  TAO_LF_Strategy &lf_strategy = orb_core->lf_strategy ();
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  TAO_ORB_Core_TSS_Resources* tss = orb_core->get_tss_resources ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #3 - 2 nested Event Loop calls with set_upcall_thread\n")));

  TSS_ASSERT (tss, leader_follower, 0, 0, false);

  std::auto_ptr<TAO_LF_Event_Loop_Thread_Helper>
    elt1 (new TAO_LF_Event_Loop_Thread_Helper(leader_follower,
                                              lf_strategy, 0));
  TSS_ASSERT (tss, leader_follower, 1, 0, true);

  leader_follower.set_upcall_thread ();
  TSS_ASSERT (tss, leader_follower, 0, 0, false);

  std::auto_ptr<TAO_LF_Event_Loop_Thread_Helper>
    elt2 (new TAO_LF_Event_Loop_Thread_Helper(leader_follower,
                                              lf_strategy, 0));
  TSS_ASSERT (tss, leader_follower, 1, 0, true);

  elt2.reset (0);
  TSS_ASSERT (tss, leader_follower, 0, 0, false);

  elt1.reset (0);
  TSS_ASSERT (tss, leader_follower, 0, 0, false);
}

// 4 - client leader thread
void Test_4 (TAO_ORB_Core* orb_core)
{
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #4 - Simple Client Leader thread\n")));

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread wait on an event
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  wrk1.put (new Wait_For_Event(event, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Synchronise with the thread before we complete its event
  synch_with_worker (wrk1);
  // Complete the event
  event.complete_event (leader_follower);

  // The thread is still inside handle_events - shutdown the
  // event processing
  wrk1.put (new Shutdown);

  // The thread should now return from being a client leader
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  wrk1.put (new Shutdown);
  wrk1.wait ();
}

// 5 - nested client leader thread
void Test_5 (TAO_ORB_Core* orb_core)
{
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #5 - 2 nested Client Leader calls\n")));

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread wait on an event
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  wrk1.put (new Wait_For_Event(event, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Wait for another event
  Test_LF_Event event2;
  wrk1.put (new Wait_For_Event(event2, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 2, true));

  // Synchronise with the thread before we complete its event
  synch_with_worker (wrk1);

  // Complete the first event - nothing should happen
  event.complete_event (leader_follower);

  wrk1.put (new TSS_Assert(orb_core, 0, 2, true));

  // Complete the second event - everything should unwind
  synch_with_worker (wrk1);
  event2.complete_event (leader_follower);

  // The thread is still inside handle_events - shutdown the
  // event processing for the inner client leader
  wrk1.put (new Shutdown);

  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // The thread is now in the handle_events for the outter
  // client-leader - the event is already complete so just
  // shutdown the cmd processing.
  wrk1.put (new Shutdown);

  // We should now we back at our initial state.
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  wrk1.put (new Shutdown);
  wrk1.wait ();
}

// 6 - nested client leader thread with set_upcall_thread
void Test_6 (TAO_ORB_Core* orb_core)
{
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #6 - 2 nested Client Leader calls with set_upcall_thread\n")));

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread wait on an event
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  wrk1.put (new Wait_For_Event(event, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Call set_upcall_thread
  wrk1.put (new Set_Upcall_Thread (leader_follower));
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Wait for another event
  Test_LF_Event event2;
  wrk1.put (new Wait_For_Event(event2, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Synchronise with the thread before we complete its event
  synch_with_worker (wrk1);

  // Complete the first event - nothing should happen
  event.complete_event (leader_follower);

  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Complete the second event - everything should unwind
  synch_with_worker (wrk1);
  event2.complete_event (leader_follower);

  // The thread is still inside handle_events - shutdown the
  // event processing for the inner client leader
  wrk1.put (new Shutdown);

  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // The thread is now in the handle_events for the outter
  // client-leader - the event is already complete so just
  // shutdown the cmd processing.
  wrk1.put (new Shutdown);

  // We should now we back at our initial state.
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  wrk1.put (new Shutdown);
  wrk1.wait ();
}

// 7 - 2 client leader threads with set_upcall_thread
void Test_7 (TAO_ORB_Core* orb_core)
{
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #7 - Client Leader yields to another client thread\n")));

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread wait on an event
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  wrk1.put (new Wait_For_Event(event, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));
  // Wait for the firs thread to be the client leader before we start
  // the second thread
  synch_with_worker (wrk1);

  // Create another worker and have it do the same
  Worker wrk2;
  wrk2.activate ();
  wrk2.put (new TSS_Assert(orb_core, 0, 0, true));
  // Make sure this test is complete before the Set_Upcall_Thread below
  synch_with_worker (wrk2);
  Test_LF_Event event2;
  wrk2.put (new Wait_For_Event(event2, transport, leader_follower));
  // Note, we can't test the new thread here - it is block waiting on
  // the follower cond var
  // wrk2.put (new TSS_Assert(orb_core, 0, 1, true));

  // Call set_upcall_thread on the first thread
  wrk1.put (new Set_Upcall_Thread (leader_follower));
  // Our second thread should now be the client leader and the first
  // thread should not.  Note, we need to first synchronise with
  // thread 2 (to make sure it is in handle_events) to avoid race
  // conditions.
  synch_with_worker (wrk2);
  wrk2.put (new TSS_Assert(orb_core, 0, 1, true));
  wrk1.put (new TSS_Assert(orb_core, 0, 0, true));

  // We should now be able to shutdown the first thread - have it
  // return from handle_events and complete its event.  If it has to
  // wait it will just go back to being a follower
  wrk1.put (new Shutdown);
  event.complete_event (leader_follower);
  synch_with_worker (wrk1);
  wrk1.put (new TSS_Assert(orb_core, 0, 0, true));
  // We can now shut-down the thread
  wrk1.put (new Shutdown);
  wrk1.wait ();

  // Now shut-down the second thread
  event2.complete_event (leader_follower);
  wrk2.put (new Shutdown);
  synch_with_worker (wrk2);
  wrk2.put (new TSS_Assert(orb_core, 0, 0, false));
  wrk2.put (new Shutdown);
  wrk2.wait ();
}

// 8 - client becomes leader when event loop thread dispatched
void Test_8 (TAO_ORB_Core* orb_core)
{
  TAO_LF_Strategy &lf_strategy = orb_core->lf_strategy ();
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #8 - client becomes leader when event thread dispatched\n")));

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread become an event loop thread
  wrk1.put (new Event_Loop_Thread (leader_follower,
                                   lf_strategy));
  wrk1.put (new TSS_Assert(orb_core, 1, 0, true));

  // Before we start the next thread synchronise with the first
  synch_with_worker (wrk1);

  // Start another thread and have it wait on an event
  Worker wrk2;
  wrk2.activate ();
  wrk2.put (new TSS_Assert(orb_core, 0, 0, true));
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  synch_with_worker (wrk2);
  wrk2.put (new Wait_For_Event(event, transport, leader_follower));

  // The new thread is a follower and thus is waiting on the follower
  // cond var - we can't test this other than to check if the leader
  // follower has clients, however, because we can't synchronise with
  // that thread such a test would contain a race condition.

  // Now dispatch the event loop thread by having it call set_upcall_thread
  wrk1.put (new Set_Upcall_Thread (leader_follower));

  // the first worker should have given up leadership and the second
  // thread should have assumed leadership.  We have to synchronise
  // with both threads before we can test anything, otherwise we could
  // catch the window where there is no leader.
  synch_with_worker (wrk1);
  synch_with_worker (wrk2);
  wrk1.put (new TSS_Assert (orb_core, 0, 0, true));
  wrk2.put (new TSS_Assert (orb_core, 0, 1, true));

  // OK, now shut everything down - first the event loop thread
  wrk1.put (new Shutdown);
  wrk1.put (new TSS_Assert (orb_core, 0, 0, true));
  wrk1.put (new Shutdown);
  wrk1.wait ();

  // Now the client thread
  wrk2.put (new TSS_Assert (orb_core, 0, 1, true));
  synch_with_worker (wrk2);
  event.complete_event (leader_follower);
  wrk2.put (new Shutdown);
  wrk2.put (new TSS_Assert (orb_core, 0, 0, false));
  wrk2.put (new Shutdown);
  wrk2.wait ();
}

// 9 - client leader thread then event loop thread
void Test_9 (TAO_ORB_Core* orb_core)
{
  TAO_LF_Strategy &lf_strategy = orb_core->lf_strategy ();
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #9 - Client Leader thread yields to Event Loop thread\n")));

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread wait on an event
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  wrk1.put (new Wait_For_Event(event, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));
  // We now need to synchronise with the worker, to make sure it
  // has processed all the cmds we have sent it
  synch_with_worker (wrk1);

  // create an event loop thread - this means a new worker
  Worker wrk2;
  wrk2.activate ();
  wrk2.put (new Event_Loop_Thread (leader_follower,
                                   lf_strategy));

  // Unfortunately there is no way to test if the event loop thread is
  // where we expect it to be (the
  // wait_for_client_leader_to_complete() method).  The only thing we
  // could check is the event_loop_threads_waiting_ count, however,
  // that is private to the TAO_Leader_Follower class.

  // We need to get the client leader thread to return from
  // process_cmd() and allow it to surrender leadership to the waiting
  // event loop thread - send it a shutdown.  The TAO_Leader_Follower
  // code may call handle_events a few more times, however, since the
  // cmd processing is shutdown (and won't be reactivated until the
  // event is complete) handle_events will just return.
  wrk1.put (new Shutdown);

  // Now test the new event loop thread
  wrk2.put (new TSS_Assert(orb_core, 1, 0, true));
  // Wait until the event loop thread is running before we test
  // the client thread
  synch_with_worker (wrk2);

  // We can't test the client thread either - it is blocked in a call
  // to the event's cond var's wait() method.  All we can do is
  // complete the event, which will signal the cond var
  event.complete_event (leader_follower);

  // The client thread should return from wait_for_event
  wrk1.put (new TSS_Assert(orb_core, 0, 0, true));
  // And the event loop thread should still be 'running'
  wrk2.put (new TSS_Assert(orb_core, 1, 0, true));

  // Some other misc checks
  synch_with_worker (wrk1);
  TEST_ASSERT ((leader_follower.has_clients () == 0));

  // OK, lets shut everything down now - the event loop thread
  // requires two shutdown cmds, one to exit the event loop thread cmd
  // and the second to exit the main cmd processor
  wrk2.put (new Shutdown);
  // Incidently there is now no leader
  wrk2.put (new TSS_Assert(orb_core, 0, 0, false));
  wrk2.put (new Shutdown);
  wrk2.wait ();

  // Shutdown the other worker
  wrk1.put (new Shutdown);
  wrk1.wait ();
}

// 10 - ET1437460 (second problem)
void Test_10 (TAO_ORB_Core* orb_core )
{
  TAO_LF_Strategy &lf_strategy = orb_core->lf_strategy ();
  TAO_Leader_Follower &leader_follower = orb_core->leader_follower ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("==========\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TEST #10 - ET1437460\n")));

  // This scenario involves:
  //  - an event loop thread
  //  - which calls set_upcall_thread
  //  - then becomes a client leader
  //  - is dispatched and then becomes a client leader again
  //  (without calling set_upcall_thread)
  //  - calls set_upcall_thread
  //  - unwinds

  // Originally this caused the leaders_ member to get set to -1
  // (the inner client leader still decremented leaders_ even
  // though set_upcall_thread was called)

  // Activate a thread
  Worker wrk1;
  wrk1.activate ();

  // Test initial conditions
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread become an event loop thread
  wrk1.put (new Event_Loop_Thread (leader_follower,
                                   lf_strategy));

  // The thread should be an event loop thread
  wrk1.put (new TSS_Assert(orb_core, 1, 0, true));

  // call set_upcall_thread
  wrk1.put (new Set_Upcall_Thread (leader_follower));

  // The thread should no longer be an event loop thread
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Have the thread wait on an event
  Test_LF_Event event;
  Test_Transport transport (0, orb_core);
  wrk1.put (new Wait_For_Event(event, transport, leader_follower));

  // The thread is still waiting on the event and thus should
  // now be a client-leader thread
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Have the thread wait on another event
  Test_LF_Event event2;
  wrk1.put (new Wait_For_Event(event2, transport, leader_follower));

  // The thread is still waiting on the event and thus should now be a
  // client-leader thread (again)
  wrk1.put (new TSS_Assert(orb_core, 0, 2, true));

  // Call set_upcall_thread
  wrk1.put (new Set_Upcall_Thread(leader_follower));

  // We now need to synchronise with the worker, to make sure it
  // has processed all the cmds we have sent it
  synch_with_worker (wrk1);

  // Now, complete the events, and then shutdown the cmd event loop
  event.complete_event (leader_follower);
  event2.complete_event (leader_follower);
  wrk1.put (new Shutdown);

  // The inner client has returned
  wrk1.put (new TSS_Assert(orb_core, 0, 1, true));

  // Shutdown the outter client thread
  wrk1.put (new Shutdown);

  // We should be back to the initial state
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Now shutdown the event loop thread
  wrk1.put (new Shutdown);
  wrk1.put (new TSS_Assert(orb_core, 0, 0, false));

  // Shutdown the other worker
  wrk1.put (new Shutdown);
  wrk1.wait ();
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // scope TSS holder within main scope
  // so we're certain it gets destroyed before the
  // ACE object manager
  ACE_TSS<Worker> workers_;
  // provide global access
  workers_p = &workers_;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        return 1;

      // Make sure the reactor is initialised in the leader_follower
      ACE_Reactor* reactor = orb->orb_core ()->leader_follower ().reactor ();
      TEST_ASSERT ((reactor != 0));

      // Ready to go
      Test_1 (orb->orb_core ());
      Test_2 (orb->orb_core ());
      Test_3 (orb->orb_core ());
      Test_4 (orb->orb_core ());
      Test_5 (orb->orb_core ());
      Test_6 (orb->orb_core ());
      Test_7 (orb->orb_core ());
      Test_8 (orb->orb_core ());
      Test_9 (orb->orb_core ());
      Test_10 (orb->orb_core ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing complete.\n")));

      // cleanup
      orb->destroy ();
    }
  catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

// ****************************************************************
