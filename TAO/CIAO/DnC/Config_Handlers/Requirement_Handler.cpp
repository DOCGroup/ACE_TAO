//$Id$ 

#ifndef REQUIREMENT_HANDLER_C
#define REQUIREMENT_HANDLER_C

#include "Requirement_Handler.h"
#include "Property_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::Property_Handler;

Deployment::Requirement *
CIAO::Config_Handler::Requirement_Handler::process_Requirement (DOMNodeIterator * iter)
{
  Deployment::Requirement_var ret_struct = 0;
  ACE_NEW_THROW_EX (ret_struct,
                    Deployment::Requirement,
                    CORBA::NO_MEMORY ());

  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());

  if (name != XStr (ACE_TEXT ("resourceType")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::Requirement_Handler::process_Requirement \
                   element mismatch expected <resourceType>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct->resourceType = Utils::parse_string (iter);

  // Process <property> sequence 
  node = iter->nextNode ();
  name  = node->getNodeName ();

  Deployment::Properties_var properties = 0;
  ACE_NEW_THROW_EX (properties,
                    Deployment::Properties,
                    CORBA::NO_MEMORY ());
  properties->length (0);
  CORBA::ULong index = 0;
  for (node = iter->nextNode ();
       name == XStr (ACE_TEXT ("property"));
       iter->nextNode (), index++)
    {
       // Increment length of sequence
       properties->length (properties->length () + 1);

       // Deep copy the value
       Deployment::Property_var property_temp = 
          Property_Handler::process_Property (iter);
       properties [index] = property_temp;

       // Get next node
       name = node->getNodeName ();
    }

  // On exit go one step back to faciliate parsing next tag
  iter->previousNode ();

  // Copy sequence on to return struct
  ret_struct->property = properties;

  // Return structure
  return ret_struct._retn ();
}

#endif /* REQUIREMENT_HANDLER_C */
