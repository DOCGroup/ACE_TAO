//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Module in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_module.h"

ACE_RCSID(be_visitor_module, module_ch, "$Id$")


// ******************************************************
// Module visitor for client header
// ******************************************************

be_visitor_module_ch::be_visitor_module_ch (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_ch::~be_visitor_module_ch (void)
{
}

int
be_visitor_module_ch::visit_module (be_module *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      // XXXASG - Modules really map to namespace. We need to see if our target
      // compiler supports namespaces or not. This visitor generates a class for a
      // module. We can have the factory generate another module visitor that can
      // generate namespaces

      os->indent (); // start from whatever indentation level we were at
      // now generate the class definition
      *os << "TAO_NAMESPACE " // << idl_global->export_macro ()
	  << " " << node->local_name () << be_nl
          << "{\n";
      os->incr_indent (0);

      // generate code for the module definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module_ch::"
                             "visit_module - "
                             "codegen for scope failed\n"), -1);
        }

      *os << be_uidt_nl
          << "}; // module " << node->name () << "\n\n";

    }
  return 0;
}
