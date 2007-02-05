#include <iostream>

#include "Driver.h"
#include "ace/UUID.h"

#include "SA_POP_Types.h"
#include "SA_Builder.h"
#include "TaskMapFileIn.h"
#include "SANet/SANetFileIn.h"

namespace CIAO
{
  namespace RACE
  {

    namespace SA_POP
    {
      Driver_i::Driver_i (CORBA::ORB_ptr orb)
        : orb_ (CORBA::ORB::_duplicate (orb)),
          iia_name_ ("RACE::InteractiveInput"),
          planner_ (0),
          screen_out_ (std::cout)
      {
      }

      Driver_i::~Driver_i (void)
      {
        delete this->planner_;
      }

      int
      Driver_i::init (const char *RM_name,
                      const char *sanet_str,
                      const char *taskmap_str)
      {
        try
          {
            // Resolve naming service
            CORBA::Object_var ns_tmp =
              this->orb_->resolve_initial_references ("NameService");
            ::CosNaming::NamingContext_var ns =
                ::CosNaming::NamingContext::_narrow (ns_tmp.in ());
            ::CosNaming::Name ns_name;
            CORBA::ULong i = 0;
            ACE_Tokenizer tok (this->iia_name_.rep ());
            tok.delimiter_replace (':', 0);
            tok.delimiter_replace ('/', 0);
            char *name_element = 0;
            while ((name_element = tok.next ()) != 0)
              {
                ns_name.length (ns_name.length () + 1);
                ns_name[i].id = CORBA::string_dup (name_element);
                ++i;
              }

            /// now try to resolve the reference to the IIA.
            CORBA::Object_var iia_tmp = ns->resolve (ns_name);
            this->iia_ =
              ::CIAO::RACE::Interactive_Input_Adapter::_narrow (iia_tmp.in ());

            // Now initialize the plan generator.
                        this->plan_gen_.init (this->orb_, true, RM_name);

            // Create SA-POP planner.
            ::SA_POP::SA_Builder builder;
            ::SANet::SANetFileIn sanet_in;
            ::SA_POP::TaskMapFileIn tm_in;

            sanet_in.build_net (sanet_str, &builder);
            tm_in.build_task_map (taskmap_str, &builder);

            this->planner_ = builder.get_planner ();

            // Add screen output adapter to planner.
            this->planner_->add_out_adapter (&(this->screen_out_));

            return 0;
          }
        catch (CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Error in initializing the driver!\n");
            return -1;
          }
      }


      void
      Driver_i::deploy_plan (const char * plan_uri)
        throw (::CORBA::SystemException)
      {
        try
          {
            // Create deploy input event and populate it.
            CIAO::RACE::Deploy_Input_var input = new OBV_CIAO::RACE::Deploy_Input;
            input->plan_uri (plan_uri);
            input->command (::CIAO::RACE::DEPLOY);

            // Now push the event to the IIA.
            this->iia_->get_consumer_deployment ()->push_Deploy_Input (input.in ());

          }
        catch (CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Exception caught in Driver::deploy_plan");
          }
      }

      void
      Driver_i::deploy_goal (const ::CIAO::RACE::GoalStructure &goal_idl)
        throw (::CORBA::SystemException)
      {
        try
          {
            // Convert goal.
            ::SA_POP::Goal goal;
            this->goal_from_idl (goal_idl, goal);

            // Run planner (with a maximum of 100 steps of spreading activation).
            this->planner_->plan (100, goal);

            // Get and convert opstring.
            ::SA_POP::Plan opstring = this->planner_->get_plan ();
            ::CIAO::RACE::OperationalString opstring_idl;
            this->opstring_to_idl (opstring, opstring_idl);
            opstring_idl.goal = goal_idl;

            // Convert opstring to deployment plan and deploy.
            ::Deployment::DeploymentPlan deploy_plan;
            if (this->opstring_to_dplan (opstring_idl, deploy_plan) == 0)
              this->deploy_plan (deploy_plan);
            else
              throw "CIAO::RACE::Driver_i::deploy_goal(): Could not convert opstring to deployment plan.";
          }
        catch (...)
          {
            ACE_ERROR ((LM_ERROR, "(%P|%t) Unknown exception\n"));
          }
      }

      int
      Driver_i::opstring_to_dplan (::CIAO::RACE::OperationalString &op_string,
                                   ::Deployment::DeploymentPlan &plan)
      {
        // Populate the label, UUID, connections, and info property fields of
        // the deployment plan.
        plan.label = op_string.name;
        plan.UUID = op_string.UUID;
        plan.connection = op_string.dataLinks;
        plan.infoProperty = op_string.properties;

        // Now, populate the instance, implementation, and artifact fields of
        // the deployment plan using the plan generator.
        int position;
        for (CORBA::ULong itr = 0; itr < op_string.instances.length (); ++itr)
          {

            ::CIAO::RACE::InstanceDescription op_instance = op_string.instances [itr];
            if (this->plan_gen_.generate_plan
                (plan,
                 op_instance.suggestedImpl.in (),
                 position))
              {
                ::Deployment::InstanceDeploymentDescription instance;
                instance.name = op_instance.name;

                // @@ This is a temporaty hack. Later on we must be using the
                // simple bin packer do perform the node assignment.
                if (strcmp ("IDL:SPACE/Ground_Station:1.0",
                            op_instance.suggestedImpl.in ()) == 0)
                  {
                    instance.node = CORBA::string_dup ("ground");
                  }
                else
                  {
                    instance.node = CORBA::string_dup ("space");
                  }

                instance.implementationRef = position;
                instance.configProperty = op_instance.configProperty;
                CORBA::ULong cur_len = plan.instance.length ();
                plan.instance.length (cur_len+1);
                plan.instance [cur_len] = instance;

              }
            else
              {
                ACE_ERROR ((LM_ERROR, "Given suggested type is not available "
                            "in the Repoman!!\n Bailing out....\n"));
                return -1;
              }
          }
        return 0;
      }

      void Driver_i::opstring_to_idl (const ::SA_POP::Plan &opstring,
        OperationalString &opstring_idl)
      {
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
        // (Get ID from Goal or Plan)
        // Create and set the opstring_idl ID.
        ACE_Utils::UUID uuid;
        ACE_Utils::UUID_GENERATOR::instance ()->generateUUID (uuid);
        opstring_idl.UUID = CORBA::string_dup (uuid.to_string()->c_str ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

        // Set opstring_idl name.
        opstring_idl.name = CORBA::string_dup (opstring.name.c_str ());

        // Set opstring_idl time windows.
        opstring_idl.startWindow.earliest.sec = opstring.start_window.first;
        opstring_idl.startWindow.earliest.usec = 0;
        opstring_idl.startWindow.latest.sec = opstring.start_window.second;
        opstring_idl.startWindow.latest.usec = 0;
        opstring_idl.endWindow.earliest.sec = opstring.end_window.first;
        opstring_idl.endWindow.earliest.usec = 0;
        opstring_idl.endWindow.latest.sec = opstring.end_window.second;
        opstring_idl.endWindow.latest.usec = 0;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
        // (Get from expected utility)
//      long priority;
        // (Get from goal)
//      QoSSpecifications QoSRequirements;
        // (???)
//      ::Deployment::Properties properties;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP


        // Map from instance IDs to instance index.
        typedef std::map < ::SA_POP::TaskInstID, CORBA::ULong> InstToIndexMap;
        InstToIndexMap inst_to_index;
        inst_to_index.clear ();

        // Add instances to opstring_idl.
        for (::SA_POP::PlanInstSet::const_iterator inst_iter = opstring.task_insts.begin ();
          inst_iter != opstring.task_insts.end (); inst_iter++)
        {
          ::SA_POP::PlanTaskInst inst = *inst_iter;

          // Instance description to populate.
          CIAO::RACE::InstanceDescription inst_desc;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
          // (Get instance ID from instance in opstring)
          // Create and set task instance ID.
          ACE_Utils::UUID inst_uuid;
          ACE_Utils::UUID_GENERATOR::instance ()->generateUUID (inst_uuid);
          inst_desc.UUID = CORBA::string_dup (inst_uuid.to_string()->c_str());
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
          // (Get from repoman)
//        ::Deployment::Properties configProperty;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

          // Set instance name, task ID, supported type ID, and suggested implementation ID.
          inst_desc.name = CORBA::string_dup (inst.name.c_str());
          inst_desc.taskId = CORBA::string_dup (::SA_POP::to_string (inst.task_id).c_str ());
          inst_desc.taskTypeId = CORBA::string_dup (inst.type_id.c_str ());
          inst_desc.suggestedImpl = CORBA::string_dup (inst.suggested_impl.c_str ());

          // Set instance time windows.
          inst_desc.startWindow.earliest.sec = inst.start_window.first;
          inst_desc.startWindow.earliest.usec = 0;
          inst_desc.startWindow.latest.sec = inst.start_window.second;
          inst_desc.startWindow.latest.usec = 0;
          inst_desc.endWindow.earliest.sec = inst.end_window.first;
          inst_desc.endWindow.earliest.usec = 0;
          inst_desc.endWindow.latest.sec = inst.end_window.second;
          inst_desc.endWindow.latest.usec = 0;

          // Add instance to opstring_idl and record index.
          CORBA::ULong inst_len = opstring_idl.instances.length ();
          opstring_idl.instances.length (inst_len + 1);
          opstring_idl.instances[inst_len] = inst_desc;
          inst_to_index.insert (std::make_pair (inst.inst_id, inst_len));
        }

        // Add opstring data links.
        CORBA::ULong conns_index = 0;
        for (::SA_POP::PlanConnSet::const_iterator conn_iter = opstring.connections.begin ();
              conn_iter != opstring.connections.end (); conn_iter++)
        {
          ::SA_POP::PlanConnection conn = *conn_iter;

          // Connection description to populate.
          Deployment::PlanConnectionDescription conn_desc;

          // Get opstring instance and port IDS for connection.
          ::SA_POP::TaskInstID taskA = conn.first_task_inst;
          ::SA_POP::TaskInstID taskB = conn.second_task_inst;
          ::SA_POP::PortID taskAport = conn.first_port;
          ::SA_POP::PortID taskBport = conn.second_port;

          // Get deployment opstring instances (port IDs are the same).
          InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
          CORBA::ULong taskAinst = taskA_iter->second;
          InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
          CORBA::ULong taskBinst = taskB_iter->second;

          // Create connection name.
          std::string conn_name = "";
          conn_name += ::SA_POP::to_string (static_cast<unsigned long>(taskAinst));
          conn_name += "_";
          conn_name += taskAport;
          conn_name += "___";
          conn_name += ::SA_POP::to_string (static_cast<unsigned long>(taskBinst));
          conn_name += "_";
          conn_name += taskBport;

          // Create connection endpoints.
          conn_desc.internalEndpoint.length (2);
          conn_desc.internalEndpoint[0].instanceRef = taskAinst;
          conn_desc.internalEndpoint[0].portName = CORBA::string_dup (taskAport.c_str ());
          conn_desc.internalEndpoint[0].provider = 1;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
          // (Get kind from data link in opstring)
//          conn_desc.internalEndpoint[0].kind = Deployment::EventEmitter;
          conn_desc.internalEndpoint[0].kind = Deployment::EventPublisher;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
          conn_desc.internalEndpoint[1].instanceRef = taskBinst;
          conn_desc.internalEndpoint[1].portName = CORBA::string_dup (taskBport.c_str ());
          conn_desc.internalEndpoint[1].provider = 0;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
          // (Get kind from data link in opstring)
          conn_desc.internalEndpoint[1].kind = Deployment::EventConsumer;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

          // Set connection name.
          conn_desc.name = CORBA::string_dup (conn_name.c_str ());

          // Add data link to opstring_idl.
          opstring_idl.dataLinks.length (conns_index + 1);
          opstring_idl.dataLinks[conns_index] = conn_desc;

          // Increment connection index.
          conns_index++;
        }

        // Add scheduling links.
        CORBA::ULong links_index = 0;
        for (::SA_POP::SchedLinkSet::const_iterator sched_iter = opstring.sched_links.begin ();
              sched_iter != opstring.sched_links.end (); sched_iter++)
        {
          ::SA_POP::SchedLink link = *sched_iter;

          // Link to populate.
          CIAO::RACE::OrderLinkDescription link_desc;

          // Set link type.
          link_desc.type = CIAO::RACE::SCHEDULING;

          // Get opstring instance and port IDS for link.
          ::SA_POP::TaskInstID taskA = link.first;
          ::SA_POP::TaskInstID taskB = link.second;

          // Get deployment opstring instances (port IDs are the same).
          InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
          CORBA::ULong taskAinst = taskA_iter->second;
          InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
          CORBA::ULong taskBinst = taskB_iter->second;

          // Create link name.
          std::string link_name = "";
          link_name += ::SA_POP::to_string (static_cast<unsigned long>(taskAinst));
          link_name += "___";
          link_name += ::SA_POP::to_string (static_cast<unsigned long>(taskBinst));

          // Create link endpoints.
          link_desc.internalEndpoint.length (2);
          link_desc.internalEndpoint[0].instanceRef = taskAinst;
          link_desc.internalEndpoint[0].provider = 1;
          link_desc.internalEndpoint[1].instanceRef = taskBinst;
          link_desc.internalEndpoint[1].provider = 0;

          // Set link name.
          link_desc.name = CORBA::string_dup (link_name.c_str ());

          // Add data link to opstring_idl.
          opstring_idl.orderLinks.length (links_index + 1);
          opstring_idl.orderLinks[links_index] = link_desc;

          // Increment link index.
          links_index++;
        }

        // Add threat links.
        links_index = opstring_idl.orderLinks.length ();
        for (::SA_POP::ThreatLinkSet::const_iterator threat_iter = opstring.threat_links.begin ();
              threat_iter != opstring.threat_links.end (); threat_iter++)
        {
          ::SA_POP::ThreatLink link = *threat_iter;

          // Link to populate.
          CIAO::RACE::OrderLinkDescription link_desc;

          // Set link type.
          link_desc.type = CIAO::RACE::THREAT;

          // Get opstring instance and port IDS for link.
          ::SA_POP::TaskInstID taskA = link.first;
          ::SA_POP::TaskInstID taskB = link.second;

          // Get deployment opstring instances (port IDs are the same).
          InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
          CORBA::ULong taskAinst = taskA_iter->second;
          InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
          CORBA::ULong taskBinst = taskB_iter->second;

          // Create link name.
          std::string link_name = "";
          link_name += ::SA_POP::to_string (static_cast<unsigned long>(taskAinst));
          link_name += "___";
          link_name += ::SA_POP::to_string (static_cast<unsigned long>(taskBinst));

          // Create link endpoints.
          link_desc.internalEndpoint.length (2);
          link_desc.internalEndpoint[0].instanceRef = taskAinst;
          link_desc.internalEndpoint[0].provider = 1;
          link_desc.internalEndpoint[1].instanceRef = taskBinst;
          link_desc.internalEndpoint[1].provider = 0;

          // Set link name.
          link_desc.name = CORBA::string_dup (link_name.c_str ());

          // Add data link to opstring_idl.
          opstring_idl.orderLinks.length (links_index + 1);
          opstring_idl.orderLinks[links_index] = link_desc;

          // Increment link index.
          links_index++;
        }

        // Add causal links.
        links_index = opstring_idl.orderLinks.length ();
        for (::SA_POP::CLSet::const_iterator cl_iter = opstring.causal_links.begin ();
              cl_iter != opstring.causal_links.end (); cl_iter++)
        {
          ::SA_POP::CausalLink link = *cl_iter;

          // Link to populate.
          CIAO::RACE::OrderLinkDescription link_desc;

          // Set link type.
          link_desc.type = CIAO::RACE::CAUSAL;

          // Get opstring instance and port IDS for link.
          ::SA_POP::TaskInstID taskA = link.first;
          ::SA_POP::TaskInstID taskB = link.second;

          // Get deployment opstring instances (port IDs are the same).
          InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
          CORBA::ULong taskAinst = taskA_iter->second;
          InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
          CORBA::ULong taskBinst = taskB_iter->second;

          // Create link name.
          std::string link_name = "";
          link_name += ::SA_POP::to_string (static_cast<unsigned long>(taskAinst));
          link_name += "___";
          link_name += ::SA_POP::to_string (static_cast<unsigned long>(taskBinst));

          // Create link endpoints.
          link_desc.internalEndpoint.length (2);
          link_desc.internalEndpoint[0].instanceRef = taskAinst;
          link_desc.internalEndpoint[0].provider = 1;
          link_desc.internalEndpoint[1].instanceRef = taskBinst;
          link_desc.internalEndpoint[1].provider = 0;

          // Set link name.
          link_desc.name = CORBA::string_dup (link_name.c_str ());

          // Add data link to opstring_idl.
          opstring_idl.orderLinks.length (links_index + 1);
          opstring_idl.orderLinks[links_index] = link_desc;

          // Increment link index.
          links_index++;
        }
      };

      void Driver_i::goal_from_idl (const GoalStructure &goal_idl,
        ::SA_POP::Goal &goal)
      {
        // Set ID and name.
        goal.goal_id = CORBA::string_dup (goal_idl.UUID);
        goal.name = CORBA::string_dup (goal_idl.name);

        // Set start window.
        goal.start_window = std::make_pair (goal_idl.startWindow.earliest.sec,
                                            goal_idl.startWindow.latest.sec);

        // Add goal conditions.
        ::CIAO::RACE::GoalConditions conds = goal_idl.goalConds;
        for (CORBA::ULong i (0); i < conds.length (); ++i) {
          goal.goal_conds.insert (std::make_pair (conds[i].condID,
                                                  conds[i].utility));
        }

        // Add relative time constraints.
        ::CIAO::RACE::TimeConstraints rel_times = goal_idl.relTimes;
        for (CORBA::ULong i (0); i < rel_times.length (); ++i) {
          goal.rel_times.insert (std::make_pair (rel_times[i].condID,
                                 rel_times[i].constraint.sec));
        }

        // Add absolute time constraints.
        ::CIAO::RACE::TimeConstraints abs_times = goal_idl.absTimes;
        for (CORBA::ULong i (0); i < abs_times.length (); ++i) {
          goal.abs_times.insert (std::make_pair (abs_times[i].condID,
                                 abs_times[i].constraint.sec));
        }

      };

      int
      Driver_i::deploy_plan (::Deployment::DeploymentPlan &plan)
      {
        try
          {
            // Create a metadata event and populate it.
            CIAO::RACE::Metadata_var metadata = new OBV_CIAO::RACE::Metadata;
            metadata->command (::CIAO::RACE::DEPLOY);
            metadata->plan (plan);

            // Now push the event to the IIA.
            this->iia_->get_consumer_meta_data ()->push_Metadata (metadata.in ());
          }
        catch (CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Exception caught\n");
            return -1;
          }

        catch (...)
          {
            ACE_ERROR ((LM_ERROR, "(%P|%t) Driver_i: Unknown exception\n"));
            return -1;
          }
        return 0;
      }
    }
  }
}
