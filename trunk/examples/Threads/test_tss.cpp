// Torture test ACE thread-specific storage...
// @(#)test_tss.cpp	1.1	10/18/96


#include "ace/Task.h"
#include "ace/Token.h"

#if defined (ACE_HAS_THREADS)

class TSS_Obj
{
public:

  TSS_Obj (void);
  ~TSS_Obj (void);

private:
  static int        count_;
  static ACE_Thread_Mutex  lock_;
};

int TSS_Obj::count_ = 0;
ACE_Thread_Mutex TSS_Obj::lock_;

TSS_Obj::TSS_Obj (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, lock_);

  count_++;
  cout << "TO+ : " << count_ << endl;
}

TSS_Obj::~TSS_Obj (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, lock_);

  count_--;
  cout << "TO- : " << count_ << endl;
}

class Test_Task
{
public:

  Test_Task (void);
  ~Test_Task (void);

  int open (void *arg);

  static void* svc (void *arg);

  static int wait_count_;
  static int max_count_;

private:
  static int count_;
};

int Test_Task::count_ = 0;
int Test_Task::wait_count_ = 0;
int Test_Task::max_count_ = 0;
int num_threads_ = 0;

ACE_Token token;

Test_Task::Test_Task (void)
{
  ACE_GUARD (ACE_Token, ace_mon, token);

  count_++;
  cout << "Test_Task+ : " 
       << count_ << " (" 
       << ACE_OS::thr_self () 
       << ")" << endl;
}

Test_Task::~Test_Task (void)
{
  ACE_GUARD (ACE_Token, ace_mon, token);

  count_--;
  cout << "Test_Task- : " 
       << count_ << " (" 
       << ACE_OS::thr_self () 
       << ")" << endl;

  wait_count_--;
}

int Test_Task::open (void *arg)
{

  ACE_Thread::spawn (Test_Task::svc, arg);

  return 0;
}


void * 
Test_Task::svc (void *arg)
{
  ACE_TSS<TSS_Obj> tss (new TSS_Obj);

  {
    ACE_GUARD_RETURN (ACE_Token, ace_mon, token, 0);

    wait_count_++;
    max_count_++;
    cout << "svc: waiting (" << ACE_OS::thr_self () << ")" << endl;
  }

  while (1)
    {   
      {
	ACE_GUARD_RETURN (ACE_Token, ace_mon, token, 0);

	if (max_count_ >= num_threads_)
	  break;
	else
	  {
	    ace_mon.release ();
	    ACE_Thread::yield ();
	    ace_mon.acquire ();
	  }
      }

      {
	ACE_GUARD_RETURN (ACE_Token, ace_mon, token, 0);

	cout << "svc: waiting (" << ACE_OS::thr_self () << ") finished" << endl;
      }
    }

  delete (Test_Task *) arg;

  return 0;
}

int 
main (int argc, char **argv)
{
  if (argc != 2)
    {
      cout << "Missing parameters!" << endl;
      return 1;
    }

  int num_Tasks = atoi (argv[1]);

  num_threads_ = num_Tasks;

  Test_Task **task_arr = (Test_Task**) new char[sizeof (Test_Task*) * num_Tasks];

  while (1)
    {
      {
	ACE_GUARD_RETURN (ACE_Token, ace_mon, token, -1);

	cout << "ReseTest_Tasking Test_Task::max_count_ from: " 
	     << Test_Task::max_count_ << endl;

	Test_Task::max_count_ = 0;
      }

      for (int i = 0; i < num_Tasks; i++)
	{
          task_arr[i] = new Test_Task;
	  task_arr[i]->open (task_arr[i]);
	}

      cout << "Waiting for first thread started..." << endl;

      for (;;)
	{
	  ACE_GUARD_RETURN (ACE_Token, ace_mon, token, -1);

	  if (Test_Task::max_count_ != 0 )
	    {
	      ace_mon.release ();
	      ACE_Thread::yield ();
	      ace_mon.acquire ();
	      break;
	    }
	  ace_mon.release ();
	  ACE_Thread::yield ();
	  ace_mon.acquire ();
	}

      {
	ACE_GUARD_RETURN (ACE_Token, ace_mon, token, -1);

	cout << "First thread started!" << endl 
	     << "Waiting for all threads finished..." << endl;
      }

      for (;;)
	{
	  ACE_GUARD_RETURN (ACE_Token, ace_mon, token, -1);

	  if (!(Test_Task::max_count_ == num_threads_ 
		&& Test_Task::wait_count_ == 0))
	    {
	      ace_mon.release ();
	      ACE_Thread::yield ();
	      ace_mon.acquire ();
	      continue;
	    }

	  cout << "Test_Task::max_count_ = " 
	       << Test_Task::max_count_ 
	       << " Test_Task::wait_count_ = " 
	       << Test_Task::wait_count_ 
	       << endl;
	  break;
	}

      {
	ACE_GUARD_RETURN (ACE_Token, ace_mon, token, -1);
	cout << "All threads finished..." << endl;
      }

      ACE_OS::sleep (2);
    }

  return 0;
}

#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
