//==================================================================
/**
 *  @file Process_Element.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "Process_Element.h"
#include "ace/Auto_Ptr.h"
#include "Utils.h"

/*
 *  Process references
 */

void
process_refs(DOMNode*& node,
             CORBA::ULongSeq& seq,
             int& index,
             IDREF_MAP& idref_map)
{
  CORBA::ULong i (seq.length ());
  seq.length (i + 1);
  seq[i] = index;
  if (node->hasAttributes())
    {
      DOMNamedNodeMap* named_node_map = node->getAttributes ();
      //auto_ptr<DOMNamedNodeMap> cleanup_nodemap (named_node_map);
      
      int length = named_node_map->getLength ();
      
      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          char* aceattrnodevalue_ch = XMLString::transcode
            (attribute_node->getNodeValue ());
          ACE_TString aceattrnodevalue = aceattrnodevalue_ch;
          XMLString::release (&aceattrnodevalue_ch);
          if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              //index = index + 1;
              idref_map.bind (index, aceattrnodevalue);
            }
        }
    }
}

void
process_ref(DOMNode*& node,
            CORBA::ULong& ref,
            int& index,
            IDREF_MAP& idref_map)
{
  ref = index;
  if (node->hasAttributes ())
    {
      DOMNamedNodeMap* named_node_map = node->getAttributes ();
      //auto_ptr<DOMNamedNodeMap> cleanup_nodemap (named_node_map);
      
      int length = named_node_map->getLength ();
      
      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          char* aceattrnodevalue_ch = XMLString::transcode
            (attribute_node->getNodeValue ());
          ACE_TString aceattrnodevalue = aceattrnodevalue_ch;
          XMLString::release (&aceattrnodevalue_ch);
          if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              //index = index + 1;
              idref_map.bind (index, aceattrnodevalue);
            }
        }
    }
}
