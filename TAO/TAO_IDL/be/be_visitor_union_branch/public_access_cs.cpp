//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_access_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch in the client inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union_branch, 
           public_access_cs, 
           "$Id$")

// *****************************************************
//  visitor for union_branch in the client
//  stubs file for the access method
// *****************************************************

be_visitor_union_branch_public_access_cs::
be_visitor_union_branch_public_access_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_branch_public_access_cs::
~be_visitor_union_branch_public_access_cs (void)
{
}

int
be_visitor_union_branch_public_access_cs::visit_union_branch (
    be_union_branch *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"), 
                        -1);
    }

  this->ctx_->node (node); // save the node

  for (unsigned long i = 0;
       i < node->label_list_length ();
       ++i)
    {
      // Check if we are printing the default case.
      if (node->label (i)->label_kind () == AST_UnionLabel::UL_default)
        {
          *os << "default:";
        }
      else
        {
          *os << "case ";
          node->gen_label_value (os, i);
          *os << ":";
        }

      if (i == (node->label_list_length () - 1))
        {
          *os << be_idt_nl;
        }
      else
        {
          *os << be_nl;
        }
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"), 
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_array (be_array *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_enum - "
                         "bad context information\n"), 
                        -1);
    }

  // For anonymous arrays, the type name has a _ prepended. We compute the
  // fullname with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  ACE_OS::memset (fname, 
                  '\0', 
                  NAMEBUFSIZE);

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name.

      if (bt->is_nested ())
        {
          be_decl *parent = be_scope::narrow_from_scope (bt->defined_in ())->decl ();
          ACE_OS::sprintf (fname, 
                           "%s::_%s", 
                           parent->full_name (),
                           bt->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, 
                           "_%s", 
                           bt->full_name ());
        }
    }
  else
    {
      ACE_OS::sprintf (fname, 
                       "%s", 
                       bt->full_name ());
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "if (alloc_flag)" << be_idt_nl
      << "this->u_." << ub->local_name () << "_ = " << fname
      << "_alloc ();" << be_uidt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_enum (be_enum *)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_enum - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl
      << "return &this->u_." << ub->local_name () << "_;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_interface (be_interface *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_interface - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "if (alloc_flag)" << be_idt_nl
      << "ACE_NEW_RETURN (this->u_." << ub->local_name () << "_, "
      << "("
      << bt->name () << "_var), 0);" << be_uidt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_interface_fwd (be_interface_fwd *)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_interface - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl
      << "return (CORBA::Object_ptr) &this->u_." << ub->local_name ()
      << "_->inout ();" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_predefined_type (be_predefined_type *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_interface - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl
          << "return &this->u_." << ub->local_name () << "_;" << be_uidt_nl;

      break;
    case AST_PredefinedType::PT_any:
      *os << "if (alloc_flag)" << be_idt_nl
          << "ACE_NEW_RETURN (this->u_." << ub->local_name ()
          << "_, " << bt->name () << ", 0);" << be_uidt_nl
          << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;

      break;
    case AST_PredefinedType::PT_void:
     *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl;

      break;
    default:
      *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl
          << "return &this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_sequence (be_sequence *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_sequence - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "if (alloc_flag)" << be_idt_nl
      << "ACE_NEW_RETURN (this->u_." << ub->local_name ()
      << "_, " << bt->name () << ", 0);" << be_uidt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_string (be_string *)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_string - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl
      << "return &this->u_." << ub->local_name () << "_;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_structure (be_structure *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_structure - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->size_type () == be_type::VARIABLE
      || node->has_constructor ())
    {
      *os << "if (alloc_flag)" << be_idt_nl
          << "ACE_NEW_RETURN (this->u_." << ub->local_name ()
          << "_, " << bt->name () << ", 0);" << be_uidt_nl
          << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }
  else
    {
      *os << "ACE_UNUSED_ARG (alloc_flag);" << be_nl
          << "return &this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }

  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"), 
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_access_cs::visit_union (be_union *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_branch_public_access_cs::"
                         "visit_union - "
                         "bad context information\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "if (alloc_flag)" << be_idt_nl
      << "ACE_NEW_RETURN (this->u_." << ub->local_name ()
      << "_, " << bt->name () << ", 0);" << be_uidt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;

  return 0;
}
