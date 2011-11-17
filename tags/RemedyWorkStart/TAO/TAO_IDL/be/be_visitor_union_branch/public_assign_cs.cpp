
//=============================================================================
/**
 *  @file    public_assign_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union Branch for the assignment operator
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// **********************************************
//  visitor for union_branch in the client stubs file generating the code for
//  the copy ctor and assignment operator
// **********************************************

// constructor
be_visitor_union_branch_public_assign_cs::
be_visitor_union_branch_public_assign_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_assign_cs::
~be_visitor_union_branch_public_assign_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_assign_cs::visit_union_branch (
    be_union_branch *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl;

  // This visitor is used when we are generating the copy ctor and
  // assignment operator for the union.
  // Individual assignment of the members takes place inside a case
  // statement because the type of member assigned is based on the value
  // of the discriminant
  for (unsigned long i = 0;
       i < node->label_list_length ();
       ++i)
    {
      // check if we are printing the default case
      if (node->label (i)->label_kind () == AST_UnionLabel::UL_default)
        {
          *os << "default:" << be_nl;
        }
      else
        {
          *os << "case ";
          node->gen_label_value (os, i);
          *os << ":" << be_nl;
        }
    }

  *os << "{" << be_idt_nl;

  // first generate the type information
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }

  *os << "}" << be_nl;
  *os << "break;";

  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

int
be_visitor_union_branch_public_assign_cs::visit_array (be_array *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // for anonymous arrays, the type name has a _ prepended. We compute the
  // full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  // save the node's local name and full name in a buffer for quick use later
  // on
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name

      if (bt->is_nested ())
        {
          be_decl *parent = be_scope::narrow_from_scope (bt->defined_in ())->decl ();
          ACE_OS::sprintf (fname, "%s::_%s", parent->full_name (),
                           bt->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", bt->full_name ());
        }
    }
  else
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", bt->full_name ());
    }

  // set the discriminant to the appropriate label
  *os << "// Make a deep copy." << be_nl;
  *os << "this->u_." << ub->local_name ()
      << "_ = " << be_idt_nl
      << fname << "_dup (u.u_."
      << ub->local_name () << "_);" << be_uidt << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_enum (be_enum *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // set the discriminant to the appropriate label
  // valid label
  *os << "this->u_." << ub->local_name () << "_ = " << be_idt_nl
      << "u.u_."
      << ub->local_name () << "_;" << be_uidt << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_interface (be_interface *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  bool bt_is_defined = node->is_defined ();

  *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl;

  // So the template will work with the macro.
  *os << "typedef "
      << bt->name () << "_var OBJECT_FIELD;" << be_nl;

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
    {
      // We are generating the copy constructor.
      *os << "ACE_NEW (" << be_idt << be_idt_nl
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

      *os << "duplicate (" << be_idt << be_idt_nl
          << "u.u_." << ub->local_name () << "_->in ()" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }
  else
    {
      // We are generating the assignment operator.
      *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
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

      *os << "duplicate (" << be_idt << be_idt_nl
          << "u.u_." << ub->local_name ()
          << "_->in ()" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl << ")," << be_uidt_nl
          << "*this" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_uidt << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_interface_fwd (
    be_interface_fwd *node
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  bool bt_is_defined = node->full_definition ()->is_defined ();

  *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl;

  // So the template will work with the macro.
  *os << "typedef "
      << bt->name () << "_var OBJECT_FIELD;" << be_nl;

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
    {
      // We are generating the copy constructor.
      *os << "ACE_NEW (" << be_idt << be_idt_nl
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

      *os << "duplicate (" << be_idt << be_idt_nl
          << "u.u_." << ub->local_name () << "_->in ()" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }
  else
    {
      // We are generating the assignment operator.
      *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
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

      *os << "duplicate (" << be_idt << be_idt_nl
          << "u.u_." << ub->local_name ()
          << "_->in ()" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl << ")," << be_uidt_nl
          << "*this" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_uidt << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_valuebox (be_valuebox *node)
{
  return this->emit_valuetype_common (node);
}

int
be_visitor_union_branch_public_assign_cs::visit_valuetype (be_valuetype *node)
{
  return this->emit_valuetype_common (node);
}

int
be_visitor_union_branch_public_assign_cs::visit_valuetype_fwd (
    be_valuetype_fwd *node
  )
{
  return this->emit_valuetype_common (node);
}

int
be_visitor_union_branch_public_assign_cs::emit_valuetype_common (be_type *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "emit_valuetype_common - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl;

  // So the template will work with the macro.
  *os << "typedef "
      << bt->name () << "_var OBJECT_FIELD;" << be_nl;
  *os << "::CORBA::add_ref (u.u_." << ub->local_name ()
      << "_->ptr ());" << be_nl;

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
    {
      // We are generating the copy constructor.
      *os << "ACE_NEW (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << "OBJECT_FIELD (u.u_." << ub->local_name () << "_->in ())"
          << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }
  else
    {
      // We are generating the assignment operator.
      *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << "OBJECT_FIELD (u.u_." << ub->local_name () << "_->in ()),"
          << be_nl
          << "*this" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_uidt << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_predefined_type (
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // set the discriminant to the appropriate label
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_object:
      // So the template will work with the macro.
      *os << "typedef ::CORBA::Object_var OBJECT_FIELD;" << be_nl;

      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
        {
          // We are generating the copy constructor.
          *os << "ACE_NEW (" << be_idt << be_idt_nl
              << "this->u_." << ub->local_name () << "_," << be_nl
              << "OBJECT_FIELD ( ::CORBA::Object"
              << "::_duplicate (u.u_." << ub->local_name ()
              << "_->in ()))" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl;
        }
      else
        {
          // We are generating the assignment operator.
          *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
              << "this->u_." << ub->local_name () << "_," << be_nl
              << "OBJECT_FIELD ( ::CORBA::Object"
              << "::_duplicate (u.u_." << ub->local_name ()
              << "_->in ()))," << be_nl
              << "*this" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl;
        }

      break;
    case AST_PredefinedType::PT_pseudo:
      *os << "this->u_." << ub->local_name () << "_ = "
          << bt->name () << "::_duplicate (u.u_."
          << ub->local_name () << "_);" << be_uidt_nl;

      break;
    case AST_PredefinedType::PT_any:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
        {
          *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
              << "{" << be_idt_nl
              << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
              << "}" << be_uidt_nl
              << "else" << be_idt_nl
              << "{" << be_idt_nl
              << "ACE_NEW (" << be_idt << be_idt_nl
              << "this->u_." << ub->local_name () << "_," << be_nl
              << bt->name () << " (*u.u_."
              << ub->local_name () << "_)" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_uidt << be_uidt_nl;
        }
      else
        {
          *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
              << "{" << be_idt_nl
              << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
              << "}" << be_uidt_nl
              << "else" << be_idt_nl
              << "{" << be_idt_nl
              << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
              << "this->u_." << ub->local_name () << "_," << be_nl
              << bt->name () << " (*u.u_."
              << ub->local_name () << "_)," << be_nl
              << "*this" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_uidt << be_uidt_nl;
        }

      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "this->u_." << ub->local_name () << "_ = "
          << "u.u_." << ub->local_name () << "_;" << be_uidt_nl;

      break;
    }

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_sequence (be_sequence *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
    {
      *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << " (*u.u_."
          << ub->local_name () << "_)" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << " (*u.u_."
          << ub->local_name () << "_)," << be_nl
          << "*this" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_string (be_string *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // set the discriminant to the appropriate label
  *os << "this->u_." << ub->local_name () << "_ = ";

  if (node->width () == (long) sizeof (char))
    {
      *os << "::CORBA::string_dup (u.u_.";
    }
  else
    {
      *os << "::CORBA::wstring_dup (u.u_.";
    }

  *os << ub->local_name () << "_);" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->size_type () == be_type::VARIABLE || node->has_constructor ())
    {
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
        {
          *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
              << "{" << be_idt_nl
              << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
              << "}" << be_uidt_nl
              << "else" << be_idt_nl
              << "{" << be_idt_nl
              << "ACE_NEW (" << be_idt << be_idt_nl
              << "this->u_." << ub->local_name () << "_," << be_nl
              << bt->name () << " (*u.u_."
              << ub->local_name () << "_)" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_uidt << be_uidt_nl;
        }
      else
        {
          *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
              << "{" << be_idt_nl
              << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
              << "}" << be_uidt_nl
              << "else" << be_idt_nl
              << "{" << be_idt_nl
              << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
              << "this->u_." << ub->local_name () << "_," << be_nl
              << bt->name () << " (*u.u_."
              << ub->local_name () << "_)," << be_nl
              << "*this" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_uidt << be_uidt_nl;
        }
    }
  else
    {
      *os << "this->u_." << ub->local_name () << "_ = u.u_."
          << ub->local_name () << "_;" << be_uidt_nl;
    }

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_public_assign_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_union (be_union *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR)
    {
      *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << " (*u.u_."
          << ub->local_name () << "_)" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << "if (u.u_." << ub->local_name () << "_ == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "this->u_." << ub->local_name () << "_ = 0;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << " (*u.u_."
          << ub->local_name () << "_)," << be_nl
          << "*this" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_union_fwd (
  be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

