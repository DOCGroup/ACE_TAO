//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_union_branch.cpp
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Union_Branch" node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

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
  TAO_OutStream *os; // output stream
  be_type *bt; // union_branch's type

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
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
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
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
    }

  // now use this enum as a "type" for the subsequent declarator
  os->indent (); // start from current indentation
  // the set method
  *os << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu) << ");// set"
      << be_nl;
  // the get method
  *os << bt->nested_type_name (bu, "_slice *") << " " << ub->local_name ()
      << " (void) const; // get method\n\n";

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
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    {
      // set method
      *os << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu, "_ptr") << ");// set" << be_nl;
      // get method
      *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
          << " (void) const; // get method\n\n";
    }
  else
    {
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
be_visitor_union_branch_public_ch::visit_string (be_string *node)
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

// **********************************************
//  visitor for union_branch in the client header generating the private information
// **********************************************

// constructor
be_visitor_union_branch_private_ch::be_visitor_union_branch_private_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_private_ch::~be_visitor_union_branch_private_ch (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_private_ch::visit_union_branch (be_union_branch *node)
{
  TAO_OutStream *os;
  be_type *bt; // union_branch's type

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
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
be_visitor_union_branch_private_ch::visit_array (be_array *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
  *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name ()
      << "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_enum (be_enum *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  *os << bt->nested_type_name (bu) << " " << ub->local_name () << "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_interface (be_interface *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // C++ does not allow an object declaration inside a union. Hence we
  // must have a pointer. This changes some of the methods
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name () <<
    "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_interface_fwd (be_interface_fwd *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // C++ does not allow an object declaration inside a union. Hence we
  // must have a pointer. This changes some of the methods
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name () <<
    "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_predefined_type (be_predefined_type *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (node->pt () == AST_PredefinedType::PT_pseudo)
    {
      // Cannot have an object inside of a union
      os->indent (); // start from current indentation
      *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name () <<
        "_;\n";
    }
  else
    {
      os->indent (); // start from current indentation
      *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
        "_;\n";
    }
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_sequence (be_sequence *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
  // C++ doesn't allow object instances inside unions, so we need a
  // pointer
  *os << bt->nested_type_name (bu) << " *" << ub->local_name () << "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_string (be_string *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  *os << "char *" << ub->local_name () << "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_structure - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
  *os << bt->nested_type_name (bu) << " " << ub->local_name () << "_;\n";
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_private_ch::visit_union (be_union *node)
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
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_union - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
  // C++ doesn't allow object instances inside unions, so we need a
  // pointer
  *os << bt->nested_type_name (bu) << " *" << ub->local_name () << "_;\n";
  return 0;
}

// *****************************************************
//  visitor for union_branch in the client inline file
// *****************************************************

// constructor
be_visitor_union_branch_public_ci::be_visitor_union_branch_public_ci (be_visitor_context *ctx)
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
  TAO_OutStream *os;
  be_type *bt; // union_branch's type

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
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
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
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

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << " val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "this->" << ub->local_name () << "_ = val;\n";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  os->decr_indent ();
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << "_slice *" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->" << ub->local_name () << "_;\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << " val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "this->" << ub->local_name () << "_ = val;\n";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  os->decr_indent ();
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->" << ub->local_name () << "_;\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "this->" << ub->local_name () << "_ = val;\n";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  os->decr_indent ();
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << "_ptr " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->" << ub->local_name () << "_;\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "this->" << ub->local_name () << "_ = val;\n";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  os->decr_indent ();
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << "_ptr " << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->" << ub->local_name () << "_;\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (" << bt->name ();
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    *os << "_ptr";
  *os << " val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "this->" << ub->local_name () << "_ = val;\n";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  os->decr_indent ();
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name ();
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    *os << "_ptr";
  *os << be_nl;
  *os << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->" << ub->local_name () << "_;\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // (1) set from a const
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";"
              << be_nl;
        }
      *os << "// set the value" << be_nl
          << "if (!this->" << ub->local_name () << "_)" << be_idt_nl
          << "this->" << ub->local_name () << "_ = new "
          << bt->name () << ";" << be_uidt_nl
          << "*this->" << ub->local_name () << "_ = val;" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

  // readonly get method
  *os << "// readonly get method " << be_nl
      << "ACE_INLINE const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_string (be_string *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // three methods to set the string value

  // (1) set method from char*
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name () << " (char *val)"
      << be_nl
      << "{" << be_idt_nl;
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "if (!this->" << ub->local_name () << "_) // does not exist"
          << be_nl
          << "  this->" << ub->local_name ()
          << "_ = new CORBA::String_var (val);" << be_nl
          << "else" << be_nl
          << "  *this->" << ub->local_name () << "_ = val;";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << be_uidt_nl << "}" << be_nl;

  // (2) set method from const char *
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const char *val)" << be_nl
      << "{\n";
  os->incr_indent ();
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "if (!this->" << ub->local_name () << "_) // does not exist"
          << be_nl
          << "  this->" << ub->local_name ()
          << "_ = new CORBA::String_var (val);" << be_nl
          << "else" << be_nl
          << "  *this->" << ub->local_name () << "_ = val;" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }

  *os << "}" << be_nl;

  // (3) set from const String_var&
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const CORBA::String_var &val)" << be_nl
      << "{" << be_idt_nl;
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "if (!this->" << ub->local_name () << "_) // does not exist"
          << be_nl
          << "  this->" << ub->local_name ()
          << "_ = new CORBA::String_var (val);" << be_nl
          << "else" << be_nl
          << "  *this->" << ub->local_name () << "_ = val;";
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << be_uidt_nl << "}" << be_nl;

  // get method
  *os << "ACE_INLINE const char *" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (void) const // get method" << be_nl
      << "{" << be_idt_nl
      << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";
  return 0;
}

int
be_visitor_union_branch_public_ci::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
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

      // first generate the enum declaration
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

  // (1) set from a const
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";"
              << be_nl;
        }
      *os << "// set the value" << be_nl
          << "this->" << ub->local_name () << "_ = val;" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

  // readonly get method
  *os << "// readonly get method " << be_nl
      << "ACE_INLINE const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_branch_public_ci::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
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
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
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

      // first generate the enum declaration
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

  // (1) set from a const
  *os << "// accessor to set the member" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;
  os->incr_indent ();  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << "*this->" << ub->local_name () << "_ = val;" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

  // readonly get method
  *os << "// readonly get method " << be_nl
      << "ACE_INLINE const " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}" << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

// **********************************************
//  visitor for union_branch in the client stubs file
// **********************************************

// constructor
be_visitor_union_branch_public_cs::be_visitor_union_branch_public_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_cs::~be_visitor_union_branch_public_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_cs::visit_union_branch (be_union_branch *node)
{
  TAO_OutStream *os; // output stream

  be_type *bt; // union_branch's type

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }
  return 0;
}

// visit array type
int
be_visitor_union_branch_public_cs::visit_array (be_array *node)
{
  return 0;
}

// visit enum type
int
be_visitor_union_branch_public_cs::visit_enum (be_enum *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // generate the typcode for enums
  ctx.state (TAO_CodeGen::TAO_ENUM_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_enum - "
                         "Bad visitor\n"
                         ), -1);
    }
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_enum - "
                         "codegen failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}

// visit sequence type
int
be_visitor_union_branch_public_cs::visit_sequence (be_sequence *node)
{
  return 0;
}

// visit string type
int
be_visitor_union_branch_public_cs::visit_string (be_string *node)
{
  return 0;
}

// visit structure type
int
be_visitor_union_branch_public_cs::visit_structure (be_structure *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // generate the inline code for structs
  ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_struct - "
                         "Bad visitor\n"
                         ), -1);
    }
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_struct - "
                         "codegen failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}

// visit union type
int
be_visitor_union_branch_public_cs::visit_union (be_union *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // generate the inline code for union
  ctx.state (TAO_CodeGen::TAO_UNION_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union - "
                         "Bad visitor\n"
                         ), -1);
    }
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union - "
                         "codegen failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}


// **********************************************
//  visitor for union_branch in the client stubs file generating the code for
//  the copy ctor and assignment operator
// **********************************************

// constructor
be_visitor_union_branch_public_assign_cs::be_visitor_union_branch_public_assign_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_assign_cs::~be_visitor_union_branch_public_assign_cs
(void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_assign_cs::visit_union_branch (be_union_branch *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend

  os = this->ctx_->stream ();

  // This visitor is used when we are generating the copy ctor and
  // assignment operator for the union.
  // Individual assignment of the members takes place inside a case
  // statement because the type of member assigned is based on the value
  // of the discriminant
  os->indent ();
  if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
    {
      *os << "case " << ub->label ()->label_val ()->n ()  << ":" << be_idt_nl;
    }
  else
    {
      *os << "case " << ub->label ()->label_val () << ":" << be_idt_nl;
    }

  *os << "this->" << ub->local_name () << "_ = u." << ub->local_name ()
      << "_;" << be_nl
      << "break;\n";
  os->decr_indent (0);

  return 0;
}
