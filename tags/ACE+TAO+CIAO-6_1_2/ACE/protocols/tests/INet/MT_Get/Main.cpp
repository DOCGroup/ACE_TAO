// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Containers_T.h"
#include "ace/Reactor.h"
#include "ace/INet/HTTP_URL.h"
#include "ace/INet/HTTP_ClientRequestHandler.h"
#include "ace/INet/String_IOStream.h"
#include "ace/Truncate.h"
#include <iostream>

int n_threads =  3;

#if defined (ACE_HAS_THREADS)
class Get_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Get_Task (ACE_Thread_Manager *thr_mgr,
            int n_threads);

  virtual int svc (void);

  const ACE_Array<ACE_CString>& results ();

private:
  void shutdown ();

  int n_threads_;
  ACE_Array<ACE_CString>  results_;
  ACE_SYNCH_MUTEX lock_;
};

Get_Task::Get_Task (ACE_Thread_Manager *thr_mgr,
                    int n_threads)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr),
    n_threads_ (n_threads)
{
  // Create worker threads.
  if (this->activate (THR_NEW_LWP, n_threads_) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("activate failed")));
  else
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) started %d threads\n"), n_threads_));
}

void Get_Task::shutdown ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             guard_,
             lock_);

  --n_threads_;
  if (n_threads_ <= 0)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) ending event loop\n")));
      ACE_Reactor::instance ()->end_event_loop ();
    }
}

int Get_Task::svc ()
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) task started\n")));

  ACE::HTTP::URL http_url;
  if (http_url.parse ("http://www.theaceorb.nl"))
    {
      ACE::HTTP::ClientRequestHandler rh;
      ACE::INet::URLStream urlin = http_url.open (rh);
      if (urlin)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) opened URL %C\n"), http_url.to_string ().c_str ()));

          ACE::IOS::CString_OStream sos;
          sos << urlin->rdbuf ();
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) result downloaded\n")));
          if (!sos.bad ())
            {
              ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                guard_,
                                lock_,
                                0);
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) storing result\n")));
              results_.size (results_.size () + 1);
              results_[results_.size ()-1] = sos.str ();
            }
        }
      else
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) failed to open URL %C\n"), http_url.to_string ().c_str ()));
      shutdown ();
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) failed to parse URL : result = %C\n"),
                            http_url.to_string ().c_str ()));
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) task finished\n")));

  return 0;
}

const ACE_Array<ACE_CString>& Get_Task::results ()
{
  return results_;
}

class Get_MultiTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Get_MultiTask (ACE_Thread_Manager *thr_mgr);

  virtual int svc (void);

  int failures ();

private:
  const char* get_url ();

  void wait_for_all ();

  void shutdown ();

  int n_threads_;
  int n_thread_starts_;
  int n_open_urls_;
  int n_fails_;
  ACE_SYNCH_MUTEX lock_;
  ACE_SYNCH_CONDITION signal_;
};

const char* urls_[] = {
  "http://www.theaceorb.nl",
  "http://www.remedy.nl",
  "http://www.google.com"
};

Get_MultiTask::Get_MultiTask (ACE_Thread_Manager *thr_mgr)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr),
    n_threads_ (sizeof (urls_)/sizeof (const char*)),
    n_thread_starts_ (0),
    n_open_urls_ (0),
    n_fails_ (n_threads_),
    signal_ (lock_)
{
  // Create worker threads.
  if (this->activate (THR_NEW_LWP, n_threads_) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("activate failed")));
}

const char* Get_MultiTask::get_url ()
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    guard_,
                    lock_,
                    0);

  const char* url = urls_[n_thread_starts_];
  ++n_thread_starts_;
  return url;
}

void Get_MultiTask::wait_for_all ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             guard_,
             lock_);

  ++n_open_urls_;

  if (n_open_urls_ == n_threads_)
    signal_.broadcast (); // signal all threads that all urls are opened
  else
    signal_.wait (); // wait for all urls to be opened
}

void Get_MultiTask::shutdown ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             guard_,
             lock_);

  --n_threads_;
  if (n_threads_ <= 0)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) ending event loop\n")));
      ACE_Reactor::instance ()->end_event_loop ();
    }
}

int Get_MultiTask::svc ()
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) multitask started\n")));

  ACE::HTTP::URL http_url;
  const char* url = get_url ();

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) parsing URL %C\n"), url));

  if (http_url.parse (url))
    {
      ACE::HTTP::ClientRequestHandler rh;
      http_url.open (rh);
      if (rh.response_stream ())
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) opened URL %C\n"), http_url.to_string ().c_str ()));
      else
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) failed to open URL %C\n"), http_url.to_string ().c_str ()));
      wait_for_all ();
      if (rh.response_stream ())
        {
          ACE::IOS::CString_OStream sos;
          sos << rh.response_stream ().rdbuf ();
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) result downloaded\n")));
          if (!sos.bad ())
            {
              ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                guard_,
                                lock_,
                                0);
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) marking result\n")));
              --n_fails_;
            }
        }
      shutdown ();
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) task finished\n")));

  return 0;
}

int Get_MultiTask::failures ()
{
  return n_fails_;
}

#endif

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) MGet_Test started\n")));

  ACE_Reactor* reactor = ACE_Reactor::instance ();

#if 1
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) starting serialized test\n")));

  Get_Task get_task (ACE_Thread_Manager::instance (),
                     n_threads);

  // run event loop until ended by last task thread
  reactor->run_event_loop ();

  // all threads really ended?
  ACE_Thread_Manager::instance ()->wait ();

  // All threads should have resulted in the same data...
  if (get_task.results ().size () != ACE_Utils::truncate_cast<size_t> (n_threads))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%d results found; ")
                  ACE_TEXT ("should be %d\n"),
                  get_task.results ().size (),
                  n_threads));
    }
  else
    {
      const ACE_Array<ACE_CString>& results = get_task.results ();
      for (int i=1;
           i < n_threads;
           ++i)
        {
          if (results[0] != results[i])
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("result %d does not match other results\n"),
                          i));
            }
        }
    }
#endif

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) starting parallelized test\n")));

  reactor->reset_event_loop ();

  Get_MultiTask get_multi_task (ACE_Thread_Manager::instance ());

  // run event loop until ended by last task thread
  reactor->run_event_loop ();

  // all threads really ended?
  ACE_Thread_Manager::instance ()->wait ();

  // All threads should have resulted in data...
  if (get_multi_task.failures () > 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%d failures found"),
                  get_multi_task.failures ()));
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) MGet_Test finished\n")));
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  return 0;
}
