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
          logger_ ("EUCON_Controller.log"),
          util_logger_("util.log"),
          rate_logger_("rate.log")
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
            this->logger_.log (msg.str());
            msg.str("");

            try
              {
                this->system_monitor_ =
                  ::CIAO::RACE::TM_Proxy::SystemSnapshot::_duplicate (
                    this->context_->get_connection_system_monitor());

                this->system_utility_ =
                  ::CIAO::RACE::TM_Proxy::Utility::_duplicate (
                    this->context_->get_connection_system_utils ());

                this->appActuator_ =
                  ::CIAO::RACE::Effector::ApplicationActuator::_duplicate (
                    this->context_->get_connection_appActuator());

                // Now we populate info regarding the initial domain.
                ::Deployment::Domain_var domain =
                    this->system_utility_->getInitialDomain ();
                this->populate_domain_info (domain.in());

                // Now activate the active object.
                if (this->activate () != 0)
                  {
                    msg << "Could not initialize the periodic task!\n";
                    this->initialized_ = false;
                  }
                else
                  {
                    this->initialized_ = true;
                    msg << "Successfully initialized the controller.\n";
                  }
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
                this->controller_.init (this->tasks_);

                // Now trying to initialize the effector.
                msg << "Trying to initialize the effector....";
                try
                  {
                    if (this->appActuator_->init (this->opstrings_))
                      {
                        msg << "done!\n";
                        // Now activate the controller.
                        this->active_ = true;
                        msg << "Successfully started the controller.\n";
                      }
                    else
                      {
                        msg << "Oops! Error "
                            << "while initializing the effector!\n"
                            << "Controller can not be started without the "
                            << "effector.... bailing out....\n";
                        this->active_ = false;
                      }
                  }
                catch (CORBA::Exception &ex)
                  {
                    msg << "Exception caught\n" << ex._info ().c_str();
                    this->active_ = false;
                  }
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

        //        ACE_Thread_Manager::instance ()->wait ();
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
        msg << "Entering register_string ()\n";
        this->logger_.log (msg.str());
        msg.str ("");
        if (this->initialized_)
          {
            if (!this->active_)
              {
                // Now creating a RACE::Task and populating its fields.
                ::CIAO::RACE::Task task;
                this->populate_task (opstring, task);
                this->tasks_.push_back (task);
                this->dump_task (task);
                msg << "\nSuccessfully registered string with ID:"
                    << opstring.ID.in () << "\n";
                ID = CORBA::string_dup(opstring.ID.in ());
                this->logger_.log (msg.str());

                // Add this string to the internal opstrings_ sequence as
                // well.
                CORBA::ULong cur_len = this->opstrings_.length ();
                this->opstrings_.length (cur_len + 1);
                this->opstrings_[cur_len] = opstring;

                return true;
              }
            else
              {
                msg << "\nCan not register an opsting while the "
                    << "controller is active.\n";
                msg << "First deactive the controller, and then try "
                    << "registering a string.\n";
                this->logger_.log (msg.str());
                return false;
              }
          }
        else
          {
            msg << "\nCan not register an opsting while the "
                << "controller has not initialized!\n";
            msg << "First initialize the controller, and then try "
                << "registering a string.\n";
            this->logger_.log (msg.str());
            return false;
          }
      }

      ::CORBA::Boolean
      Controller::unregister_string (const char * ID)
      {

        std::stringstream msg;
        msg << "Entering unregister_string ()\n";
        this->logger_.log (msg.str());
        msg.str ();
        if (!this->active_)
          {
            // First try to remove the task corresponding to this ID from
            // the internal task_ vector.


            // We use this vaiable to locate the position of the task in
            // the internal opstring_ CORBA::Sequence.
            int pos = -1;
            int i = 0;
            for (std::vector<::CIAO::RACE::Task>::iterator
                   itr = this->tasks_.begin ();
                 itr != this->tasks_.end ();
                 itr ++, ++i)
              {
                if (ACE_OS::strcmp ((*itr).UUID.c_str (), ID) == 0)
                  {
                    pos = i;
                    msg << "Found ID "
                        << ID
                        << "within internal task vector!\n"
                        << "Now removing it....";
                    // Now remove the element.
                    this->tasks_.erase (itr);
                    msg << "done!\n";
                    break;
                  }
              }
            this->logger_.log (msg.str());
            msg.str ();
            // Now that we have removed the task from the task_ vector, we
            // need to remove it from the internal opstring_ sequence as
            // well.

            // If the position is != -1, then we have found the task
            // position.
            if (pos >= 0)
              {
                msg << "Removing task from internal opstring sequence...";

                for (::CORBA::ULong i = pos;
                     i < this->opstrings_.length () - 1;
                     ++i)
                  {
                    this->opstrings_ [i] = this->opstrings_ [i + 1];
                  }
                this->opstrings_.length (this->opstrings_.length () - 1);
                msg << "done!\n";
                this->logger_.log (msg.str());
                return true;
              }
            else
              {
                msg << "Given ID "
                    << ID
                    << "is not registered with the controller!\n";
                this->logger_.log (msg.str());
                return false;
              }
          }
        else
          {
            msg << "\nCan not unregister an opsting while the "
                << "controller is active.\n";
            msg << "First deactive the controller, and then try "
                << "unregistering a string.\n";
            this->logger_.log (msg.str());
            return false;
          }
      }


      void
      Controller::control_action ()
      {

        // Invoke the MPC controller.
        this->tasks_ =
          this->controller_.control_period
          (this->domain_,this->tasks_);

        // Now we update our internal opstrings
        // sequence and dump out the new rates.
        std::stringstream msg, rates;
        for (size_t itr = 0;
             (itr < this->tasks_.size() &&
              itr < this->opstrings_.length());
             ++itr)
          {
            msg << "Delta rate for task: "
                << this->tasks_[itr].UUID.c_str()
                << " is: "
                << this->tasks_[itr].delta_rate
                << "\n";

            // Update the rates.
            this->tasks_[itr].curr_rate +=
              this->tasks_[itr].delta_rate;

            this->opstrings_[itr].rate.currRate =
              this->tasks_[itr].curr_rate;

            // Dump the rates to a file.
            rates << this->tasks_[itr].curr_rate
                  << "\t"
                  << this->tasks_[itr].delta_rate
                  << "\t";
          }
        rates << "\n";
        this->rate_logger_.log (rates.str());
        ACE_DEBUG ((LM_DEBUG, "%s", msg.str ().c_str ()));

        // Invoke the application effector to modify the
        // application execution rate.
        if (!this->appActuator_->modifyApplications
            (this->opstrings_))
          {
            msg << "Error while invoking effector!\n"
                << "Bailing out.....\n";
          }
        this->logger_.log (msg.str ());

      }


      int
      Controller::svc ()
      {
        while (this->initialized_)
          {
            std::stringstream msg;
            std::stringstream rates;
            try
              {
                msg << "In controller periodic task!\n"
                    << "Trying to obtain the current domain...";
                ::Deployment::Domain_var domain =
                    this->system_monitor_->getSnapshot ();
                msg << "done!\nNow parsing it...";
                this->logger_.log (msg.str());
                msg.str("");
                this->populate_domain_info (domain.in());
                msg <<  "Parsing complete!\n";
                this->logger_.log (msg.str ());
                // We perform the control action only if the controller is
                // active.
                if (this->active_)
                  {
                    this->control_action ();
                  }
                ACE_OS::sleep (this->interval_);
              }
            catch (::CORBA::Exception &ex)
              {
                ACE_PRINT_EXCEPTION (ex, "Exception caught!\n");
                return -1;
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
        this->logger_.log (msg.str ());
        msg.str ("");

        for (::CORBA::ULong i = 0; i < nodes.length(); ++i)
          {
            ::CIAO::RACE::Node r_node = this->populate_node (nodes[i]);
            // Now that the node structure has been fully populated, we add
            // it to the doamin.
            temp_domain.nodes.push_back (r_node);
            msg << "Added node: " << nodes [i].name.in()
                << " to the doamin structure.\n";
            this->logger_.log (msg.str ());
            msg.str ("");
          }
        this->util_logger_.log (std::string ("\n"));
        this->dump_domain (temp_domain);
        this->domain_ = temp_domain;
        return true;
      }

      ::CIAO::RACE::Node
      Controller::populate_node (const ::Deployment::Node &d_node)
      {
        std::stringstream msg;
        std::stringstream util;
        ::CIAO::RACE::Node r_node;

        // Set formatting.
        util.setf (ios::fixed, ios::floatfield);
        util.precision (5);

        msg << "Entering populate_node ()\n";
        msg << "Workin on node: " << d_node.name.in() << "...\n";
        r_node.UUID = d_node.name.in();
        r_node.label = d_node.label.in();
        // Obtian info regarding every resource in the domain.
        ::CIAO::RACE::Resource resource;
        ::Deployment::Resources resources = d_node.resource;
        for (::CORBA::ULong j = 0; j < resources.length(); ++j)
          {
            // We care only about the processor resource.
            if (ACE_OS::strcmp (resources [j].name.in(), "Processor") == 0)
              {

                msg << "Trying to obtain current utilization and "
                    << "utilization set-point for resource "
                    << resources [j].name.in() << "\n";

                resource.label = resources [j].name.in();

                // We need to parse the properties associated
                // with individual resources to obtain its current
                // resource utilization and utilization setpoint.

                ::Deployment::SatisfierProperties props =
                    resources [j].property;
                for (::CORBA::ULong k = 0; k < props.length(); ++k)
                  {
                    // Deployment::DnC_Dump::dump (props [k]);

                    if (ACE_OS::strcmp (props [k].name.in (), "Setpoint")
                        == 0)
                      {
                        CORBA::Any value = props [k].value;
                        CORBA::TypeCode_var tc = value.type ();
                        if (tc->kind () == CORBA::tk_double)
                          {
                            value >>= resource.set_point;
                            msg << "Obtained set point! Value is: "
                                << resource.set_point << "\n";
//                             ACE_DEBUG ((LM_DEBUG, "\n%f\t",
//                                         resource.set_point));

                          }
                      }
                    else if (ACE_OS::strcmp (props [k].name.in (),
                                             "Current")== 0)
                      {
                        CORBA::Any value = props [k].value;
                        CORBA::TypeCode_var tc = value.type ();
                        if (tc->kind () == CORBA::tk_double)
                          {
                            value >>= resource.util;

                            // Temporary hack! We are multiplying by
                            // the number of physical processors!
                            //resource.util *= 4;
                            msg << "Obtained curr util! Value is: "
                                << resource.util << "\n";
//                             ACE_DEBUG ((LM_DEBUG, "%f\n", resource.util));

                          }
                      }
                  }

                // Dump these values to a file.
                util << d_node.name.in()
                     << "\t"
                     << resource.set_point
                     << "\t"
                     << resource.util
                     << "\t";
                // Now that we have populated the resource info, we add it
                // to the node.
                r_node.resources.push_back (resource);
                msg << "Added resource \"" << resources [j].name.in()
                    << "\" to the node structure.\n";
              }
          }
        msg << "Leaving populate_node ()\n";
        this->logger_.log (msg.str ());
        this->util_logger_.log (util.str());
        return r_node;
      }

      void
      Controller::populate_task
      (const ::CIAO::RACE::OperationalString & opstring,
       ::CIAO::RACE::Task & task)
      {
        std::stringstream msg;
        msg << "In populate_task()\n";
        task.UUID = ::CORBA::string_dup (opstring.ID.in());
        task.max_rate = opstring.rate.maxRate;
        task.min_rate = opstring.rate.minRate;
        task.curr_rate = opstring.rate.currRate;
        task.delta_rate = 0;

        for (::CORBA::ULong i = 0; i < opstring.instance.length(); ++i)
          {
            ::CIAO::RACE::Execution_Time time;
            time.BCET.sec =
              opstring.instance[i].executionTime.earliest.sec;
            time.BCET.usec =
              opstring.instance[i].executionTime.earliest.usec;

            time.WCET.sec =
              opstring.instance[i].executionTime.latest.sec;
            time.WCET.usec =
              opstring.instance[i].executionTime.latest.usec;

            // If either BCET or WCET is equal to 0, we don't
            // add that component to the subtak list.
            if ((time.BCET.sec + time.BCET.usec == 0) ||
                (time.WCET.sec + time.WCET.usec == 0))
              {
                continue;
              }

            ::CIAO::RACE::Subtask subtask;
            subtask.exec_time = time;
            subtask.UUID = opstring.instance[i].ID.in();
            subtask.label = opstring.instance[i].name.in();
            int node_ref = this->node_ref (opstring.instance[i].node.in());
            if (node_ref != -1)
              {
                subtask.node = node_ref;
              }
            else
              {
                msg << "ERROR! Given node for subtask : "
                    << opstring.instance[i].ID.in()
                    << "is not present in the domain!\n";
                continue;
              }
            task.subtasks.push_back (subtask);
          }
        msg << "Leaving populate_task ()\n";
        this->logger_.log (msg.str ());
      }

      int Controller::node_ref (const char *node)
      {
        for (size_t itr = 0; itr < this->domain_.nodes.size (); ++itr)
          {
            // Compare the given name with the names of the nodes in the
            // domain.
            if (ACE_OS::strcmp (node,
                                this->domain_.nodes[itr].UUID.c_str())== 0)
              {
                return itr;
              }
          }
        return -1;
      }

      void
      Controller::dump_task (::CIAO::RACE::Task task)
      {
        std::stringstream msg;

        msg << "Task details...\n";
        msg << "UUID: " << task.UUID;
        msg << "\nLabel: " << task.label;
        msg << "\nmin_rate: " << task.min_rate;
        msg << "\nmax_rate: " << task.max_rate;
        msg << "\ncurr_rate: " << task.curr_rate;
        msg << "\ndelta_rate: " << task.delta_rate;

        msg << "\nSubtask info... \n";
        for (size_t itr = 0; itr < task.subtasks.size(); ++itr)
          {
            msg << "\tSubtask: " << itr+1;
            msg << "\n\tUUID: " << task.subtasks[itr].UUID;
            msg << "\n\tlabel: " << task.subtasks[itr].label;
            msg << "\n\tnode: " << task.subtasks[itr].node;
            msg << "\n\tBCET: "
                << task.subtasks[itr].exec_time.BCET.sec
                << " seconds and "
                << task.subtasks[itr].exec_time.BCET.usec
                << " micro seconds.";
            msg << "\n\tWCET: "
                << task.subtasks[itr].exec_time.WCET.sec
                << " seconds and "
                << task.subtasks[itr].exec_time.WCET.usec
                << " micro seconds.\n";
          }

        this->logger_.log (msg.str());
      }

      void Controller::dump_domain (const CIAO::RACE::Domain &domain)
      {
        std::stringstream msg;
        msg << "Domain details...\n";
	std::vector<CIAO::RACE::Node>::const_iterator it;
	for (it = domain.nodes.begin(); it != domain.nodes.end(); ++it)
          {
            msg << "Node "
                << (*it).resources[0].UUID.c_str ()
                << "\tUtil = "
                << (*it).resources[0].util
                << "\tSet-point = "
                << (*it).resources[0].set_point
                << "\n";
          }
        this->logger_.log (msg.str());
      }
    }
  }
}

#endif /* CONTROLLER_CPP */
