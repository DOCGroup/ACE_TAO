//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    amh_sh.cpp
//
// = DESCRIPTION
//    Visitor generating AMH skeleton code for Operation node in the
//    skeleton header.
//
// = AUTHOR
//    Mayur Deshpande <mayur@ics.uci.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "ast_decl.h"
#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_amh_sh, "$Id$")


// ******************************************************
// Visitor for generating AMH skeleton for "operation" in skeleton header.
// ******************************************************

be_visitor_amh_operation_sh::be_visitor_amh_operation_sh (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_operation_sh::~be_visitor_amh_operation_sh (void)
{
}

int
be_visitor_amh_operation_sh::visit_operation (be_operation *node)
{
  // If there is an argument of type "native", return immediately.
  if (node->has_native ())
    return 0;

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl;

  *os << "static void ";
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_req," << be_nl
      << "void *_tao_obj," << be_nl
      << "void *_tao_servant_upcall" << be_nl
      << "TAO_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt << "\n\n";
  

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf =
    be_interface::narrow_from_scope (node->defined_in ());
  if (this->ctx_->attribute () != 0)
    intf = be_interface::narrow_from_scope (this->ctx_->attribute()->defined_in ());

  if (intf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_operation_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Step 1 : Generate return type: always void
  os->indent ();
  *os << "virtual void ";

  // Step 2: Generate the method name
  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "set_";
        }
      else
        {
          *os << "get_";
        }
    }

  // STEP 3: Generate the argument list with the appropriate
  //         mapping. For these we grab a visitor that generates the
  //         parameter listing. We also generate the ResponseHandler
  //         argument 'on the fly' and add it to the argument list
  *os << node->local_name() << " (" << be_idt << be_idt_nl;

  char *buf;
  // @@ TODO this must be kept consistent with the code in
  //    be_visitor_interface/amh_sh.cpp
  intf->compute_full_name ("", "ResponseHandler_ptr", buf);

  *os << buf << " _tao_rh";
  delete[] buf;

  if (node->argument_count () > 0)
    {
      *os << ",";
    }

  be_visitor_operation_arglist arglist_visitor (this->ctx_);
  if (arglist_visitor.visit_scope (node) == -1)
    return -1;
  if (arglist_visitor.gen_environment_decl (1, node) == -1)
    return -1;
  *os << be_uidt_nl << ")" << be_uidt;
  if (arglist_visitor.gen_throw_spec (node) == -1)
    return -1;
  *os << " = 0;\n" << be_nl;

  return 0;
}
