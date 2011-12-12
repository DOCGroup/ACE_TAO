//==============================================================
/**
 *  @file  ESD_Handler.h
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Common.h"
#include "tao/Basic_Types.h"
#include "Config_Handlers_Export.h"
#include "IDREF_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct EventServiceDeploymentDescription;
  class EventServiceDeploymentDescriptions;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class DeploymentPlan;
    class EventServiceDeploymentDescription;

   /*
    * @class ESD_Handler
    *
    * @brief Handler class for <EventServiceDeploymentDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC EventServiceDeploymentDescription objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */

    class Config_Handlers_Export ESD_Handler
    {
    public:
      static void es_deployment_descrs (
          const DeploymentPlan &src,
          ::Deployment::EventServiceDeploymentDescriptions& dest);

      static EventServiceDeploymentDescription es_deployment_descr (const Deployment::EventServiceDeploymentDescription &src);

      static IDREF_Base<CORBA::ULong> IDREF;

    private:
      static void es_deployment_descr (
          const EventServiceDeploymentDescription &src,
          ::Deployment::EventServiceDeploymentDescription &dest,
          CORBA::ULong pos);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IDD_HANDLER_H*/
