// $Id$

// Stress testing tread creation using ACE_Task.

// Author: Detlef Becker <Detlef.Becker@med.siemens.de>

#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"

class TestTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
   TestTask(ACE_Thread_Manager *thrmgr = ACE_Service_Config::thr_mgr ());
   ~TestTask() {};

   int open (void * = 0);
   int svc (void);
   int close (u_long);
   int shutdownRq(void);
   int synch();
};

TestTask::TestTask(ACE_Thread_Manager *thrmgr)
: ACE_Task<ACE_MT_SYNCH> (thrmgr)
{
}

int
TestTask::open (void *)
{
   return this->activate ();
}

int
TestTask::svc (void)
{
   while (!thr_mgr_->testcancel(ACE_OS::thr_self()))
     {
       ACE_Time_Value sleep_time (0, 350000);
       ACE_OS::sleep (sleep_time);
     }
   return 0;
}

int
TestTask::close (u_long)
{
   return 0;
}

int
TestTask::shutdownRq (void)
{
   return thr_mgr_->cancel_grp(grp_id_);
}

int
TestTask::synch (void)
{
   return thr_mgr_->wait_grp(grp_id_);
}

int
main (int, char *[])
{
        const int       numTasks = 1000;
        unsigned int loopCnt = 0;
        unsigned int errCnt = 0;

  ACE_Thread_Manager *thr_mgr = ACE_Service_Config::thr_mgr ();

  TestTask      *TaskArrPtr;

  while (1)
  {
          int ii;
          TaskArrPtr = new TestTask[numTasks];

          cout << "Opening Tasks!" << loopCnt
               << " " << errCnt << endl;

          for (ii =0; ii < numTasks; ii++)
          {
                  TaskArrPtr[ii].open();
          }

          ACE_OS::sleep (1);

          cout << "Cancelling Tasks!" << loopCnt
               << " " << errCnt << endl;

          for (ii = 0; ii < numTasks; ii++)
          {
                  TaskArrPtr[ii].shutdownRq();
          }

          cout << "Synching Tasks!" << loopCnt
               << " " << errCnt << endl;

          for (ii = 0; ii < numTasks; ii++)
          {
                  if (-1 == TaskArrPtr[ii].synch())
                  {
                          cout << "Error in synch! " << loopCnt
                               << " " << errCnt << " " << ii << endl;
                          errCnt++;
                  }
          }

          cout << "thr_mgr->wait ();!" << loopCnt
               << " " << errCnt << endl;

          thr_mgr->wait ();

          delete [] TaskArrPtr;

          loopCnt++;

  }

  return 0;
}
