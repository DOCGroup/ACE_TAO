//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Module in the server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_module.h"

ACE_RCSID(be_visitor_module, module_sh, "$Id$")


// ************************************************************
// Module visitor for server header
// ************************************************************

be_visitor_module_sh::be_visitor_module_sh (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_sh::~be_visitor_module_sh (void)
{
}

int
be_visitor_module_sh::visit_module (be_module *node)
{
  TAO_OutStream *os; // output stream

  if (!node->srv_hdr_gen () && !node->imported ()) // not generated and not imported
    {
      os = this->ctx_->stream ();

      // generate the skeleton class name

      os->indent (); // start with whatever indentation level we are at

      // now generate the class definition. The prefix POA_ is prepended to our
      // name only if we are the outermost module
      *os << "TAO_NAMESPACE "; // << idl_global->export_macro ()

      if (!node->is_nested ())
          // we are outermost module
          *os << " POA_" << node->local_name () << be_nl;
      else
          // we are inside another module
          *os << " " << node->local_name () << be_nl;

      *os << "{" << be_nl
          << be_idt;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module_sh::"
                             "visit_module - "
                             "codegen for scope failed\n"), -1);
        }

      os->decr_indent ();
      *os << "};\n\n";
    }
  return 0;

}
