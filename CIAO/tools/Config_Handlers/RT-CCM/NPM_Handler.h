/**
 * @file NPM_Handler.h
 *
 * $Id$
 */


#ifndef CIAO_CONFIG_HANDLERS_NPM_HANDLER_H
#define CIAO_CONFIG_HANDLERS_NPM_HANDLER_H

#include /**/ "ace/pre.h"

#include "RT_CCM_Handlers_Export.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
    namespace Config_Handlers
    {
      class NWPriorityModelPolicyDef;

      /**
       * @class NPM_Handler
       * @brief Handler class for NetworkPriorityModelPolicyDefs
       */

      class RT_CCM_Handlers_Export NPM_Handler
      {
      public:
        static bool nw_priority_model_pd (const NWPriorityModelPolicyDef &src,
          ::CIAO::DAnCE::NWPriorityModelPolicyDef &dest);

        static NWPriorityModelPolicyDef nw_priority_model_pd (
          const ::CIAO::DAnCE::NWPriorityModelPolicyDef &src);
      };
    }

}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_NPM_HANDLER_H */
