// $Id$

#include "Thread_Task.h"
#include "ace/Atomic_Op.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> guid_index;

RTScheduling::Current::IdType*
Thread_Task::guids (void)
{
  return this->guid_;
}

int
Thread_Task::activate_task (CORBA::ORB_ptr orb)
{
  try
    {
      this->orb_ = CORBA::ORB::_duplicate (orb);

      CORBA::Object_var current_obj = this->orb_->resolve_initial_references ("RTScheduler_Current");

      this->current_ = RTScheduling::Current::_narrow (current_obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception:");
    }

  long flags = THR_NEW_LWP | THR_JOINABLE;
  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
                                            4) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Insufficient privilege to run this test.\n")),
                          -1);
    }
  active_thread_count_ = 4;

  return 0;
}

#if defined (THREAD_CANCELLED)
#undef THREAD_CANCELLED
#endif /* THREAD_CANCELLED */

int
Thread_Task::svc (void)
{
  try
    {
      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;

      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Chamber of Secrets",
                                                sched_param,
                                                implicit_sched_param);

      size_t count = 0;
      RTScheduling::Current::IdType_var id = this->current_->id ();
      ACE_OS::memcpy (&count,
                      id->get_buffer (),
                      id->length ());


      this->current_->begin_scheduling_segment ("Potter",
                                                sched_param,
                                                implicit_sched_param);

      this->guid_[guid_index++] = id.in ();

      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Harry",
                                                sched_param,
                                                implicit_sched_param);


      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        RTScheduling::Current::NameList_var name_list = this->current_->current_scheduling_segment_names ();

        ACE_DEBUG ((LM_DEBUG,
                    "Scheduling Segments for DT %d :\n",
                    count));

        for (unsigned int i = 0; i < name_list->length ();++i)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Scheduling Segment Name - %C\n",
                        (*name_list) [i].in ()));
          }
      }

      ACE_OS::sleep (10);

      this->current_->end_scheduling_segment (name);
      //  End - Nested Scheduling Segment



      this->current_->end_scheduling_segment (name);
      //  End - Nested Scheduling Segment

      this->current_->end_scheduling_segment (name);


    }
  catch (const CORBA::THREAD_CANCELLED& )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Distributable Thread Cancelled - Expected Exception\n"));
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        --active_thread_count_;
        if (active_thread_count_ == 0)
          orb_->shutdown ();
      }

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
  return 0;
}
