//==================================================================
/**
 *  @file  Cap_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */
//==================================================================

#include "Cap_Handler.h"
#include "SP_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"
#include <iostream>
#include "string.h"

#include "Process_Basic_Type.h"
#include "Process_Element.h"

BEGIN_DEPLOYMENT_NAMESPACE

void
CAP_Handler::process_Capability (Deployment::Capability &cap)
{
  //Check if the Schema IDs for both the elements match
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName ());

      else if (node_name == XStr
               (ACE_TEXT ("Deployment:Capability")))
        {
        }
      else if
        (process_string (this->iter_, node_name, "name", cap.name));
      else if
        (process_string_seq(this->iter_, node_name, "resourceType", cap.resourceType));
      else if
        (process_sequence_common<Deployment::SatisfierProperty>
         (this->doc_, this->iter_, node,
          node_name, "property", cap.property,
          &SP_Handler::process_SatisfierProperty,
          this->id_map_));
      else
        {
          this->iter_->previousNode ();
          break;
        }
    }
}

END_DEPLOYMENT_NAMESPACE
