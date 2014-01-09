// $Id$

#include "be_extended_port.h"
#include "be_visitor.h"

#include "ast_porttype.h"

be_extended_port::be_extended_port (
      UTL_ScopedName *n,
      AST_PortType *porttype_ref)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_ext_port,
              n),
    AST_Field (AST_Decl::NT_ext_port,
               porttype_ref,
               n),
    AST_Extended_Port (n,
                       porttype_ref),
    be_decl (AST_Decl::NT_ext_port,
             n),
    be_field (porttype_ref,
              n)
{
}

be_extended_port::~be_extended_port (void)
{
}

be_porttype *
be_extended_port::port_type (void) const
{
  return
    be_porttype::narrow_from_decl (
      this->AST_Extended_Port::port_type ());
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

