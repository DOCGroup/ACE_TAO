#ifndef POLLER_CPP
#define POLLER_CPP

#include "Poller.h"

namespace CIAO
{
  namespace RACE
  {

    Poller::Poller (CIAO::TargetManagerExt_ptr TM_ptr,
                    ::CIAO::RACE::Execution_Time_Monitor_ptr monitor,
                    ACE_Reactor *reactor,
                    size_t interval)
      : TM_ (CIAO::TargetManagerExt::_duplicate (TM_ptr)),
        monitor_ (::CIAO::RACE::Execution_Time_Monitor::_duplicate (monitor)),
        orb_reactor_ (reactor),
        //        interval_ (interval),
        interval_ (20),
        start_time_ (110),
        //        start_time_ (20),
        active_ (false),
        initialized_ (false),
        stack_ (20),
        N (0)
    {
    }

    Poller::~Poller ()
    {}



    void
    Poller::init ()
    {
      if (!this->initialized_)
        {
          /// Get the object reference of the Node Managers from the
          /// TargetManager.
          try
            {
              ACE_DEBUG ((LM_DEBUG, "Trying to get the NMs "
                          "from the TM...."));
              this->node_manager_seq_ = this->TM_->get_all_node_managers ();
//              this->curr_host_infos_ = this->TM_->get_host_cpu ();
              /// Now we create the log files for each logical node.
//               for (CORBA::ULong ctr = 0;
//                    ctr < this->curr_host_infos_->length (); ++ctr)
//                 {
//                   this->create_log_file
//                     (this->curr_host_infos_.in () [ctr].hostname.in (),
//                      CPU);
//                 }
            }
          catch (CORBA::Exception & ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in Poller::init:");
              this->initialized_ = false;
            }
          ACE_DEBUG ((LM_DEBUG, "done\n"));
          this->initialized_ = true;
        }
    }

    int
    Poller::start_poller ()
    {
      if (!this->active_)
        {
          this->orb_reactor_->owner (ACE_Thread::self ());
          this->timer_id_ = this->orb_reactor_->schedule_timer
                            (this,
                             this,
                             this->start_time_,
                             this->interval_);
          if (this->timer_id_ < 0)
            {
              ACE_ERROR ((LM_ERROR, "[CIAO::RACE] Poller: Could not "
                          "register periodic task with the reactor!\n"));
              return -1;
            }
          ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Poller: Registered periodic "
                      "task with id %d.\n", this->timer_id_));
          this->active_ = true;
          return 0;
        }
      return 1;
    }

    int
    Poller::stop_poller ()
    {
      if (this->active_)
        {
          this->active_ = false;
          return this->orb_reactor_->cancel_timer (this->timer_id_);
        }
      return -1;
    }

    void
    Poller::set_muf_initial_priority (App_String & app_string)
    {
      app_string.priority_ =
        static_cast <int> ((10 * app_string.importance_) +
                                   (60.0 - app_string.deadline_ / 25));

//       ACE_DEBUG ((LM_DEBUG, "Setting the initial priority "
//                   "of string %s to: %d ...",
//                   app_string.label_.in (),
//                   app_string.priority_));

      if (this->set_priority (app_string) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error while modifying the OS priority "
                      "for app-string %s\n",
                      app_string.label_.in ()));
        }
      else
        {
//          ACE_DEBUG ((LM_DEBUG, "done!\n"));
        }
    }



    void
    Poller::set_dms_initial_priority (App_String & app_string)
    {
      app_string.priority_ =
        static_cast <int> (80 - (app_string.deadline_ / 20));

      ACE_DEBUG ((LM_DEBUG, "Setting the initial priority "
                  "of string %s to: %d ...",
                  app_string.label_.in (),
                  app_string.priority_));

      if (this->set_priority (app_string) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error while modifying the OS priority "
                      "for app-string %s\n",
                      app_string.label_.in ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "done!\n"));
        }
    }

    int
    Poller::set_priority (const App_String & app_string)
    {
      ::Deployment::Sched_Params params;

      params.policy_ = ACE_SCHED_FIFO;
      params.priority_ = app_string.priority_;
      params.scope_ = ACE_SCOPE_PROCESS;
      try
        {
          for (size_t i = 0; i < app_string.components_.size (); ++i)
            {
              app_string.components_[i].node_mgr_->set_priority
                (app_string.plan_id_.in (),
                 app_string.components_[i].id_.in (),
                 params);

            }
        }
      catch (CORBA::Exception & ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Exception caught in "
                               "Poller::set_priority:");
          return -1;
        }
      return 0;
    }

    int
    Poller::register_string (App_String & string)
    {
      ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Poller: entering reg string\n"));
      string.id_ =
        CORBA::string_dup (string.critical_path_[0].node.in ());
      this->populate_info (string);
      this->set_dms_initial_priority (string);
      //this->set_muf_initial_priority (string);
      this->mutex_.acquire ();
      {
        this->strings_.push_back (string);
      }
      this->mutex_.release ();

      ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Poller: leaving reg string\n"));

      return 0;
    }

    void
    Poller::populate_info (App_String & string)
    {
      ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Poller: entering populate info\n"));
      if (this->initialized_)
        {
          // @@ TODO: This is not the best way to implement this. Ideally,
          // we may want to store the NodeManager references in a hashmap
          // and look it up using its name.
          for (size_t i = 0; i < string.components_.size (); ++i)
            {
              for (CORBA::ULong j = 0;
                   j < this->node_manager_seq_->length (); ++j)
                {
                  if (ACE_OS::strcmp
                      (string.components_[i].node_.in (),
                       this->node_manager_seq_[j].host_.in ()) == 0)
                    {
                      string.components_[i].node_mgr_ =
                        CIAO::NodeManagerDaemon::_duplicate
                        (this->node_manager_seq_[j].node_mgr_);
                      break;
                    }
                }
            }
        }
      ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Poller: leaving leaving populate info\n"));
    }


    size_t
    Poller::deadline_miss_count (const Delays &delay,
                                 CORBA::Double deadline)
    {
      size_t count = 0;
      for (CORBA::ULong itr = 0; itr < delay.length (); ++itr)
        {
          if (delay [itr] > deadline)
            {
              count ++;
            }
        }
      return count;
    }

    void
    Poller::dump_delays (const Delays &delay)
    {

      for (CORBA::ULong itr = 0; itr < delay.length (); ++itr)
        {
          ACE_DEBUG ((LM_DEBUG, "%d\n", delay [itr]));
        }
    }

    int
    Poller::select_task ()
    {
      int index = -1;
      double deadline = INT_MAX;

      for (size_t i = 0; i < this->strings_.size (); ++i)
        {
          if (this->strings_ [i].importance_ == 0)
            {
              if (this->strings_ [i].deadline_ < deadline)
                {
                  deadline = this->strings_ [i].deadline_;
                  index = i;
                }
            }
        }
      return index;
    }


    void
    Poller::perform_control (double g_miss_ratio, double be_miss_ratio)
    {
      size_t index = 0;
      if (g_miss_ratio > 0)
        {
//          ACE_DEBUG ((LM_DEBUG, "DEADLINE MISS IN GUARANTEED CLASS!!!!\n"));

          /// Since we have deadline misses in the guaranteed class,
          /// demote any best effort tasks that were promoted to the
          ///  guaranteed class.
          if (!this->stack_.is_empty ())
            {
              this->stack_.pop (index);

              /// Right now, if a task has an importance value of 5, then
              /// it is scheduled in the guaranteed "priority band", and
              /// scheduled in best effort "priority band" if it has an
              /// importance value of 0. Therefore, demote any previously
              /// promoted best effort tasks by modifying their importance
              /// parameter.
//               ACE_DEBUG ((LM_DEBUG, "MOVING BE TASK %s "
//                           "BACK TO BE CLASS\n",
//                          this->strings_ [index].label_.in () ));
              this->strings_ [index].importance_ = 0;
              this->set_muf_initial_priority (this->strings_ [index]);
            }
        }
      else if (be_miss_ratio > 0)
        {
//           ACE_DEBUG ((LM_DEBUG,
//                       "DEADLINE MISS IN BEST EFFORT CLASS!!!!\n"));
          this->N++;

          if (this->N > 5)
            {
              /// Since we have deadline misses in the best effort class
              /// and no deadline misses in the guaranteed class for N
              /// conscecutive sampling periods, promote a task from the
              /// best effort class to the guaranteed class.
              int ret = this->select_task ();

              if (ret != -1)
                {
                  index = ret;
                  /// Right now, if a task has an importance value of
                  /// 5, then it is scheduled in the guaranteed
                  /// "priority band", and scheduled in best effort
                  /// priority band if it has an importance value of
                  /// 0. Therefore, promote a best effort task to the
                  /// guaranteed class by modifying its importance
                  /// parameter.
                  this->strings_ [index].importance_ = 5;
                  this->stack_.push (index);
                  this->set_muf_initial_priority (this->strings_ [index]);
//                   ACE_DEBUG ((LM_DEBUG, "MOVING BE TASK %s "
//                               "TO GUARANTEED CLASS\n",
//                              this->strings_ [index].label_.in () ));
                  /// Reset the value of N.
                  this->N = 0;
                }
            }
        }
      else
        {
          this->N = 0;
        }
    }


    int
    Poller::handle_timeout (const ACE_Time_Value &,
                            const void *)
    {
      try
        {
          size_t g_count, g_miss_count, be_count, be_miss_count;
          g_count = g_miss_count = be_count = be_miss_count = 0;
          /// Acquire the mutex.
          this->mutex_.acquire ();
          for (size_t i = 0; i < this->strings_.size (); ++i)
            {
              Delays_var delay =
                this->monitor_->get_delays (this->strings_ [i].id_.in ());


              size_t miss_count =
                this->deadline_miss_count (delay.in (),
                                           this->strings_ [i].deadline_);

              if (this->strings_ [i].group_ == GUARANTEED)
                {
                  g_count += delay->length ();
                  g_miss_count += miss_count;
                }
              else
                {
                  be_count += delay->length ();
                  be_miss_count += miss_count;
                }

              ACE_DEBUG ((LM_DEBUG, "%f  ",
                          static_cast <double> (miss_count) / delay->length ()));
            }

          double g_miss_ratio = g_miss_count / static_cast <double> (g_count);
          double be_miss_ratio = be_miss_count / static_cast <double> (be_count);

          ACE_DEBUG ((LM_DEBUG, "%f  %f\n",
                      g_miss_ratio,
                      be_miss_ratio));

          //  this->perform_control (g_miss_ratio, be_miss_ratio);
        }
      catch (CORBA::Exception & ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Exception caught in Poller::handle_timeout:");
          this->mutex_.release ();
          return 0;
        }
      this->mutex_.release ();
      return 0;

    }

  }
}
#endif /* POLLER_CPP */
