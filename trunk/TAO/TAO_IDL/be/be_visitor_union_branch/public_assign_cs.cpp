//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_assign_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch for the assignment operator
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, public_assign_cs, "$Id$")


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
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch

  TAO_OutStream *os = this->ctx_->stream ();

  // This visitor is used when we are generating the copy ctor and
  // assignment operator for the union.
  // Individual assignment of the members takes place inside a case
  // statement because the type of member assigned is based on the value
  // of the discriminant
  os->indent ();
  if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
    {
      *os << "case " << ub->label ()->label_val ()->n ()  << ":" << be_nl;
      *os << "{" << be_idt << "\n";
    }
  else
    {
      *os << "case " << ub->label ()->label_val () << ":" << be_nl;
      *os << "{" << be_idt << "\n";
    }

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
  *os << "break;\n";
  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

int
be_visitor_union_branch_public_assign_cs::visit_array (be_array *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // for anonymous arrays, the type name has a _ prepended. We compute the
  // fullname with or without the underscore and use it later on.
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
          ACE_OS::sprintf (fname, "%s::_%s", parent->fullname (), 
                           bt->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", bt->fullname ());
        }
    }
  else
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", bt->fullname ());
    }

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free "
          << "it on an assignment" << be_nl;
      *os << fname << "_var " << ub->local_name () 
          << "_var (this->u_." << ub->local_name () << "_);" << be_nl;
      *os << "// make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = " << fname
          << "_dup (u.u_." << ub->local_name () << "_);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->u_." << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }

  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_enum (be_enum *)
{
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl
          << "this->u_." << ub->local_name () << "_ = u.u_."
          << ub->local_name () << "_;" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_interface (be_interface *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
          << ub->local_name () << "_->ptr ());" << be_nl;
      *os << "// make a copy" << be_nl;
      *os << ub->local_name () << "_var = " << bt->name ()
          << "::_duplicate (u.u_." << ub->local_name () << "_->ptr ());" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "*this->u_." << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_interface_fwd (be_interface_fwd *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
          << ub->local_name () << "_->ptr ());" << be_nl;
      *os << "// make a copy" << be_nl;
      *os << ub->local_name () << "_var = " << bt->name ()
          << "::_duplicate (u.u_." << ub->local_name () << "_->ptr ());" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "*this->u_." << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_predefined_type (be_predefined_type *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      switch (node->pt ())
        {
        case AST_PredefinedType::PT_pseudo:
          *os << "// set the value" << be_nl;
          *os << "// store current val in a _var so as to free it on an assignment"
              << be_nl;
          *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
              << ub->local_name () << "_);" << be_nl;
          *os << "// make a copy" << be_nl;
          *os << ub->local_name () << "_var = " << bt->name ()
              << "::_duplicate (u.u_." << ub->local_name () << "_);" << be_nl;
          *os << "// the _var gives up ownership" << be_nl;
          *os << "this->u_." << ub->local_name () << "_ = "
              << ub->local_name () << "_var._retn ();" << be_uidt_nl;
          break;
        case AST_PredefinedType::PT_any:
          *os << "// set the value" << be_nl;
          *os << "// store current val in a _var so as to free it on an assignment"
              << be_nl;
          *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
              << ub->local_name () << "_);" << be_nl;
          *os << "// make a deep copy" << be_nl;
          *os << ub->local_name () << "_var = new " << bt->name ()
              << " (*u.u_." << ub->local_name () << "_);" << be_nl;
          *os << "// the _var gives up ownership" << be_nl;
          *os << "this->u_." << ub->local_name () << "_ = "
              << ub->local_name () << "_var._retn ();" << be_uidt_nl;
          break;
        case AST_PredefinedType::PT_void:
          break;
        default:
          *os << "// set the value" << be_nl
              << "this->u_." << ub->local_name () << "_ = u.u_."
              << ub->local_name () << "_;" << be_uidt_nl;
          break;
        }

    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_sequence (be_sequence *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
          << ub->local_name () << "_);" << be_nl;
      *os << "// make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = new " << bt->name ()
          << " (*u.u_." << ub->local_name () << "_);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->u_." << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << "CORBA::String_var " << ub->local_name () << "_var (this->u_."
          << ub->local_name () << "_);" << be_nl;
      *os << "// make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = CORBA::string_dup (u.u_."
          << ub->local_name () << "_);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->u_." << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}

int
be_visitor_union_branch_public_assign_cs::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      if (bt->size_type () == be_type::VARIABLE)
        {
          *os << "// set the value" << be_nl;
          *os << "// store current val in a _var so as to free it on an assignment"
              << be_nl;
          *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
              << ub->local_name () << "_);" << be_nl;
          *os << "// make a deep copy" << be_nl;
          *os << ub->local_name () << "_var = new " << bt->name ()
              << " (*u.u_." << ub->local_name () << "_);" << be_nl;
          *os << "// the _var gives up ownership" << be_nl;
          *os << "this->u_." << ub->local_name () << "_ = "
              << ub->local_name () << "_var._retn ();" << be_uidt_nl;
        }
      else
        {
          *os << "// set the value" << be_nl;
          *os << "this->u_." << ub->local_name () << "_ = u.u_."
              << ub->local_name () << "_;" << be_uidt_nl;
        }
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
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
                         "(%N:%l) be_visitor_union_branch_public_assign_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->u_."
          << ub->local_name () << "_);" << be_nl;
      *os << "// make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = new " << bt->name ()
          << " (*u.u_." << ub->local_name () << "_);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->u_." << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  return 0;
}
