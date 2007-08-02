//==============================================
/*
 *     @file TP_Handler.h
 *
 *     @author Lucas Seibert <lseibert@dre.vanderbilt.edu>
 *     @author Will Otte <wotte@dre.vanderbilt.edu>
 *  $Id$
 */
//==============================================

#ifndef CIAO_CONFIG_HANDLERS_TP_HANDLER_H
#define CIAO_CONFIG_HANDLERS_TP_HANDLER_H

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
    class ThreadpoolDef;

    /*
     * @class TP_Handler
     * A brief Handler class for <ThreadpoolDef>
     */

    class RT_CCM_Handlers_Export TP_Handler
    {
    public:
      static bool thread_pool (const ThreadpoolDef &src,
                               ::CIAO::DAnCE::ORS_Threadpool &dest);

      static ThreadpoolDef thread_pool (const ::CIAO::DAnCE::ORS_Threadpool &src);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TP_HANDLER_H */
