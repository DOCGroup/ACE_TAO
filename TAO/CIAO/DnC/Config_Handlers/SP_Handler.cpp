//$Id$

#ifndef SP_HANDLER_C
#define SP_HANDLER_C

#include "SP_Handler.h"
#include "Any_Handler.h"
#include "SPK_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::Any_Handler;

void
CIAO::Config_Handler::SP_Handler::
process_SatisfierProperty (DOMNodeIterator * iter,
                           Deployment::SatisfierProperty &property)
{
  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());
  if (name != XStr (ACE_TEXT ("name")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::SP_Handler::process_RequirementSatisfier \
                   element mismatch expected <name>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate name field in the Structure
  property.name = Utils::parse_string (iter);

  // Check if any <SatisfierProperty> elements are present
  node = iter->nextNode ();
  name = node->getNodeName ();

  if (name != XStr (ACE_TEXT ("Deployment:SatisfierPropertyKind")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::SP_Handler::process_SP \
                   did not find any <SatisfierProperty> tags"));
      // Shift back
      iter->previousNode ();
    }
  else
    SPK_Handler::process_SatisfierPropertyKind (iter, property.kind);

  // Process Any type
  node = iter->nextNode ();
  name = node->getNodeName ();
  if (name != XStr (ACE_TEXT ("Deployment:Any")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::SP_Handler::process_SP \
                   did not fine any <Andy> tags"));
      // Shift back
      iter->previousNode ();
    }
  else
    Any_Handler::process_Any (iter, property.value);
}

#endif /* RS_HANDLER_C */
