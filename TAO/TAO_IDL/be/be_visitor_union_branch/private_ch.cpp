//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    private_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union branch for the private part of the
//    class.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union_branch, 
           private_ch, 
           "$Id$")

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
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Case of anonymous array in union
      *os << "_" << bt->local_name () << "_slice *" << ub->local_name ()
          << "_;" << be_nl;
    }
  else
    {
      *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name ()
          << "_;" << be_nl;
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_enum (be_enum *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  *os << bt->nested_type_name (bu) << " " << ub->local_name ()
      << "_;" << be_nl;

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_interface (be_interface *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;" << be_nl;

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  *os << bt->nested_type_name (bu, "_var")
      << " *" << ub->local_name () << "_;" << be_nl;

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (node->pt () == AST_PredefinedType::PT_object)
    {
      *os << bt->name () << "_var"
          << " *" << ub->local_name () << "_;" << be_nl;
    }
  else if (node->pt () == AST_PredefinedType::PT_pseudo)
    {
      *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
          << "_;" << be_nl;
    }
  else if (node->pt () == AST_PredefinedType::PT_any)
    {
      // Cannot have an object inside of a union. In addition, an Any is a
      // variable data type.
      *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
          << "_;" << be_nl;
    }
  else
    {
      *os << bt->nested_type_name (bu) << " " << ub->local_name ()
          << "_;" << be_nl;
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_sequence (be_sequence *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // C++ doesn't allow object instances inside unions, so we need a
  // pointer.
  *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
      << "_;" << be_nl;

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (node->width () == (long) sizeof (char))
    {
      *os << "char *" << ub->local_name () << "_;" << be_nl;
    }
  else
    {
      *os << "CORBA::WChar *" << ub->local_name () << "_;" << be_nl;
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // If we are variable sized, we need a pointer type.
  if (node->size_type () == AST_Type::VARIABLE
      || node->has_constructor ())
    {
      *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
          << "_;" << be_nl;
    }
  else
    {
      *os << bt->nested_type_name (bu) << " " << ub->local_name ()
          << "_;" << be_nl;
    }

  return 0;
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
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // C++ doesn't allow object instances inside unions, so we need a
  // pointer.
  *os << bt->nested_type_name (bu) << " *" << ub->local_name ()
      << "_;" << be_nl;

  return 0;
}
