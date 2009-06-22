// $Id$

#include "Thread_Task.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

Thread_Task::Thread_Task (CORBA::ORB_ptr orb)
 : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Thread_Task::activate_task (int thr_count)
{

  try
    {
      ACE_NEW_RETURN (shutdown_lock_,
                      TAO_SYNCH_MUTEX,
                      -1);

      ACE_NEW_RETURN (lock_,
                      TAO_SYNCH_MUTEX,
                      -1);

      active_thread_count_ = thr_count;

      CORBA::Object_var current_obj = this->orb_->resolve_initial_references ("RTScheduler_Current");

      this->current_ = RTScheduling::Current::_narrow (current_obj.in ());

      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Making an end_scheduling_segment call without first calling begin_scheduling_segment\n"));
          this->current_->end_scheduling_segment (name);
        }
      catch (const CORBA::BAD_INV_ORDER& )
        {
          ACE_DEBUG ((LM_DEBUG,
            "End Scheduling Segment is out of context - Expected Exception\n"));
        }
      catch (const CORBA::Exception& ex)
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
      catch (const CORBA::BAD_INV_ORDER& )
        {
        ACE_DEBUG ((LM_DEBUG,
              "Update Scheduling Segment is out of context - Expected Exception\n"));
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Update Scheduling Segment is out of context:");
        }
    }
  catch (const CORBA::Exception& ex)
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
Thread_Task::svc (void)
{
  try
    {
      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;

      this->current_->begin_scheduling_segment ("Fellowship of the Rings",
                                                sched_param,
                                                implicit_sched_param);

      size_t count = 0;
      ACE_OS::memcpy (&count,
                      this->current_->id ()->get_buffer (),
                      this->current_->id ()->length ());

      ACE_DEBUG ((LM_DEBUG,
                  "Starting Distributable Thread %d with 3 nested scheduling segments....\n",
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
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *lock_, -1);
        ACE_DEBUG ((LM_DEBUG,
                    "Segment Names for DT %d :\n",
                    count));

        for (unsigned int i = 0; i < segment_name_list->length (); i ++)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "%s\n",
                        (*segment_name_list)[i].in ()));
          }
      }

      this->current_->end_scheduling_segment (name);
      //End - Nested Scheduling Segment

      this->current_->end_scheduling_segment (name);

      this->current_->end_scheduling_segment (name);
      //End - Nested Scheduling Segment

      ACE_DEBUG ((LM_DEBUG,
                  "DT %d terminated ...\n",
                  count));

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *shutdown_lock_,-1);
        --active_thread_count_;
        if (active_thread_count_ == 0)
          {
            // Without this sleep, we will occasionally get BAD_INV_ORDER
            // exceptions on fast dual processor machines.
            ACE_OS::sleep (1);

            orb_->shutdown (0);
          }
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}

Thread_Task::~Thread_Task (void)
{
  delete shutdown_lock_;
  delete lock_;
}
