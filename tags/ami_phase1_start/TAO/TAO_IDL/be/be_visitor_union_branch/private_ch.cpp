//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    private_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union branch for the private part of the
//    class.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, private_ch, "$Id$")


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
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
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

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // case of anonymous array in union
      os->indent ();
      *os << "_" << bt->local_name () << "_slice *" << ub->local_name ()
          << "_;\n";
    }
  else
    {
      os->indent ();
      *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name ()
          << "_;\n";
    }

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
  // must have a pointer. This changes some of the methods.
  // We use TAO_Object_Field_T<> so that _upcast() and _downcast() will work.
  *os << "TAO_Object_Field_T<" << bt->nested_type_name (bu, "")
      << "> *" << ub->local_name () << "_;\n";
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
  // must have a pointer. This changes some of the methods.
  // We use TAO_Object_Field_T<> so that _upcast() and _downcast() will work.
  *os << "TAO_Object_Field_T<" << bt->nested_type_name (bu, "")
      << "> *" << ub->local_name () << "_;\n";
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
      // check if we are dealing with a CORBA::Object
      if (!ACE_OS::strcmp (bt->local_name ()->get_string (), "Object"))
        {
          *os << "TAO_Object_Field_T<CORBA::Object> *" << ub->local_name () 
              << "_;\n";
        }
      else
        *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name () 
            << "_;\n";
    }
  else if (node->pt () == AST_PredefinedType::PT_any)
    {
      // Cannot have an object inside of a union. In addition, an Any is a
      // variable data type
      os->indent (); // start from current indentation
      *os << bt->nested_type_name (bu) << " *" << ub->local_name () <<
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
be_visitor_union_branch_private_ch::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get union branch
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_private_ch::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
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

  // if we are variable sized, we need a pointer type
  if (node->size_type () == be_type::VARIABLE)
    {
      *os << bt->nested_type_name (bu) << " *" << ub->local_name () << "_;\n";
    }
  else
    {
      *os << bt->nested_type_name (bu) << " " << ub->local_name () << "_;\n";
    }

  return 0;
}

int
be_visitor_union_branch_private_ch::visit_typedef (be_typedef *node)
{
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
  // C++ doesn't allow instances of classes
  *os << bt->nested_type_name (bu) << " *" << ub->local_name () << "_;\n";
  return 0;
}
