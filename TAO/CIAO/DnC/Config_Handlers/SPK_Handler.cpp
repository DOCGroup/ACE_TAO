//$Id$ 

#ifndef SPK_HANDLER_C
#define SPK_HANDLER_C

#include "SPK_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

Deployment::SatisfierPropertyKind
CIAO::Config_Handler::SPK_Handler::process_SatisfierPropertyKind (DOMNodeIterator * iter)
{
  // -- SatisfierPropertyKind enum
  XStr quantity  ("Quantity");
  XStr capacity  ("Capacity");
  XStr minimum   ("Minimum");
  XStr maximum   ("Maximum");
  XStr attribute ("Attribute");
  XStr selection ("Selection");

  XStr kind = XStr (Utils::parse_string (iter));
  if (kind == quantity)
     return Deployment::Quantity;
  else if (kind = capacity)
     return Deployment::Capacity;
  else if (kind == minimum)
     return Deployment::Minimum;
  else if (kind == maximum)
     return Deployment::Maximum;
  else if (kind == attribute)
      return Deployment::Attribute;
  else if (kind == selection)
      return Deployment::Selection;
  
  // Something wrong here.. Throw exception
  ACE_DEBUG ((LM_DEBUG, 
              "Config_Handler::SPK_Handler::process_SPK \
               illegal <SatisfierPropertyKind> value found \n")); 
  ACE_THROW (CORBA::INTERNAL ());
}

#endif /* SPK_HANDLER_C */
