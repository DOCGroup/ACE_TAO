//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ch.cpp
//
// = DESCRIPTION
//    Visitor for Array code generation in client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"

ACE_RCSID(be_visitor_array, array_ch, "$Id$")


// ************************************************************************
//  visitor for array declaration in client header
// ************************************************************************

be_visitor_array_ch::be_visitor_array_ch (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_ch::~be_visitor_array_ch (void)
{
}

int be_visitor_array_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  be_decl *scope = this->ctx_->scope (); // scope in which it is used

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_hdr_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the ifdefined macro
  os->gen_ifdef_macro (node->flatname ());

  os->indent ();
  if (this->ctx_->tdef ())
    {
      // this is a typedef to an array node
      *os << "typedef ";
    }
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }
  *os << " " << node->local_name ();
  if (node->gen_dimensions (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "gen dimensions failed\n"),
                        -1);
    }
  *os << ";" << be_nl;
  *os << "typedef ";
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }
  *os << " " << node->local_name () << "_slice";
  if (node->gen_dimensions (os, 1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "gen slice dimensions failed\n"),
                        -1);
    }
  *os << ";\n";

  // typedef the _var, _out, and _forany types
  if (node->gen_var_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "var_defn failed\n"),
                        -1);
    }
  // a class is generated for an out defn only for a variable length struct
  if (node->size_type () == be_decl::VARIABLE)
    {
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ch::"
                             "visit_argument - "
                             "out_defn failed\n"),
                            -1);
        }
    }
  else
    {
      // fixed size
      os->indent ();
      // if we are a typedefed array, we can use the TYPE name to define an
      // _out type. However, for anonymous arrays that do not give rise to a
      // new type, we use the base type for defining an out type
      if (this->ctx_->tdef ())
        {
          *os << "typedef " << node->local_name () << " "
              << node->local_name () << "_out;\n";
        }
      else
        {
          *os << "typedef ";
          if (bt->accept (this) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_array_ch::"
                                 "visit_array - "
                                 "base type decl failed\n"),
                                -1);
            }
          *os << " " << node->local_name () << "_out";
          if (node->gen_dimensions (os) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_array_ch::"
                                 "visit_array - "
                                 "gen dimensions failed\n"),
                                -1);
            }
          *os << ";" << be_nl;
        }
    }

  if (node->gen_forany_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "forany_defn failed\n"),
                        -1);
    }
  // the _alloc, _dup, copy, and free methods. If the node is nested, the
  // methods become static
  os->indent ();
  *os << "static " << node->nested_type_name (scope, "_slice") << " *";
  *os << node->nested_type_name (scope, "_alloc") << " (void);" << be_nl;
  *os << "static " << node->nested_type_name (scope, "_slice") << " *";
  *os << node->nested_type_name (scope, "_dup") << " (const ";
  *os << node->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;
  *os << "static void " << node->nested_type_name (scope, "_copy") << " (";
  *os << node->nested_type_name (scope, "_slice") << " *_tao_to, const ";
  *os << node->nested_type_name (scope, "_slice") << " *_tao_from);" << be_nl;
  *os << "static void " << node->nested_type_name (scope, "_free") << " (";
  *os << node->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;

  // is this a typedefined array? if so, then let the typedef deal with
  // generation of the typecode
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // generate the endif macro
  os->gen_endif ();

  node->cli_hdr_gen (1);
  return 0;
}
