// $Id$
#include "Exec_Dest_Status.h"

#include "dance/Deployment/Deployment_DeploymentPlanC.h"

namespace dance_test
{
  Exec_Dest_Status::Instance_Status::Instance_Status (void)
    : destructor_ (false)
  {
  }

  Exec_Dest_Status::Exec_Dest_Status (void)
    : instances_ ()
  {

  }

  Exec_Dest_Status::~Exec_Dest_Status (void)
  {

  }

  void
  Exec_Dest_Status::plan (const ::Deployment::DeploymentPlan *plan)
  {
    if (plan == 0)
      {
        ACE_ERROR ((LM_ERROR, "Error:  Nil plan passed to exec_dest_status\n"));
        return;
      }

    for (CORBA::ULong i = 0; i < plan->instance.length (); ++i)
      {
        this->instances_[plan->instance[i].name.in ()] = Instance_Status ();
      }
  }

  void
  Exec_Dest_Status::destructor_called (const std::string &id)
  {
    ACE_DEBUG ((LM_TRACE, "Got destructor invocation for id <%C>\n",
                id.c_str ()));
    this->instances_[id].destructor_ = true;
  }

  bool
  Exec_Dest_Status::check_destructor (const std::string &id)
  {
    return this->instances_[id].destructor_;
  }
}
