//$Id$

#ifndef CEPE_HANDLER_C
#define CEPE_HANDLER_C

#include "CEPE_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

::Deployment::ComponentExternalPortEndpoint *
CIAO::Config_Handler::CEPE_Handler::process_ComponentExternalPortEndpoint (DOMNodeIterator * iter)
{
  ::Deployment::ComponentExternalPortEndpoint_var ret_struct = 0;
  ACE_NEW_RETURN (ret_struct,
                  ::Deployment::ComponentExternalPortEndpoint (),
                  NULL);

  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());

  if (name != XStr (ACE_TEXT ("portName")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::ID_Handler::process_ImplDependency \
                   element mismatch expected <portName>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct->portName = Utils::parse_string (iter);

  // Return structure
  return ret_struct._retn ();
}

#endif /* CEPE_HANDLER_C */
