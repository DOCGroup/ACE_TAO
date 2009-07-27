// $Id$

#include "ast_mirror_port.h"
#include "ast_porttype.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_mirror_port,
           "$Id$")

AST_Mirror_Port::AST_Mirror_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref,
    AST_PortType::T_ARGLIST *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_mirror_port,
              n),
    AST_Field (AST_Decl::NT_mirror_port,
               porttype_ref,
               n),
    AST_Extended_Port (n,
                       porttype_ref,
                       template_args)
{
}

AST_Mirror_Port::~AST_Mirror_Port (void)
{
}

void
AST_Mirror_Port::destroy (void)
{
  this->AST_Extended_Port::destroy ();
}

void
AST_Mirror_Port::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "port ");

  this->local_name ()->dump (o);
  
  // TODO - template args
}

int
AST_Mirror_Port::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_mirror_port (this);
}

IMPL_NARROW_FROM_DECL (AST_Mirror_Port)

