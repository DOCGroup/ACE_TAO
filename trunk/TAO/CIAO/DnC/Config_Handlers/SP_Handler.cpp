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
  for (DOMNode * node = iter->nextNode (); ; node = iter->nextNode ())
    {
      // Get name of the current element
      XStr name (node->getNodeName ());

      if (name == XStr (ACE_TEXT ("name")))
        // Populate name field in the Structure
        property.name = Utils::parse_string (iter);
      else
        if (name == XStr (ACE_TEXT ("Deployment:SatisfierPropertyKind")))
            // Populate the Satisfier property kind value
          SPK_Handler::process_SatisfierPropertyKind (iter, property.kind);
        else
          if (name == XStr (ACE_TEXT ("Deployment:Any")))
            // Populate value for the Property
            Any_Handler::process_Any (iter, property.value);
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

#endif /* RS_HANDLER_C */
