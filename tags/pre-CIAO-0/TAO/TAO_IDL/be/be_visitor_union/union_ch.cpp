//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Unions in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_union.h"
#include "be_visitor_typecode/typecode_decl.h"

ACE_RCSID(be_visitor_union, union_ch, "$Id$")


// ******************************************************
// for client header
// ******************************************************

be_visitor_union_ch::be_visitor_union_ch (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ch::~be_visitor_union_ch (void)
{
}

// Visit the Union node and its scope.
int be_visitor_union_ch::visit_union (be_union *node)
{
  TAO_OutStream *os;
  be_type *bt;

  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      // Generate the ifdefined macro for the union type.
      os->gen_ifdef_macro (node->flat_name ());

      *os << "class " << node->local_name () << ";" << be_nl;
      *os << "class " << node->local_name () << "_var;" << be_nl << be_nl;

      *os << "class " << be_global->stub_export_macro () << " "
          << node->local_name () << be_nl
          << "{" << be_nl
          << "public:" << be_idt_nl

      // Generate default and copy constructors.
          << node->local_name () << " (void);" << be_nl
          << node->local_name () << " (const " << node->local_name ()
          << " &);" << be_nl
      // Generate destructor.
          << "~" << node->local_name () << " (void);" << be_nl;

      *os << "static void _tao_any_destructor (void*);"
          << be_nl << be_nl;

      // Generate assignment operator.
      *os << node->local_name () << " &operator= (const "
          << node->local_name () << " &);" << be_nl << be_nl;

      // Retrieve the disriminant type.
      bt = be_type::narrow_from_decl (node->disc_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "bad disciminant type\n"), 
                            -1);
        }

      // The discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CH);
      be_visitor_union_discriminant_ch visitor (&ctx);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             " visit_union - "
                             "codegen for discriminant failed\n"), -1);
        }

      // Generate the _var_type typedef
      // but we must protect against certain versions of g++.
      *os << "\n#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_nl;
      *os << "typedef " << node->local_name () << "_var _var_type;\n"
          << "#endif /* ! __GNUC__ || g++ >= 2.8 */" << be_nl << be_nl;

      // Now generate the public defn for the union branch members. For this,
      // set our state to reflect what we are aiming to do.
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CH);

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "codegen for public defn of union members\n"),
                            -1);
        }

      // Now check if we need to generate the _default () method.
      be_union::DefaultValue dv;

      if (node->default_value (dv) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "computing default value failed\n"),
                            -1);
        }

      if ((dv.computed_ != 0) && (node->default_index () == -1))
        {
          // Only if all cases are not covered AND there is no explicit
          // default, we get the _default () method.
          *os << "void _default (void);";
        }

      *os << be_uidt_nl;

      // Now generate the private data members of the union.
      *os << "private:" << be_idt_nl;
      *os << bt->nested_type_name (node) << " disc_;" << be_nl;
      *os << bt->nested_type_name (node) << " holder_;" << be_nl << be_nl;
      // Emit the ACE_NESTED_CLASS macro.

      // The members are inside of a union.
      *os << "union" << be_nl;
      *os << "{" << be_idt_nl;
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PRIVATE_CH);

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "codegen for private members of union\n"), -1);
        }

      *os << be_uidt_nl;
      *os << "} u_;" << be_nl << be_nl;

      // The reset method (TAO extension).
      *os << "// TAO extension." << be_nl;
      *os << "void _reset (" << bt->nested_type_name (node)
          << ", CORBA::Boolean /* finalize */);" << be_nl;
      *os << "// Frees any allocated storage." << be_uidt_nl;
      *os << "}; //" << node->name () << be_nl << be_nl;

      if (be_global->tc_support ())
        {
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
          be_visitor_typecode_decl tc_visitor (&ctx);

          if (tc_visitor.visit_union (node) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_ch::"
                                 "visit_union - "
                                 "TypeCode declaration failed\n"), 
                                -1);
            }
        }

      os->gen_endif ();

      // Generate the ifdefined macro for the _var type.
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // Generate var definition.
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "codegen for _var\n"), 
                            -1);
        }

      os->gen_endif ();

      // Generate the ifdefined macro for the array type.
      os->gen_ifdef_macro (node->flat_name (), "_out");

      // A class is generated for an out defn only for a variable
      // length struct.
      if (node->size_type () == be_decl::VARIABLE)
        {
          if (node->gen_out_defn () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_ch::"
                                 "visit_union - "
                                 "codegen for _out\n"), 
                                -1);
            }
        }
      else
        {
          *os << "typedef " << node->local_name () << " &"
              << node->local_name () << "_out;" << be_nl << be_nl;
        }

      os->gen_endif ();
      node->cli_hdr_gen (I_TRUE);
    }

  return 0;
}
