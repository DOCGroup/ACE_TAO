//$Id$

#include "CEPE_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

void
CIAO::Config_Handler::CEPE_Handler::
process_ComponentExternalPortEndpoint (DOMNodeIterator * iter,
                                       Deployment::ComponentExternalPortEndpoint
                                       &ret_struct)
{
  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());

  if (name != XStr (ACE_TEXT ("portName")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers:: \
                   CEPE_Handler::process_ComponentExtPortEndPoint \
                   element mismatch expected <portName>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct.portName = Utils::parse_string (iter);
}
