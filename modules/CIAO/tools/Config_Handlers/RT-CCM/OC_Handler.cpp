// $Id$

#include "OC_Handler.h"
#include "OR_Handler.h"
#include "PS_Handler.h"
#include "CIAOServerResources.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {

    bool OC_Handler::orb_config (
                                 const ORBConfigs &src,
                                 ::CIAO::DAnCE::ORBConfigs &dest)
    {
      if (src.resources_p ())
        OR_Handler::orb_resources (src.resources (), dest.orb_resources);

      CORBA::ULong pos (dest.policy_set.length ());

      dest.policy_set.length (pos + src.count_policySet ());

      for (ORBConfigs::policySet_const_iterator i = src.begin_policySet ();
           i != src.end_policySet ();
           ++i)
        {
          PS_Handler::policy_set (*i, dest.policy_set[pos++]);
        }

      return true;
    }


    ORBConfigs OC_Handler::orb_config (
                                       const ::CIAO::DAnCE::ORBConfigs &src)
    {
      ORBConfigs oc;

      oc.resources(OR_Handler::orb_resources (src.orb_resources));

      size_t len = src.policy_set.length();
      for(size_t i = 0; i < len; i++)
        {
          oc.add_policySet (PS_Handler::policy_set (src.policy_set [i]));
        }

      return oc;
    }
  }
}
