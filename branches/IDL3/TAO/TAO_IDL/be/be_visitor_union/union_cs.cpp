//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Unions in the client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union, 
           union_cs, 
           "$Id$")


// ******************************************************
// For client stubs.
// ******************************************************

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
  TAO_OutStream *os;
  be_type *bt = 0;

  if (!node->cli_stub_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      be_visitor_context ctx (*this->ctx_);
      // The discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement. We need to generate its
      // typecode.

      bt = be_type::narrow_from_decl (node->disc_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit_union - "
                             "bad discriminant type\n"), -1);
        }

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CS);
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

      *os << "// *************************************************************"
          << be_nl;
      *os << "// Operations for union " << node->name () << be_nl
          << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__ << be_nl;
      *os << "// *************************************************************\n\n";

      // Generate the copy constructor and the assignment operator here.
      os->indent ();
      *os << node->name () << "::" << node->local_name () << " (void)" << be_nl
          << "{" << be_idt_nl
          << "ACE_OS::memset (&this->disc_, 0, sizeof (this->disc_));" << be_nl
          << "ACE_OS::memset (&this->u_, 0, sizeof (this->u_));" << be_nl
          << "this->disc_ = ";

      // The default constructor must initialize the discriminator
      // to the first case label value found in the union declaration
      // so that, if the uninitialized union is inserted into an Any,
      // the Any destructor's call to deep_free() will work properly.
      UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);

      // Just get the union's first member.
      AST_Decl *d = si.item ();

      be_union_branch *ub = be_union_branch::narrow_from_decl (d);

      // Get the first label in its list.
      AST_UnionLabel *ul = ub->label (0);

      if (ul->label_kind () == AST_UnionLabel::UL_label)
        {
          ub->gen_label_value (os);
        }
      else
        {
          ub->gen_default_label_value (os, node);
        }

      *os << ";";

      *os << be_uidt_nl << "}" << be_nl << be_nl;

      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);

      // So we know we are generating the copy constructor.
      this->ctx_->sub_state (TAO_CodeGen::TAO_UNION_COPY_CONSTRUCTOR);

      *os << node->name () << "::" << node->local_name ()
          << " (const ::" << node->name () << " &u)"
          << be_nl;
      *os << "{" << be_idt_nl;
      *os << "this->disc_ = u.disc_;" << be_nl;
      *os << "switch (this->disc_)" << be_nl;
      *os << "{" << be_idt_nl;

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
      if (node->default_index () == -1)
        {
          *os << "default:" << be_nl
              << "break;";
        }

      *os << be_uidt_nl << "}" << be_uidt_nl
          << "}" << be_nl << be_nl;

      *os << "// destructor" << be_nl
          << node->name () << "::~" << node->local_name ()
          << " (void)" << be_nl
          << "{" << be_idt_nl
          << "// finalize" << be_nl
          << "this->_reset (this->disc_, 1);" << be_uidt_nl
          << "}" << be_nl << be_nl;

      if (!node->is_local ())
        {
          *os << "void "
              << node->name ()
              << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
              << "{" << be_idt_nl
              << node->local_name () << " *tmp = ACE_static_cast ("
              << node->local_name () << "*, _tao_void_pointer);" << be_nl
              << "delete tmp;" << be_uidt_nl
              << "}\n" << be_nl;
        }

      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);

      // Reset this for generating the assignment operator.
      this->ctx_->sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);

      // Assignment operator.
      os->indent ();
      *os << "// assignment operator" << be_nl;
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
      *os << "this->_reset (u.disc_, 0);" << be_nl;
      *os << "this->disc_ = u.disc_;" << be_nl << be_nl;
      // now switch based on the disc value
      *os << "switch (this->disc_)" << be_nl;
      *os << "{" << be_idt_nl;

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
      if (node->default_index () == -1)
        {
          *os << "default:" << be_nl
              << "break;" << be_uidt_nl;
        }
      else
        {
          *os << be_uidt_nl;
        }

      *os << "}" << be_nl << be_nl;
      *os << "return *this;" << be_uidt_nl;
      *os << "}\n\n";

      // The reset method.
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS);
      os->indent ();
      *os << "// reset method to reset old values of a union" << be_nl;
      *os << "void " << node->name () << "::_reset (" << bt->name ()
          << ", CORBA::Boolean /*finalize*/)" << be_nl;
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
      if (node->default_index () == -1)
        {
          os->decr_indent (0);
          *os << "default:" << be_nl;
          os->incr_indent ();
          *os << "break;";
        }

      *os << be_uidt_nl << "}" << be_uidt_nl
          << "}\n\n";

      if (!node->is_local () && be_global->tc_support ())
        {
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
          ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);

          be_visitor_typecode_defn tc_visitor (&ctx);

          if (tc_visitor.visit_union (node) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_cs::"
                                 "visit_union - "
                                 "TypeCode definition failed\n"), 
                                -1);
            }
        }

      node->cli_stub_gen (I_TRUE);
    }

  return 0;
}
