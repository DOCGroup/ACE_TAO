//$Id$

#include "Requirement_Handler.h"
#include "Property_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Property_Handler;

void
CIAO::Config_Handler::Requirement_Handler::process_Requirement (DOMNodeIterator * iter,
                                                                Deployment::Requirement &ret_struct)
{
  int valid_resourceType = 0, valid_property = 0, valid_name = 0;
  for (DOMNode * node = iter->nextNode (); 
       node != 0; 
       node = iter->nextNode ())
  {
    XStr name (node->getNodeName ());
    if (name == XStr (ACE_TEXT ("resourceType")))
    {
      valid_resourceType = 1;
      // Populate the structure
      ret_struct.resourceType = 
        CIAO::Config_Handler::Utils::parse_string (iter);
    }
    else if (name == XStr (ACE_TEXT ("property")))
    {
      Deployment::Properties_var properties = 0;
      ACE_NEW_THROW_EX (properties,
                        Deployment::Properties,
                        CORBA::NO_MEMORY ());
      properties->length (0);
      for (node = iter->nextNode ();
           name == XStr (ACE_TEXT ("property"));
           iter->nextNode ())
      {
        ::CORBA::ULong index = properties->length ();
        // Increment length of sequence
        properties->length (properties->length () + 1);

        // Deep copy the value
        CIAO::Config_Handler::Property_Handler::
          process_Property (iter, properties [index]);
      }
      valid_property = 1;

      // On exit go one step back to faciliate parsing next tag
      iter->previousNode ();

      // Copy sequence on to return struct
      ret_struct.property = properties;
    }
    else if (name == XStr (ACE_TEXT ("name")))
    {
      valid_name = 1;
      ret_struct.name = CIAO::Config_Handler::Utils::parse_string (iter);
    }
    else 
    {
      if (! valid_resourceType || 
          ! valid_property     ||
          ! valid_name)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Config_Handlers::Requirement_Handler::process_Requirement \
                     element mismatch expected <resourceType>, <property> or <name>"));
        ACE_THROW (CORBA::INTERNAL ());
      }

      // On exit go one step back as we have read one too many
      iter->previousNode ();
      break;
    }
  }
}
