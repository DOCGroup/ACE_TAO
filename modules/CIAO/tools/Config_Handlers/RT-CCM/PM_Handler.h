/**
 * @file PM_Handler.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */


#ifndef CIAO_CONFIG_HANDLERS_PM_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PM_HANDLER_H

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
      class PriorityModelPolicyDef;

      /**
       * @class PS_Handler
       * @brief Handler class for PriorityModelPolicyDefs
       */

      class RT_CCM_Handlers_Export PM_Handler
      {
      public:
        static bool priority_model_pd (const PriorityModelPolicyDef &src,
                                       ::CIAO::DAnCE::PriorityModelPolicyDef &dest);

        static PriorityModelPolicyDef priority_model_pd (const ::CIAO::DAnCE::PriorityModelPolicyDef &src);
      };
    }

}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFI_HANDLERS_PM_HANDLER_H */
