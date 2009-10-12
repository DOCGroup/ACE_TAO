/**
 * @file CNPM_Handler.h
 *
 * $Id$
 */


#ifndef CIAO_CONFIG_HANDLERS_CNPM_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CNPM_HANDLER_H

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
      class CNWPriorityModelPolicyDef;

      /**
       * @class CNPM_Handler
       * @brief Handler class for client NetworkPriorityModelPolicyDefs
       */

      class RT_CCM_Handlers_Export CNPM_Handler
      {
      public:
        static bool cnw_priority_pd (const CNWPriorityModelPolicyDef &src,
          ::CIAO::DAnCE::CNWPriorityModelPolicyDef &dest);

        static CNWPriorityModelPolicyDef cnw_priority_pd (
          const ::CIAO::DAnCE::CNWPriorityModelPolicyDef &src);
      };
    }

}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CNPM_HANDLER_H */
