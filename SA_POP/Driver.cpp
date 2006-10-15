#include "Converter.h"
#include "ace/Get_Opt.h"
#include "ace/UUID.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "SA_POP_Types.h"
#include "SA_Builder.h"
#include "TaskMapFileIn.h"
#include "SANet/SANetFileIn.h"
#include "LogScreenOut.h"

int main (int argc, char **argv)
{

  CIAO::RACE::OperationalString opstring;
  std::string sanet_str;
  std::string taskmap_str;

  ACE_Get_Opt get_opts (argc, argv, "s:t:");
  int c = 0;
  while ((c = get_opts ()) != -1)
  {
    switch (c) {
      case 's':
        sanet_str = get_opts.opt_arg ();
        break;

      case 't':
        taskmap_str = get_opts.opt_arg ();
        break;
      default:
        break;
    }
  }

  SA_POP_DEBUG (SA_POP_DEBUG_NORMAL, "\nSA-POP START\n");

  SA_POP::SA_Builder builder;
  SANet::SANetFileIn sanet_in;
  SA_POP::TaskMapFileIn tm_in;

  sanet_in.build_net (sanet_str, &builder);
  tm_in.build_task_map (taskmap_str, &builder);

  SA_POP::Planner *planner = builder.get_planner ();

  // Log output plan to screen.
  SA_POP::LogScreenOut screen_out (std::cout);
  planner->add_out_adapter (&screen_out);

  // Run planner (with a maximum of 50 steps of spreading activation).
  planner->plan (50);

  SA_POP::Plan plan = planner->get_plan ();
  
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // (Get ID from Goal or Plan)
  // Create and set the opstring ID.
  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->generateUUID (uuid);
  opstring.UUID = CORBA::string_dup (uuid.to_string()->c_str ());
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  // Set opstring name.
  opstring.name = CORBA::string_dup (plan.name.c_str ());

  // Set opstring time windows.
  opstring.startWindow.Earliest.sec = plan.start_window.first;
  opstring.startWindow.Earliest.usec = 0;
  opstring.startWindow.Latest.sec = plan.start_window.second;
  opstring.startWindow.Latest.usec = 0;
  opstring.endWindow.Earliest.sec = plan.end_window.first;
  opstring.endWindow.Earliest.usec = 0;
  opstring.endWindow.Latest.sec = plan.end_window.second;
  opstring.endWindow.Latest.usec = 0;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // (Get from expected utility)
//        long priority;
  // (Get from goal)
//        QoSSpecifications QoSRequirements;
  // (???)
//        ::Deployment::Properties properties;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP


  // Map from instance IDs to instance index.
  typedef std::map <SA_POP::TaskInstID, CORBA::ULong> InstToIndexMap;
  InstToIndexMap inst_to_index;
  inst_to_index.clear ();

  // Buffer for strings.
  char buffer[64];

  // Add instances to opstring.
  for (SA_POP::PlanInstSet::iterator inst_iter = plan.task_insts.begin ();
    inst_iter != plan.task_insts.end (); inst_iter++)
  {
    SA_POP::PlanTaskInst inst = *inst_iter;

    // Instance description to populate.
    CIAO::RACE::InstanceDescription inst_desc;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // (Get instance ID from instance in plan)
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
    inst_desc.taskId = CORBA::string_dup (itoa (inst.task_id, buffer, 64));
    inst_desc.taskTypeId = CORBA::string_dup (inst.type_id.c_str ());
    inst_desc.suggestedImpl = CORBA::string_dup (inst.suggested_impl.c_str ());

    // Set instance time windows.
    inst_desc.startWindow.Earliest.sec = inst.start_window.first;
    inst_desc.startWindow.Earliest.usec = 0;
    inst_desc.startWindow.Latest.sec = inst.start_window.second;
    inst_desc.startWindow.Latest.usec = 0;
    inst_desc.endWindow.Earliest.sec = inst.end_window.first;
    inst_desc.endWindow.Earliest.usec = 0;
    inst_desc.endWindow.Latest.sec = inst.end_window.second;
    inst_desc.endWindow.Latest.usec = 0;

    // Add instance to opstring and record index.
    CORBA::ULong inst_len = opstring.instances.length ();
    opstring.instances.length (inst_len + 1);
    opstring.instances[inst_len] = inst_desc;
    inst_to_index.insert (std::make_pair (inst.inst_id, inst_len));
  }

  // Add plan data links.
  CORBA::ULong conns_index = 0;
  for (SA_POP::PlanConnSet::iterator conn_iter = plan.connections.begin ();
        conn_iter != plan.connections.end (); conn_iter++)
  {
    SA_POP::PlanConnection conn = *conn_iter;

    // Connection description to populate.
    Deployment::PlanConnectionDescription conn_desc;

    // Get plan instance and port IDS for connection.
    SA_POP::TaskInstID taskA = conn.first_task_inst;
    SA_POP::TaskInstID taskB = conn.second_task_inst;
    SA_POP::PortID taskAport = conn.first_port;
    SA_POP::PortID taskBport = conn.second_port;

    // Get deployment plan instances (port IDs are the same).
    InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
    CORBA::ULong taskAinst = taskA_iter->second;
    InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
    CORBA::ULong taskBinst = taskB_iter->second;

    // Create connection name.
    std::string conn_name = "";
    conn_name += itoa (taskAinst, buffer, 64);
    conn_name += "_";
    conn_name += taskAport;
    conn_name += "___";
    conn_name += itoa (taskBinst, buffer, 64);
    conn_name += "_";
    conn_name += taskBport;

    // Create connection endpoints.
    conn_desc.internalEndpoint.length (2);
    conn_desc.internalEndpoint[0].instanceRef = taskAinst;
    conn_desc.internalEndpoint[0].portName = CORBA::string_dup (taskAport.c_str ());
    conn_desc.internalEndpoint[0].provider = 1;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // (Get kind from data link in plan)
    conn_desc.internalEndpoint[0].kind = Deployment::EventPublisher;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    conn_desc.internalEndpoint[1].instanceRef = taskBinst;
    conn_desc.internalEndpoint[1].portName = CORBA::string_dup (taskBport.c_str ());
    conn_desc.internalEndpoint[1].provider = 0;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // (Get kind from data link in plan)
    conn_desc.internalEndpoint[1].kind = Deployment::EventConsumer;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    // Set connection name.
    conn_desc.name = CORBA::string_dup (conn_name.c_str ());

    // Add data link to opstring.
    opstring.dataLinks.length (conns_index + 1);
    opstring.dataLinks[conns_index] = conn_desc;

    // Increment connection index.
    conns_index++;
  }

  // Add scheduling links.
  CORBA::ULong links_index = 0;
  for (SA_POP::SchedLinkSet::iterator sched_iter = plan.sched_links.begin ();
        sched_iter != plan.sched_links.end (); sched_iter++)
  {
    SA_POP::SchedLink link = *sched_iter;

    // Link to populate.
    CIAO::RACE::OrderLinkDescription link_desc;

    // Set link type.
    link_desc.type = CIAO::RACE::SCHEDULING;

    // Get plan instance and port IDS for link.
    SA_POP::TaskInstID taskA = link.first;
    SA_POP::TaskInstID taskB = link.second;

    // Get deployment plan instances (port IDs are the same).
    InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
    CORBA::ULong taskAinst = taskA_iter->second;
    InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
    CORBA::ULong taskBinst = taskB_iter->second;

    // Create link name.
    std::string link_name = "";
    link_name += itoa (taskAinst, buffer, 64);
    link_name += "___";
    link_name += itoa (taskBinst, buffer, 64);

    // Create link endpoints.
    link_desc.internalEndpoint.length (2);
    link_desc.internalEndpoint[0].instanceRef = taskAinst;
    link_desc.internalEndpoint[0].provider = 1;
    link_desc.internalEndpoint[1].instanceRef = taskBinst;
    link_desc.internalEndpoint[1].provider = 0;

    // Set link name.
    link_desc.name = CORBA::string_dup (link_name.c_str ());

    // Add data link to opstring.
    opstring.orderLinks.length (links_index + 1);
    opstring.orderLinks[links_index] = link_desc;

    // Increment link index.
    links_index++;
  }

  // Add threat links.
  links_index = opstring.orderLinks.length ();
  for (SA_POP::ThreatLinkSet::iterator threat_iter = plan.threat_links.begin ();
        threat_iter != plan.threat_links.end (); threat_iter++)
  {
    SA_POP::ThreatLink link = *threat_iter;

    // Link to populate.
    CIAO::RACE::OrderLinkDescription link_desc;

    // Set link type.
    link_desc.type = CIAO::RACE::THREAT;

    // Get plan instance and port IDS for link.
    SA_POP::TaskInstID taskA = link.first;
    SA_POP::TaskInstID taskB = link.second;

    // Get deployment plan instances (port IDs are the same).
    InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
    CORBA::ULong taskAinst = taskA_iter->second;
    InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
    CORBA::ULong taskBinst = taskB_iter->second;

    // Create link name.
    std::string link_name = "";
    link_name += itoa (taskAinst, buffer, 64);
    link_name += "___";
    link_name += itoa (taskBinst, buffer, 64);

    // Create link endpoints.
    link_desc.internalEndpoint.length (2);
    link_desc.internalEndpoint[0].instanceRef = taskAinst;
    link_desc.internalEndpoint[0].provider = 1;
    link_desc.internalEndpoint[1].instanceRef = taskBinst;
    link_desc.internalEndpoint[1].provider = 0;

    // Set link name.
    link_desc.name = CORBA::string_dup (link_name.c_str ());

    // Add data link to opstring.
    opstring.orderLinks.length (links_index + 1);
    opstring.orderLinks[links_index] = link_desc;

    // Increment link index.
    links_index++;
  }

  // Add causal links.
  links_index = opstring.orderLinks.length ();
  for (SA_POP::CLSet::iterator cl_iter = plan.causal_links.begin ();
        cl_iter != plan.causal_links.end (); cl_iter++)
  {
    SA_POP::CausalLink link = *cl_iter;

    // Link to populate.
    CIAO::RACE::OrderLinkDescription link_desc;

    // Set link type.
    link_desc.type = CIAO::RACE::CAUSAL;

    // Get plan instance and port IDS for link.
    SA_POP::TaskInstID taskA = link.first;
    SA_POP::TaskInstID taskB = link.second;

    // Get deployment plan instances (port IDs are the same).
    InstToIndexMap::iterator taskA_iter = inst_to_index.find (taskA);
    CORBA::ULong taskAinst = taskA_iter->second;
    InstToIndexMap::iterator taskB_iter = inst_to_index.find (taskB);
    CORBA::ULong taskBinst = taskB_iter->second;

    // Create link name.
    std::string link_name = "";
    link_name += itoa (taskAinst, buffer, 64);
    link_name += "___";
    link_name += itoa (taskBinst, buffer, 64);

    // Create link endpoints.
    link_desc.internalEndpoint.length (2);
    link_desc.internalEndpoint[0].instanceRef = taskAinst;
    link_desc.internalEndpoint[0].provider = 1;
    link_desc.internalEndpoint[1].instanceRef = taskBinst;
    link_desc.internalEndpoint[1].provider = 0;

    // Set link name.
    link_desc.name = CORBA::string_dup (link_name.c_str ());

    // Add data link to opstring.
    opstring.orderLinks.length (links_index + 1);
    opstring.orderLinks[links_index] = link_desc;

    // Increment link index.
    links_index++;
  }

/*
  // Print out opstring instances.
  std::cout << "Opstring Instances:" << std::endl;
  if (opstring.instances.length () > 0) {
    for (CORBA::ULong i (0); i < opstring.instances.length (); ++i) {
      CORBA::String_var impl  = CORBA::string_dup (opstring.instances[i].suggestedImpl);
      std::cout << "Inst: " << i << " -> Impl: " << impl << std::endl;
    }
  }
  std::cout << std::endl;

  // Print out opstring connections.
  Deployment::PlanConnectionDescriptions check_conns = opstring.dataLinks;
  std::cout << "Opstring Connections:" << std::endl;
  for (CORBA::ULong i (0); i < check_conns.length (); ++i) {
    std::cout << "name " << check_conns[i].name << std::endl;
    std::cout << "InstA " << check_conns[i].internalEndpoint[0].instanceRef << "; ";
    std::cout << "PortA " << check_conns[i].internalEndpoint[0].portName << "; ";
    std::cout << "ProviderA " << check_conns[i].internalEndpoint[0].provider << "; ";
    std::cout << "KindA " << check_conns[i].internalEndpoint[0].kind << "; ";
    std::cout << std::endl << " -> " << std::endl;
    std::cout << "InstB " << check_conns[i].internalEndpoint[1].instanceRef << "; ";
    std::cout << "PortB " << check_conns[i].internalEndpoint[1].portName << "; ";
    std::cout << "ProviderB " << check_conns[i].internalEndpoint[1].provider << "; ";
    std::cout << "KindB " << check_conns[i].internalEndpoint[1].kind << "; ";
    std::cout << std::endl << std::endl;
  }
*/

  // Planner no longer needed.
  delete planner;

  SA_POP_DEBUG (SA_POP_DEBUG_NORMAL, "\nSA-POP END\n");


  try
    {
      ::CIAO::RACE::Converter converter (argc, argv);

      ::Deployment::DeploymentPlan deploy_plan;

      if (converter.convert (opstring, deploy_plan) == 0)
        {
          converter.deploy_plan (deploy_plan);
        }
      else
        {
          return -1;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) Unknown exception\n"));
      return -1;
    }

  return 0;
}
