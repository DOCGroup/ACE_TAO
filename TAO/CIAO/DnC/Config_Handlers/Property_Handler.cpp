//$Id$

#include "Property_Handler.h"
#include "Any_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::Any_Handler;

void
CIAO::Config_Handler::Property_Handler::process_Property (DOMNodeIterator * iter,
                                                          Deployment::Property &property)
{
  int valid_name = 0, valid_value = 0;
  for (DOMNode * node = iter->nextNode (); node != 0; node = iter->nextNode ())
    {
      //Check if the Schema IDs for both the elements match
      XStr name (node->getNodeName ());
      if (name == XStr (ACE_TEXT ("name")))
        {
          valid_name = 1;
          property.name = Utils::parse_string (iter);
        }
      else if (name == XStr (ACE_TEXT ("value")))
        {
          valid_value = 1;
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

                          // Process the value node present there
                          Any_Handler::process_Any (iter, property.value);
                        }
                    }
                }
            }
          else
            // Process the value associated
            Any_Handler::process_Any (iter, property.value);
        }
      else 
        {
          if (! valid_name || ! valid_value)
            {
              ACE_DEBUG ((LM_DEBUG, "Config_Handlers::Property_Handler::\
                          process_Property element mismatch expected <name> \
                          or <value>"));
              ACE_THROW (CORBA::INTERNAL ());
            }

          // Processed one element more go back one and exit
          iter->previousNode ();
          break;
        }
    }/* End Outer for */
}
