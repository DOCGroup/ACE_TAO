// $Id$
#ifndef POLLER_CPP
#define POLLER_CPP

#include "Poller.h"
#include "ace/Sched_Params.h"
#include "ace/OS.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {

    Poller::Poller (CIAO::TargetManagerExt_ptr TM_ptr,
                    CUTS::Path_Measurement_ptr PM_ptr,
                    CUTS::BDC_Control_Handle_ptr BDC_ptr,
                    size_t interval)
      : TM_ (CIAO::TargetManagerExt::_duplicate (TM_ptr)),
        PM_ (CUTS::Path_Measurement::_duplicate (PM_ptr)),
        BDC_ (CUTS::BDC_Control_Handle::_duplicate (BDC_ptr)),
        interval_ (interval),
        active_ (false),
        controller_log_ (ACE_OS::fopen ("controller.log", "w")),
        initialized_ (false),
        perform_control_ (true)
    {
      if (this->controller_log_ == 0)
        {
          ACE_DEBUG ((LM_ERROR, "Couldn't open log file for writing!\n"));
        }
      ACE_OS::fprintf (this->controller_log_, "%d\t%d\t%d"
                       "\t%d\t%d\n", 0, 0, 0, 0);
      ACE_OS::fflush (this->controller_log_);

    }


    void
    Poller::init ()
    {
      if (!this->initialized_)
        {
          /// Get the object reference of the Node Managers from the
          /// TargetManager.
          try
            {
              if (CIAO::debug_level () > 5)
                {
                  ACE_DEBUG ((LM_DEBUG, "RACE %N %l Trying to get "
                              "the NMs from the TM..."));
                }

              this->node_manager_seq_ = this->TM_->get_all_node_managers ();
              this->initialized_ = true;
            }
          catch (CORBA::Exception & ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in Poller::init:");
            }
          if (CIAO::debug_level () > 5)
            {
              ACE_DEBUG ((LM_DEBUG,"done!\n"));
            }
        }
    }


    int
    Poller::start_poller ()
    {
      if (!this->active_ && this->initialized_)
        {
          if (CIAO::debug_level () > 5)
            {
              ACE_DEBUG ((LM_DEBUG,"RACE %N %l "
                          "Activating the controller thread..."));
            }

          this->active_ = true;
          if (this->activate () == -1)
            {
              ACE_DEBUG ((LM_ERROR, "Oops, ERROR in activating the "
                         "controller thread!!!\n"));
              return -1;
            }
          if (CIAO::debug_level () > 5)
            {
              ACE_DEBUG ((LM_DEBUG, "done!\n"));
            }

        }
      return 0;

    }

    int
    Poller::stop_poller ()
    {
      if (this->active_)
        {
          if (CIAO::debug_level () > 5)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "RACE %N %l"
                          "De-activating the controller thread..."));
            }
          this->active_ = false;
          ACE_Thread_Manager::instance ()->wait ();

          if (CIAO::debug_level () > 5)
            {
              ACE_DEBUG ((LM_DEBUG, "done!\n"));
            }
        }
      return 0;
    }

    void
    Poller::start_controller ()
    {
      this->perform_control_ = true;
      if (CIAO::debug_level () > 5 )
        {
          ACE_DEBUG ((LM_DEBUG, "RACE %N %l Controller is now on.\n"));
        }
    }

    void
    Poller::stop_controller ()
    {
      this->perform_control_ = false;
      if (CIAO::debug_level () > 5 )
        {
          ACE_DEBUG ((LM_DEBUG, "RACE %N %l Controller is now off\n"));
        }
    }


    void
    Poller::populate_info (App_String & string)
    {
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
                        (this->node_manager_seq_[j].node_mgr_.in());
                      break;
                    }
                }
            }
        }
    }


    int
    Poller::unregister_string (const char * UUID)
    {
      if (CIAO::debug_level () > 5 )
        {
          ACE_DEBUG ((LM_DEBUG, "RACE %N %l "
                      "Unregistering string with uuid: %s ...\n",
                      UUID));
        }

      if (this->strings_.find (UUID) == 0)
        {
          App_String temp;
          this->strings_.find (UUID, temp);
          try
            {
              this->PM_->unbind_to_path (temp.cuts_id_);
              this->strings_.unbind (UUID);
              if (CIAO::debug_level () > 5 )
                {
                  ACE_DEBUG ((LM_DEBUG, "done.\n"));
                }
              return 0;
            }
          catch (CORBA::Exception & ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in "
                                   "Poller::unregister_string:");
            }
        }
      return -1;
    }


    int
    Poller::register_string (App_String & string)
    {
      if (CIAO::debug_level () > 5 )
        {
          ACE_DEBUG ((LM_DEBUG, "RACE %N %l "
                      "Deadline is: %.2f", string.deadline_));
        }
      try
        {

          string.cuts_id_ = this->PM_->bind_to_path
                            (string.critical_path_,
//                             CUTS::Path_Measurement::BPF_PROCESS);
                             CUTS::Path_Measurement::BPF_BOTH);

          this->populate_info (string);
          this->strings_.bind (string.plan_id_.in (), string);
        }
      catch (CORBA::Exception & ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Exception caught in "
                               "Poller::register_string: ");
          return -1;
        }
      return 0;
    }

    void
    Poller::perform_control (const App_String & app_string)
    {
      if (CIAO::debug_level () > 5 )
        {
          ACE_DEBUG ((LM_DEBUG, "RACE %N %l "
                      "Application string with plan id %s "
                      "is  not meeting its deadline!\n"
                      "Therefore, increasing the OS priority...",
                      app_string.plan_id_.in ()));
        }

      ::Deployment::Sched_Params params;
      params.policy_ = ACE_SCHED_FIFO;

      /// Run the NodeApplication at priority 80.
      params.priority_ = 80;

      params.scope_ = ACE_SCOPE_PROCESS;
      for (size_t i = 0; i < app_string.components_.size (); ++i)
        {
          try
            {
              app_string.components_[i].node_mgr_->set_priority
                (app_string.plan_id_.in (),
                 app_string.components_[i].id_.in (),
                 params);
              if (CIAO::debug_level () > 5 )
                {
                  ACE_DEBUG ((LM_DEBUG, "done.\n"));
                }
            }
          catch (CORBA::Exception & ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in "
                                   "Poller::perform_control:");
            }
       }
    }

    int
    Poller::svc ()
    {
      CUTS::Event_Time_Info event_info;
      CUTS::Time_Info time;
      while (this->active_)
        {
          try
            {
              if (this->strings_.current_size ())
                {
                  this->BDC_->collect_performance_data ();

                  for (Strings_Iterator itr = this->strings_.begin ();
                       itr != this->strings_.end ();
                       itr++)
                    {
                      event_info = this->PM_->execution_time
                                   ( (*itr).int_id_.cuts_id_);

                      time = event_info.time;

                      ACE_DEBUG ((LM_DEBUG, "ID %d: exec time (min/avg/max):"
                                  "(%dms/%dms/%dms)\t"
                                  "number of events (min/max): (%d/%d)\n",
                                  (*itr).int_id_.cuts_id_,
                                  time.min,
                                  time.total,
                                  time.max,
                                  event_info.min_events,
                                  event_info.max_events));
                      ACE_OS::fprintf (this->controller_log_, "%d\t%d\t%d"
                                       "\t%d\t%d\n",
                                       time.min, time.total, time.max,
                                       event_info.min_events,
                                       event_info.max_events);
                      ACE_OS::fflush (this->controller_log_);


                      if ((time.total > (*itr).int_id_.deadline_) &&
                          this->perform_control_ )
                        {
                          this->perform_control ((*itr).int_id_);
                        }
                    }
                }

            }
          catch (CORBA::Exception & ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in Poller::svc:");
              break;
            }
          ACE_OS::sleep (this->interval_);
        }
      return 0;
    }

  } /* namespace RACE */
} /* namespace CIAO */

#endif /* POLLER_CPP */
