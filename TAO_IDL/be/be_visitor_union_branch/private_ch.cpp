
//=============================================================================
/**
 *  @file    private_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union branch for the private part of the
 *  class.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// **********************************************
//  visitor for union_branch in the client header generating the private information
// **********************************************

// Constructor.
be_visitor_union_branch_private_ch::be_visitor_union_branch_private_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

// Destructor.
be_visitor_union_branch_private_ch::~be_visitor_union_branch_private_ch (void)
{
}

// Visit the union_branch node.
int
be_visitor_union_branch_private_ch::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_array (be_array *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_array - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Case of anonymous array in union
      *os << "_" << bt->local_name () << "_slice *" << ub->local_name ()
          << "_;";
    }
  else
    {
      *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name ()
          << "_;";
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_enum (be_enum *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << bt->nested_type_name (bu) << " " << ub->local_name ()
      << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_interface (be_interface *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_valuebox (be_valuebox *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_valuebox - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_valuetype (be_valuetype *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;
  be_typedef *td = this->ctx_->alias ();

  // Check if we are visiting this node via a visit to a typedef node
  if (td != 0)
    {
      bt = td;
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_object
      || pt == AST_PredefinedType::PT_abstract)
    {
      *os << bt->name () << "_var"
          << " *" << ub->local_name () << "_;";
    }
  else if (pt == AST_PredefinedType::PT_pseudo)
    {
      *os << bt->nested_type_name (bu, "_ptr")
          << " " << ub->local_name () << "_;";
    }
  else if (pt == AST_PredefinedType::PT_value)
    {
      *os << bt->nested_type_name (bu, " *")
          << " " << ub->local_name () << "_;";
    }
  else if (pt == AST_PredefinedType::PT_any)
    {
      // Cannot have an object inside of a union. In addition, an Any is a
      // variable data type.
      *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
          << "_;";
    }
  else
    {
      *os << bt->nested_type_name (bu) << " " << ub->local_name ()
          << "_;";
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_sequence (be_sequence *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  // C++ doesn't allow object instances inside unions, so we need a
  // pointer.
  *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
      << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << "char *" << ub->local_name () << "_;";
    }
  else
    {
      *os << "::CORBA::WChar *" << ub->local_name () << "_;";
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  // If we are variable sized, we need a pointer type.
  if (node->size_type () == AST_Type::VARIABLE
      || node->has_constructor ())
    {
      *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
          << "_;";
    }
  else
    {
      *os << bt->nested_type_name (bu) << " " << ub->local_name ()
          << "_;";
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_private_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_union (be_union *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  // C++ doesn't allow object instances inside unions, so we need a
  // pointer.
  *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
      << "_;";

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_union_fwd (
  be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

