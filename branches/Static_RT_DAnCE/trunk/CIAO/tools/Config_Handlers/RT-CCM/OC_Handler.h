//============================================
/*
 *    @file     OC_Handler.h
 *
 *    @author   Lucas Seibert <lseibert@dre.vanderbilt.edu>
 *    @author   Will Otte <wotte@dre.vanderbilt.edu>

 * $Id$
 */
//============================================

#ifndef CIAO_CONFIG_HANDLERS_OC_HANDLER_H
#define CIAO_CONFIG_HANDLERS_OC_HANDLER_H

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
    class ORBConfigs;

    /*
     * @class OC_Handler
     *
     * @brief Handler class for <ORBConfigs>
     */

    class RT_CCM_Handlers_Export OC_Handler
    {
    public:
      static bool orb_config (const ORBConfigs &src,
                              ::CIAO::DAnCE::ORBConfigs &dest);

      static ORBConfigs orb_config (const ::CIAO::DAnCE::ORBConfigs &src);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* OC_HANDLER_H */

