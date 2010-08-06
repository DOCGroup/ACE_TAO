//======================================
/**
 *   @file    SRD_Handler.h
 *
 *   @author Lucas Seibert <lseibert@dre.vanderbilt.edu>
 *   @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */
//======================================

#ifndef CIAO_CONFIG_HANDLERS_SRD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_SRD_HANDLER_H
#include /**/ "ace/pre.h"

#include "RT_CCM_Handlers_Export.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Config_Handlers
  {
    class ServerResourcesDef;

    /*
     * @class SRD_Handler
     *
     * @brief Handler class for <ServerResourcesDef
     */

    class RT_CCM_Handlers_Export SRD_Handler
    {
    public:
      class NoSRD {};

      SRD_Handler (const ACE_TCHAR *file);

      SRD_Handler(ServerResourcesDef *srd);

      SRD_Handler(::CIAO::DAnCE::ServerResource *srd);

      ~SRD_Handler (void);

      ::CIAO::DAnCE::ServerResource const *srd_idl (void) const;

      ::CIAO::DAnCE::ServerResource *srd_idl (void);

      ServerResourcesDef const *srd_xsc (void) const;

      ServerResourcesDef *srd_xsc (void);

    private:
      bool build_srd ();


      bool build_xsc ();

      auto_ptr < ::CIAO::DAnCE::ServerResource > idl_srd_;

      auto_ptr <ServerResourcesDef> srd_;

      bool retval_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* SRD_HANDLER_H */
