
//=============================================================================
/**
 *  @file    array.cpp
 *
 *  $Id$
 *
 *  Generic visitor for code generation for Arrays
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "array.h"

be_visitor_array::be_visitor_array (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array::~be_visitor_array (void)
{
}

int be_visitor_array::visit_array (be_array *)
{
  // Must be overloaded by derived visitors.
  return -1;
}

int
be_visitor_array::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_array::visit_interface (be_interface *node)
{
  return this->emit_common (node);
}

int
be_visitor_array::visit_interface_fwd (be_interface_fwd *node)
{
  return this->emit_common (node);
}

int
be_visitor_array::visit_valuebox (be_valuebox *node)
{
  return this->emit_common (node);
}

int
be_visitor_array::visit_valuetype (be_valuetype *node)
{
  return this->emit_common (node);
}

int
be_visitor_array::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->emit_common (node);
}

int
be_visitor_array::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_PredefinedType::PredefinedType pt = node->pt ();

  *os << "::";

  if (pt == AST_PredefinedType::PT_pseudo
      || pt == AST_PredefinedType::PT_object)
    {
      *os << node->name () << "_var";
    }
  else
    {
      *os << node->name ();
    }

  return 0;
}

int
be_visitor_array::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_array::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->width () == (long) sizeof (char))
    {
      *os << "::TAO::String_Manager";
    }
  else
    {
      *os << "::TAO::WString_Manager";
    }

  return 0;
}

int
be_visitor_array::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_array::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_array::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_Type *pbt = node->primitive_base_type ();
  AST_Decl::NodeType nt = pbt->node_type ();
  AST_PredefinedType::PredefinedType pt = AST_PredefinedType:: PT_void;
  int result = 0;

  // We check for these first, because in these cases, we replace the
  // entire slice type with one of the strings below, instead of using
  // the node's type name, possibly suffixed with '_var'.
  if (nt == AST_Decl::NT_string)
    {
      *os << "::TAO::String_Manager";

      return 0;
    }
  else if (nt == AST_Decl::NT_wstring)
    {
      *os << "::TAO::WString_Manager";

      return 0;
    }

  result = this->visit_node (node);

  if (nt == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (pbt);
      pt = pdt->pt ();
    }

  // We must append a "_var" for typedefs of interfaces, CORBA::Objects or
  // typecodes.
  if (nt == AST_Decl::NT_interface
      || nt == AST_Decl::NT_interface_fwd
      || pt == AST_PredefinedType::PT_pseudo
      || pt == AST_PredefinedType::PT_object)
    {
      *os << "_var";
    }

  return result;
}

// helper
int
be_visitor_array::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_ARRAY_CH)
    {
      *os << bt->nested_type_name (this->ctx_->scope ()->decl ());
    }
  else
    {
      *os << bt->name ();
    }

  return 0;
}

int
be_visitor_array::emit_common (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_ARRAY_CH)
    {
      *os << bt->nested_type_name (this->ctx_->scope ()->decl (), "_var");
    }
  else
    {
      *os << bt->name () << "_var";
    }

  return 0;
}
