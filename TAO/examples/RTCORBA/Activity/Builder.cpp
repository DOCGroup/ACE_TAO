#include "Builder.h"

#include "ace/Get_Opt.h"
#include "POA_Holder.h"
#include "Job_i.h"
#include "Thread_Task.h"
#include "Activity.h"

#include <cstddef>
#include <utility>

Builder::Builder ()
  : poa_count_ {},
    task_count_ {},
    job_count_ {}
{
  ACTIVITY::instance ()->builder (this);
}

Builder::~Builder () = default;

int
Builder::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_TCHAR const *current_arg {};

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-TaskCount"))))
        {
          task_count_ = ACE_OS::atoi (current_arg);
          if (task_count_ < 0)
            return -1;
          task_list_.reserve (static_cast<std::size_t> (task_count_));
          arg_shifter.consume_arg ();
        }
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JobCount"))))
        {
          job_count_ = ACE_OS::atoi (current_arg);
          if (job_count_ < 0)
            return -1;
          job_list_.reserve (static_cast<std::size_t> (job_count_));
          arg_shifter.consume_arg ();
        }
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-POACount"))))
        {
          poa_count_ = ACE_OS::atoi (current_arg);
          if (poa_count_ < 0)
            return -1;
          poa_list_.reserve (static_cast<std::size_t> (poa_count_));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ThreadTask")) == 0)
        {
          if (this->task_list_.size () < static_cast<std::size_t> (this->task_count_))
            {
              arg_shifter.consume_arg ();

              Periodic_Task *task_pointer {};
              ACE_NEW_RETURN (task_pointer, Thread_Task (), -1);
              std::unique_ptr<Periodic_Task> task (task_pointer);

              if (task->init_task (arg_shifter) == -1)
                return -1;

              this->task_list_.push_back (std::move (task));
            }
          else
            return -1;
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Job")) == 0)
        {
          if (this->job_list_.size () < static_cast<std::size_t> (this->job_count_))
            {
              arg_shifter.consume_arg ();

              Job_i *job_pointer {};
              ACE_NEW_RETURN (job_pointer, Job_i (), -1);
              std::unique_ptr<Job_i> job (job_pointer);

              if (job->init (arg_shifter) == -1)
                return -1;

              this->job_list_.push_back (job.release ());
            }
          else
            return -1;
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-POA")) == 0)
        {
          if (this->poa_list_.size () < static_cast<std::size_t> (this->poa_count_))
            {
              arg_shifter.consume_arg ();

              POA_Holder *poa_holder_pointer {};
              ACE_NEW_RETURN (poa_holder_pointer, POA_Holder (), -1);
              std::unique_ptr<POA_Holder> poa_holder (poa_holder_pointer);

              if (poa_holder->init (arg_shifter) == -1)
                return -1;

              this->poa_list_.push_back (std::move (poa_holder));
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
  static bool already_cleaned {};
  if (already_cleaned)
    return 0;

  this->task_list_.clear ();
  this->job_list_.clear ();
  this->poa_list_.clear ();

  already_cleaned = true;

  return 0;
}

POA_LIST const &
Builder::poa_list () const
{
  return this->poa_list_;
}

TASK_LIST const &
Builder::task_list () const
{
  return this->task_list_;
}

JOB_LIST const &
Builder::job_list () const
{
  return this->job_list_;
}


ACE_STATIC_SVC_DEFINE(Builder,
                      ACE_TEXT ("Builder"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (Builder),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (ACE_Local_Service, Builder)
