//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, union_ci, "$Id$")


// ******************************************************
// for client inline
// ******************************************************

be_visitor_union_ci::be_visitor_union_ci (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ci::~be_visitor_union_ci (void)
{
}

// visit the Union node and its scope
int be_visitor_union_ci::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same


  if (!node->cli_inline_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      *os << "// *************************************************************"
          << be_nl;
      *os << "// Inline operations for union " << node->name () << be_nl;
      *os << "// *************************************************************\n\n";

      // generate the default constructor and the destructor here
      os->indent ();
      *os << "// destructor" << be_nl
          << "ACE_INLINE" << be_nl
          << node->name () << "::~" << node->local_name () << " (void)" << be_nl
          << "{" << be_idt_nl
          << "// finalize" << be_nl
          << "this->_reset (this->disc_, 1);" << be_uidt_nl
          << "}" << be_nl << be_nl;

      // the virtual overloaded _reset method
      *os << "// this reset method is used by the decoding engine" << be_nl;
      *os << "ACE_INLINE void" << be_nl
          << node->name () << "::_reset (void)" << be_nl
          << "{" << be_idt_nl
          << "this->_reset (this->disc_, 1);" << be_nl
          << "ACE_OS::memcpy (&this->u_, 0, sizeof (this->u_));" << be_uidt_nl
          << "}" << be_nl << be_nl;

      // the virtual overloaded _discriminant method
      *os << "// returns pointer to the discriminant" << be_nl;
      *os << "ACE_INLINE void *" << be_nl
          << node->name () << "::_discriminant (void)" << be_nl
          << "{" << be_idt_nl
          << "return &this->disc_;" << be_uidt_nl
          << "}\n\n";

      // now check if we need to generate the _default () method
      be_union::DefaultValue dv;
      if (node->default_value (dv) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "computing default value failed\n"),
                            -1);
        }
      if ((dv.computed_ != 0) && (node->default_index () == -1))
        {
          // only if all cases are not covered AND there is no explicit
          // default, we get the _default () method
          os->indent ();
          *os << "// the implicit _default () method" << be_nl;
          *os << "ACE_INLINE void " << be_nl
              << node->name () << "::_default ()" << be_nl
              << "{" << be_idt_nl
              << "this->disc_ = ";
          switch (node->udisc_type ())
            {
            case AST_Expression::EV_short:
              *os << dv.u.short_val;
              break;
            case AST_Expression::EV_ushort:
              *os << dv.u.ushort_val;
              break;
            case AST_Expression::EV_long:
              *os << dv.u.long_val;
              break;
            case AST_Expression::EV_ulong:
              *os << dv.u.ulong_val;
              break;
            case AST_Expression::EV_char:
              os->print ("%d", dv.u.char_val);
              break;
            case AST_Expression::EV_bool:
              *os << dv.u.bool_val;
              break;
            case AST_Expression::EV_any:
              *os << dv.u.enum_val;
              break;
            case AST_Expression::EV_longlong:
            case AST_Expression::EV_ulonglong:
              // unimplemented
            default:
              // error caught earlier.
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_ci::"
                                 "visit_union - "
                                 "bad or unimplemented discriminant type\n"),
                            -1);
            }
          *os << be_uidt_nl << "}\n\n";
        }

      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      bt = be_type::narrow_from_decl (node->disc_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "bad discriminant type\n"), -1);
        }

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CI); // set current code
                                                          // gen state
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "bad visitor\n"), -1);
        }
      // generate code for the discriminant
      if (bt->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit union - "
                             "codegen for discrminant failed\n"), -1);
        }

      // now generate the implementation of the access methods for the
      // union. For this set our state
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CI);
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for scope failed\n"), -1);
        }

      // generate the ifdefined macro for the array type
      os->gen_ifdef_macro (node->flatname (), "_var");
      if (node->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for _var failed\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdefined macro for the array type then generate the _out
      // impl
      os->gen_ifdef_macro (node->flatname (), "_out");
      if (node->size_type () == be_decl::VARIABLE
          && node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for _out failed\n"), -1);
        }
      os->gen_endif ();

      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}
