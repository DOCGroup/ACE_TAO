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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"


// *****************************************************
//  visitor for union_branch in the client inline file
// *****************************************************

// constructor
be_visitor_union_branch_public_ci::
be_visitor_union_branch_public_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_ci::
~be_visitor_union_branch_public_ci (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_ci::
visit_union_branch (be_union_branch *node)
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
      << "{" << be_idt_nl;
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
          << ub->local_name () << "_);" << be_nl;
      *os << "// release old and make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = " << bt->name ()
          << "_dup (val);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << "ACE_INLINE " << bt->name () << "_slice *" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl;
  *os << "return this->" << ub->local_name () << "_;" << be_uidt_nl;
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
          << ub->local_name () << "_);" << be_nl;
      *os << "// release old storage and make a copy" << be_nl;
      *os << ub->local_name () << "_var = " << bt->name ()
          << "::_duplicate (val);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
          << ub->local_name () << "_);" << be_nl;
      *os << "// release old storage and make a copy" << be_nl;
      *os << ub->local_name () << "_var = " << bt->name ()
          << "::_duplicate (val);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
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
  *os << " val) // set" << be_nl
      << "{" << be_idt_nl;
  // set the discriminant to the appropriate label
  if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
    {
      // valid label
      *os << "// set the discriminant val" << be_nl;
      // check if the case label is a symbol or a literal
      if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
        {
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      switch (node->pt ())
        {
        case AST_PredefinedType::PT_pseudo:
          *os << "// set the value" << be_nl;
          *os << "// store current val in a _var so as to free it on an assignment"
              << be_nl;
          *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
              << ub->local_name () << "_);" << be_nl;
          *os << "// release old storage and make a copy" << be_nl;
          *os << ub->local_name () << "_var = " << bt->name ()
              << "::_duplicate (val);" << be_nl;
          *os << "// the _var gives up ownership" << be_nl;
          *os << "this->" << ub->local_name () << "_ = "
              << ub->local_name () << "_var._retn ();" << be_uidt_nl;
          break;
        case AST_PredefinedType::PT_any:
          *os << "// set the value" << be_nl;
          *os << "// store current val in a _var so as to free it on an assignment"
              << be_nl;
          *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
              << ub->local_name () << "_);" << be_nl;
          *os << "// release old and make a deep copy" << be_nl;
          *os << ub->local_name () << "_var = new " << bt->name ()
              << " (val);" << be_nl;
          *os << "// the _var gives up ownership" << be_nl;
          *os << "this->" << ub->local_name () << "_ = "
              << ub->local_name () << "_var._retn ();" << be_uidt_nl;
          break;
        case AST_PredefinedType::PT_void:
          break;
        default:
          *os << "// set the value" << be_nl
              << "this->" << ub->local_name () << "_ = val;" << be_uidt_nl;
        }
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // get method
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE " << bt->name () << "_ptr" << be_nl;
      *os << bu->name () << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->" << ub->local_name () << "_;" << be_uidt_nl
          << "}\n\n";
      break;
    case AST_PredefinedType::PT_any:
      // get method with read-only access
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE const " << bt->name () << be_nl;
      *os << bu->name () << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
          << "}" << be_nl;

      // get method with read/write access
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE " << bt->name () << be_nl;
      *os << bu->name () << "::" << ub->local_name ()
          << " (void)" << be_nl
          << "{" << be_idt_nl
          << "return *this->" << ub->local_name () << "_;" << be_uidt_nl
          << "}\n\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // get method
      *os << "// retrieve the member" << be_nl
          << "ACE_INLINE " << bt->name () << be_nl;
      *os << bu->name () << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->" << ub->local_name () << "_;" << be_uidt_nl
          << "}\n\n";
    }
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";"
              << be_nl;
        }
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
          << ub->local_name () << "_);" << be_nl;
      *os << "// release old and make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = new " << bt->name ()
          << " (val);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
          << ub->local_name () << "_);" << be_nl;
      *os << "// release old and make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = CORBA::string_dup (val);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
   }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << bt->name () << "_var " << ub->local_name ()
          << "_var (val);" << be_nl
          << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl
          << bt->name () << "_var " << ub->local_name ()
          << "_var = val;" << be_nl
          << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
    }
  else
    {
      // default label
      // XXXASG - TODO
    }
  *os << "}" << be_nl;

  // get method
  *os << "ACE_INLINE const char *" << be_nl
      << bu->name () << "::" << ub->local_name ()
      << " (void) const // get method" << be_nl
      << "{" << be_idt_nl
      << "return this->" << ub->local_name () << "_;" << be_uidt_nl
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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";"
              << be_nl;
        }
      if (bt->size_type () == be_type::VARIABLE)
        {
          *os << "// set the value" << be_nl;
          *os << "// store current val in a _var so as to free it on an assignment"
              << be_nl;
          *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
              << ub->local_name () << "_);" << be_nl;
          *os << "// release old and make a deep copy" << be_nl;
          *os << ub->local_name () << "_var = new " << bt->name ()
              << " (val);" << be_nl;
          *os << "// the _var gives up ownership" << be_nl;
          *os << "this->" << ub->local_name () << "_ = "
              << ub->local_name () << "_var._retn ();" << be_uidt_nl;
        }
      else
        {
          *os << "this->" << ub->local_name () << "_ = val;" << be_uidt_nl;
        }
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
      << "{" << be_idt_nl;
  if (bt->size_type () == be_type::VARIABLE)
    *os << "return *this->" << ub->local_name () << "_;" << be_uidt_nl;
  else
    *os << "return this->" << ub->local_name () << "_;" << be_uidt_nl;
  *os << "}" << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << "ACE_INLINE " << bt->name () << " &" << be_nl
      << bu->name () << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl;
  if (bt->size_type () == be_type::VARIABLE)
    *os << "return *this->" << ub->local_name () << "_;" << be_uidt_nl;
  else
    *os << "return this->" << ub->local_name () << "_;" << be_uidt_nl;
  *os << "}\n\n";

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
          *os << "this->_reset (" << ub->label ()->label_val ()->n ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
              << ";" << be_nl;
        }
      else
        {
          *os << "this->_reset (" << ub->label ()->label_val ()
              << ", 0);" << be_nl;
          *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
            be_nl;
        }
      *os << "// set the value" << be_nl;
      *os << "// store current val in a _var so as to free it on an assignment"
          << be_nl;
      *os << bt->name () << "_var " << ub->local_name () << "_var (this->"
          << ub->local_name () << "_);" << be_nl;
      *os << "// release old and make a deep copy" << be_nl;
      *os << ub->local_name () << "_var = new " << bt->name ()
          << " (val);" << be_nl;
      *os << "// the _var gives up ownership" << be_nl;
      *os << "this->" << ub->local_name () << "_ = "
          << ub->local_name () << "_var._retn ();" << be_uidt_nl;
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
