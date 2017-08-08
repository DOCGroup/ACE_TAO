#include "Builder.h"

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
}

int
Builder::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;

  int task_count = 0;
  int poa_count = 0;
  int job_count = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-TaskCount"))))
        {
          task_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (task_list_, Periodic_Task*[task_count_], -1);
          ACE_OS::memset (this->task_list_,
                          0,
                          this->task_count_ * sizeof (this->task_list_[0]));
          arg_shifter.consume_arg ();
        }
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JobCount"))))
        {
          job_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (job_list_, Job_i*[job_count_], -1);
          ACE_OS::memset (this->job_list_,
                          0,
                          this->job_count_ * sizeof (this->job_list_[0]));
          arg_shifter.consume_arg ();
        }
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-POACount"))))
        {
          poa_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (poa_list_, POA_Holder*[poa_count_], -1);
          ACE_OS::memset (this->poa_list_,
                          0,
                          this->poa_count_ * sizeof (this->poa_list_[0]));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ThreadTask")) == 0)
        {
          if (task_count < this->task_count_)
            {
              arg_shifter.consume_arg ();

              Periodic_Task *task = 0;

              ACE_NEW_RETURN (task, Thread_Task (), -1);

              if (task->init_task (arg_shifter) == -1)
                return -1;

              task_list_[task_count++] = task;
            }
          else
            return -1;
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Job")) == 0)
        {
          if (job_count < this->job_count_)
            {
              arg_shifter.consume_arg ();

              Job_i *job;

              ACE_NEW_RETURN (job, Job_i (), -1);

              if (job->init (arg_shifter) == -1)
                return -1;

              this->job_list_[job_count++] = job;
            }
          else
            return -1;
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-POA")) == 0)
        {
          if (poa_count < this->poa_count_)
            {
              arg_shifter.consume_arg ();

              POA_Holder *poa_holder = 0;

              ACE_NEW_RETURN (poa_holder, POA_Holder (), -1);

              if (poa_holder->init (arg_shifter) == -1)
                {
                  delete poa_holder;
                  return -1;
                }

              this->poa_list_[poa_count++] = poa_holder;
            }
          else
            return -1;
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
  // It's only a hack for proper cleanup of this badly designed test.
  static bool already_cleaned = false;
  if (already_cleaned)
    return 0;

  int count;

  for (count = 0; count < this->task_count_; ++count)
    delete this->task_list_[count];
  delete [] this->task_list_;

  delete [] this->job_list_;

  for (count = 0; count < this->poa_count_; ++count)
    delete this->poa_list_[count];
  delete [] this->poa_list_;

  already_cleaned = true;

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


ACE_STATIC_SVC_DEFINE(Builder,
                      ACE_TEXT ("Builder"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (Builder),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (ACE_Local_Service, Builder)
