// $Id$

#include "Node_Splitter.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  Node_Splitter::Node_Splitter (const Deployment::DeploymentPlan &plan)
    : plan_ (plan)
    {
    }

  bool Node_Splitter::match_instance (CORBA::ULong instance,
                                      const FILTER &filter) const
    {
      if (filter.is_empty ()) return true;

      return ACE_OS::strcmp (filter.c_str (),
                             this->plan_.instance[instance].node.in ()) == 0;
    }

  bool Node_Splitter::match_sub_plan (CORBA::ULong instance,
                                      const KEY &sub_plan_key) const
    {
      return ACE_OS::strcmp (sub_plan_key.c_str (),
                             this->plan_.instance[instance].node.in ()) == 0;
    }

  void Node_Splitter::prepare_sub_plan (CORBA::ULong instance,
                                        Deployment::DeploymentPlan &sub_plan,
                                        KEY &sub_plan_key)
    {
      if (ACE_OS::strcmp (sub_plan_key.c_str (),
                          this->plan_.instance[instance].node.in ()) != 0)
        {
          // set sub plan key to node name for instance
          sub_plan_key = this->plan_.instance[instance].node.in ();
        }

      if (ACE_OS::strlen (sub_plan.label.in ()) == 0)
        {
          // derive descriptive label
          ACE_CString sub_label ("Split plan from ");
          if (this->plan_.label.in ())
            {
              sub_label += this->plan_.label.in ();
            }
          else
            {
              sub_label += this->plan_.UUID.in ();
            }
          sub_label += " for Node ";
          sub_label += sub_plan_key;
          sub_plan.label = CORBA::string_dup (sub_label.c_str ());
        }
    }

  void Node_Splitter::finalize_sub_plan (Deployment::DeploymentPlan & /*sub_plan*/,
                                         KEY & /*sub_plan_key*/)
    {
      // nothing to do
    }
}
