//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_ch.cpp
//
// = DESCRIPTION
//    Visitor generating the accessor and modifier declarations
//    for valuetype fields in the valuetype class (header).
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_union_branch/public_ch.cpp
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be_visitor_valuetype, field_ch, "$Id$")


// Constructor.
be_visitor_valuetype_field_ch::be_visitor_valuetype_field_ch
    (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
  setenclosings ("",";");
}

// Destructor.
be_visitor_valuetype_field_ch::~be_visitor_valuetype_field_ch (void)
{
}

// Visit the field node.
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

// =Visit operations on all possible data types (valuetype state member).

// visit array type
int
be_visitor_valuetype_field_ch::visit_array (be_array *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // This is the case of an anonymous array inside a union.

      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the array declaration.
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_array - "
                             "Bad visitor\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }

      delete visitor;
      // Now use this array as a "type" for the subsequent declarator
      // the set method.
      *os << pre_op () << "void " << ub->local_name () << " ("
          << "_" << bt->local_name () << ")"
          << post_op () << "    // set" << be_nl;
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
      *os << pre_op () << "void " << ub->local_name () << " ("
          << bt->name () << ")" << post_op () << "    // set"
          << be_nl;
      // The get method.
      *os << pre_op()
          << bt->name () << "_slice *" << ub->local_name ()
          << " (void)" << post_op () << be_nl;
      // The get (read/write) method.
      *os << pre_op () << "const "
          << bt->name () << "_slice *" << ub->local_name ()
          << " (void) const" << post_op () << "\n\n";
    }

  return 0;
}

// Visit enum type.
int
be_visitor_valuetype_field_ch::visit_enum (be_enum *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the enum declaration.
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_enum - "
                             "Bad visitor\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }

      delete visitor;
    }

  // Now use this enum as a "type" for the subsequent declarator
  // the set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << ")" << post_op () << "    // set"
      << be_nl;
  // The get method.
  *os << pre_op () << bt->name () << " " << ub->local_name ()
      << " (void) const" << post_op () << "     // get method"
      << be_nl << be_nl;

  return 0;
}

// Visit interface type.
int
be_visitor_valuetype_field_ch::visit_interface (be_interface *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Set method.
  *os << pre_op() << "void " << ub->local_name () << " ("
      << bt->name () << "_ptr"
      << ")" << post_op() << "    // set" << be_nl;
  // Get method.
  *os << pre_op()
      << bt->name () << "_ptr " << ub->local_name ()
      << " (void) const" << post_op() << "     // get method"
      << be_nl << be_nl;

  return 0;
}

// Visit interface forward type.
int
be_visitor_valuetype_field_ch::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << "_ptr"
      << ")" << post_op () << "    // set" << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << "_ptr " << ub->local_name ()
      << " (void) const" << post_op () << "     // get method"
      << be_nl << be_nl;

  return 0;
}

// Visit valuetype type.
int
be_visitor_valuetype_field_ch::visit_valuetype (be_valuetype *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << " *"
      << ")" << post_op () << "    // set" << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << " *" << ub->local_name ()
      << " (void) const" << post_op () << "     // get method"
      << be_nl << be_nl;

  return 0;
}

// Visit valuetype forward type.
int
be_visitor_valuetype_field_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " ("
      << bt->name () << " *"
      << ")" << post_op () << "    // set" << be_nl;
  // Get method.
  *os << pre_op ()
      << bt->name () << " *" << ub->local_name ()
      << " (void) const" << post_op () << "     // get method"
      << be_nl << be_nl;

  return 0;
}

// Visit predefined type.
int
be_visitor_valuetype_field_ch::visit_predefined_type (be_predefined_type *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // set method.
      *os << pre_op () << "void " << ub->local_name () << " ("
          << bt->name () << "_ptr)"
          << post_op () << "    // set" << be_nl;
        // Get method.
      *os << pre_op ()
          << bt->name () << "_ptr " << ub->local_name ()
          << " (void) const" << post_op () << "     // get method"
          << be_nl << be_nl;
      break;
    case AST_PredefinedType::PT_any:
      // Set method.
      *os << pre_op () << "void " << ub->local_name () << " ("
          << bt->name () << ")"
          << post_op () << "    // set" << be_nl;
      // Get method (read-only).
      *os << pre_op () << "const " << bt->name () << " "
          << ub->local_name () << " (void) const"
          << post_op () << "     // get method"
          << be_nl << be_nl;
      // Get method (read/write).
      *os << pre_op () << bt->name () << " "
          << ub->local_name () << " (void)"
          << post_op () << "     // get method"
          << be_nl << be_nl;
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // Set method.
      *os << pre_op () << "void " << ub->local_name () << " ("
          << bt->name () << ")"
          << post_op () << "    // set" << be_nl;
      // Get method.
      *os << pre_op () << bt->name () << " " << ub->local_name ()
          << " (void) const" << post_op () << "     // get method"
          << be_nl << be_nl;
    }
  return 0;
}

// Visit sequence type.
int
be_visitor_valuetype_field_ch::visit_sequence (be_sequence *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Not a typedef and bt is defined here.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the sequence declaration.
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_sequence - "
                             "Bad visitor\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }

      delete visitor;

      // Generate the anonymous sequence member typedef
      // but we must protect against certain versions of g++.
      // This provides a consistent name to use instead of the
      // implementation-specific name.
      os->decr_indent (0);
      *os << "#if !defined (__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_idt_nl
          << "typedef " << bt->nested_type_name (bu)
          << " _" << ub->local_name () << "_seq;" << be_uidt_nl;
      *os << "#endif /* ! __GNUC__ || ACE_HAS_GNUG_PRE_2_8 */\n" << be_nl;
      os->incr_indent ();

    }

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " (const "
      << bt->name () << " &)"
      << post_op () << "    // set" << be_nl;
  // Read-only.
  *os << pre_op () << "const " << bt->name () << " &"
      << ub->local_name  () << " (void) const"
      << post_op () << "     // get method (read only)" << be_nl;
  // Read/write.
  *os << pre_op () << bt->name () << " &" << ub->local_name ()
      << " (void)"
      << post_op () << "     // get method (read/write only)"
      << be_nl << be_nl;

  return 0;
}

// Visit string type.
int
be_visitor_valuetype_field_ch::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Three methods to set the string value.
  if (node->width () == (long) sizeof (char))
    {
      *os << pre_op ()
          << "void " << ub->local_name () << " (char *)"
          << post_op () << "     // set" << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const char *)"
          << post_op () << "     // set" << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const CORBA::String_var&)"
          << post_op () << "     // set" << be_nl;
      // Get method.
      *os << pre_op () << "const char *" << ub->local_name ()
          << " (void) const" << post_op () << "     // get method"
          << be_nl << be_nl;
    }
  else
    {
      *os << pre_op ()
          << "void " << ub->local_name () << " (CORBA::WChar *)"
          << post_op () << "     // set" << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const CORBA::WChar *)"
          << post_op () << "     // set" << be_nl;
      *os << pre_op ()
          << "void " << ub->local_name () << " (const CORBA::WString_var&)"
          << post_op () << "     // set" << be_nl;
      // Get method.
      *os << pre_op() << "const CORBA::WChar *" << ub->local_name ()
          << " (void) const" << post_op() << "     // get method"
          << be_nl << be_nl;
    }

  return 0;
}

// Visit structure type.
int
be_visitor_valuetype_field_ch::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Not a typedef and bt is defined here.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the sequence declaration.
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_structure - "
                             "Bad visitor\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_structure - "
                             "codegen failed\n"),
                            -1);
        }

      delete visitor;
    }

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " (const "
      << bt->name () << " &)"
      << post_op () << "    // set" << be_nl;
    // Read-only.
  *os << pre_op () << "const " << bt->name () << " &";
  *os << ub->local_name  () << " (void) const"
      << post_op () << "     // get method (read only)" << be_nl
    // Read/write.
      << pre_op () << bt->name () << " &" << ub->local_name ()
      << " (void)" << post_op () << "     // get method (read/write only)"
      << be_nl << be_nl;

  return 0;
}

// Visit typedefed type.
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

// Visit union type.
int
be_visitor_valuetype_field_ch::visit_union (be_union *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();
  be_type *bt;

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

  // Not a typedef and bt is defined here.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the sequence declaration.
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_union - "
                             "Bad visitor\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }

      delete visitor;
    }

  // Set method.
  *os << pre_op () << "void " << ub->local_name () << " (const "
      << bt->name () << " &)"
      << post_op () << "    // set" << be_nl;
    // Read-only.
  *os << pre_op () << "const " << bt->name () << " &"
      << ub->local_name  () << " (void) const"
      << post_op () << "     // get method (read only)"
      << be_nl;
    // Read/write.
  *os << pre_op () << bt->name () << " &" << ub->local_name ()
      << " (void)" << post_op () << "     // get method (read/write only)"
      << be_nl << be_nl;

  return 0;
}

void
be_visitor_valuetype_field_ch::setenclosings (const char *pre, const char *post)
{
  pre_op_ = pre;
  post_op_ = post;
}

const char*
be_visitor_valuetype_field_ch::pre_op ()
{
  return pre_op_;
}

const char*
be_visitor_valuetype_field_ch::post_op ()
{
  return post_op_;
}
