//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Arrays in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"

// ************************************************************************
//  visitor for array declaration in client inline
// ************************************************************************

be_visitor_array_ci::be_visitor_array_ci (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_ci::~be_visitor_array_ci (void)
{
}

int be_visitor_array_ci::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  be_decl *scope = this->ctx_->scope (); // scope in which it is used

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_inline_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // alloc method
  os->indent (); // start from current indentation
  *os << "ACE_INLINE " << node->name () << "_slice *" << be_nl;
  *os << node->name () << "_alloc (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return new ";
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }

  if (node->gen_dimensions (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "gen_client_inline - "
                         "dimensions codegen failed\n"),
                        -1);
    }

  *os << ";" << be_uidt_nl;
  *os << "}\n\n";

  // free method
  os->indent ();
  *os << "ACE_INLINE void" << be_nl
      << node->name () << "_free (" << node->name ()
      << "_slice *_tao_slice)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "delete [] _tao_slice;" << be_uidt_nl;
  *os << "}\n\n";

  // generate code for the _var, _out, and _forany types
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "var_defn failed\n"),
                        -1);
    }
  if (node->size_type () == be_decl::VARIABLE)
    {
      if (node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ci::"
                             "visit_array - "
                             "out_defn failed\n"),
                            -1);
        }
    }

  if (node->gen_forany_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "forany_defn failed\n"),
                        -1);
    }

  // is this a typedefined array? if so, then let the typedef deal with
  // generation of the typecode
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "TypeCode definition failed\n"
                             ), -1);
        }
      delete visitor;
    }

  node->cli_inline_gen (1);
  return 0;
}
