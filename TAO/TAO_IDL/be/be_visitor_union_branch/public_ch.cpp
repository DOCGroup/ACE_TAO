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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, public_ch, "$Id$")


// **********************************************
//  visitor for union_branch in the client header file
// **********************************************

// constructor
be_visitor_union_branch_public_ch::be_visitor_union_branch_public_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_ch::~be_visitor_union_branch_public_ch (void)
{
}

// visit the union_branch node
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
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }
  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

// visit array type
int
be_visitor_union_branch_public_ch::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union node
  be_type *bt;

  // check if we are visiting this via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // this is the case of an anonymous array inside a union

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the array declaration
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
      // now use this array as a "type" for the subsequent declarator
      os->indent (); // start from current indentation
      // the set method
      *os << "void " << ub->local_name () << " ("
          << "_" << bt->local_name () << ");// set" << be_nl;
      // the get method
      *os << "_" << bt->local_name () << "_slice * " << ub->local_name ()
          << " (void) const; // get method\n\n";
    }
  else
    {
      // now use this array as a "type" for the subsequent declarator
      os->indent (); // start from current indentation
      // the set method
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");// set"
          << be_nl;
      // the get method
      *os << bt->nested_type_name (bu, "_slice *") << " " << ub->local_name ()
          << " (void) const; // get method\n\n";
    }

  return 0;
}

// visit enum type
int
be_visitor_union_branch_public_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union node
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // now use this enum as a "type" for the subsequent declarator
  os->indent (); // start from current indentation
  // the set method
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu) << ");// set"
      << be_nl;
  // the get method
  *os << bt->nested_type_name (bu) << " " << ub->local_name ()
      << " (void) const; // get method\n\n";

  return 0;
}

// visit interface type
int
be_visitor_union_branch_public_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set method
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ");// set" << be_nl;
  // get method
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " (void) const; // get method\n\n";
  return 0;
}

// visit interface forward type
int
be_visitor_union_branch_public_ch::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set method
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ");// set" << be_nl;
  // get method
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " (void) const; // get method\n\n";
  return 0;
}

// visit predefined type
int
be_visitor_union_branch_public_ch::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // set method
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu, "_ptr") << ");// set" << be_nl;
        // get method
      *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
          << " (void) const; // get method\n\n";
      break;
    case AST_PredefinedType::PT_any:
      // set method
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");// set" << be_nl;
      // get method (read-only)
      *os << "const " << bt->nested_type_name (bu) << " "
          << ub->local_name () << " (void) const; // get method\n\n";
      // get method (read/write)
      *os << bt->nested_type_name (bu) << " "
          << ub->local_name () << " (void); // get method\n\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // set method
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");// set" << be_nl;
      // get method
      *os << bt->nested_type_name (bu) << " " << ub->local_name ()
          << " (void) const; // get method\n\n";
    }
  return 0;
}

// visit sequence type
int
be_visitor_union_branch_public_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  os->indent ();
  // set method
  *os << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);// set" << be_nl;
  // read-only
  *os << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const; // get method (read only)"
      << be_nl;
  // read/write
  *os << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void); // get method (read/write only)\n\n";

  return 0;
}

// visit string type
int
be_visitor_union_branch_public_ch::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
  // three methods to set the string value
  *os << "void " << ub->local_name () << " (char *); // set" << be_nl;
  *os << "void " << ub->local_name () << " (const char *); // set"
      << be_nl;
  *os << "void " << ub->local_name () << " (const CORBA::String_var&); // set"
      << be_nl;
  //get method
  *os << "const char *" << ub->local_name ()
      << " (void) const; // get method\n\n";
  return 0;
}

// visit structure type
int
be_visitor_union_branch_public_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_structure - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_structure - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_structure - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  os->indent ();
  // set method
  *os << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);// set" << be_nl
    // read-only
      << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const; // get method (read only)"
      << be_nl
    // read/write
      << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void); // get method (read/write only)\n\n";

  return 0;
}

// visit typedefed type
int
be_visitor_union_branch_public_ch::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  os->indent (); // start from current indentation level
  this->ctx_->alias (node);     // save the node for use in code generation and
                               // indicate that the union_branch of the union_branch node
                               // is a typedefed quantity

  // make a decision based on the primitive base type
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_spec_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// visit union type
int
be_visitor_union_branch_public_ch::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  os->indent ();
  // set method
  *os << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);// set" << be_nl
    // read-only
      << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const; // get method (read only)"
      << be_nl
    // read/write
      << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void); // get method (read/write only)\n\n";

  return 0;
}
