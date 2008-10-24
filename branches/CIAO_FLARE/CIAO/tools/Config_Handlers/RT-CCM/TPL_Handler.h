//===============================================
/*
 *   @file TPL_Handler.h
 *
 *   @author Lucas Seibert <lseibert@dre.vanderbilt.edu>
 *   @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 *  $Id$
 */
//===============================================

#ifndef CIAO_CONFIG_HANDLERS_TPL_HANDLER_H
#define CIAO_CONFIG_HANDLERS_TPL_HANDLER_H

#include /**/ "ace/pre.h"

#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"

#include "RT_CCM_Handlers_Export.h"

namespace CIAO
{
    namespace Config_Handlers
    {
    class ThreadpoolWithLanesDef;

    /*
     *  @class TPL_Handler
     *  A brief Handler class for <ThreadpoolWithLanesDef>
     */

    class RT_CCM_Handlers_Export TPL_Handler
    {
    public:

        static bool threadpool_with_lanes (
        const ThreadpoolWithLanesDef &src,
        ::CIAO::DAnCE::ORS_ThreadpoolWithLanes &dest);

        static ThreadpoolWithLanesDef threadpool_with_lanes (
        const ::CIAO::DAnCE::ORS_ThreadpoolWithLanes &src);
    };
    }
}

#include /**/ "ace/post.h"
#endif /* TPL_HANDLER_H */

