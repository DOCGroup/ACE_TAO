#include "ast_tmpl_port.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_tmpl_port,
           "$Id$")

AST_Tmpl_Port::AST_Tmpl_Port (
      UTL_ScopedName *n,
      AST_PortType *porttype_ref)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_field,
              n),
    AST_Field (AST_Decl::NT_field,
               porttype_ref,
               n)
{
}

AST_Tmpl_Port::~AST_Tmpl_Port (void)
{
}

AST_PortType *
AST_Tmpl_Port::port_type (void) const
{
  return AST_PortType::narrow_from_decl (this->field_type ());
}

void
AST_Tmpl_Port::destroy (void)
{
  this->AST_Field::destroy ();
}

void
AST_Tmpl_Port::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "port ");

  this->local_name ()->dump (o);

  // TODO - template params
}

int
AST_Tmpl_Port::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_tmpl_port (this);
}

IMPL_NARROW_FROM_DECL (AST_Tmpl_Port)

