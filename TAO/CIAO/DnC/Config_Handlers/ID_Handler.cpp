//$Id$

#ifndef ID_HANDLER_C
#define ID_HANDLER_C

#include "ID_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

Deployment::ImplementationDependency *
CIAO::Config_Handler::ID_Handler::process_ImplementationDependency (DOMNodeIterator * iter)
{
  Deployment::ImplementationDependency_var ret_struct = 0;
  ACE_NEW_THROW_EX (ret_struct,
                    Deployment::ImplementationDependency,
                    CORBA::NO_MEMORY ());

  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());

  if (name != XStr (ACE_TEXT ("elementType")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::ID_Handler::process_ImplDependency \
                   element mismatch expected <elementType>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct->requiredType = Utils::parse_string (iter);

  // Return structure
  return ret_struct._retn ();
}

#endif /* ID_HANDLER_C */
