
      //==============================================================
/**
 *  @file  MDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_MDD_Handler_H
#define CIAO_CONFIG_HANDLERS_MDD_Handler_H
#include /**/ "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Config_Handlers_export.h"
#include "ace/config-lite.h"


namespace Deployment
{
  class MonolithicDeploymentDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class MonolithicDeploymentDescription;

    /*
    * @class MDD_Handler
    *
    * @brief Handler class for <MonolithicDeploymentDescription> types.
    *
    * This class defines handler methods to map values from XSC
    * MonolithicDeploymentDescription objects, parsed from the
    * descriptor files, to the corresponding CORBA IDL types.
    *
    */

    class Config_Handlers_Export MDD_Handler
    {
    public:

      MDD_Handler (void);
      virtual ~MDD_Handler (void);

      void get_MonolithicDeploymentDescription (
          Deployment::MonolithicDeploymentDescription& toconfig,
          MonolithicDeploymentDescription& desc);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_MDD_Handler_H */
