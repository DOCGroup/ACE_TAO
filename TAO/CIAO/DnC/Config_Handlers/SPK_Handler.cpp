//$Id$

#ifndef SPK_HANDLER_C
#define SPK_HANDLER_C

#include "SPK_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

void
CIAO::Config_Handler::SPK_Handler::
process_SatisfierPropertyKind (DOMNodeIterator * iter,
                               Deployment::SatisfierPropertyKind &kind)
{
  // -- SatisfierPropertyKind enum
  XStr quantity  ("Quantity");
  XStr capacity  ("Capacity");
  XStr minimum   ("Minimum");
  XStr maximum   ("Maximum");
  XStr attribute ("Attribute");
  XStr selection ("Selection");

  XStr kind_str = XStr (Utils::parse_string (iter));
  if (kind_str == quantity)
     kind = Deployment::Quantity;
  else if (kind_str = capacity)
     kind = Deployment::Capacity;
  else if (kind_str == minimum)
     kind = Deployment::Minimum;
  else if (kind_str == maximum)
     kind = Deployment::Maximum;
  else if (kind_str == attribute)
     kind = Deployment::Attribute;
  else if (kind_str == selection)
     kind = Deployment::Selection;

  // Something wrong here.. Throw exception
  ACE_DEBUG ((LM_DEBUG,
              "Config_Handler::SPK_Handler::process_SPK \
               illegal <SatisfierPropertyKind> value found \n"));
  ACE_THROW (CORBA::INTERNAL ());
}

#endif /* SPK_HANDLER_C */
