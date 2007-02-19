/**
 * @file PS_Handler.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 *  $Id$
 */


#ifndef CIAO_CONFIG_HANDLERS_PS_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PS_HANDLER_H

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
      class PolicySet;

      /**
       * @class PS_Handler
       * @brief Handler class for PolicySets
       */

      class RT_CCM_Handlers_Export PS_Handler
      {
      public:
        static bool policy_set (const PolicySet &src,
                                ::CIAO::DAnCE::PolicySet &dest);

        static PolicySet policy_set (const ::CIAO::DAnCE::PolicySet &src);
      };

    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFI_HANDLERS_PS_HANDLER_H */
