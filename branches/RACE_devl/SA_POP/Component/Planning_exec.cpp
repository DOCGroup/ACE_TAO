// -*- C++ -*-
// $Id$

#include "Planning_exec.h"
#include "ciao/CIAO_common.h"
#include "ace/UUID.h"
#include "common/OpStringC.h"

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
      namespace CIDL_SA_POP_Component_Impl
      {
        //==================================================================
        // Facet Executor Implementation Class:   Planning_exec_i
        //==================================================================

        Planning_exec_i::Planning_exec_i (void)
          : is_init_ (false),
          planner_ (0),
          screen_out_ (std::cout)
        {
        }

        Planning_exec_i::~Planning_exec_i (void)
        {
        }

        // Operations from ::CIAO::RACE::SA_POP::Planning

        ::CORBA::Boolean
        Planning_exec_i::init (
          const char * task_net_file,
          const char * task_map_file
        )
        {
            ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::init ()\n"));
          // Create SA-POP planner.
          ::SA_POP::SA_Builder builder;
          ::SANet::SANetFileIn sanet_in;
          ::SA_POP::TaskMapFileIn tm_in;


          ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::init () Before building SAN\n"));
          sanet_in.build_net (task_net_file, &builder);
          ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::init () After building SAN\n"));

          tm_in.build_task_map (task_map_file, &builder);

          ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::init () After building TM\n"));

          this->planner_ = builder.get_planner ();
          ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::init () After getting ptr\n"));

          // Add screen output adapter to planner.
          this->planner_->add_out_adapter (&(this->screen_out_));
          ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::init () After adding output adapter\n"));

          this->is_init_ = true;
          return this->is_init_;


       }

        ::CORBA::Boolean
        Planning_exec_i::generate_opstring (
          const ::CIAO::RACE::GoalStructure & goal,
          ::CIAO::RACE::OperationalString_out opstring
        )
        {
            ACE_DEBUG ((LM_DEBUG, "In Plannin_exec_i::generate_opstring ()\n"));
          // Check to make sure planner is initialized.
          if (!this->is_init_ || !this->planner_)
            return false;

          // Convert goal.
          ::SA_POP::Goal goal_sa;
          this->goal_from_idl (goal, goal_sa);

          // Run planner (with a maximum of 500 steps of spreading activation).
          this->planner_->plan (500, goal_sa);

          // Get and convert opstring.
          ::SA_POP::Plan opstring_sa = this->planner_->get_plan ();
          ::CIAO::RACE::OperationalString opstring_idl;
          this->opstring_to_idl (opstring_sa, opstring_idl);
          opstring_idl.goal = goal;

          opstring = new ::CIAO::RACE::OperationalString (opstring_idl);
          return true;
        }

        void Planning_exec_i::opstring_to_idl (const ::SA_POP::Plan &opstring,
          ::CIAO::RACE::OperationalString &opstring_idl)
        {
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
          // (Get ID from Goal or Plan)
          // Create and set the opstring_idl ID.
          ACE_Utils::UUID uuid;
          ACE_Utils::UUID_GENERATOR::instance ()->generateUUID (uuid);
          opstring_idl.ID = CORBA::string_dup (uuid.to_string()->c_str ());
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
//          long priority;
          // (Get from goal)
//          QoSSpecifications QoSRequirements;
          // (???)
//          ::Deployment::Properties properties;
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
            inst_desc.ID = CORBA::string_dup (inst_uuid.to_string()->c_str());
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
            // (Get from repoman)
//            ::Deployment::Properties configProperty;
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
            CORBA::ULong inst_len = opstring_idl.instance.length ();
            opstring_idl.instance.length (inst_len + 1);
            opstring_idl.instance[inst_len] = inst_desc;
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
//            conn_desc.internalEndpoint[0].kind = Deployment::EventEmitter;
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
            opstring_idl.dataLink.length (conns_index + 1);
            opstring_idl.dataLink[conns_index] = conn_desc;

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
            opstring_idl.orderLink.length (links_index + 1);
            opstring_idl.orderLink[links_index] = link_desc;

            // Increment link index.
            links_index++;
          }

          // Add threat links.
          links_index = opstring_idl.orderLink.length ();
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
            opstring_idl.orderLink.length (links_index + 1);
            opstring_idl.orderLink[links_index] = link_desc;

            // Increment link index.
            links_index++;
          }

          // Add causal links.
          links_index = opstring_idl.orderLink.length ();
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
            opstring_idl.orderLink.length (links_index + 1);
            opstring_idl.orderLink[links_index] = link_desc;

            // Increment link index.
            links_index++;
          }
        };

        void Planning_exec_i::goal_from_idl (const ::CIAO::RACE::GoalStructure &goal_idl,
          ::SA_POP::Goal &goal)
        {
          // Set ID and name.
          goal.goal_id = CORBA::string_dup (goal_idl.ID);
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

      }
    }
  }
}

