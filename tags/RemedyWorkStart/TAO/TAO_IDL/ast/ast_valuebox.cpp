// $Id$

#include "ast_valuebox.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

AST_Decl::NodeType const
AST_ValueBox::NT = AST_Decl::NT_valuebox;

AST_ValueBox::AST_ValueBox (UTL_ScopedName *n,
                            AST_Type       *boxed_type)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_valuebox,
              n, true),
    AST_Type (AST_Decl::NT_valuebox,
              n),
    AST_ConcreteType (AST_Decl::NT_valuebox, n),
    pd_boxed_type (boxed_type)
{
}

AST_ValueBox::~AST_ValueBox (void)
{
}

AST_Type *
AST_ValueBox::boxed_type (void) const
{
  return this->pd_boxed_type;
}

void
AST_ValueBox::dump (ACE_OSTREAM_TYPE &o)
{

  this->dump_i (o, "valuetype ");

  this->local_name ()->dump (o);
  this->dump_i (o, " ");
  this->pd_boxed_type->dump (o);
}

int
AST_ValueBox::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_valuebox (this);
}

void
AST_ValueBox::destroy (void)
{
  this->AST_ConcreteType::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_ValueBox)


