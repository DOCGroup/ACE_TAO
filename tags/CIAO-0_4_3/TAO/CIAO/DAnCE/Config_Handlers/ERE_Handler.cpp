//$Id$

#include "ERE_Handler.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

void
CIAO::Config_Handler::ERE_Handler::
process_ExternalReferenceEndpoint (DOMNodeIterator * iter,
                                   Deployment::ExternalReferenceEndpoint &ret_struct)
{
  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());

  if (name != XStr (ACE_TEXT ("location")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::ERE_Handler::process_ExternalRefEndPoint \
                   element mismatch expected <location>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct.location = Utils::parse_string (iter);
}
