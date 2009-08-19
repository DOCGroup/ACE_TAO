#include "ast_tmpl_mirror_port.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_tmpl_mirror_port,
           "$Id$")

AST_Tmpl_Mirror_Port::AST_Tmpl_Mirror_Port (
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
                   porttype_ref)
{
}

AST_Tmpl_Mirror_Port::~AST_Tmpl_Mirror_Port (void)
{
}

void
AST_Tmpl_Mirror_Port::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "mirrorport ");

  this->local_name ()->dump (o);

  // TODO - template params
}

int
AST_Tmpl_Mirror_Port::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_tmpl_mirror_port (this);
}

IMPL_NARROW_FROM_DECL (AST_Tmpl_Mirror_Port)

