// $Id$

#include "be_mirror_port.h"
#include "be_visitor.h"

#include "ast_porttype.h"

ACE_RCSID (be,
           be_mirror_port,
           "$Id$")

be_mirror_port::be_mirror_port (
      UTL_ScopedName *n,
      AST_PortType *porttype_ref,
      UTL_NameList *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_mirror_port,
              n),
    AST_Field (AST_Decl::NT_mirror_port,
               porttype_ref,
               n),
    AST_Extended_Port (n,
                       porttype_ref,
                       template_args),
    AST_Mirror_Port (n,
                     porttype_ref,
                     template_args),
    be_field (porttype_ref,
              n),
    be_extended_port (n,
                      porttype_ref,
                      template_args)
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

