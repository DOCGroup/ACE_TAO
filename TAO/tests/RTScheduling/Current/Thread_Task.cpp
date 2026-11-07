#include "Thread_Task.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"
#include <cstring>
#include <new>

Thread_Task::Thread_Task (CORBA::ORB_ptr orb)
 : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Thread_Task::activate_task (int thr_count)
{
  try
    {
      this->shutdown_lock_.reset (new (std::nothrow) TAO_SYNCH_MUTEX);
      if (!this->shutdown_lock_)
        {
          errno = ENOMEM;
          return -1;
        }

      this->lock_.reset (new (std::nothrow) TAO_SYNCH_MUTEX);
      if (!this->lock_)
        {
          errno = ENOMEM;
          return -1;
        }

      active_thread_count_ = thr_count;

      CORBA::Object_var const current_obj =
        this->orb_->resolve_initial_references ("RTScheduler_Current");

      this->current_ = RTScheduling::Current::_narrow (current_obj.in ());

      char const * const name = nullptr;
      CORBA::Policy_ptr const sched_param = nullptr;
      CORBA::Policy_ptr const implicit_sched_param = nullptr;

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Making an end_scheduling_segment call without first calling begin_scheduling_segment\n"));
          this->current_->end_scheduling_segment (name);
        }
      catch (CORBA::BAD_INV_ORDER const &)
        {
          ACE_DEBUG ((LM_DEBUG,
            "End Scheduling Segment is out of context - Expected Exception\n"));
        }
      catch (CORBA::Exception const & ex)
        {
          ex._tao_print_exception ("\n");
        }

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Making an update_scheduling_segment call without first calling begin_scheduling_segment\n"));

          this->current_->update_scheduling_segment (name,
                                                     sched_param,
                                                     implicit_sched_param);
        }
      catch (CORBA::BAD_INV_ORDER const &)
        {
        ACE_DEBUG ((LM_DEBUG,
              "Update Scheduling Segment is out of context - Expected Exception\n"));
        }
      catch (CORBA::Exception const & ex)
        {
          ex._tao_print_exception (
            "Update Scheduling Segment is out of context:");
        }
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception:");
    }

  long flags = THR_NEW_LWP | THR_JOINABLE;
  if (this->activate (flags,
                      thr_count) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Insufficient privilege to run this test.\n")),
                          -1);
    }
  return 0;
}
int
Thread_Task::svc ()
{
  try
    {
      char const * const name = nullptr;
      CORBA::Policy_ptr const sched_param = nullptr;
      CORBA::Policy_ptr const implicit_sched_param = nullptr;

      this->current_->begin_scheduling_segment ("Fellowship of the Rings",
                                                sched_param,
                                                implicit_sched_param);

      size_t count {};
      RTScheduling::Current::IdType_var const id = this->current_->id ();
      std::memcpy (&count,
                   id->get_buffer (),
                   id->length ());

      ACE_DEBUG ((LM_DEBUG,
                  "Starting Distributable Thread %B with 3 nested scheduling segments....\n",
                  count));

      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Two Towers",
                                                sched_param,
                                                implicit_sched_param);

      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("The Return of the King",
                                                sched_param,
                                                implicit_sched_param);


      RTScheduling::Current::NameList_var segment_name_list =
        this->current_->current_scheduling_segment_names ();

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *this->lock_, -1);
        ACE_DEBUG ((LM_DEBUG,
                    "Segment Names for DT %B :\n",
                    count));

        for (CORBA::ULong i = 0; i < segment_name_list->length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "%C\n",
                        (*segment_name_list)[i].in ()));
          }
      }

      this->current_->end_scheduling_segment (name);
      //End - Nested Scheduling Segment

      this->current_->end_scheduling_segment (name);

      this->current_->end_scheduling_segment (name);
      //End - Nested Scheduling Segment

      ACE_DEBUG ((LM_DEBUG,
                  "DT %B terminated ...\n",
                  count));

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *this->shutdown_lock_, -1);
        --this->active_thread_count_;
        if (this->active_thread_count_ == 0)
          {
            // Without this sleep, we will occasionally get BAD_INV_ORDER
            // exceptions on fast dual processor machines.
            ACE_OS::sleep (1);

            this->orb_->shutdown (false);
          }
      }
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
