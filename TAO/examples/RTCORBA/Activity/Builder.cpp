#include "Builder.h"

#include "ace/Get_Opt.h"
#include "POA_Holder.h"
#include "Job_i.h"
#include "Thread_Task.h"
#include "Activity.h"

#include <new>
#include <utility>

Builder::Builder()
  :poa_count_ (0),
   task_count_ (0),
   job_count_ (0)
{
  ACTIVITY::instance()->builder (this);
}

int
Builder::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_TCHAR const *current_arg = nullptr;

  int task_count = 0;
  int poa_count = 0;
  int job_count = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-TaskCount"))))
        {
          task_count_ = ACE_OS::atoi (current_arg);
          std::unique_ptr<Periodic_Task *[]> task_list (
            new (std::nothrow) Periodic_Task *[task_count_] {});
          if (!task_list)
            return -1;
          task_list_ = std::move (task_list);
          arg_shifter.consume_arg ();
        }
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JobCount"))))
        {
          job_count_ = ACE_OS::atoi (current_arg);
          std::unique_ptr<Job_i *[]> job_list (
            new (std::nothrow) Job_i *[job_count_] {});
          if (!job_list)
            return -1;
          job_list_ = std::move (job_list);
          arg_shifter.consume_arg ();
        }
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-POACount"))))
        {
          poa_count_ = ACE_OS::atoi (current_arg);
          std::unique_ptr<POA_Holder *[]> poa_list (
            new (std::nothrow) POA_Holder *[poa_count_] {});
          if (!poa_list)
            return -1;
          poa_list_ = std::move (poa_list);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ThreadTask")) == 0)
        {
          if (task_count < this->task_count_)
            {
              arg_shifter.consume_arg ();

              Periodic_Task *task_pointer = nullptr;
              ACE_NEW_RETURN (task_pointer, Thread_Task (), -1);
              std::unique_ptr<Periodic_Task> task (task_pointer);

              if (task->init_task (arg_shifter) == -1)
                return -1;

              task_list_[task_count++] = task.release ();
            }
          else
            return -1;
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Job")) == 0)
        {
          if (job_count < this->job_count_)
            {
              arg_shifter.consume_arg ();

              Job_i *job_pointer = nullptr;
              ACE_NEW_RETURN (job_pointer, Job_i (), -1);
              std::unique_ptr<Job_i> job (job_pointer);

              if (job->init (arg_shifter) == -1)
                return -1;

              this->job_list_[job_count++] = job.release ();
            }
          else
            return -1;
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-POA")) == 0)
        {
          if (poa_count < this->poa_count_)
            {
              arg_shifter.consume_arg ();

              POA_Holder *poa_holder_pointer = nullptr;
              ACE_NEW_RETURN (poa_holder_pointer, POA_Holder (), -1);
              std::unique_ptr<POA_Holder> poa_holder (poa_holder_pointer);

              if (poa_holder->init (arg_shifter) == -1)
                return -1;

              this->poa_list_[poa_count++] = poa_holder.release ();
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
Builder::fini ()
{
  // It's only a hack for proper cleanup of this badly designed test.
  static bool already_cleaned = false;
  if (already_cleaned)
    return 0;

  int count;

  for (count = 0; count < this->task_count_; ++count)
    delete this->task_list_[count];
  this->task_list_.reset ();

  this->job_list_.reset ();

  for (count = 0; count < this->poa_count_; ++count)
    delete this->poa_list_[count];
  this->poa_list_.reset ();

  already_cleaned = true;

  return 0;
}

int
Builder::poa_list (POA_LIST& poa_list) const
{
  poa_list = poa_list_.get ();
  return poa_count_;
}

int
Builder::task_list (TASK_LIST& task_list) const
{
  task_list = task_list_.get ();
  return task_count_;
}

int
Builder::job_list (JOB_LIST& job_list) const
{
  job_list = job_list_.get ();
  return job_count_;
}


ACE_STATIC_SVC_DEFINE(Builder,
                      ACE_TEXT ("Builder"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (Builder),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (ACE_Local_Service, Builder)
