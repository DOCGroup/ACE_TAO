//$Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IAD_Handler.h"
#include "NIA_Handler.h"

#include "Process_Basic_Type.h"
#include "Process_Element.h"

#include <iostream>

using std::cerr;
using std::endl;

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the package configuration and populate it
void NIA_Handler::process_NamedImplementationArtifact
(::Deployment::NamedImplementationArtifact &nia)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
        (ACE_TEXT ("Deployment:NamedImplementationArtifact")))
        {
        }
      else if
        (process_string(this->iter_, node_name, "name", nia.name));
      else if
        (process_element_remote<Deployment::ImplementationArtifactDescription, IAD_Handler>
         (this->doc_, this->iter_, node,
          node_name, "referencedArtifact", nia.referencedArtifact,
          &IAD_Handler::process_ImplementationArtifactDescription, this->id_map_));
    }
}

END_DEPLOYMENT_NAMESPACE
