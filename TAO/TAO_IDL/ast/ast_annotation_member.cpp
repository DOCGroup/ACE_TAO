/**
 * Abstract Syntax Tree Node for a Member Value of an Annotation
 */

#include "ast_annotation_member.h"
#include "ast_type.h"
#include "utl_scope.h"

#include "ast_enum.h"
#include "ast_enum_val.h"

AST_Decl::NodeType const AST_Annotation_Member::NT =
  AST_Decl::NT_annotation_member;

AST_Annotation_Member::AST_Annotation_Member (
  AST_Expression::ExprType expr_type, AST_Type *type, UTL_ScopedName *name)
  : AST_Decl (NT, name),
    AST_Field (type, name, vis_NA),
    expr_type_ (expr_type),
    value_ (nullptr)
{
}

AST_Annotation_Member::AST_Annotation_Member (
  AST_Decl::NodeType nt,
  AST_Expression::ExprType expr_type, AST_Type *type, UTL_ScopedName *name)
  : AST_Decl (nt, name),
    AST_Field (nt, type, name, vis_NA),
    expr_type_ (expr_type),
    value_ (nullptr)
{
}

AST_Annotation_Member::AST_Annotation_Member (
  UTL_ScopedName *name,
  AST_Annotation_Member *other)
  : AST_Decl (name, dynamic_cast<AST_Decl*>(other)),
    AST_Field (name, dynamic_cast<AST_Field*>(other)),
    expr_type_ (other->expr_type ()),
    value_ (other->value () ?
      new AST_Expression (other->value (), other->expr_type ()) : nullptr)
{
}

AST_Annotation_Member::~AST_Annotation_Member ()
{
  delete value_;
}

void
AST_Annotation_Member::destroy ()
{
  if (value_)
    {
      value_->destroy ();
    }
  AST_Field::destroy ();
}

void
AST_Annotation_Member::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Field::dump (o);
  if (field_type () && value_)
    {
      dump_i (o, " default ");
      if (field_type ()->node_type () == AST_Decl::NT_enum)
        {
          dynamic_cast<AST_Enum*> (field_type ())->
            lookup_by_value (value_)->name ()->dump (o);
        }
      else
        {
          value_->dump (o);
        }
    }
}

AST_Expression *
AST_Annotation_Member::value ()
{
  return value_;
}

void
AST_Annotation_Member::value (AST_Expression *value)
{
  if (value_)
    {
      value_->destroy ();
      delete value_;
    }

  value_ = value;
}

AST_Expression::ExprType
AST_Annotation_Member::expr_type ()
{
  return expr_type_;
}

bool
AST_Annotation_Member::invalid_value ()
{
  return !value () || !value ()->ev ();
}
