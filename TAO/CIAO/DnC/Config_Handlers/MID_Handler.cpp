// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "NIA_Handler.h"
#include "MID_Handler.h"
#include "IR_Handler.h"
#include "Process_Element.h"

#include <iostream>

using std::cerr;
using std::endl;

BEGIN_DEPLOYMENT_NAMESPACE

void MID_Handler::process_MonolithicImplementationDescription
(::Deployment::MonolithicImplementationDescription &mid)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "execParameter", mid.execParameter,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else if
        (process_sequence_remote<Deployment::NamedImplementationArtifact, NIA_Handler>
         (this->doc_, this->iter_, node,
          node_name, "primaryArtifact", mid.primaryArtifact,
          &NIA_Handler::process_NamedImplementationArtifact, this->id_map_));
      else if
        (process_sequence_remote<Deployment::ImplementationRequirement, IR_Handler>
         (this->doc_, this->iter_, node,
          node_name, "deployRequirement", mid.deployRequirement,
          &IR_Handler::process_ImplementationRequirement, this->id_map_));
      else
        {
          // ??? How did we get here ???
          ACE_THROW (CORBA::INTERNAL());
        }
    }
}

END_DEPLOYMENT_NAMESPACE
