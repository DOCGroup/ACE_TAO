// $Id$

#include "ast_extended_port.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

AST_Decl::NodeType const
AST_Extended_Port::NT = AST_Decl::NT_ext_port;

AST_Extended_Port::AST_Extended_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_ext_port,
              n),
    AST_Field (AST_Decl::NT_ext_port,
               porttype_ref,
               n)
{
}

AST_Extended_Port::~AST_Extended_Port (void)
{
}

AST_PortType *
AST_Extended_Port::port_type (void) const
{
  return AST_PortType::narrow_from_decl (this->field_type ());
}

void
AST_Extended_Port::destroy (void)
{
  this->AST_Field::destroy ();
}

void
AST_Extended_Port::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "port ");

  this->local_name ()->dump (o);
}

int
AST_Extended_Port::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_extended_port (this);
}

IMPL_NARROW_FROM_DECL (AST_Extended_Port)

