
//=============================================================================
/**
 *  @file    field_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating the accessor and modifier declarations
 *  for valuetype fields in the valuetype class (header).
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 *   derived from be_visitor_union_branch/public_ch.cpp
 */
//=============================================================================


be_visitor_valuetype_field_ch::be_visitor_valuetype_field_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx),
    visibility_ (AST_Field::vis_NA)
{
  setenclosings ("", ";");
}

be_visitor_valuetype_field_ch::~be_visitor_valuetype_field_ch (void)
{
}

int
be_visitor_valuetype_field_ch::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_union_branch - "
                         "Bad type\n"),
                        -1);
    }

  this->ctx_->node (node); // save the node

  this->visibility_ = node->visibility ();

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_union_branch - "
                         "codegen failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types (valuetype state member).

int
be_visitor_valuetype_field_ch::visit_array (be_array *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_array - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // This is the case of an anonymous array inside a valuetype.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      be_visitor_array_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }

      ctx.state (TAO_CodeGen::TAO_ROOT_CH);

      // Now use this array as a "type" for the subsequent declarator
      // the set method.
      *os << pre_op () << "void " << ub->local_name () << " ("
          << "_" << bt->local_name () << ")"
          << post_op () << be_nl;
      // The get method.
      *os << pre_op () << "const _" << bt->local_name ()
          << "_slice * " << ub->local_name ()
          << " (void) const" << post_op () << be_nl;
      *os << pre_op () << "_" << bt->local_name ()
          << "_slice * " << ub->local_name ()
          << " (void)" << post_op ();
    }
  else
    {
      // Now use this array as a "type" for the subsequent declarator.
      // The set method.
      *os << pre_op () << "void " << ub->local_name () << " (const "
          << bt->name () << ")" << post_op () << be_nl;
      // The get method.
      *os << pre_op ()
          << bt->name () << "_slice *" << ub->local_name ()
          << " (void)" << post_op () << be_nl;
      // The get (read/write) method.
      *os << pre_op () << "const "
          << bt->name () << "_slice *" << ub->local_name ()
          << " (void) const" << post_op ();
    }

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_enum (be_enum *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_enum_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Now use this enum as a "type" for the subsequent declarator
  // the set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << ")" << post_op () << be_nl;
  // The get method.
  *os << pre_op () << bt->name () << " " << ub->local_name ()
      << " (void) const" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_interface (be_interface *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ( ::"
      << bt->name () << "_ptr"
      << ")" << post_op () << be_nl;
  // Get method.
  *os << pre_op ()
      << "::" << bt->name () << "_ptr " << ub->local_name ()
      << " (void) const" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << "_ptr"
      << ")" << post_op () << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << "_ptr " << ub->local_name ()
      << " (void) const" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_valuebox (be_valuebox *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_valuebox - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << " *"
      << ")" << post_op () << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << " *" << ub->local_name ()
      << " (void) const" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_valuetype (be_valuetype *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_valuetype - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << " *"
      << ")" << post_op () << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << " *" << ub->local_name ()
      << " (void) const" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_field_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_valuetype_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << " *"
      << ")" << post_op () << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << " *" << ub->local_name ()
      << " (void) const" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_valuetype_field_ch::visit_predefined_type (be_predefined_type *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      // Set method.
      *os << pre_op () << "void " << ub->local_name () << " ( ::"
          << bt->name () << "_ptr)"
          << post_op () << be_nl;
      // Get method.
      *os << pre_op ()
          << "::" << bt->name () << "_ptr " << ub->local_name ()
          << " (void) const" << post_op ();
      break;
    case AST_PredefinedType::PT_any:
      // Set method.
      *os << pre_op () << "void " << ub->local_name ()
          << " (const ::"  << bt->name () << " &)"
          << post_op () << be_nl;
      // Get method (read-only).
      *os << pre_op () << "const ::" << bt->name () << " &"
          << ub->local_name () << " (void) const"
          << post_op () << be_nl;
      // Get method (read/write).
      *os << pre_op () << "::" << bt->name () << " &"
          << ub->local_name () << " (void)"
          << post_op ();
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // Set method.
      *os << pre_op () << "void " << ub->local_name ()
          << " (const ::" << bt->name () << ")"
          << post_op () << be_nl;
      // Get method.
      *os << pre_op () << "::" << bt->name ()
          << " " << ub->local_name ()
          << " (void) const" << post_op ();
    }

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_sequence (be_sequence *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Not a typedef and bt is defined here.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu)
      && this->ctx_->state () != TAO_CodeGen::TAO_VALUETYPE_OBV_CH)
    {
      be_field *member_node =
        be_field::narrow_from_decl (this->ctx_->node ());
      node->field_node (member_node);

      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_ch visitor (&ctx);

      if (this->visibility_ == AST_Field::vis_PRIVATE)
        {
          *os << be_uidt_nl << "public:" << be_idt_nl;
        }

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }

      if (this->visibility_ == AST_Field::vis_PRIVATE)
        {
          *os << be_uidt_nl << be_nl << "protected:" << be_idt;
        }

      // Generate the anonymous sequence member typedef.
      // This provides a consistent name to use instead of the
      // implementation-specific name.
      *os << be_nl_2 << "typedef " << bt->nested_type_name (bu)
          << " _" << ub->local_name () << "_seq;" << be_nl;
    }

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " (const "
      << bt->name () << " &)"
      << post_op () << be_nl;
  // Read-only.
  *os << pre_op () << "const " << bt->name () << " &"
      << ub->local_name  () << " (void) const"
      << post_op () << be_nl;
  // Read/write.
  *os << pre_op () << bt->name () << " &" << ub->local_name ()
      << " (void)"
      << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os  << be_nl_2<< "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Three methods to set the string value.
  if (node->width () == (long) sizeof (char))
    {
      *os << pre_op ()
          << "void " << ub->local_name () << " (char *val)"
          << post_op () << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const char *val)"
          << post_op () << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const ::CORBA::String_var &val)"
          << post_op () << be_nl;
      // Get method.
      *os << pre_op () << "const char *" << ub->local_name ()
          << " (void) const" << post_op ();
    }
  else
    {
      *os << pre_op ()
          << "void " << ub->local_name () << " ( ::CORBA::WChar *val)"
          << post_op () << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const ::CORBA::WChar *val)"
          << post_op () << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const ::CORBA::WString_var &val)"
          << post_op () << be_nl;
      // Get method.
      *os << pre_op() << "const ::CORBA::WChar *" << ub->local_name ()
          << " (void) const" << post_op();
    }

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os  << be_nl_2<< "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Not a typedef and bt is defined here.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_structure - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " (const "
      << bt->name () << " &)"
      << post_op () << be_nl;
    // Read-only.
  *os << pre_op () << "const " << bt->name () << " &";
  *os << ub->local_name  () << " (void) const"
      << post_op () << be_nl
    // Read/write.
      << pre_op () << bt->name () << " &" << ub->local_name ()
      << " (void)" << post_op ();

  return 0;
}

int
be_visitor_valuetype_field_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_spec_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_valuetype_field_ch::visit_union (be_union *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
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
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os  << be_nl_2<< "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Not a typedef and bt is defined here.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " (const "
      << bt->name () << " &)"
      << post_op () << be_nl;
    // Read-only.
  *os << pre_op () << "const " << bt->name () << " &"
      << ub->local_name  () << " (void) const"
      << post_op () << be_nl;
    // Read/write.
  *os << pre_op () << bt->name () << " &" << ub->local_name ()
      << " (void)" << post_op ();

  return 0;
}

void
be_visitor_valuetype_field_ch::setenclosings (const char *pre,
                                              const char *post)
{
  pre_op_ = pre;
  post_op_ = post;
}

const char*
be_visitor_valuetype_field_ch::pre_op (void)
{
  return pre_op_;
}

const char*
be_visitor_valuetype_field_ch::post_op (void)
{
  return post_op_;
}
