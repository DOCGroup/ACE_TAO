//$Id$

#include "RS_Handler.h"
#include "SP_Handler.h"
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "Utils.h"

void
CIAO::Config_Handler::RS_Handler::
process_RequirementSatisfier (DOMNodeIterator * iter,
                              Deployment::RequirementSatisfier &ret_struct)
{
  // boolean to represent valid <RequirementSatisfier> definition
  int valid_name = 0, valid_resourceType = 0, valid_property  = 0;

  for (DOMNode * node = iter->nextNode (); ; node = iter->nextNode ())
    {
      XStr name (node->getNodeName ());
      if (name == XStr (ACE_TEXT ("name")))
        {
          ret_struct.name = CIAO::Config_Handler::Utils::parse_string (iter);
          valid_name = 1;
        }
      else if (name == XStr (ACE_TEXT ("resourceType")))
        {
          valid_resourceType = 1;
          // Requirement: Atleast one <resourceType> definition present
          ::CORBA::StringSeq_var res_seq = 0;
          ACE_NEW_THROW_EX (res_seq,
                            ::CORBA::StringSeq (1),
                            CORBA::NO_MEMORY ());
          res_seq->length (0);

          while (name == XStr (ACE_TEXT ("resourceType")))
            {
              ::CORBA::ULong index = res_seq->length ();
              res_seq->length (res_seq->length () + 1);
              res_seq [index] = 
                CIAO::Config_Handler::Utils::parse_string (iter);
              name = node->getNodeName ();
            }

          // On exit go one step back to faciliate parsing next tag
          iter->previousNode ();

          // Assign this sequence to the Return Structure
          ret_struct.resourceType = res_seq;
        }

      else if (name == XStr (ACE_TEXT ("property")))
        {
          valid_property = 1;
          Deployment::SatisfierProperties_var property_seq = 0;
          DOMNodeIterator * property_iter = 0;
          ACE_NEW_THROW_EX (property_seq,
                            Deployment::SatisfierProperties,
                            CORBA::NO_MEMORY ());
          property_seq->length (0);
          while (name == XStr (ACE_TEXT ("property")))
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
                          XMLString::release (attr_node_value_ch);
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
                } 
              else
                property_iter = iter;

              // Copy the property onto the sequence
              ::CORBA::ULong index = property_seq->length ();
              property_seq->length (property_seq->length () + 1);
              CIAO::Config_Handler::SP_Handler::
                process_SatisfierProperty (property_iter,
                                           property_seq [index]);

              // Get next node
              iter->nextNode ();
              name = node->getNodeName ();
            }

          // On exit go one step back to faciliate parsing next tag
          iter->previousNode ();

          // Copy the sequence to return structure
          ret_struct.property = property_seq;
        }
      else
        {
          if (! valid_name || ! valid_resourceType || ! valid_property)
            ACE_DEBUG ((LM_DEBUG, "Config_Handlers::RS_Handler::process_ \
                        RequirementSatisfier element mismatch expected <name>,\
                        <resourceType> <property>\n"));
          // On exit go one step back as one might have processed the next 
          // valid tag
          iter->previousNode ();

          // Exit out of the for loop
          break;
        }
    }
}
