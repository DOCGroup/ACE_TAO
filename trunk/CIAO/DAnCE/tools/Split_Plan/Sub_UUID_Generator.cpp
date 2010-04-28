// $Id$

#include "Sub_UUID_Generator.h"
#include "ace/UUID.h"
#include "ace/Auto_Ptr.h"

namespace DAnCE
{
  Unique_Sub_UUID_Generator::Unique_Sub_UUID_Generator ()
    {
    }

  void Unique_Sub_UUID_Generator::generate_sub_uuid (
      const Deployment::DeploymentPlan &parent_plan,
      Deployment::DeploymentPlan &sub_plan)
    {
      auto_ptr <ACE_Utils::UUID> uuid (ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID ());
      ACE_CString sub_uuid_str (parent_plan.UUID.in ());
      sub_uuid_str += "[";
      sub_uuid_str += uuid->to_string ()->c_str ();
      sub_uuid_str += "]";
      sub_plan.UUID = CORBA::string_dup (sub_uuid_str.c_str ());
    }

  Copy_UUID_Generator::Copy_UUID_Generator ()
    {
    }

  void Copy_UUID_Generator::generate_sub_uuid (
      const Deployment::DeploymentPlan &parent_plan,
      Deployment::DeploymentPlan &sub_plan)
    {
      sub_plan.UUID = CORBA::string_dup (parent_plan.UUID.in ());
    }

};
