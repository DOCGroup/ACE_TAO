/**
 * @file CB_Handler.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */



#ifndef CIAO_CONFIG_HANDLERS_CB_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CB_HANDLER_H

#include /**/ "ace/pre.h"

#include "RT_CCM_Handlers_Export.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "IDREF_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Config_Handlers
  {
    class ConnectionBandsDef;

    /*
     * @class CB_Handler
     * A brief Handler class for <ConnectionBands>
     */

    class RT_CCM_Handlers_Export CB_Handler
    {
    public:
      static bool connection_band (const ConnectionBandsDef &src,
                                   ::CIAO::DAnCE::ORS_ConnectionBands &dest);

      static ConnectionBandsDef connection_band (const ::CIAO::DAnCE::ORS_ConnectionBands &src);

      //          static IDREF_Base<::CIAO::DAnCE::ORS_ConnectionBands> IDREF;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CB_HANDLER_H */
