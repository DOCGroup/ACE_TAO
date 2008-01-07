#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include "Controller.h"
#include <string>
#include <sstream>
#include "ace/OS_NS_unistd.h"
#include "Config_Handlers/DnC_Dump.h"

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
        std::stringstream msg;
        msg << "Successfully created the controller object.\n";
        this->logger_.log (msg.str());
      }

      Controller::~Controller ()
      {}

      bool
        Controller::init_controller ()
      {
        if (!this->initialized_)
        {
          std::stringstream msg;
          msg << "Trying to initializing the controller.\n";
          try
          {
            this->system_monitor_ =
              ::CIAO::RACE::TM_Proxy::SystemSnapshot::_duplicate (
              this->context_->get_connection_system_monitor());

            this->system_utility_ =
              ::CIAO::RACE::TM_Proxy::Utility::_duplicate (
              this->context_->get_connection_system_utils ());

            // Now we populate info regarding the initial domain.
            ::Deployment::Domain_var domain =
              this->system_utility_->getInitialDomain ();
            this->populate_domain_info (domain.in());

            this->initialized_ = true;
            msg << "Successfully initialized the controller.\n";
          }
          catch (::CORBA::Exception &ex)
          {
            msg << "Exception caught in Controller::"
                <<  "init_controller."              
                << ex._info ().c_str();

            this->initialized_ = false;
          }
          this->logger_.log (msg.str());
        }

        return this->initialized_;
      }

      bool
        Controller::start_controller ()
      {
        std::stringstream msg;
        msg << "Trying to start the controller.\n";
        if (!this->active_)
        {
          if (this->initialized_)
          {
            this->controller_.init_domain(this->domain_);

            // This method should be called only after all opstrings
            // have been registered!
            //this->controller_.init (this->tasks_);

            // Now activate the active object.
            if (this->activate () != 0)
            {
              msg << "Could not initialize the periodic task!\n";
              this->active_ = false;
            }
            msg << "Successfully started the controller.\n";
            this->active_ = true;
          }
          else
          {
            msg << "Oops! The controller can not be started until "
              << "is has been initialized!\n";
          }
        }
        else
        {
          msg << "Oops! The controller has already running!... "
            << "Nothing more to do... so bailing out\n";
        }
        this->logger_.log (msg.str());
        return this->active_;
      }

      bool
        Controller::stop_controller ()
      {
        std::stringstream msg;
        msg << "Trying to stop the controller.... ";
        this->active_ = false;
        ACE_Thread_Manager::instance ()->wait ();
        msg << "done!\n";
        this->logger_.log (msg.str());
        return true;
      }

      bool
        Controller::register_string (
        const ::CIAO::RACE::OperationalString & opstring,
        ::CORBA::String_out ID)
      {
        std::stringstream msg;
        msg << "Trying to register string... ";
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
          msg << "done!\nSuccessfully registered string with ID:"
            << opstring.ID.in () << "\n";
        }
        else
        {
          msg << "\nCan not register an opsting while the "
            << "controller is active.\n";
          msg << "First deactive the controller, and then try "
            << "registering a string.\n";
        }
        this->logger_.log (msg.str());
        return true;
      }

      int
        Controller::svc ()
      {
        while (this->active_)
        {
          try
          {
            ACE_DEBUG ((LM_DEBUG, "In controller periodic task!\nTrying to obtain the current domain...."));
            ::Deployment::Domain_var domain =
              this->system_monitor_->getSnapshot ();
            ACE_DEBUG ((LM_DEBUG, "done!\nNow parsing it..."));
            this->populate_domain_info (domain.in());
            ACE_DEBUG ((LM_DEBUG, "done!\n"));
            //this->tasks_ = this->controller_.control_period(this->domain_,this->tasks_);
            ACE_OS::sleep (this->interval_);            
          }
          catch (::CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Exception caught!\n");            
            this->active_ = false;
          }          
        }
        return 0;
      }

      bool
        Controller::populate_domain_info
        (const ::Deployment::Domain& domain)
      {
        /// First, we dump the contents of the domain structure.
        //  Deployment::DnC_Dump::dump (domain);        

        std::stringstream msg;
        msg << "Entering populate_doamin_info.\n";
        ::CIAO::RACE::Domain temp_domain;

        // First obtain the nodes in the domain.
        ::Deployment::Nodes nodes = domain.node;
        ::CIAO::RACE::Node node;

        // For each node in the system domain, populate RACE::Domain info.
        msg << "Obtaining resource info for each node in the domain.\n";
        
        for (::CORBA::ULong i = 0; i < nodes.length(); ++i)
        {
          msg << "Workin on node: " << nodes [i].name.in() << "...\n";
          node.UUID = nodes [i].name.in();
          node.label = nodes [i].label.in();

          // Obtian info regarding every resource in the domain.
          ::CIAO::RACE::Resource resource;
          ::Deployment::Resources resources = nodes [i].resource;          
          for (::CORBA::ULong j = 0; j < resources.length(); ++j)
          {
            // We care only about the processor resource.
            if (ACE_OS::strcmp (resources [j].name.in(), "Processor") == 0)
            {                

              msg << "Trying to obtain current utilization and "
                  << "utilization set-point for resource "
                  << resources [j].name.in() << "\n";
        
              resource.UUID = resources [j].name.in();

              // We need to parse the properties associated
              // with individual resources to obtain its current
              // resource utilization and utilization setpoint.

              ::Deployment::SatisfierProperties props =
                resources [j].property;
              for (::CORBA::ULong k = 0; k < props.length(); ++k)
              {
                Deployment::DnC_Dump::dump (props [k]);

                if (ACE_OS::strcmp (props [k].name.in (), "Setpoint") == 0)
                {
                  CORBA::Any value = props [k].value;
                  CORBA::TypeCode_var tc = value.type ();
                  if (tc->kind () == CORBA::tk_double)
                  {
                    value >>= resource.set_point;
                    msg << "Obtained set point! Value is: "
                        << resource.set_point << "\n";                    
                  }
                }
                else if (ACE_OS::strcmp (props [k].name.in (), "Current")
                  == 0)
                {
                  CORBA::Any value = props [k].value;
                  CORBA::TypeCode_var tc = value.type ();
                  if (tc->kind () == CORBA::tk_double)
                  {
                    value >>= resource.util;
                    msg << "Obtained curr util! Value is: "
                        << resource.util << "\n";

                  }
                }
              }
              // Now that we have populated the resource info, we add it
              // to the node.
              node.resources.push_back (resource);
              msg << "Added resource \"" << resources [j].name.in()
                  << "\" to the node structure.\n";
            }
          }
          // Now that the node structure has been fully populated, we add
          // it to the doamin. 
          temp_domain.nodes.push_back (node);          
          msg << "Added node: " << nodes [i].name.in()
              << " to the doamin structure.\n";          
        }
        this->logger_.log (msg.str());
        this->domain_ = temp_domain;
        return true;
      }
    }
  }
}

#endif /* CONTROLLER_CPP */
