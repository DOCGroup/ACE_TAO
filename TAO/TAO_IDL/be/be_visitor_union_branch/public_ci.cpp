//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch in the client inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, public_ci, "$Id$")


// *****************************************************
//  visitor for union_branch in the client inline file
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
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }

  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

int
be_visitor_union_branch_public_ci::visit_array (be_array *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
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

      // First generate the inline operations for this anonymous array type.
      ctx.state (TAO_CodeGen::TAO_ARRAY_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }

      delete visitor;
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
  os->indent ();
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << fname
      << " val)// set" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// set the value" << be_nl
      << "this->u_." << ub->local_name () << "_ = "
      << fname << "_dup (val);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Get method.
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << fname << "_slice *" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_enum (be_enum *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
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
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  os->indent ();
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << " val)// set" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// set the value" << be_nl
      << "this->u_." << ub->local_name () << "_ = val;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Get method.
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_interface (be_interface *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  os->indent ();
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)// set" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  idl_bool bt_is_defined = node->is_defined ();

  *os << ";" << be_nl
      << "typedef "
      << bt->nested_type_name (bu, "_var")
      << " OBJECT_FIELD;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl;

      if (bt_is_defined)
        {
          *os << "OBJECT_FIELD (" << bt->name () << "::";
        }
      else
        {
          *os << "OBJECT_FIELD (tao_" << node->flat_name ();
        }

  *os << "_duplicate (val))" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Get method.
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << "_ptr " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_->ptr ();" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_interface_fwd (be_interface_fwd *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  os->indent ();
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)// set" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  // Default label.
  else
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  idl_bool bt_is_defined = node->full_definition ()->is_defined ();

  *os << ";" << be_nl
      << "typedef "
      << bt->nested_type_name (bu, "_var")
      << " OBJECT_FIELD;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl;

      if (bt_is_defined)
        {
          *os << "OBJECT_FIELD (" << bt->name () << "::";
        }
      else
        {
          *os << "OBJECT_FIELD (tao_" << node->flat_name ();
        }

  *os << "_duplicate (val))" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Get method.
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << "_ptr " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_->ptr ();" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  os->indent ();
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ();

  if (node->pt () == AST_PredefinedType::PT_pseudo)
    {
      *os << "_ptr";
    }

  *os << " val)" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl;

  switch (node->pt ())
    {
      case AST_PredefinedType::PT_pseudo:
        if (!ACE_OS::strcmp (node->local_name ()->get_string (), "Object"))
          {
            *os << "typedef CORBA::Object_var OBJECT_FIELD;" << be_nl
                << "ACE_NEW (" << be_idt << be_idt_nl
                << "this->u_." << ub->local_name () << "_," << be_nl
                << "OBJECT_FIELD (CORBA::Object::_duplicate (val))"
                << be_uidt_nl
                << ");" << be_uidt << be_uidt_nl;
          }
        else
          {
            *os << "this->u_." << ub->local_name () << "_ = "
                << bt->name () << "::_duplicate (val);" << be_uidt_nl;
          }
        break;

      case AST_PredefinedType::PT_any:
        *os << "ACE_NEW (" << be_idt << be_idt_nl
            << "this->u_." << ub->local_name ()
            << "_," << be_nl
            << bt->name () << " (val)" << be_uidt_nl
            << ");" << be_uidt << be_uidt_nl;
        break;

      case AST_PredefinedType::PT_void:
        break;

      default:
        *os << "// set the value" << be_nl
            << "this->u_." << ub->local_name ()
            << "_ = val;" << be_uidt_nl;
    }

  *os << "}" << be_nl << be_nl;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // Get method.
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE " << bt->name () << "_ptr" << be_nl
          << bu->name () << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl;

      if (!ACE_OS::strcmp (bt->local_name ()->get_string (), "Object"))
        {
          *os << "return this->u_." << ub->local_name ()
              << "_->ptr ();" << be_uidt_nl;
        }
      else
        {
          *os << "return this->u_." << ub->local_name ()
              << "_;" << be_uidt_nl;
        }

      *os << "}\n\n";
      break;
    case AST_PredefinedType::PT_any:
      // Get method with read-only access.
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE const " << bt->name () << be_nl
          << bu->name () << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
          << "}" << be_nl << be_nl;

      // Get method with read/write access
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE " << bt->name () << be_nl
          << bu->name () << "::" << ub->local_name ()
          << " (void)" << be_nl
          << "{" << be_idt_nl
          << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
          << "}\n\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // Get method.
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE " << bt->name () << be_nl
          << bu->name () << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
          << "}\n\n";
    }

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_sequence (be_sequence *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
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

      // First generate inline operations for this anonymous sequence type.
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }

      delete visitor;
    }

  // (1) Set from a const.
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << bt->name () << " (val)" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Readonly get method.
  *os << "// readonly get method " << be_nl
      << "ACE_INLINE const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Read/write get method.
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_string (be_string *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Three methods to set the string value.

  // (1) Set method from char* or wchar*.
  os->indent ();
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << bu->name () << "::" << ub->local_name () << " (char *val)";
    }
  else
    {
      *os << bu->name () << "::" << ub->local_name ()
          << " (CORBA::WChar *val)";
    }

  *os << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// set the value" << be_nl
      << "this->u_." << ub->local_name () << "_ = val;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // (2) Set method from const char * or const wchar *.
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const char *val)" << be_nl << "{" << be_idt_nl;
    }
  else
    {
      *os << " (const CORBA::WChar *val)" << be_nl << "{" << be_idt_nl;
    }

  *os << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// set the value" << be_nl
      << "this->u_." << ub->local_name () << "_ = ";

  if (node->width () == (long) sizeof (char))
    {
      *os << "CORBA::string_dup (val);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }
  else
    {
      *os << "CORBA::wstring_dup (val);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // (3) Set from const String_var& or WString_var&
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const CORBA::String_var &val)" << be_nl;
    }
  else
    {
      *os << " (const CORBA::WString_var &val)" << be_nl;
    }

  *os << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "// set the value" << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << "CORBA::String_var " << ub->local_name ();
    }
  else
    {
      *os << "CORBA::WString_var " << ub->local_name ();
    }

  *os << "_var = val;" << be_nl
      << "this->u_." << ub->local_name () << "_ = "
      << ub->local_name () << "_var._retn ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Get method.
  if (node->width () == (long) sizeof (char))
    {
      *os << "ACE_INLINE const char *" << be_nl;
    }
  else
    {
      *os << "ACE_INLINE const CORBA::WChar *" << be_nl;
    }

  *os << bu->name () << "::" << ub->local_name ()
      << " (void) const // get method" << be_nl
      << "{" << be_idt_nl
      << "return this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_structure (be_structure *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_structure - "
                         "bad context information\n"
                         ), -1);
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
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_structure - "
                             "Bad visitor\n"
                             ), -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_structure - "
                             "codegen failed\n"
                             ), -1);
        }

      delete visitor;
    }

  // (1) Set from a const.
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
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

  *os << "}" << be_nl << be_nl;

  // Readonly get method.
  *os << "// readonly get method " << be_nl
      << "ACE_INLINE const " << bt->name () << " &" << be_nl
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

  *os << "}" << be_nl << be_nl;

  // Read/write get method.
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
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

  *os << "}\n\n";

  return 0;
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
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_union (be_union *node)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch ();
  be_union *bu =
    this->ctx_->be_scope_as_union ();
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
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_union - "
                         "bad context information\n"
                         ), -1);
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
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ci::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }

      delete visitor;
    }

  // (1) Set from a const.
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl
      << "// set the discriminant val" << be_nl
      << "this->_reset (";

  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      ub->gen_label_value (os);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_label_value (os);
    }
  else
  // We have an explicit default case.
    {
      ub->gen_default_label_value (os, bu);
      *os << ", 0);" << be_nl
          << "this->disc_ = ";
      ub->gen_default_label_value (os, bu);
    }

  *os << ";" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << bt->name () << " (val)" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Readonly get method.
  *os << "// readonly get method " << be_nl
      << "ACE_INLINE const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Read/write get method.
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->u_." << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}
