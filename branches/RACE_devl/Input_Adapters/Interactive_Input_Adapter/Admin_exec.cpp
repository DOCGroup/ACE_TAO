// $Id$
#include "Admin_exec.h"
#include <sstream>
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/DnC_Dump.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_Interactive_IA_Component_Impl
      {
        Admin_exec_i::Admin_exec_i (CCM_Interactive_IA_Component_Context_ptr
                                    context,
                                    Logger &logger)
          : logger_ (logger)
        {
          std::stringstream msg;
          try
            {
              msg << "Trying to get objref of the conductor....";
              this->conductor_ = context->get_connection_conductor ();
              msg << "done!\n"
                  << "Now trying to initialize the system\n";
              if (this->conductor_->init_system ())
                {
                  msg << "Successfully initialized the system.\n";
                }
              else
                {
                  msg << "Error while initializing the system.\n";

                }
            }
          catch (CORBA::Exception &ex)
            {
              msg << "\nException caught::Admin_exec:: "
                  << "Error while resolving references to "
                  << "RACE Conductor object!\n";
              msg << ex._info ().c_str();
            }
          this->logger_.log (msg.str());

        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Admin_exec_i::add_plan (const char * filename, ::CORBA::String_out ID )
        {
          ::CIAO::RACE::OperationalString opstring;
          this->plan_to_opstring (filename, opstring);
          try
            {
              return this->conductor_->add_string (opstring, ID);
            }
          catch (CORBA::Exception &ex)
            {
              std::stringstream msg;
              msg << "Exception caught in Admin_exec_i::"
                "add_plan.\n";
              msg << ex._info ().c_str();
              this->logger_.log (msg.str ());

            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::delete_plan (const char * ID)
        {
          try
            {
              return this->conductor_->delete_string (ID);
            }
          catch (UnknownID &ex)
            {
              std::stringstream msg;
              msg << "Given string ID is not know to RACE. ID:";
              msg << ID;
              this->logger_.log (msg.str ());
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_plan (const char * filename,
                                   ::CORBA::String_out ID)
        {
          std::stringstream msg;
          ::CIAO::RACE::OperationalString opstring;
          msg << "In Admin_exec_i::deploy_plan.\n"
              << "Trying to convert deployment plan to opstring.";
          this->logger_.log (msg.str ());
          this->plan_to_opstring (filename, opstring);
          msg.str("");
          msg << "deploy_plan: Now, we try to invoke "
              << "the conductor.\nWaiting......";
          this->logger_.log (msg.str ());
          // @@ Currently, there is no CLEAN way to "reset" the stringstream.
          // As a make shift, we do this.
          msg.str("");
          try
            {
              CORBA::Boolean ret = this->conductor_->deploy_string
                                   (opstring, ID);
              msg << "done! Successfully deployed the plan.\n";
              this->logger_.log (msg.str ());
              return ret;

            }
          catch (CORBA::Exception &ex)
            {
              msg << "Exception caught in "
                  << "Interactive_IA::Admin_exec_i::deploy_plan.\n";
              msg << ex._info ().c_str();
              this->logger_.log (msg.str ());
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_plan_with_id (const char * ID)
        {
          try
            {
              return this->conductor_->deploy_string_with_id (ID);
            }
          catch (UnknownID &ex)
            {
              std::stringstream msg;
              msg << "Given string ID is not know to RACE. ID:";
              msg << ID;
              this->logger_.log (msg.str ());
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::modify_plan (const char * filename,
                                   const char * ID )
        {
          ::CIAO::RACE::OperationalString opstring;
          this->plan_to_opstring (filename, opstring);

          try
            {
              return this->conductor_->modify_string(opstring, ID);
            }
          catch (UnknownID &ex)
            {
              std::stringstream msg;
              msg << "Given string ID is not know to RACE. ID:";
              msg << ID;
              this->logger_.log (msg.str ());
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::tear_down_plan (const char *ID)
        {
          try
            {
              return this->conductor_->tear_down_string (ID);
            }
          catch (UnknownID &ex)
            {
              std::stringstream msg;
              msg << "Given string ID is not know to RACE. ID:";
              msg << ID;
              this->logger_.log (msg.str ());
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::start_controller()
        {
          std::stringstream msg;
          msg << "Admin_exec_i::start_controller (): "
              << "Trying to start the controller.\n";
          this->logger_.log (msg.str ());
          try
            {
              bool ret = this->conductor_->start_controller ();
              if (ret)
                {
                  msg << "Done!\n";
                  this->logger_.log (msg.str ());
                  return true;
                }
              else
                {
                  msg << "Oops! Error while starting the controller!\n";
                  this->logger_.log (msg.str ());
                  return false;
                }
            }
          catch (UnknownID &ex)
            {
              msg << "\nException caught::start_controller::"
                  << ex._info ().c_str();
              this->logger_.log (msg.str ());
              return false;
            }
        }


        ::CORBA::Boolean
        Admin_exec_i::stop_controller()
        {
          std::stringstream msg;
          msg << "Admin_exec_i::stop_controller (): "
              << "Trying to stop the controller.\n";
          this->logger_.log (msg.str ());
          try
            {
              bool ret = this->conductor_->stop_controller();
              if (ret)
                {
                  msg << "Done!\n";
                  this->logger_.log (msg.str ());
                  return true;
                }
              else
                {
                  msg << "Oops! Error while stopping the controller!\n";
                  this->logger_.log (msg.str ());
                  return false;
                }
            }
          catch (UnknownID &ex)
            {
              msg << "\nException caught::stop_controller::"
                  << ex._info ().c_str();
              this->logger_.log (msg.str ());
              return false;
            }
        }

        void
        Admin_exec_i::plan_to_opstring
        (const char *filename,
         ::CIAO::RACE::OperationalString & op_string)
        {
          std::stringstream msg;

          msg << "plan_to_opstring: Converting plan to opstring\n";
          // First, we parse the XML file to obtain the deployment plan
          // IDL.
          ACE_Auto_Ptr <::Deployment::DeploymentPlan > plan;
          msg << "Opening the plan...";
          Config_Handlers::XML_File_Intf xfi (filename);
          plan.reset (xfi.get_plan ());
          msg << "done!\n"
              << "Now that we have the IDL, we populate the op_string.";
          this->logger_.log (msg.str());
          msg.str("");

          // Before we do anything, we dump the populated IDL.
          // Deployment::DnC_Dump::dump (*plan.get());
          msg << "Populating basic info.\n";
          this->update_basic_info (plan.get (), op_string);
          msg << "Populating rate info.\n";
          this->update_rate_info (plan.get (), op_string);
          msg << "Populating instance info.\n";
          this->update_instance_info (plan.get (), op_string);
          msg << "plan_to_opstring: Conversion complete!\n";
          this->logger_.log (msg.str());
        }


        void
        Admin_exec_i::update_basic_info
        (const ::Deployment::DeploymentPlan * plan,
         ::CIAO::RACE::OperationalString & op_string)
        {
          // Now that we have the IDL, we populate the op_string.
          op_string.name = plan->label;
          op_string.ID = plan->UUID;
          op_string.dataLink = plan->connection;
          op_string.infoProperty = plan->infoProperty;
          op_string.implementation = plan->implementation;
          op_string.artifact = plan->artifact;
          const char * effectorID = this->get_effector_ID (plan->infoProperty);
          if (effectorID != 0)
            {
              op_string.effectorID = effectorID;
            }
          else
            {
              std::stringstream msg;
              msg << "ERROR! Effector_ID for plan: "
                  << plan->UUID << " is not in the descriptors!\n";
              this->logger_.log (msg.str());
            }
        }

        void
        Admin_exec_i::update_rate_info
        (const ::Deployment::DeploymentPlan * plan,
         ::CIAO::RACE::OperationalString & op_string)
        {
          // We further need to parse the min, max rate info properties.
          std::stringstream msg;
          ::CIAO::RACE::ExecutionRate rates;
          double val = this->parse_rate_info ("min_rate", plan->infoProperty);
          if (val != -1)
            {
              rates.minRate = val;
              msg << "Min rate of plan: "
                  << plan->UUID << "is: " << val << ".\n";
            }
          else
            {
              msg << "ERROR: min_rate for plan: "
                  << plan->UUID << " is not present in the descriptors!\n";
              rates.minRate = 0;
            }

          val = this->parse_rate_info ("max_rate", plan->infoProperty);
          if (val != -1)
            {
              rates.maxRate = val;
              msg << "Max rate of plan: "
                  << plan->UUID << "is: " << val << ".\n";;
            }
          else
            {
              msg << "ERROR: max_rate for plan: "
                  << plan->UUID << " is not present in the descriptors!\n";
              rates.maxRate = 0;
            }

          val = this->parse_rate_info ("curr_rate", plan->infoProperty);
          if (val != -1)
            {
              rates.currRate = val;
              msg << "Curr rate of plan: "
                  << plan->UUID << "is: " << val << ".\n";;
            }
          else
            {
              msg << "ERROR: curr_rate for plan: "
                  << plan->UUID << " is not present in the descriptors!\n";
              rates.currRate = 0;
            }
          op_string.rate = rates;

          this->logger_.log (msg.str ());
        }

        void
        Admin_exec_i::update_instance_info
        (const ::Deployment::DeploymentPlan * plan,
         ::CIAO::RACE::OperationalString & op_string)
        {
          std::stringstream msg;
          /// Now copy over each instance description.
          for (CORBA::ULong itr = 0;
               itr < plan->instance.length (); ++itr)
            {
              ::CIAO::RACE::InstanceDescription op_instance;
              ::Deployment::InstanceDeploymentDescription instance =
                  plan->instance [itr];
              op_instance.name = instance.name;
              op_instance.node = instance.node;
              op_instance.implementationRef = instance.implementationRef;
              op_instance.configProperty = instance.configProperty;

              // We further need to populate the BCET and WCET for each
              // instance.
              ::CIAO::RACE::TimeValue BCET, WCET;
              // @ NOTE: We assume that the time captured in the
              // descriptors are in micro seconds!
              BCET.sec = 0;
              WCET.sec = 0;
              int val = this->parse_properties
                        ("BCET", instance.configProperty);
              if (val != -1)
                {
                  BCET.usec = val;
                  msg << "BCET for instance: "
                      << instance.name
                      << " is: " << val << ".\n";;
                }
              else
                {
                  msg << "ERROR: BCET for instance: "
                      << instance.name
                      << " is not present in the descriptors!\n";
                  BCET.usec = 0;

                }
              val = this->parse_properties ("WCET", instance.configProperty);

              if (val != -1)
                {
                  WCET.usec = val;
                  msg << "WCET for instance: "
                      << instance.name
                      << " is: " << val << ".\n";;
                }
              else
                {
                  msg << "ERROR: WCET for instance: "
                      << instance.name
                      << " is not present in the descriptors!\n";
                  WCET.usec = 0;

                }
              op_instance.executionTime.earliest = BCET;
              op_instance.executionTime.latest = WCET;

              // Insert the new op_instance into the op_string.
              CORBA::ULong cur_len = op_string.instance.length ();
              op_string.instance.length (cur_len+1);
              op_string.instance [cur_len] = op_instance;
            }
          this->logger_.log (msg.str());
        }

        int
        Admin_exec_i::parse_properties (const char* name,
                                        const ::Deployment::Properties &props)
        {
          /// Parse the property sequence to obtain the value of the
          /// desired property such as BCET/WCET.
          for (CORBA::ULong ctr (0); ctr < props.length (); ++ctr)
            {
              const ::Deployment::Property & property = props [ctr];
              if (ACE_OS::strcmp (property.name.in (), name) == 0)
                {
                  ACE_DEBUG ((LM_DEBUG, "Found property %s...",  name));
                  if (property.value.type ()->kind () == CORBA::tk_long)
                    {
                      int value;
                      property.value>>= value;
                      ACE_DEBUG ((LM_DEBUG, "and value is %d\n",  value));
                      return value;
                    }
                  else
                    {
                      return -1;
                    }
                }
            }
          return -1;
        }

        double
        Admin_exec_i::parse_rate_info (const char* name,
                                       const ::Deployment::Properties &props)
        {
          /// Parse the property sequence to obtain the value of the
          /// desired property such as min-rate/max-rate/curr_rate.
          for (CORBA::ULong ctr (0); ctr < props.length (); ++ctr)
            {
              const ::Deployment::Property & property = props [ctr];
              if (ACE_OS::strcmp (property.name.in (), name) == 0)
                {
                  ACE_DEBUG ((LM_DEBUG, "Found property %s...",  name));
                  if (property.value.type ()->kind () == CORBA::tk_double)
                    {
                      double value;
                      property.value>>= value;
                      ACE_DEBUG ((LM_DEBUG, "and value is %f\n",  value));
                      return value;
                    }
                  else
                    {
                      return -1;
                    }
                }
            }
          return -1;
        }


        const char*
        Admin_exec_i::get_effector_ID (const ::Deployment::Properties &props)
        {
          /// Parse the property sequence to obtain the value of effector ID.
          for (CORBA::ULong ctr (0); ctr < props.length (); ++ctr)
            {
              const ::Deployment::Property & property = props [ctr];
              if (ACE_OS::strcmp (property.name.in (), "Effector_ID") == 0)
                {
                  ACE_DEBUG ((LM_DEBUG, "Found Effector ID..."));
                  if (property.value.type ()->kind () == CORBA::tk_string)
                    {
                      const char* value;
                      property.value>>= value;
                      ACE_DEBUG ((LM_DEBUG, "and value is %s\n",
                                  value));
                      return ::CORBA::string_dup (value);
                    }
                  else
                    {
                      return 0;
                    }
                }
            }
          return 0;
        }

      }
    }
  }
}
