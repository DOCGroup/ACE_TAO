
#include "NM_Monitor_Impl.h"
#include "MonitorController.h"
#include "NodeManager_Impl.h"
#include <fstream>
#include "ace/High_Res_Timer.h"

// Implementation skeleton constructor
Onl_Monitor_NM_Monitor_i::
Onl_Monitor_NM_Monitor_i (::CIAO::MonitorController* controller,
                          ::CIAO::NodeManager_Impl_Base* node_mgr)
  : controller_ (controller),
    node_manager_ (node_mgr)
{
}

// Implementation skeleton destructor
Onl_Monitor_NM_Monitor_i::~Onl_Monitor_NM_Monitor_i (void)
{
}

void Onl_Monitor_NM_Monitor_i::monitor_app_QoS (
    const ::Deployment::DeploymentPlan & plan)
{
  ACE_DEBUG ((LM_DEBUG, "\t\nInside the monitor_app_QoS\n"));

  auto_ptr<Deployment::Domain> domain =
      controller_->get_initial_domain ();

  ACE_DEBUG ((LM_DEBUG, "\t\nAfter the get_initial_domain\n"));
  ACE_CString key (plan.UUID.in ());
  key += "@";
  key += domain->node[0].name.in ();

  ACE_DEBUG ((LM_DEBUG, "The key is %s\n", key.c_str ()));

  try {
    CORBA::Object_var obj = node_manager_->get_NAM (key);

    Deployment::NodeApplicationManager_var nam =
      Deployment::NodeApplicationManager::_narrow (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "Making a call to monitor qos %s\n", key.c_str ()));
    obj = nam->monitor_qos (plan);
    ACE_DEBUG ((LM_DEBUG, "AFter Making a call to monitor qos \%s\n", key.c_str ()));

    this->qos_monitor_ = Onl_Monitor::App_Monitor::_narrow (obj);
  }
  catch (CORBA::Exception& ex)
    {

      ex._tao_print_exception ("(%P|%t) NM_Monitor_i::monitor_app_QoS ()\t\n");
      throw;
    }
}

::Deployment::Domain * Onl_Monitor_NM_Monitor_i::get_resource_data (
    void)
{
  // Add your implementation here

  ACE_DEBUG ((LM_DEBUG, "Inside the get_resource_data\n"));
  Deployment::Domain_var domainv;

  // time stamp the call to measure overhead of monitoring ...

  ACE_High_Res_Timer time;
  time.start ();

  domainv = (this->controller_->update_data_for_TM ());

  ::Deployment::QoSSpecifications_var qos_seq = this->qos_monitor_->get_app_QoS ();

  time.stop ();

  domainv->node[0].qos_seq = qos_seq;

  //  ACE_Time_Value tv;
  //time.elapsed_time (tv);

  ACE_hrtime_t tm;
  time.elapsed_microseconds (tm);

  std::string file_prox = domainv->node[0].name.in ();

  file_prox += "_Tprox";

  std::ofstream out (file_prox.c_str (), ios::app);
  //out << tv.msec () << std::endl;
  out << tm << std::endl;
  out.close ();

  return domainv._retn ();
}

void Onl_Monitor_NM_Monitor_i::register_app_monitor (
    ::Onl_Monitor::App_Monitor_ptr monitor)
{
  // Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Inside the register_app_monitor\n"));
}
