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
//    Visitor generating AMH and AMH-RH skeleton code for Operation node in the
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
  // Nothing to be done for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << "\n// \t *** AMH operation declaration starts here ***\n";

  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Step 1 : Generate return type: always void
  os->indent ();
  *os << "virtual void ";
  // only for source: *os << intf->full_skel_name () << "::";


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
  *os << node->local_name();

  /*
  // STEP 3: Generate the argument list with the appropriate
  //         mapping. For these we grab a visitor that generates the
  //         parameter listing. We also generate the ResponseHandler
  //         argument 'on the fly' and add it to the argument list

  //ACE_CString rh_name;
  //ACE_OSTREAM temp;
  AST_Decl *parent = ScopeAsDecl (intf->defined_in ());
  if (parent)
  {
  *os << parent->name ();
  }
  *os << intf->local_name () ;
  //<< "ResponseHandler *response_handler, ";
  AST_Type *rh_field_type = new AST_Type;

  // Create the argument
  Identifier *id = new Identifier ("ResponseHandler");
  UTL_IdList *list = new UTL_IdList (id, 0);
  be_argument *rh_arg = new be_argument (AST_Argument::dir_IN,
  rh_field_type,
  list,
  0);
  node->add_argument_to_scope (rh_arg);
  */

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_SH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_amh_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  *os << be_nl;

  return 0;
}


// ******************************************************
// Visitor for generating AMH-RH skeleton for "operation"
// in skeleton header.
// ******************************************************

be_visitor_amh_rh_operation_sh::be_visitor_amh_rh_operation_sh (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_rh_operation_sh::~be_visitor_amh_rh_operation_sh (void)
{
}

int
be_visitor_amh_rh_operation_sh::visit_operation (be_operation *node)
{
  // Nothing to be done for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << "\n// \t *** AMH-RH operation declaration starts here ***\n";

  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Step 1 : Generate return type: always void
  os->indent ();
  *os << "virtual void ";
  // only for source: *os << intf->full_skel_name () << "::";


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
  *os << node->local_name();

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_SH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_amh_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  *os << be_nl;

  return 0;
}
