#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include "Controller.h"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      Controller::Controller (
        ::CIAO::RACE::Controller::CCM_EUCON_Controller_Context_ptr
        context,
        ::CORBA::Long interval)
        : context_ (CCM_EUCON_Controller_Context::_duplicate (context)),
        interval_ (interval),
        active_ (false),
        initialized_ (false),
        logger_ ("EUCON_Controller.log")
      {
      }

      Controller::~Controller ()
      {}

      bool
        Controller::init_controller ()
      {
        if (!this->initialized_)
        {
          std::string msg = "Trying to initializing the controller.\n";                     
          try
          {
            this->system_monitor_ = 
              ::CIAO::RACE::TM_Proxy::SystemSnapshot::_duplicate (
              this->context_->get_connection_system_monitor());

            // @@ NOTE: We need to initialize the domain by obtaining the system domain info from the 
            // Target Manager before we return.

            this->initialized_ = true;
            msg += "Successfully initialized the controller.\n";                          
          }
          catch (::CORBA::Exception &ex)
          {
            msg += "Exception caught in Controller::"
              "init_controller.\nUnable to get the system_monitor object reference!";
            msg += ex._info ().c_str();            
            this->initialized_ = false;
          }
          this->logger_.log (msg);
        }

        return this->initialized_;
      }

      bool
        Controller::start_controller ()
      {
        std::string msg = "Trying to start the controller.\n";
        if (!this->active_)
        {
          if (this->initialized_)
          {
            this->controller_.init_domain(this->domain_);

            // This method should be called only after all opstrings have been registered!
            this->controller_.init (this->tasks_);
            if (this->activate () != 0)
            {
              msg += "Could not initialize the periodic task!\n";
              this->active_ = false;              
            }
            msg += "Successfully started the controller.\n";              
            this->active_ = true;
          }
          else
          {
            msg += "Oops! The controller can not be started until is has been initialized!\n";
          }
        }
        else
        {
          msg += "Oops! The controller has already running!... Nothing more to do... so bailing out\n";
        }
        this->logger_.log (msg);
        return this->active_;
      }

      bool
        Controller::stop_controller ()
      {
        std::string msg = "Trying to stop the controller.\n";            
        this->active_ = false;
        ACE_Thread_Manager::instance ()->wait ();
        msg += "Successfully stoped the controller.\n";              
        this->logger_.log (msg);
        return true;
      }



      bool
        Controller::register_string (
        const ::CIAO::RACE::OperationalString & opstring,
        ::CORBA::String_out ID)
      {
        std::string msg = "Trying to register string\n";
        if (!this->active_)
        {
          ID = ::CORBA::string_dup (opstring.ID.in ());

          // Now creating a RACE::Task and populating its fields.
          ::CIAO::RACE::Task task;
          task.max_rate = opstring.rate.maxRate;
          task.min_rate = opstring.rate.minRate;
          task.curr_rate = opstring.rate.currRate;

          ::CIAO::RACE::Execution_Time time;
          ::CIAO::RACE::Subtask subtask;

          for (::CORBA::ULong i = 0; i < opstring.instance.length(); ++i)
          {
            time.BCET.sec = opstring.instance[i].executionTime.earliest.sec;
            time.WCET.sec = opstring.instance[i].executionTime.latest.sec;
            subtask.exec_time = time;
            subtask.UUID = opstring.instance[i].ID.in();
            subtask.label = opstring.instance[i].name.in();
            subtask.node = opstring.instance[i].nodeRef;
            task.subtasks.push_back (subtask);        
          }

          this->tasks_.push_back (task);
          msg += "Successfully registered string with ID:";
          msg += opstring.ID.in ();
          msg += "\n";
        }
        else
        {
          msg += "Can not register an opsting while the controller is active.\n";
          msg += "First deactive the controller, and then try registering a string.\n";
        }
        this->logger_.log (msg);
        return true;
      }

      int
        Controller::svc ()
      {
        while (this->active_)
        {
          // @@ Before the invoke the controller, we need to obtain the current system snapshot.
          this->tasks_ = 
            this->controller_.control_period(this->domain_,this->tasks_);
          //ACE_OS::sleep (this->interval_);
        }
        return 0;
      }

    }
  }
}

#endif /* CONTROLLER_CPP */
