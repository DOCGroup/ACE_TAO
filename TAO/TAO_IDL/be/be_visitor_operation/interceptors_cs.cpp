//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_interceptors_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the client stubs.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_interceptors_cs, "$Id$")


// ******************************************************
// primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_interceptors_cs::be_visitor_operation_interceptors_cs (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_cs::~be_visitor_operation_interceptors_cs (
    void
  )
{
}

int
be_visitor_operation_interceptors_cs::visit_operation (be_operation *node)
{

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);
  be_visitor  *visitor = tao_cg->make_visitor (&ctx);

  // Save the node.
  this->ctx_->node (node);

  // Start with the current indentation level.
  os->indent ();

  // Generate the ClientRequestInfo object definition per operation
  // to be used by the interecptors.
  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << parent->full_name () << "::";
    }

  *os << "TAO_ClientRequestInfo_"<< node->flat_name ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << "::"
      << "TAO_ClientRequestInfo_"<< node->flat_name ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << " (" << be_idt << be_idt_nl
      << "TAO_GIOP_Invocation *_tao_invocation," << be_nl
      << "CORBA::Object_ptr _tao_target";

  // Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }

  delete visitor;
  os->indent ();
  *os << be_uidt_nl << ")" << be_uidt_nl;

  // Generate the member list and set each member but before that,
  // its necessary to pass on some args to the base class.
  os->indent ();
  *os << "  : TAO_ClientRequestInfo (_tao_invocation, _tao_target)";

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }

  delete visitor;

  os->decr_indent ();
  *os << be_nl << "{}\n\n";

  // -----------------------------------------------------------------
  // PortableInterceptor::ClientRequestInfo::arguments()
  // -----------------------------------------------------------------
  *os << "Dynamic::ParameterList *" << be_nl;

  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << parent->full_name () << "::";
    }

  // The interceptors cant modify "in" and "out" parameters.
  *os << "TAO_ClientRequestInfo_" << node->flat_name ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << "::arguments (CORBA::Environment &ACE_TRY_ENV)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))"<<  be_uidt_nl
      << "{" << be_idt_nl
      << "// Generate the argument list on demand." << be_nl
      << "Dynamic::ParameterList *parameter_list ="  << be_idt_nl
      << "TAO_RequestInfo_Util::make_parameter_list (ACE_TRY_ENV);"
      << be_uidt_nl
      << "ACE_CHECK_RETURN (0);" << be_nl
      << be_nl;

  if (node->argument_count () == 0 ||
      // Now make sure that we have some in and inout
      // parameters. Otherwise, there is nothing to be put into
      // the Dyanmic::Paramlist.
      (!(this->has_param_type (node, AST_Argument::dir_IN)) &&
       !(this->has_param_type (node, AST_Argument::dir_INOUT))))
    {
      *os << "return parameter_list;" << be_uidt_nl;
    }
  else
    {
      *os << "Dynamic::ParameterList_var safe_parameter_list = "
          << "parameter_list;" << be_nl;

      // The insertion operator is different for different nodes.
      // We change our scope to go to the argument scope to
      // be able to decide this.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for argument pre invoke failed\n"),
                            -1);
        }

      delete visitor;

      *os << be_nl
          << "return safe_parameter_list._retn ();" << be_uidt_nl;
    }

  *os << "}\n\n";

  os->decr_indent ();

  // -----------------------------------------------------------------
  // PortableInterceptor::ClientRequestInfo::exceptions()
  // -----------------------------------------------------------------
  *os << "Dynamic::ExceptionList *" << be_nl;

  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << parent->full_name () << "::";
    }

  *os << "TAO_ClientRequestInfo_" << node->flat_name ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << "::exceptions (CORBA::Environment &ACE_TRY_ENV)"<< be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "// Generate the exception list on demand." << be_nl
      << "Dynamic::ExceptionList *exception_list ="  << be_idt_nl
      << "TAO_RequestInfo_Util::make_exception_list (ACE_TRY_ENV);"
      << be_uidt_nl
      << "ACE_CHECK_RETURN (0);" << be_nl
      << be_nl;

  if (!node->exceptions ())
    {
      *os << "return exception_list;" << be_uidt_nl;
    }
  else
    {
      *os << "Dynamic::ExceptionList_var safe_exception_list = "
          << "exception_list;" << be_nl;

      // We change our scope to be able to generate the exceptionlist.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for exceptlist failed\n"),
                            -1);
        }

      delete visitor;

      *os << be_nl
          << "return safe_exception_list._retn ();" << be_uidt_nl;
    }

  *os << "}\n\n" << be_nl;

  os->decr_indent ();

  // -----------------------------------------------------------------
  // PortableInterceptor::ClientRequestInfo::result()
  // -----------------------------------------------------------------
  *os << "CORBA::Any * " << be_nl;

  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << parent->full_name () << "::";
    }

  *os << "TAO_ClientRequestInfo_" << node->flat_name ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << "::result (CORBA::Environment &ACE_TRY_ENV)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "// Generate the result on demand." << be_nl;

  bt = be_type::narrow_from_decl (node->return_type ());

  if (this->void_return_type (bt))
    {
      // Return an Any with tk_void TypeCode.
      *os << "CORBA::Boolean tk_void_any = 1;" << be_nl
          << "CORBA::Any *result_any ="  << be_idt_nl
          << "TAO_RequestInfo_Util::make_any (tk_void_any, ACE_TRY_ENV);"
          << be_uidt_nl
          << "ACE_CHECK_RETURN (0);" << be_nl
          << be_nl
          << "return result_any;" << be_uidt_nl;
    }
  else
    {
      *os << "CORBA::Boolean tk_void_any = 0;" << be_nl
          << "CORBA::Any *result_any ="  << be_idt_nl
          << "TAO_RequestInfo_Util::make_any (tk_void_any, ACE_TRY_ENV);"
          << be_uidt_nl
          << "ACE_CHECK_RETURN (0);" << be_nl
          << be_nl
          << "CORBA::Any_var safe_result_any = "
          << "result_any;" << be_nl << be_nl;

      // Generate the insertion of result into Any.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_RESULT);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for result failed\n"),
                            -1);
        }

      delete visitor;

      *os << "return safe_result_any._retn ();" << be_uidt_nl;
    }

  *os << "}\n\n";

  os->decr_indent ();

  // -----------------------------------------------------------------
  // Update the result.
  // -----------------------------------------------------------------
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
    }

  // Grab the right visitor to generate the return type accessor if
  // its not void since we cant have a private member to be of void
  // type.
  if (!this->void_return_type (bt))
    {
      *os << "void " << be_nl;

      if (node->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (node->defined_in ())->decl ();

          *os << parent->full_name () << "::";
        }

      *os << "TAO_ClientRequestInfo_"<< node->flat_name ();

      // We need the interface node in which this operation was defined. However,
      // if this operation node was an attribute node in disguise, we get this
      // information from the context and add a "_get"/"_set" to the flat
      // name to get around the problem of overloaded methods which are
      // generated for attributes.
      if (this->ctx_->attribute ())
        {
          bt = be_type::narrow_from_decl (node->return_type ());

          if (!bt)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interceptors_ch::"
                                 "visit_operation - "
                                 "Bad return type\n"),
                                -1);
            }

          // Grab the right visitor to generate the return type if its not
          // void it means it is not the accessor.
          if (!this->void_return_type (bt))
            {
              *os << "_get";
            }
          else
            {
              *os << "_set";
            }
        }

      *os << "::"
          << "result (";

      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_CH);
      ctx.sub_state (TAO_CodeGen::TAO_INTERCEPTORS_INFO_STUB);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

      os->indent ();
      *os << " result)" << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "// update the result " << be_nl
          << "this->_result = result;" << be_uidt_nl
          << "}\n\n";
    }

  return 0;
}
