//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, interface_fwd_ci, "$Id$")


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

  if (!node->cli_inline_gen () && !node->imported ())
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
