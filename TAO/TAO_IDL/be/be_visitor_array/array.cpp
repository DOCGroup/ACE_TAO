//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array.cpp
//
// = DESCRIPTION
//    Generic visitor for code generation for Arrays
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_array, 
           array, 
           "$Id$")


// ************************************************************************
//  generic visitor for array declaration
// ************************************************************************

be_visitor_array::be_visitor_array (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array::~be_visitor_array (void)
{
}

int be_visitor_array::visit_array (be_array *)
{
  // must be overloaded by derived visitors
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
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_ARRAY_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_var");
  else
    *os << bt->name () << "_var";
  return 0;
}

int
be_visitor_array::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_ARRAY_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_var");
  else
    *os << bt->name () << "_var";
  return 0;
}

int
be_visitor_array::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    *os << node->name () << "_var";
  else
    *os << node->name ();
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
      *os << "TAO_String_Manager";
    }
  else
    {
      *os << "TAO_WString_Manager";
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
  int result = this->visit_node (node);

  TAO_OutStream *os = this->ctx_->stream ();
  AST_Decl::NodeType nt = node->primitive_base_type ()->node_type ();

  if (nt == AST_Decl::NT_interface || nt == AST_Decl::NT_interface_fwd)
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
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_ARRAY_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}
