//======================================
/**
 *   @file    NetQoS_Handler.h
 *
 *   @author Lucas Seibert <lseibert@dre.vanderbilt.edu>
 *   @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id: NetQoS_Handler.h 72251 2006-04-21 16:29:20Z wotte $
 */
//======================================

#ifndef CIAO_CONFIG_HANDLERS_NETQOS_HANDLER_H
#define CIAO_CONFIG_HANDLERS_NETQOS_HANDLER_H
#include /**/ "ace/pre.h"

#include "NetQoS_Handler_Export.h"
#include "ciao/NetQoSC.h"
#include "ace/Auto_Ptr.h"
// #include "Utils/XML_Helper.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Config_Handlers
  {
    class NetQoSRequirementsDef;

    /*
     * @class NetQoS_Handler
     *
     * @brief Handler class for <NetQoSRequirementsDef
     */

    class NetQoS_Handler_Export NetQoS_Handler
    {
    public:
      class NoNetQoS {};

      NetQoS_Handler (const ACE_TCHAR *file);

      NetQoS_Handler(NetQoSRequirementsDef *netqos);

      NetQoS_Handler(::CIAO::DAnCE::NetQoSRequirement *netqos);

      ~NetQoS_Handler (void);

      ::CIAO::DAnCE::NetQoSRequirement const *netqos_idl (void) const
        throw (NoNetQoS);

      ::CIAO::DAnCE::NetQoSRequirement *netqos_idl (void)
        throw (NoNetQoS);

      NetQoSRequirementsDef const *netqos_xsc (void) const
        throw (NoNetQoS);

      NetQoSRequirementsDef *netqos_xsc (void)
        throw (NoNetQoS);

    private:
      bool build_netqos ();


      bool build_xsc ();

      auto_ptr < ::CIAO::DAnCE::NetQoSRequirement > idl_netqos_;

      auto_ptr <NetQoSRequirementsDef> netqos_;

      bool retval_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* NETQOS_HANDLER_H */
