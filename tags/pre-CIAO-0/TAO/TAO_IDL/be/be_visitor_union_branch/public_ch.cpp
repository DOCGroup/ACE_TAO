//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union_branch in the public part.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, public_ch, "$Id$")


// **********************************************
//  visitor for union_branch in the client header file
// **********************************************

// Constructor.
be_visitor_union_branch_public_ch::be_visitor_union_branch_public_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// Destructor.
be_visitor_union_branch_public_ch::~be_visitor_union_branch_public_ch (void)
{
}

// Visit the union_branch node.
int
be_visitor_union_branch_public_ch::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types  that a union_branch can be.

// Visit array type.
int
be_visitor_union_branch_public_ch::visit_array (be_array *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_array - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
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
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_array - "
                             "Bad visitor\n"
                             ),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_array - "
                             "codegen failed\n"
                             ),
                            -1);
        }

      delete visitor;

      // Now use this array as a "type" for the subsequent declarator
      // the set method.
      *os << "void " << ub->local_name () << " ("
          << "_" << bt->local_name () << ");" << be_nl;
      // The get method.
      *os << "_" << bt->local_name () << "_slice * " << ub->local_name ()
          << " (void) const; // get method" << be_nl << be_nl;
    }
  else
    {
      // Now use this array as a "type" for the subsequent declarator
      // the set method.
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");"
          << be_nl;
      // The get method.
      *os << bt->nested_type_name (bu, "_slice *") << " " << ub->local_name ()
          << " (void) const;" << be_nl << be_nl;
    }

  return 0;
}

// Visit enum type.
int
be_visitor_union_branch_public_ch::visit_enum (be_enum *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_enum - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
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
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ),
                            -1);
        }

      delete visitor;
    }

  // Now use this enum as a "type" for the subsequent declarator
  // the set method.
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu) << ");"
      << be_nl;
  // the get method.
  *os << bt->nested_type_name (bu) << " " << ub->local_name ()
      << " (void) const;" << be_nl << be_nl;

  return 0;
}

// Visit interface type.
int
be_visitor_union_branch_public_ch::visit_interface (be_interface *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_interface - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " (void) const;" << be_nl << be_nl;
  return 0;
}

// Visit interface forward type.
int
be_visitor_union_branch_public_ch::visit_interface_fwd (be_interface_fwd *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " (void) const;" << be_nl << be_nl;

  return 0;
}

// Visit valuetype type.
int
be_visitor_union_branch_public_ch::visit_valuetype (be_valuetype *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_valuetype - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " (void) const;" << be_nl << be_nl;

  return 0;
}

// Visit valuetype forward type.
int
be_visitor_union_branch_public_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_valuetype_fwd - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set method.
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ")" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " (void) const;" << be_nl << be_nl;

  return 0;
}

// Visit predefined type.
int
be_visitor_union_branch_public_ch::visit_predefined_type (be_predefined_type *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu, "_ptr") << ");" << be_nl;
      *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
          << " (void) const;" << be_nl << be_nl;
      break;
    case AST_PredefinedType::PT_any:
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");" << be_nl;
      *os << "const " << bt->nested_type_name (bu) << " "
          << ub->local_name () << " (void) const;" << be_nl << be_nl;
      *os << bt->nested_type_name (bu) << " "
          << ub->local_name () << " (void);" << be_nl << be_nl;
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");" << be_nl;
      *os << bt->nested_type_name (bu) << " " << ub->local_name ()
          << " (void) const;" << be_nl << be_nl;
    }

  return 0;
}

// visit sequence type
int
be_visitor_union_branch_public_ch::visit_sequence (be_sequence *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_sequence - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
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
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ),
                            -1);
        }

      delete visitor;

      // Generate the anonymous sequence member typedef
      // but we must protect against certain versions of g++.
      // This provides a consistent name to use instead of the
      // implementation-specific name.
      *os << "\n#if !defined (__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_nl
          << "typedef " << bt->nested_type_name (bu)
          << " _" << ub->local_name () << "_seq;\n";
      *os << "#endif /* ! __GNUC__ || ACE_HAS_GNUG_PRE_2_8 */"
          << be_nl << be_nl;
    }

  *os << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);" << be_nl;
  *os << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const;"
      << be_nl;
  *os << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void);" << be_nl << be_nl;

  return 0;
}

// Visit string type.
int
be_visitor_union_branch_public_ch::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_string - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Three methods to set the string value
  if (node->width () == (long) sizeof (char))
    {
      *os << "void " << ub->local_name () << " (char *);" << be_nl;
      *os << "void " << ub->local_name () << " (const char *);"
          << be_nl;
      *os << "void " << ub->local_name () << " (const CORBA::String_var&);"
          << be_nl;
      *os << "const char *" << ub->local_name ()
          << " (void) const;" << be_nl << be_nl;
    }
  else
    {
      *os << "void " << ub->local_name () << " (CORBA::WChar *);" << be_nl;
      *os << "void " << ub->local_name () << " (const CORBA::WChar *);"
          << be_nl;
      *os << "void " << ub->local_name () << " (const CORBA::WString_var&);"
          << be_nl;
      *os << "const CORBA::WChar *" << ub->local_name ()
          << " (void) const;" << be_nl << be_nl;
    }

  return 0;
}

// Visit structure type.
int
be_visitor_union_branch_public_ch::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_structure - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // First generate the sequence declaration.
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_structure - "
                             "Bad visitor\n"
                             ),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_structure - "
                             "codegen failed\n"
                             ),
                            -1);
        }

      delete visitor;
    }

  *os << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);" << be_nl
      << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const;"
      << be_nl
      << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void);" << be_nl << be_nl;

  return 0;
}

// Visit typedefed type.
int
be_visitor_union_branch_public_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_spec_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

// Visit union type.
int
be_visitor_union_branch_public_ch::visit_union (be_union *node)
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
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the union declaration.
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_union - "
                             "Bad visitor\n"
                             ),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_union - "
                             "codegen failed\n"
                             ),
                            -1);
        }

      delete visitor;
    }

  *os << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);" << be_nl
      << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const;"
      << be_nl
      << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void);" << be_nl << be_nl;

  return 0;
}
