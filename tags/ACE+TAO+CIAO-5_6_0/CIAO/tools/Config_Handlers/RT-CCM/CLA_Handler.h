//=======================================================
/* @file    CLA_Handler.h
 *
 * @author  Lucas Seibert <lseibert@dre.vanderbilt.edu
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */
//=======================================================

#ifndef CIAO_CONFIG_HANDLERS_CLA_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CLA_HANDLER_H

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

    class ServerResourcesDef;
    class ServerCmdlineOptions;

    /*
     * class CLA_Handler
     * This is a brief Handler class for <CommandlineArg>
     */

    class RT_CCM_Handlers_Export CLA_Handler
    {
    public:
      static bool
      commandline_arguments (
                             const ServerResourcesDef &src,
                             ::CIAO::DAnCE::CommandlineArgs &dest);

      static ServerCmdlineOptions
      commandline_argument (const ::CIAO::DAnCE::CommandlineArgs &src);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CLA_HANDLER_H */
