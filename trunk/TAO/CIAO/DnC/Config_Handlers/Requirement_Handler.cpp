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
              // Increment length of sequence
              ::CORBA::ULong index = properties->length ();
              properties->length (properties->length () + 1);
              DOMNodeIterator * property_iter = 0;

              // Check for <href> tags
              if (node->hasAttributes ())
                {
                  xercesc::DOMNamedNodeMap * named_node_map =
                    node->getAttributes ();
                  int length = named_node_map->getLength ();

                  if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode * attr_node = named_node_map->item (j);
                          XStr attr_node_name = attr_node->getNodeName ();
                          ACE_TString attr_node_value =
                            XMLString::transcode (attr_node->getNodeValue ());
                          if (attr_node_name = XStr (ACE_TEXT ("href")))
                            {
                              XMLURL url (attr_node_value.c_str ());
                              DOMNodeIterator * value_iter =
                                Utils::parse_href_tag (url, 
                                                       node->getOwnerDocument ());
                              // Get to the root-node
                              value_iter->nextNode ();

                              // Process the property href tag
                              property_iter = value_iter;
                            }
                        }
                    }
                  else
                    property_iter = iter;

                  // Deep copy the value
                  CIAO::Config_Handler::Property_Handler::
                    process_Property (property_iter, properties [index]);
                }  
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
                          "Config_Handlers::Requirement_Handler::process_\
                          Requirement element mismatch expected <resourceType>\
                          , <property> or <name>"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          // On exit go one step back as we have read one too many
          iter->previousNode ();
          break;
        }
    }
}
