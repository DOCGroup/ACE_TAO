// $Id$

#include "be_extended_port.h"
#include "be_visitor.h"

#include "ast_porttype.h"

ACE_RCSID (be,
           be_extended_port,
           "$Id$")

be_extended_port::be_extended_port (
      UTL_ScopedName *n,
      AST_PortType *porttype_ref,
      AST_PortType::T_ARGLIST *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_ext_port,
              n),
    AST_Field (AST_Decl::NT_ext_port,
               porttype_ref,
               n),
    AST_Extended_Port (n,
                       porttype_ref,
                       template_args),
    be_field (porttype_ref,
              n)
{
}

be_extended_port::~be_extended_port (void)
{
}

void
be_extended_port::destroy (void)
{
  this->AST_Extended_Port::destroy ();
  this->be_field::destroy ();
}


int
be_extended_port::accept (be_visitor *visitor)
{
  return visitor->visit_extended_port (this);
}

IMPL_NARROW_FROM_DECL (be_extended_port)

