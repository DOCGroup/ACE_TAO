
//=============================================================================
/**
 *  @file    field_cs.cpp
 *
 *  $Id$
 *
 *   Visitor for the Valuetype class.
 *   This one generates code for accessor and modifier functions of
 *   valuetype state members (in the stub file).
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> derived from be_visitor_union_branch/public_ci.cpp
 */
//=============================================================================


be_visitor_valuetype_field_cs::be_visitor_valuetype_field_cs (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    in_obv_space_ (0)
{
  setenclosings ("");
}

be_visitor_valuetype_field_cs::~be_visitor_valuetype_field_cs (void)
{
}

int
be_visitor_valuetype_field_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types that a field can be

int
be_visitor_valuetype_field_cs::visit_array (be_array *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_array - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
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

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parent's name.
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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (const " << fname
      << " val)" << be_nl
      << "{" << be_idt_nl;
  *os << fname << "_copy ("
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ", val);" << be_uidt_nl;
  *os << "}" << be_nl;

  *os << "/// Retrieve the member." << be_nl
      << this->pre_op () << "const " << fname << "_slice *" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl;
  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix () << ";" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "/// Retrieve the member." << be_nl
      << this->pre_op () << fname << "_slice *" << be_nl;

      this->op_name (bu,
                     os);

  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl;
  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix () << ";" << be_uidt_nl;
  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_enum (be_enum *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op() << "void" << be_nl;

      this->op_name (bu,
                     os);

  *os << "::" << ub->local_name () << " (" << bt->name ()
      << " val)" << be_nl
      << "{" << be_idt_nl;

  *os << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = val;" << be_uidt_nl;

  *os << "}" << be_nl;

  *os << "/// Retrieve the member." << be_nl
      << this->pre_op () << bt->name () << be_nl;

      this->op_name (bu,
                     os);

  *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{" << be_idt_nl;

  *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl;

  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_interface (be_interface *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

      this->op_name (bu,
                     os);

  *os << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)" << be_nl
      << "{" << be_idt_nl;

  *os << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix () << " =" << be_idt_nl
      << "TAO::Objref_Traits< ::" << bt->name ()
      << ">::duplicate (val);" << be_uidt << be_uidt_nl;
  *os << "}" << be_nl_2;

  *os << "/// Retrieve the member." << be_nl
      << this->pre_op () << bt->name () << "_ptr" << be_nl;

      this->op_name (bu,
                     os);

  *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{" << be_idt_nl;

  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ".in ();" << be_uidt_nl;

  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)" << be_nl
      << "{" << be_idt_nl;

  *os << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = " << bt->name () << "::_duplicate (val);" << be_uidt_nl;
  *os << "}" << be_nl_2;

  *os << "/// Retrieve the member" << be_nl
      << this->pre_op () << bt->name () << "_ptr" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{" << be_idt_nl;

  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ".in ();" << be_uidt_nl;

  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_valuebox (be_valuebox *node)
{
  return this->valuetype_common (node);
}

int
be_visitor_valuetype_field_cs::valuetype_common (be_type *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (" << bt->name ()
      << "* val)" << be_nl
      << "{" << be_idt_nl;

  *os << "::CORBA::add_ref (val);" << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = val;" << be_uidt_nl;
  *os << "}" << be_nl_2;

  *os << "/// Retrieve the member" << be_nl
      << this->pre_op () << bt->name () << " *" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{" << be_idt_nl;

  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ".in ();" << be_uidt_nl;

  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_valuetype (be_valuetype *node)
{
  return this->valuetype_common (node);
}

int
be_visitor_valuetype_field_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_field_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (" << bt->name ()
      << "* val)" << be_nl
      << "{" << be_idt_nl;

  *os << "::CORBA::add_ref (val);" << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = val;" << be_uidt_nl;
  *os << "}" << be_nl_2;

  *os << "/// Retrieve the member." << be_nl
      << this->pre_op () << bt->name () << " *" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{" << be_idt_nl;

  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix
      ()
      << ".in ();" << be_uidt_nl;

  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_valuetype_field_cs::visit_predefined_type (be_predefined_type *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_predef... - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu, os);

  AST_PredefinedType::PredefinedType pt = node->pt ();
  bool is_object = (pt == AST_PredefinedType::PT_object
                    || pt == AST_PredefinedType::PT_pseudo);

  // IN parameter semantics call for 'const' qualifier except
  // for interface types, which here are Object and TypeCode.
  *os << "::" << ub->local_name ()
      << " (" << (is_object ? "" : "const ") << bt->name ();

  if (is_object)
    {
      *os << "_ptr";
    }
  else if (pt == AST_PredefinedType::PT_any)
    {
      *os << " &";
    }

  *os << " val)" << be_nl
      << "{" << be_idt_nl;

  switch (pt)
  {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      *os << "this->" << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix () << " = ::"
          << bt->name () << "::_duplicate (val);" << be_uidt_nl;

      break;
    case AST_PredefinedType::PT_any:
      *os << "this->" << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix () << " = val;" << be_uidt_nl;

      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "// Set the value." << be_nl
          << "this->" << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix ()
          << " = val;" << be_uidt_nl;

      break;
  }

  *os << "}" << be_nl_2;

  switch (node->pt ())
  {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      *os << "/// Retrieve the member" << be_nl
          << this->pre_op () << "::" << bt->name () << "_ptr" << be_nl;

      this->op_name (bu,
                     os);

      *os << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->"
          << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix ()
          << ".in ();" << be_uidt_nl
          << "}";

      break;
    case AST_PredefinedType::PT_any:
      *os << "// Retrieve the member." << be_nl
          << this->pre_op () << "const ::" << bt->name () << " &" << be_nl;

      this->op_name (bu, os);

      *os << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->"
          << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix ()
          << ";" << be_uidt_nl
          << "}" << be_nl_2;

      *os << "// Retrieve the member" << be_nl
          << this->pre_op () << "::" << bt->name () << " &" << be_nl;

      this->op_name (bu, os);

      *os << "::" << ub->local_name ()
          << " (void)" << be_nl
          << "{" << be_idt_nl
          << "return this->"
          << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix ()
          << ";" << be_uidt_nl
          << "}";

      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "// Retrieve the member" << be_nl
          << this->pre_op () << "::" << bt->name () << be_nl;

      this->op_name (bu, os);

      *os << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->"
          << bu->field_pd_prefix () << ub->local_name ()
          << bu->field_pd_postfix ()
          << ";" << be_uidt_nl
          << "}";

      break;
  }

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_sequence (be_sequence *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // (1) set from a const
  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  *os << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = val;" << be_uidt_nl;
  *os << "}" << be_nl_2;

  *os << "/// Readonly get method." << be_nl
      << this->pre_op () << "const " << bt->name () << " &" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name ()
      << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "/// Read/write get method." << be_nl
      << this->pre_op () << bt->name () << " &" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix()
      << ";" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Three methods to set the string value.

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  if (node->width () == (long) sizeof (char))
    {
      *os << "::" << ub->local_name () << " (char *val)";
    }
  else
    {
      *os << "::" << ub->local_name () << " ( ::CORBA::WChar *val)";
    }

  *os << be_nl
      << "{" << be_idt_nl;
  *os << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = (val);" << be_uidt_nl
      << "}" << be_nl_2;

  // (2) Set method from const char * or const wchar*.
  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);
  *os << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const char *val)" << be_nl;
    }
  else
    {
      *os << " (const ::CORBA::WChar *val)" << be_nl;
    }

  *os << "{" << be_idt_nl;

  *os << "// Set the value." << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = ";

  if (node->width () == (long) sizeof (char))
    {
      *os << "::CORBA::string_dup (val);" << be_uidt_nl;
    }
  else
    {
      *os << "::CORBA::wstring_dup (val);" << be_uidt_nl;
    }

  *os << "}" << be_nl_2;

  // (3) Set from const String_var&.
  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const ::CORBA::String_var &val)" << be_nl;
    }
  else
    {
      *os << " (const ::CORBA::WString_var &val)" << be_nl;
    }

  *os << "{" << be_idt_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << "::CORBA::String_var " << ub->local_name ();
    }
  else
    {
      *os << "::CORBA::WString_var " << ub->local_name ();
    }

  *os << "_var = val;" << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = "
      << ub->local_name () << "_var._retn ();" << be_uidt_nl;
  *os << "}" << be_nl_2;

  if (node->width () == (long) sizeof (char))
    {
      *os << this->pre_op () << "const char *" << be_nl;
    }
  else
    {
      *os << this->pre_op () << "const ::CORBA::WChar *" << be_nl;
    }

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name ()
      << " (void) const // get method" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_structure - "
                             "codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // (1) Set from a const.
  *os << "/// Accessor to set the member." << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  *os << "this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << " = val;" << be_uidt_nl;

  *os << "}" << be_nl;

  *os << "/// Readonly get method." << be_nl
      << this->pre_op () << "const " << bt->name () << " &" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl;
  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl;
  *os << "}" << be_nl;

  *os << "/// Read/write get method." << be_nl
      << this->pre_op () << bt->name () << " &" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl;
  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl;
  *os << "}";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_union (be_union *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "/// Accessor to set the member" << be_nl
      << this->pre_op () << "void" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;
  *os << "this->" << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix () << " = val;" << be_uidt_nl;
  *os << "}" << be_nl;

  *os << "/// Readonly get method." << be_nl
      << this->pre_op () << "const " << bt->name () << " &" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl
      << "}" << be_nl;

  *os << "/// Read/write get method." << be_nl
      << this->pre_op () << bt->name () << " &" << be_nl;

  this->op_name (bu,
                 os);

  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix () << ub->local_name ()
      << bu->field_pd_postfix ()
      << ";" << be_uidt_nl
      << "}";

  return 0;
}

void
be_visitor_valuetype_field_cs::setenclosings (const char *pre)
{
  this->pre_op_ = pre;
}

const char*
be_visitor_valuetype_field_cs::pre_op ()
{
  return this->pre_op_;
}

// Retrieve the fully scoped skeleton name.
void
be_visitor_valuetype_field_cs::op_name (be_valuetype *node,
                                        TAO_OutStream *os)
{
  if (this->in_obv_space_)
    {
      *os << node->full_obv_skel_name ();
    }
  else
    {
      *os << node->name ();
    }
}
