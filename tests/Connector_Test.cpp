// $Id$

// ==========================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Connector_Test.cpp
//
// = DESCRIPTION
//     This is a test of ACE_Connector framework with ACE_LSOCK_Connector.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ==========================================================================

#include "test_config.h"
#include "ace/Connector.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Event.h"
#include "ace/Event_Handler.h"
#include "ace/LSOCK_Acceptor.h"
#include "ace/LSOCK_Connector.h"
#include "ace/LSOCK_Stream.h"
#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Time_Value.h"

ACE_RCSID(tests, Connector_Test, "$Id$")

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL))


// This test is really checking to see if the correct callbacks happen and
// nothing bad happens. Results remembers what happens so it can be verified.
class Handler;
struct Results
{
  Handler *handler_;
  bool opened_;
  bool closed_;
  ACE_Event done_;

  Results () : handler_ (0), opened_ (false), closed_ (false) {}
  void wait() { done_.wait (); }
};

class Handler : public ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
{
  typedef ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH> parent;

public:
  // Need default ctor for ACE_Connector. It should never be called.
  Handler() : results_ (*new Results)
  { ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shouldn't be here...\n"))); }

  Handler (Results &r)
    : ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>(), results_ (r)
    { results_.handler_ = this; }

  virtual int open (void * = 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: Handler %@ open\n"), this));
    results_.opened_ = true;
    results_.done_.signal ();
    parent::close ();
    return 0;
  }

  virtual int close (u_long flags = 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: Handler %@ close\n"), this));
    results_.closed_ = true;
    results_.done_.signal ();
    return parent::close (flags);
  }

private:
  Results &results_;
};

class Connector : public ACE_Connector<Handler, ACE_LSOCK_CONNECTOR>
{
public:
  Connector (ACE_Reactor *reactor, Results &results)
    : ACE_Connector<Handler, ACE_LSOCK_CONNECTOR>(reactor), results_ (results)
  {}

  virtual int make_svc_handler (Handler *&sh)
  { sh = new Handler (results_); return 0; }

private:
  Results &results_;
};

// This class's constructor waits for a timeout to be handled, assumedly from
// a reactor event loop running in another thread.
class Waiter : public ACE_Event_Handler
{
public:
  Waiter (ACE_Reactor *r)
  {
    ACE_Time_Value now (0, 0);
    if (-1 == r->schedule_timer (const_cast<Waiter *>(this),
                                 0,
                                 now))
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("schedule_timer")));
    else
      done_.wait ();
  }
  ~Waiter () { done_.remove (); }

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void * = 0)
    { done_.signal (); return 0; }

private:
  ACE_Event done_;
};


// This better tests races if the reactor is already running in another
// thread before starting the connects.
static void *
run_reactor(void *r)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: Starting event loop...\n")));
  ACE_Reactor *reactor = reinterpret_cast<ACE_Reactor *>(r);
  return (void *)(reactor->run_reactor_event_loop ());
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Connector_Test"));

  ACE_Dev_Poll_Reactor dp;
  ACE_Reactor r (&dp);

  if (-1 == ACE_Thread_Manager::instance ()->spawn_n (2,
                                                      ACE_THR_FUNC (run_reactor),
                                                      (void *) &r))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("thread create failed")),
                      -1);
  Waiter w (&r);     // Wait for reactor event loop to get going

  int status = 0;
  ACE_TCHAR *addr_name = ACE_TEXT ("connector_test");
  ACE_OS::unlink (addr_name);   // Clean up from possible previous test
  ACE_UNIX_Addr addr (addr_name);
  // Use the reactor and also a 0 timeout.
  ACE_Synch_Options sync (ACE_Synch_Options::USE_REACTOR |
                          ACE_Synch_Options::USE_TIMEOUT  );

  Handler *h = 0;

  {
    Results results;
    Connector c (&r, results);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: Trying without a listener...\n")));
    c.connect (h, addr, sync);
    results.wait ();
    if (results.opened_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Socket opened but shouldn't\n")));
        status = 1;
        h->destroy ();
      }
    if (!results.closed_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Socket not closed but should\n")));
        status = 1;
      }
  }

  {
    // Now try one that works...
    Results results;
    Connector c (&r, results);

    ACE_OS::unlink (addr_name);   // Clean up from first test
    ACE_LSOCK_Acceptor a (addr);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: Trying with a listener...\n")));
    h = 0;
    c.connect (h, addr, sync);
    results.wait ();
    if (!results.opened_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Socket not opened but should\n")));
        status = 1;
      }
    if (results.closed_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Socket closed but shouldn't\n")));
        status = 1;
      }
  }

  r.end_reactor_event_loop ();
  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return status;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Connector_Test"));
  ACE_DEBUG ((LM_INFO,
	      ACE_TEXT ("Connector_Test requires threads and dev/[e]poll.")));
  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS && (ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL) */
