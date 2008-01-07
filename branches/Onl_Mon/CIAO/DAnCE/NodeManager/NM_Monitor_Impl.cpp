#include "NM_Monitor_Impl.h"
#include "MonitorController.h"
#include "NodeManager_Impl.h"
#include <fstream>
#include "ace/High_Res_Timer.h"

// To set RT params.
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"

// Implementation skeleton constructor
Onl_Monitor_NM_Monitor_i::
Onl_Monitor_NM_Monitor_i (::CIAO::MonitorController* controller,
                          ::CIAO::NodeManager_Impl_Base* node_mgr)
  : controller_ (controller),
    node_manager_ (node_mgr)
{
      int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "NM_Monitor_Impl (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "NM_Monitor_Impl (%P|%t): sched_params failed\n"));
    }
  ACE_DEBUG ((LM_DEBUG,
              "NM_Monitor_Impl (%P|%t): Done setting RT Sched params!\n"));

}

// Implementation skeleton destructor
Onl_Monitor_NM_Monitor_i::~Onl_Monitor_NM_Monitor_i (void)
{
}

//void Onl_Monitor_NM_Monitor_i::monitor_app_QoS (
//    const ::Deployment::DeploymentPlan & plan)
void Onl_Monitor_NM_Monitor_i::monitor_app_QoS (
    const ::Onl_Monitor::Plan_Seq & plans)
{
  //  ACE_DEBUG ((LM_DEBUG, "\t\nInside the monitor_app_QoS\n"));

  for (CORBA::ULong i = 0;i < plans.length ();i++)
  {
    ::Deployment::DeploymentPlan plan = plans[i];
    auto_ptr<Deployment::Domain> domain =
        controller_->get_initial_domain ();

  //  ACE_DEBUG ((LM_DEBUG, "\t\nAfter the get_initial_domain\n"));
    ACE_CString key (plan.UUID.in ());
    key += "@";
    key += domain->node[0].name.in ();

    ACE_DEBUG ((LM_DEBUG, "The key is %s\n", key.c_str ()));

    CORBA::Object_var obj;

    try {
	   obj = node_manager_->get_NAM (key);
    }
    catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception ("(%P|%t) NM_Monitor_i::monitor_app_QoS ()\t\n");
      ACE_DEBUG ((LM_DEBUG, "TM_DAEMON::Does not have a Component in this Node"));
      continue;
    }

    try {
      Deployment::NodeApplicationManager_var nam =
        Deployment::NodeApplicationManager::_narrow (obj.in ());

      ACE_DEBUG ((LM_DEBUG, "Making a call to monitor qos %s\n", key.c_str ()));
      obj = nam->monitor_qos (plan);

      if (obj == NULL)
        continue;

      ACE_DEBUG ((LM_DEBUG, "AFter Making a call to monitor qos \%s\n", key.c_str ()));

      Onl_Monitor::App_Monitor_var qos_monitor;
      qos_monitor = Onl_Monitor::App_Monitor::_narrow (obj);

      this->qos_monitor_seq_.push_back (qos_monitor);
    }
    catch (CORBA::Exception& ex)
    {

      ex._tao_print_exception ("(%P|%t) NM_Monitor_i::monitor_app_QoS ()\t\n");
      throw;
    }
  }
}

::Deployment::Domain * Onl_Monitor_NM_Monitor_i::get_resource_data (
    void)
{
  //  ACE_DEBUG ((LM_DEBUG, "Inside the get_resource_data\n"));
  Deployment::Domain_var domainv = this->controller_->update_data_for_TM ();

 // now get the qos data from each NAM
  domainv->node[0].qos_seq.length (this->qos_monitor_seq_.size ());

  for (unsigned int i = 0;i < qos_monitor_seq_.size ();i++)
  {
    ::Deployment::QoSSpecifications_var qos_seq =
	    this->qos_monitor_seq_[i]->get_app_QoS ();
    domainv->node[0].qos_seq[i] = (*qos_seq)[0];
  }
  return domainv._retn ();
}

void Onl_Monitor_NM_Monitor_i::register_app_monitor (
    ::Onl_Monitor::App_Monitor_ptr monitor)
{
  // Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Inside the register_app_monitor\n"));
}
