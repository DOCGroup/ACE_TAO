//$Id$ 

#ifndef PROPERTY_HANDLER_C
#define PROPERTY_HANDLER_C

#include "Property_Handler.h"
#include "Any_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::Any_Handler;

Deployment::Property *
CIAO::Config_Handler::Property_Handler::process_Property (DOMNodeIterator * iter)
{
  Deployment::Property_var ret_struct = 0;
  ACE_NEW_THROW_EX (ret_struct,
                    Deployment::Property,
                    CORBA::NO_MEMORY ());

  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());
  if (name != XStr (ACE_TEXT ("name")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::Property_Handler::process_Property \
                   element mismatch expected <name>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct->name = Utils::parse_string (iter);

  // Process its value
  node = iter->nextNode ();
  name = node->getNodeName ();
  if (name != XStr (ACE_TEXT ("value")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::Property_Handler::process_Property \
                   element mismatch expected <value>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Process the value associated 
  ret_struct->value = Any_Handler::process_Any (iter);

  // Return structure
  return ret_struct._retn ();
}

#endif /* PROPERTY_HANDLER_C */
