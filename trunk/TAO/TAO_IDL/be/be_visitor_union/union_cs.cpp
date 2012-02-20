
//=============================================================================
/**
 *  @file    union_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Unions in the client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_cs::be_visitor_union_cs (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_cs::~be_visitor_union_cs (void)
{
}

// Visit the Union_cs node and its scope.
int be_visitor_union_cs::visit_union (be_union *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_visitor_context ctx (*this->ctx_);
  // The discriminant type may have to be defined here if it was an enum
  // declaration inside of the union statement. We need to generate its
  // typecode.

  be_type *bt = be_type::narrow_from_decl (node->disc_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cs::"
                         "visit_union - "
                         "bad discriminant type\n"), -1);
    }

  be_visitor_union_discriminant_cs disc_visitor (&ctx);

  if (bt->accept (&disc_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cs::"
                         "visit union - "
                         "codegen for discrminant failed\n"),
                        -1);
    }

  // First generate code for any of the members (if required, e.g.,
  // anonymous sequences, structs, unions, arrays).
  this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CS);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cs"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Now generate the operations on the union such as the copy constructor
  // and the assignment operator.

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the copy constructor and the assignment operator here.
  *os << be_nl_2
      << node->name () << "::" << node->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "ACE_OS::memset (&this->u_, 0, sizeof (this->u_));" << be_nl;

  // The default constructor must initialize the discriminator
  // to the first case label value found in the union declaration
  // so that, if the uninitialized union is inserted into an Any,
  // the Any destructor's call to deep_free() will work properly.

  *os << "this->disc_ = ";

  UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
  be_union_branch *ub = 0;

  // In case we have some bogus enum values from an enum declared
  // in our scope.
  while (ub == 0)
    {
      // Just get the union's first member.
      AST_Decl *d = si.item ();

      ub = be_union_branch::narrow_from_decl (d);
      si.next ();
    }

  // Get the first label in its list.
  AST_UnionLabel *ul = ub->label (0);

  AST_Union::DefaultValue dv;
  // This can indicate an error in the return value, but it is
  // caught elsewhere.
  (void) node->default_value (dv);

  bool test = dv.computed_ == 0
              && ul->label_kind () == AST_UnionLabel::UL_label;

  if (test)
    {
      ub->gen_label_value (os);
    }
  else
    {
      ub->gen_default_label_value (os, node);
    }

  *os << ";";

  if (dv.computed_ == 0)
    {
      *os << be_nl;
      be_visitor_union_branch_public_constructor_cs const_visitor (this->ctx_);
      if (ub->accept (&const_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit union - "
                             "codegen for constructor failed\n"),
                            -1);
        }
    }

  *os << be_uidt_nl << "}" << be_nl_2;

  this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);

  // So we know we are generating the copy constructor.
  this->ctx_->sub_state (TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR);

  *os << node->name () << "::" << node->local_name ()
      << " (const ::" << node->name () << " &u)"
      << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->disc_ = u.disc_;" << be_nl;
  *os << "switch (this->disc_)" << be_nl;
  *os << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cs"
                         "visit_union - "
                         "codegen for copy ctor failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->gen_empty_default_label ())
    {
      *os << be_nl
          << "default:" << be_nl
          << "break;";
    }

  *os << be_uidt_nl << "}" << be_uidt_nl
      << "}" << be_nl_2;

  *os << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl
      << "{" << be_idt_nl
      << "// Finalize." << be_nl
      << "this->_reset ();" << be_uidt_nl
      << "}" << be_nl_2;

  if (be_global->any_support ())
    {
      *os << "void "
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp =" << be_idt_nl
          << "static_cast<"
          << node->local_name () << " *> (_tao_void_pointer);" << be_uidt_nl
          << "delete tmp;" << be_uidt_nl
          << "}" << be_nl_2;
    }

  this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);

  // Reset this for generating the assignment operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);

  // Assignment operator.
  *os << node->name () << " &" << be_nl;
  *os << node->name () << "::operator= (const ::"
      << node->name () << " &u)" << be_nl;
  *os << "{" << be_idt_nl;
  // First check for self-assignment.
  *os << "if (&u == this)" << be_idt_nl
      << "{" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;
  // Reset and set the discriminant.
  *os << "this->_reset ();" << be_nl;
  *os << "this->disc_ = u.disc_;" << be_nl_2;
  // now switch based on the disc value
  *os << "switch (this->disc_)" << be_nl;
  *os << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cs"
                         "visit_union - "
                         "codegen for assign op failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->gen_empty_default_label ())
    {
      *os << be_nl
          << "default:" << be_nl
          << "break;";
    }

  *os << be_uidt_nl
      << "}" << be_nl_2;
  *os << "return *this;" << be_uidt_nl;
  *os << "}" << be_nl_2;

  // The reset method.
  this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS);

  *os << "/// Reset method to reset old values of a union." << be_nl;
  *os << "void " << node->name () << "::_reset (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "switch (this->disc_)" << be_nl;
  *os << "{" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cs"
                         "visit_union - "
                         "codegen for reset failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->gen_empty_default_label ())
    {
      *os << be_nl
          << "default:" << be_nl
          << "break;";
    }

  *os << be_uidt_nl << "}" << be_uidt_nl
      << "}";

  if (be_global->tc_support ())
    {
      ctx = *this->ctx_;
      // ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      TAO::be_visitor_union_typecode tc_visitor (&ctx);

      if (tc_visitor.visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit_union - "
                             "TypeCode definition failed\n"),
                            -1);
        }
    }

  node->cli_stub_gen (true);
  return 0;
}
