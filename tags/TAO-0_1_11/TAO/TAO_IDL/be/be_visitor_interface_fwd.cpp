//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface_fwd.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Interface_Fwd
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client header file
// ********************************************************************

be_visitor_interface_fwd_ch::be_visitor_interface_fwd_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ch::~be_visitor_interface_fwd_ch (void)
{
}

// visit the Interface_Fwd_Ch node and its scope
int
be_visitor_interface_fwd_ch::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_hdr_gen ())
    {
      os->indent (); // start from the current

      // all we do in this is generate a forward declaration of the class
      *os << "class " << node->local_name () << ";" << be_nl;

      // generate the ifdefined macro for the _ptr type
      os->gen_ifdef_macro (node->flatname (), "_ptr");

      // generate the _ptr declaration
      *os << "typedef " << node->local_name () << " *" << node->local_name ()
          << "_ptr;\n";

      os->gen_endif ();

      // enclose under an ifdef macro
      os->gen_ifdef_macro (node->flatname (), "_var");

      // generate the _var declaration
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_fwd_ch::"
                             "visit_interface_fwd - "
                             "codegen for _var failed\n"), -1);
        }
      // gen an endif
      os->gen_endif ();

      // enclose under an ifdef macro
      os->gen_ifdef_macro (node->flatname (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_fwd_ch::"
                             "visit_interface_fwd - "
                             "codegen for _out failed\n"), -1);
        }
      // generate the endif macro
      os->gen_endif ();
      node->cli_hdr_gen (I_TRUE);
    }
  return 0;
}

// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client inline file
// ********************************************************************

be_visitor_interface_fwd_ci::be_visitor_interface_fwd_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ci::~be_visitor_interface_fwd_ci (void)
{
}

// visit the Interface_Fwd_ci node and its scope
int
be_visitor_interface_fwd_ci::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_inline_gen ())
    {

      // generate the ifdefined macro for  the _var type
      os->gen_ifdef_macro (node->flatname (), "_var");

      if (node->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_fwd_ci::"
                             "visit_interface_fwd - "
                             "codegen for _var failed\n"), -1);
        }

      os->gen_endif ();

      // generate the ifdefined macro for  the _out type
      os->gen_ifdef_macro (node->flatname (), "_out");

      if (node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_fwd_ci::"
                             "visit_interface_fwd - "
                             "codegen for _out failed\n"), -1);
        }
      os->gen_endif ();

      node->cli_stub_gen (I_TRUE);
    }
  return 0;
}
