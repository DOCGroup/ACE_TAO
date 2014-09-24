// $Id$

#include "be_mirror_port.h"
#include "be_visitor.h"

#include "ast_porttype.h"

be_mirror_port::be_mirror_port (
      UTL_ScopedName *n,
      AST_PortType *porttype_ref)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_mirror_port,
              n),
    AST_Field (AST_Decl::NT_mirror_port,
               porttype_ref,
               n),
    AST_Extended_Port (n,
                       porttype_ref),
    AST_Mirror_Port (n,
                     porttype_ref),
    be_decl (AST_Decl::NT_interface,
             n),
    be_field (porttype_ref,
              n),
    be_extended_port (n,
                      porttype_ref)
{
}

be_mirror_port::~be_mirror_port (void)
{
}

void
be_mirror_port::destroy (void)
{
  this->AST_Mirror_Port::destroy ();
  this->be_extended_port::destroy ();
}


int
be_mirror_port::accept (be_visitor *visitor)
{
  return visitor->visit_mirror_port (this);
}

IMPL_NARROW_FROM_DECL (be_mirror_port)

