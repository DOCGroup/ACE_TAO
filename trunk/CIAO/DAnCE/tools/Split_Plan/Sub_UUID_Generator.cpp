// $Id$

#include "Sub_UUID_Generator.h"
#include <sstream>

namespace DAnCE
{
  Unique_Sub_UUID_Generator::Unique_Sub_UUID_Generator ()
    {
    }

  void Unique_Sub_UUID_Generator::generate_sub_uuid (
      const Deployment::DeploymentPlan &parent_plan,
      Deployment::DeploymentPlan &sub_plan,
      CORBA::ULong sub_plan_index)
    {
      std::ostringstream sub_uuid_str;
      sub_uuid_str << parent_plan.UUID.in () << "_" << sub_plan_index;
      sub_plan.UUID = CORBA::string_dup (sub_uuid_str.str ().c_str ());
    }

  Copy_UUID_Generator::Copy_UUID_Generator ()
    {
    }

  void Copy_UUID_Generator::generate_sub_uuid (
      const Deployment::DeploymentPlan &parent_plan,
      Deployment::DeploymentPlan &sub_plan,
      CORBA::ULong /*sub_plan_index*/)
    {
      sub_plan.UUID = CORBA::string_dup (parent_plan.UUID.in ());
    }
}
