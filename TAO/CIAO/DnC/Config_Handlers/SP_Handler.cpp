//$Id$

#include "SP_Handler.h"
#include "Any_Handler.h"
#include "SPK_Handler.h"
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::Any_Handler;

void
CIAO::Config_Handler::SP_Handler::
process_SatisfierProperty (DOMNodeIterator * iter,
                           Deployment::SatisfierProperty &property)
{
  for (DOMNode * node = iter->nextNode (); ; node = iter->nextNode ())
    {
      // Get name of the current element
      XStr name (node->getNodeName ());

      if (name == XStr (ACE_TEXT ("name")))
        property.name = Utils::parse_string (iter);
      else if (name == XStr (ACE_TEXT ("kind")))
        SPK_Handler::process_SatisfierPropertyKind (iter, property.kind);
      else if (name == XStr (ACE_TEXT ("value")))
        {
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
                      char*  attr_node_value_ch =
                        XMLString::transcode (attr_node->getNodeValue ());
                      ACE_TString attr_node_value = attr_node_value_ch;
                      auto_ptr<char> cleanup_char (attr_node_value_ch);
                      if (attr_node_name = XStr (ACE_TEXT ("href")))
                        {
                          XMLURL url (attr_node_value.c_str ());
                          DOMNodeIterator * value_iter =
                            Utils::parse_href_tag (url, 
                                                   node->getOwnerDocument ());

                          // Get to the root-node
                          value_iter->nextNode ();

                          // Process the property href tag
                          Any_Handler::process_Any (value_iter, 
                                                    property.value);
                        }
                    }
                }
            }
          else
            // Populate value for the Property
            Any_Handler::process_Any (iter, property.value);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Config_Handlers::SP_Handler::process_ \
                      RequirementSatisfier element mismatch expected <name> \
                      or <PropertyKind> <Deployment:Any>"));

          // Get the previous iterator
          iter->previousNode ();

          // Exit from the for loop
          break;
        }
    }
}
