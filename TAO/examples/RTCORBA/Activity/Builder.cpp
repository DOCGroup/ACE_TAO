//$Id$
#include "Builder.h"

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "POA_Holder.h"
#include "Job_i.h"
#include "Thread_Task.h"
#include "Activity.h"

Builder::Builder(void)
  :poa_count_ (0),
   poa_list_ (0),
   task_count_ (0),
   task_list_ (0),
   job_count_ (0),
   job_list_ (0)
{
  ACTIVITY::instance()->builder (this);
}

Builder::~Builder(void)
{
  delete[] this->poa_list_;
  delete[] this->task_list_;
  delete[] this->job_list_;
}

int
Builder::init (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;

  int task_count = 0;
  int poa_count = 0;
  int job_count = 0;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-TaskCount")))
        {
          task_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (task_list_, Periodic_Task*[task_count_], -1);
          arg_shifter.consume_arg ();
        }
      if ((current_arg = arg_shifter.get_the_parameter ("-JobCount")))
        {
          job_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (job_list_, Job_i*[job_count_], -1);
          arg_shifter.consume_arg ();
        }
      if ((current_arg = arg_shifter.get_the_parameter ("-POACount")))
        {
          poa_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (poa_list_, POA_Holder*[poa_count_], -1);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-ThreadTask") == 0)
        {
          arg_shifter.consume_arg ();

          Periodic_Task *task;

          ACE_NEW_RETURN (task, Thread_Task (), -1);

          if (task->init_task (arg_shifter) == -1)
            return -1;

          task_list_[task_count++] = task;
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Job") == 0)
        {
          arg_shifter.consume_arg ();

          Job_i *job;

          ACE_NEW_RETURN (job, Job_i (), -1);

          if (job->init (arg_shifter) == -1)
            return -1;

          this->job_list_[job_count++] = job;
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-POA") == 0)
        {
          arg_shifter.consume_arg ();

          POA_Holder *poa_holder;

          ACE_NEW_RETURN (poa_holder, POA_Holder (), -1);

          if (poa_holder->init (arg_shifter) == -1)
            {
              delete poa_holder;
              return -1;
            }

          this->poa_list_[poa_count++] = poa_holder;
        }
        else
        {
          arg_shifter.ignore_arg ();
        }
    }

  return 0;
}

int
Builder::fini (void)
{
  return 0;
}

int
Builder::poa_list (POA_LIST& poa_list)
{
  poa_list = poa_list_;
  return poa_count_;
}

int
Builder::task_list (TASK_LIST& task_list)
{
  task_list = task_list_;
  return task_count_;
}

int
Builder::job_list (JOB_LIST& job_list)
{
  job_list = job_list_;
  return job_count_;
}

ACE_SVC_FACTORY_DEFINE (Builder)

ACE_STATIC_SVC_DEFINE(Builder,
                      ACE_TEXT ("Builder"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (Builder),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)
