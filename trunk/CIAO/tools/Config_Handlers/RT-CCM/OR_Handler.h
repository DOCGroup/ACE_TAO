//=========================================================
/*
 *  @file OR_Handler.h
 *
 *  @author Lucas Seibert <lseibert@dre.vanderbilt.edu
 *  @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */
//=========================================================

#ifndef CIAO_CONFIG_HANDLERS_OR_HANDLER_H
#define CIAO_CONFIG_HANDLERS_OR_HANDLER_H

#include /**/ "ace/pre.h"

#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "RT_CCM_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Config_Handlers
  {
    class ORBResources;

    /* Class description
     * @class OR_Handler
     * @function A brief Handler class for the <ORBResource> member
     */

    class RT_CCM_Handlers_Export OR_Handler
    {
    public:

      static bool orb_resources (const ORBResources &src,
                                 ::CIAO::DAnCE::ORBResources &dest);

      class No_Resource {};

      static ORBResources orb_resources (const ::CIAO::DAnCE::ORBResources &src)
        throw (No_Resource);


    };
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_CONFIG_HANDLERS_OR_HANDLER_H */
