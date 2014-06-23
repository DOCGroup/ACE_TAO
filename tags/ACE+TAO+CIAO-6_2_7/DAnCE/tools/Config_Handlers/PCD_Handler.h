//==============================================================
/**
 *  @file  PCD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_PCD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PCD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"
#include "Utils/Functors.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct PlanConnectionDescription;
  class PlanConnectionDescriptions;
}

namespace DAnCE
{

  namespace Config_Handlers
  {

    class PlanConnectionDescription;

    /*
     * @class PCD_Handler
     *
     * @brief Handler class for <PlanConnectionDescription> types.
     *
     * This class defines handler methods to map values from
     * XSC PlanConnectionDescription objects, parsed from
     * the descriptor files, to the corresponding CORBA IDL type.
     *
     */

    class Config_Handlers_Export PCD_Handler{

    public:

      PCD_Handler (void);
      virtual ~PCD_Handler (void);

      ///This method takes a <Deployment::PlanConnectionDescription>
      ///and maps the values from the passed in XSC
      ///PlanConnectionDescription to its members.
      static void handle_PlanConnectionDescription (const PlanConnectionDescription& desc,
                                          ::Deployment::PlanConnectionDescription& toconfig);

      //This method takes a <Deployment::PlanConnectionDescription>
      //converts it into a <Config_Handler::PlanConnectionDescription>
      //and returns the value
      static PlanConnectionDescription
        get_PlanConnectionDescription (const Deployment::PlanConnectionDescription &src);
    };

    typedef Sequence_Handler < PlanConnectionDescription,
                               ::Deployment::PlanConnectionDescriptions,
                               ::Deployment::PlanConnectionDescription,
                               PCD_Handler::handle_PlanConnectionDescription > PCD_Functor;

  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_PCD_HANDLER_H*/
