// $Id$

#include "PM_Handler.h"
#include "CIAOServerResources.hpp"
#include "ace/Auto_Ptr.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    bool
    PM_Handler::priority_model_pd (const PriorityModelPolicyDef &src,
                                   ::CIAO::DAnCE::PriorityModelPolicyDef &dest)
    {
      switch (src.priority_model ().integral ())
        {
        case PriorityModel::SERVER_DECLARED_l:
          dest.priority_model = ::CIAO::DAnCE::SERVER_DECLARED;
          break;

        case PriorityModel::CLIENT_PROPAGATED_l:
          dest.priority_model = ::CIAO::DAnCE::CLIENT_PROPAGATED;
          break;

        default:
          ACE_ERROR ((LM_ERROR,
                      "Unknown priority model passed to priority_model_pd\n"));
          return false;
        }

      if (src.server_priority_p ())
        dest.server_priority = static_cast < ACE_INT32 > (src.server_priority ());
      else
        dest.server_priority = 0;

      return true;
    }


    PriorityModelPolicyDef
    PM_Handler::priority_model_pd (const ::CIAO::DAnCE::PriorityModelPolicyDef &src)
    {
      auto_ptr <PriorityModelPolicyDef> pmd;

      switch (src.priority_model)
        {
        case ::CIAO::DAnCE::CLIENT_PROPAGATED:
          pmd.reset (new PriorityModelPolicyDef (PriorityModel::CLIENT_PROPAGATED));
          break;

        case ::CIAO::DAnCE::SERVER_DECLARED:
          pmd.reset (new PriorityModelPolicyDef (PriorityModel::SERVER_DECLARED));
          break;

        default:
          ACE_ERROR ((LM_ERROR,
                      "Invalid priority model given to priority_model_pd\n"));
          throw 1;
        }

      pmd->server_priority (XMLSchema::int_ (src.server_priority));

      return *pmd;
    }


  }



}

