//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation, "$Id$")


// ************************************************************
// Generic Operation visitor
// ************************************************************

be_visitor_operation::be_visitor_operation (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation::~be_visitor_operation (void)
{
}


// Is the operation return type void?
int
be_visitor_operation::void_return_type (be_type *bt)
{
  if (bt->node_type () == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType::PredefinedType pdt =
        be_predefined_type::narrow_from_decl (bt)->pt ();

      if (pdt == AST_PredefinedType::PT_void)
        {
          return 1;
        }
    }

  return 0;
}

int
be_visitor_operation::has_param_type (be_operation *node,
                                      AST_Argument::Direction dir)
{
  return (node->count_arguments_with_direction (dir) != 0);
}

size_t
be_visitor_operation::count_non_out_parameters (be_operation *node)
{
  // @@ Once the valuetype issue discussed below is fixed we can
  //    replace this routine with:
  //
  // return node->count_arguments_with_direction (AST_Argument::dir_IN
  //                                              | AST_Argument::dir_INOUT);
  //
  size_t count = 0;

  // Initialize an iterator to iterate thru our scope.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      be_argument *bd =
        be_argument::narrow_from_decl (si.item ());

      // We do not generate insertion operators for valuetypes
      // yet. Do not include them in the count.
      be_valuetype *vt =
        be_valuetype::narrow_from_decl (bd->field_type ());

      if (bd && (bd->direction () != AST_Argument::dir_OUT) && !vt)
        {
          ++count;
        }
    }

  return count;
}


// Method to generate the throw specs for exceptions that are thrown by the
// operation.
int
be_visitor_operation::gen_throw_spec (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  const char *throw_spec_open = "throw (";
  const char *throw_spec_close = ")";

  if (!be_global->use_raw_throw ())
    {
      throw_spec_open = "ACE_THROW_SPEC ((";
      throw_spec_close = "))";
    }

  be_decl *scope = this->ctx_->scope ();
  be_interface *iface = be_interface::narrow_from_decl (scope);

  // Check if this is IF and it's not VT.
  if (iface != 0 && !iface->is_valuetype ())
    {
      *os << be_idt_nl << throw_spec_open;
      *os << be_idt_nl << "CORBA::SystemException";
    }

  if (node->exceptions ())
    {
      int i = 0;

      // Initialize an iterator to iterate thru the exception list.
      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done ();
           ei.next ())
        {
          be_exception *excp =
            be_exception::narrow_from_decl (ei.item ());

          if (excp == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation"
                                 "gen_throw_spec - "
                                 "bad exception node\n"),
                                -1);

            }

          if (i == 0 && iface != 0 && iface->is_valuetype ())
            {
              *os << be_idt_nl;
            }

          if (i != 0 || !(iface != 0 && iface->is_valuetype ()))
            {
              *os << be_nl << ", ";
            }

          *os << excp->name ();
          ++i;
        }
    }

  // Check if this is IF and it's not VT.
  if (iface != 0 && !iface->is_valuetype ())
    {
      *os << be_uidt_nl << throw_spec_close << be_uidt;
    }

  return 0;
}

int
be_visitor_operation::gen_environment_decl (int argument_emitted,
                                            be_operation *node)
{
  // Generate the CORBA::Environment parameter for the alternative mapping.
  if (be_global->exception_support ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Use ACE_ENV_SINGLE_ARG_DECL or ACE_ENV_ARG_DECL depending on
  // whether the operation node has parameters.
  const char *env_decl = "ACE_ENV_SINGLE_ARG_DECL";
  
  if (this->ctx_->sub_state () 
        == TAO_CodeGen::TAO_AMH_RESPONSE_HANDLER_OPERATION
      && node->argument_count () == 0)
    {
      // Response handler operations don't use the environment arg
      // unless there are other args in the operation.
      env_decl = "ACE_ENV_SINGLE_ARG_DECL_NOT_USED";
      this->ctx_->sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);
    }
  else if (argument_emitted || node->argument_count () > 0)
    {
      env_decl = "ACE_ENV_ARG_DECL";
    }

  *os << be_nl;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      // Last argument is always CORBA::Environment.
      *os << env_decl << "_WITH_DEFAULTS";
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH:
      // Last argument is always CORBA::Environment.
      *os << env_decl;
      break;
    default:
      *os << env_decl;
      break;
    }

  return 0;
}

// Method that returns the appropriate CORBA::Environment variable.
const char *
be_visitor_operation::gen_environment_var ()
{
  static const char *ace_try_env_decl = "ACE_DECLARE_NEW_CORBA_ENV;";
  static const char *null_env_decl = "";

  // Check if we are generating stubs/skeletons for
  // true C++ exception support.
  if (be_global->exception_support ())
    {
      return ace_try_env_decl;
    }
  else
    {
      return null_env_decl;
    }
}

int
be_visitor_operation::gen_raise_exception (be_type *return_type,
                                           const char *exception_name,
                                           const char *exception_arguments)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (be_global->use_raw_throw ())
    {
      *os << "throw "
          << exception_name << "(" << exception_arguments << ");\n";
      return 0;
    }

  int is_void =
    return_type == 0 || this->void_return_type (return_type);

  if (is_void)
    {
      *os << "ACE_THROW (";
    }
  else
    {
      *os << "ACE_THROW_RETURN (";
    }

  *os << exception_name << " (" << exception_arguments << ")";

  if (is_void)
    {
      *os << ");\n";
      return 0;
    }

  *os << ",";

  // Non-void return type.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);

  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_raise_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");\n";
  return 0;
}

int
be_visitor_operation::gen_check_exception (be_type *return_type)
{
  TAO_OutStream *os = this->ctx_->stream ();
  os->indent ();

  if (return_type == 0 || this->void_return_type (return_type))
    {
      *os << "ACE_CHECK;\n";
      return 0;
    }

  // Non-void return type....
  *os << "ACE_CHECK_RETURN (";
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);

  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_check_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");\n";
  return 0;
}

int
be_visitor_operation::gen_check_interceptor_exception (be_type *return_type)
{
  TAO_OutStream *os = this->ctx_->stream ();
  os->indent ();

  if (return_type == 0 || this->void_return_type (return_type))
    {
      *os << "TAO_INTERCEPTOR_CHECK;\n";
      return 0;
    }

  // Non-void return type.
  *os << "TAO_INTERCEPTOR_CHECK_RETURN (";
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);

  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_check_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");\n";
  return 0;
}
