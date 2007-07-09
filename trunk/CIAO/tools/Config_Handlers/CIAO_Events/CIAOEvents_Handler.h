//================================================
/**
 *  @file  CIAOEvents_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_CIAOEvents_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CIAOEvents_HANDLER_H

#include /**/ "ace/pre.h"

#include "Utils/XML_Helper.h"
#include "DAnCE/Deployment/Deployment_EventsC.h"
#include "CIAO_Events_Handlers_Export.h"
#include "DAnCE/Deployment/DeploymentC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO
{
  namespace Config_Handlers
  {
    class CIAOEventsDef;

    /*
     * @class CIAOEvents_Handler
     *
     * @brief Handler class for <CIAOEvents> types.
     *
     * This class defines handler methods to map values from
     * XSC objects, parsed from the descriptor files, to the
     * corresponding CORBA IDL type for the schema element.
     *
     */
    class CIAO_Events_Handlers_Export CIAOEvents_Handler
    {

    public:
      class NoESD {};

      CIAOEvents_Handler (const ACE_TCHAR *file);

      CIAOEvents_Handler(CIAOEventsDef *esd);

      ~CIAOEvents_Handler (void);

      CIAO::DAnCE::EventServiceDeploymentDescriptions const *esd_idl (void) const;

      CIAO::DAnCE::EventServiceDeploymentDescriptions *esd_idl (void);

    private:
      bool build_esd ();

      auto_ptr <CIAO::DAnCE::EventServiceDeploymentDescriptions > idl_esd_;

      auto_ptr <CIAOEventsDef> esd_;

      bool retval_;

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CIAOEvents_HANDLER_H*/
