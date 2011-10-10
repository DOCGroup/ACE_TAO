//================================================
/**
 *  @file  STD_IAD_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_STD_IAD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_STD_IAD_HANDLER_H

#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ImplementationArtifactDescription;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class ImplementationArtifactDescription;

    /*
     * @class STD_IAD_Handler
     *
     * @brief Handler class for <CCMImplementationArtifactDescription> types.
     *
     * This class defines handler methods to map values from
     * XSC objects, parsed from the descriptor files, to the
     * corresponding CORBA IDL type for the schema element.
     *
     */
    class Config_Handlers_Export STD_IAD_Handler
    {

    public:
      /// Maps the values from the XSC object
      /// <ImplementationArtifactDescription> to the CORBA IDL type
      /// <Deployment::ImplementationArtifactDescription>.
     static bool
     impl_artifact_descr (
       const ImplementationArtifactDescription &desc,
       ::Deployment::ImplementationArtifactDescription &toconfig);

//@@Note: The reverse part has yet to be handed in the future
//     ImplementationArtifactDescription
//     impl_artifact_descr (
//         const Deployment::ComponentPortDescription& src);
    };
 }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_STD_IAD_HANDLER_H*/
