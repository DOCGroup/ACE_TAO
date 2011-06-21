
//=============================================================================
/**
 *  @file    public_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union Branch in the client inline file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// *****************************************************
//  Visitor for union_branch in the client inline file.
// *****************************************************

// constructor
be_visitor_union_branch_public_ci::be_visitor_union_branch_public_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_ci::~be_visitor_union_branch_public_ci (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_ci::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  this->ctx_->node (node);

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

// Visit operations on all possible data types that a union_branch can be.

int
be_visitor_union_branch_public_ci::visit_array (be_array *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // If bt is not a typedef and is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);

      // Set the node to be the node being visited. Scope is still the same.
      ctx.node (node);

      // First generate the inline operations for this anonymous array type.
      be_visitor_array_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // For anonymous arrays, the type name has a _ prepended. We compute the
  // full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  // If bt is not a typedef and is defined inside the union
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name.

      if (bt->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (bt->defined_in ())->decl ();
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
      // Typedefed node.
      ACE_OS::sprintf (fname,
                       "%s",
                       bt->full_name ());
    }

  // Set method.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << fname
      << " val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// Set the value." << be_nl
      << "this->u_." << ub->local_name () << "_ = "
      << fname << "_dup (val);" << be_uidt_nl
      << "}" << be_nl_2;

  // Get method.
  *os << "/// Retrieve the member." << be_nl
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_enum (be_enum *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  // heck if we are visiting this node via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << " val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// Set the value." << be_nl
      << "this->u_." << ub->local_name () << "_ = val;" << be_uidt_nl
      << "}" << be_nl_2;

  // Get method.
  *os << "/// Retrieve the member." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_interface (be_interface *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
    }

  bool bt_is_defined = node->is_defined ();

  *os << ";" << be_nl
      << "typedef "
      << bt->nested_type_name (bu, "_var")
      << " OBJECT_FIELD;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl;

      if (bt_is_defined)
        {
          *os << "OBJECT_FIELD (" << be_idt << be_idt_nl
              << bt->name () << "::_";
        }
      else
        {
          *os << "OBJECT_FIELD (" << be_idt << be_idt_nl
              << "TAO::Objref_Traits<" << node->name () << ">::";
        }

  *os << "duplicate (val)" << be_uidt_nl << ")" << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Get method.
  *os << "/// Retrieve the member." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << "_ptr " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_->in ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_interface_fwd (be_interface_fwd *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
    }

  bool bt_is_defined = node->full_definition ()->is_defined ();

  *os << ";" << be_nl
      << "typedef "
      << bt->nested_type_name (bu, "_var")
      << " OBJECT_FIELD;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl;

      if (bt_is_defined)
        {
          *os << "OBJECT_FIELD (" << be_idt << be_idt_nl
              << bt->name () << "::_";
        }
      else
        {
          *os << "OBJECT_FIELD (" << be_idt << be_idt_nl
              << "TAO::Objref_Traits<" << node->name () << ">::";
        }

  *os << "duplicate (val)" << be_uidt_nl << ")" << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Get method.
  *os << "/// Retrieve the member." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << "_ptr " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_->in ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_valuebox (be_valuebox *node)
{
  return this->emit_valuetype_common (node);
}

int
be_visitor_union_branch_public_ci::visit_valuetype (be_valuetype *node)
{
  return this->emit_valuetype_common (node);
}

int
be_visitor_union_branch_public_ci::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->emit_valuetype_common (node);
}

int
be_visitor_union_branch_public_ci::emit_valuetype_common  (be_type *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "emit_valuetype_common - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << " *val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "::CORBA::add_ref (val);" << be_nl
      << "typedef "
      << bt->nested_type_name (bu, "_var")
      << " OBJECT_FIELD;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << "OBJECT_FIELD (val)" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Get method.
  *os << "/// Retrieve the member." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << "* " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_->in ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (";


  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_pseudo
      || pt == AST_PredefinedType::PT_object
      || pt == AST_PredefinedType::PT_abstract)
    {
      *os << "const ::" << bt->name () << "_ptr";
    }
  else if (pt == AST_PredefinedType::PT_value)
    {
      *os << "::" << bt->name () << " *";
    }
  else if (pt == AST_PredefinedType::PT_any)
    {
      *os << "const ::" << bt->name () << " &";
    }
  else
    {
      *os << bt->name ();
    }

  *os << " val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl;

  switch (pt)
    {
      case AST_PredefinedType::PT_object:
        *os << "typedef ::CORBA::Object_var OBJECT_FIELD;" << be_nl
            << "ACE_NEW (" << be_idt << be_idt_nl
            << "this->u_." << ub->local_name () << "_," << be_nl
            << "OBJECT_FIELD ( ::CORBA::Object::_duplicate (val))"
            << be_uidt_nl
            << ");" << be_uidt << be_uidt_nl;

        break;
      case AST_PredefinedType::PT_abstract:
        *os << "typedef ::CORBA::AbstractBase_var OBJECT_FIELD;" << be_nl
            << "ACE_NEW (" << be_idt << be_idt_nl
            << "this->u_." << ub->local_name () << "_," << be_nl
            << "OBJECT_FIELD ( ::CORBA::AbstractBase::_duplicate (val))"
            << be_uidt_nl
            << ");" << be_uidt << be_uidt_nl;

        break;
      case AST_PredefinedType::PT_pseudo:
        *os << "this->u_." << ub->local_name () << "_ = ::"
            << bt->name () << "::_duplicate (val);" << be_uidt_nl;

        break;
      case AST_PredefinedType::PT_value:
        *os << "::CORBA::add_ref (val);" << be_nl
            << "this->u_." << ub->local_name ()
            << "_ = val;" << be_uidt_nl;

        break;
      case AST_PredefinedType::PT_any:
        *os << "ACE_NEW (" << be_idt << be_idt_nl
            << "this->u_." << ub->local_name ()
            << "_," << be_nl
            << "::" << bt->name () << " (val)" << be_uidt_nl
            << ");" << be_uidt << be_uidt_nl;

        break;
      case AST_PredefinedType::PT_void:
        break;
      default:
        *os << "// Set the value." << be_nl
            << "this->u_." << ub->local_name ()
            << "_ = val;" << be_uidt_nl;

        break;
    }

  *os << "}" << be_nl_2;

  switch (pt)
    {
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_abstract:
        // Get method.
        *os << "/// Retrieve the member." << be_nl
            << "ACE_INLINE" << be_nl
            << "::" << bt->name () << "_ptr" << be_nl
            << bu->name () << "::" << ub->local_name ()
            << " (void) const" << be_nl
            << "{" << be_idt_nl;
        *os << "return this->u_." << ub->local_name ()
            << "_->in ();" << be_uidt_nl;
        *os << "}";

        break;
      case AST_PredefinedType::PT_pseudo:
        // Get method.
        *os << "/// Retrieve the member." << be_nl
            << "ACE_INLINE" << be_nl
            << "::" << bt->name () << "_ptr" << be_nl
            << bu->name () << "::" << ub->local_name ()
            << " (void) const" << be_nl
            << "{" << be_idt_nl;
        *os << "return this->u_." << ub->local_name ()
            << "_;" << be_uidt_nl;
        *os << "}";

        break;
      case AST_PredefinedType::PT_value:
        // Get method.
        *os << "/// Retrieve the member." << be_nl
            << "ACE_INLINE" << be_nl
            << "::" << bt->name () << " *" << be_nl
            << bu->name () << "::" << ub->local_name ()
            << " (void) const" << be_nl
            << "{" << be_idt_nl;
        *os << "return this->u_." << ub->local_name ()
            << "_;" << be_uidt_nl;
        *os << "}";

        break;
      case AST_PredefinedType::PT_any:
        // Get method with read-only access.
        *os << "/// Retrieve the member." << be_nl
            << "ACE_INLINE" << be_nl
            << "const ::" << bt->name () << " &" << be_nl
            << bu->name () << "::" << ub->local_name ()
            << " (void) const" << be_nl
            << "{" << be_idt_nl
            << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
            << "}" << be_nl_2;

        // Get method with read/write access
        *os << "/// Retrieve the member." << be_nl
            << "ACE_INLINE" << be_nl
            << "::" << bt->name () << " &" << be_nl
            << bu->name () << "::" << ub->local_name ()
            << " (void)" << be_nl
            << "{" << be_idt_nl
            << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
            << "}";
        break;
      case AST_PredefinedType::PT_void:
        break;
      default:
        // Get method.
        *os << "/// Retrieve the member." << be_nl
            << "ACE_INLINE" << be_nl
            << "::" << bt->name () << be_nl
            << bu->name () << "::" << ub->local_name ()
            << " (void) const" << be_nl
            << "{" << be_idt_nl
            << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
            << "}";

        break;
    }

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_sequence (be_sequence *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // (1) Set from a const.
  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << bt->name () << " (val)" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Readonly get method.
  *os << "/// Readonly get method." << be_nl
      << "ACE_INLINE" << be_nl
      << "const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl_2;

  // Read/write get method.
  *os << "/// Read/write get method." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_string (be_string *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Three methods to set the string value.

  // (1) Set method from char* or wchar*.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << bu->name () << "::" << ub->local_name () << " (char *val)";
    }
  else
    {
      *os << bu->name () << "::" << ub->local_name ()
          << " ( ::CORBA::WChar *val)";
    }

  *os << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "this->u_." << ub->local_name () << "_ = val;" << be_uidt_nl
      << "}" << be_nl_2;

  // (2) Set method from const char * or const wchar *.
  *os << "// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const char *val)" << be_nl << "{" << be_idt_nl;
    }
  else
    {
      *os << " (const ::CORBA::WChar *val)" << be_nl << "{" << be_idt_nl;
    }

  *os << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// Set the value." << be_nl
      << "this->u_." << ub->local_name () << "_ = ";

  if (node->width () == (long) sizeof (char))
    {
      *os << "::CORBA::string_dup (val);" << be_uidt_nl
          << "}" << be_nl_2;
    }
  else
    {
      *os << "::CORBA::wstring_dup (val);" << be_uidt_nl
          << "}" << be_nl_2;
    }

  // (3) Set from const String_var& or WString_var&
  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const ::CORBA::String_var &val)" << be_nl;
    }
  else
    {
      *os << " (const ::CORBA::WString_var &val)" << be_nl;
    }

  *os << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// Set the value." << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << "::CORBA::String_var " << ub->local_name ();
    }
  else
    {
      *os << "::CORBA::WString_var " << ub->local_name ();
    }

  *os << "_var = val;" << be_nl
      << "this->u_." << ub->local_name () << "_ = "
      << ub->local_name () << "_var._retn ();" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "ACE_INLINE" << be_nl;

  // Get method.
  if (node->width () == (long) sizeof (char))
    {
      *os << "const char *" << be_nl;
    }
  else
    {
      *os << "const ::CORBA::WChar *" << be_nl;
    }

  *os << bu->name () << "::" << ub->local_name ()
      << " (void) const // get method" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_structure (be_structure *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // If bt is not a typedef and is defined inside the union
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);

      // Set the node to be the node being visited. Scope is still the same.
      ctx.node (node);

      // First generate the struct declaration
      be_visitor_structure_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_structure - "
                             "codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // (1) Set from a const.
  *os << "/// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl;

  if (bt->size_type () == be_type::VARIABLE
      || node->has_constructor ())
    {
      *os << "ACE_NEW (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << " (val)" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << "this->u_." << ub->local_name () << "_ = val;" << be_uidt_nl;
    }

  *os << "}" << be_nl_2;

  // Readonly get method.
  *os << "// Readonly get method." << be_nl
      << "ACE_INLINE" << be_nl
      << "const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl;

  if (bt->size_type () == be_type::VARIABLE
      || node->has_constructor ())
    {
      *os << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }
  else
    {
      *os << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }

  *os << "}" << be_nl_2;

  // Read/write get method.
  *os << "// Read/write get method." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl;

  if (bt->size_type () == be_type::VARIABLE
      || node->has_constructor ())
    {
      *os << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }
  else
    {
      *os << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl;
    }

  *os << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_structure_fwd (be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_public_ci::visit_typedef (be_typedef *node)
{
  // Save the typedef node for use in code generation.
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_union (be_union *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // If bt is not a typedef and is defined inside the union
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);

      // Set the node to be the node being visited. Scope is still the same.
      ctx.node (node);

      // First generate the union declaration.
      be_visitor_union_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // (1) Set from a const.
  *os << "// Accessor to set the member." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl
      << "// Set the discriminant value." << be_nl
      << "this->_reset ();" << be_nl
      << "this->disc_ = ";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << bt->name () << " (val)" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Readonly get method.
  *os << "// Readonly get method." << be_nl
      << "ACE_INLINE" << be_nl
      << "const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl_2;

  // Read/write get method.
  *os << "// Read/write get method." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

