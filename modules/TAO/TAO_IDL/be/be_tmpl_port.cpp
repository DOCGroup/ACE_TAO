// $Id$

#include "be_tmpl_port.h"
#include "be_visitor.h"

ACE_RCSID (be,
           be_tmpl_port,
           "$Id$")

be_tmpl_port::be_tmpl_port (
      UTL_ScopedName *n,
      AST_PortType *porttype_ref)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_field,
              n),
    AST_Field (AST_Decl::NT_field,
               porttype_ref,
               n),
    AST_Tmpl_Port (n,
                   porttype_ref),
    be_field (porttype_ref,
              n)
{
}

be_tmpl_port::~be_tmpl_port (void)
{
}

void
be_tmpl_port::destroy (void)
{
  this->AST_Tmpl_Port::destroy ();
  this->be_field::destroy ();
}


int
be_tmpl_port::accept (be_visitor *visitor)
{
  return visitor->visit_tmpl_port (this);
}

IMPL_NARROW_FROM_DECL (be_tmpl_port)

