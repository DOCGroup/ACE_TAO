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

ACE_RCSID (be_visitor_operation,
           operation,
           "$Id$")

// ************************************************************
// Generic Operation visitor
// ************************************************************

be_visitor_operation::be_visitor_operation (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    operation_name_ (0)
{
}

be_visitor_operation::~be_visitor_operation (void)
{
  delete [] operation_name_;
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

int
be_visitor_operation::is_amh_exception_holder (be_interface *node)
{
  int is_an_amh_exception_holder = 0;
  const char *amh_underbar = "AMH_";
  const char *node_name = node->local_name ();

  if( amh_underbar[0] == node_name[0] &&
      amh_underbar[1] == node_name[1] &&
      amh_underbar[2] == node_name[2] &&
      amh_underbar[3] == node_name[3]
      ) // node name starts with "AMH_"
    {
      //ACE_DEBUG ((LM_DEBUG, "Passed first test of amh_excepholder \n"));
      const char *last_E = ACE_OS::strrchr (node->full_name (), 'E');
      if (last_E != 0
          && ACE_OS::strcmp (last_E, "ExceptionHolder") == 0)
        {
          //ACE_DEBUG ((LM_DEBUG, "be_visitor_operation: Passed second test of amh_excepholder \n"));
          is_an_amh_exception_holder = 1;
        }
    }

  return is_an_amh_exception_holder;
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

  UTL_Scope *scope = node->defined_in ();
  be_interface *iface = be_interface::narrow_from_scope (scope);

  /***************************************************************************/
  // 2.6
  // Generate the Right Throw Spec if it is an AMH ExceptionHolder
  /***************************************************************************/
  // Check if this is (IF and it's not VT) or (it is an AMH ExceptionHolder).
  if (iface != 0)
    {
      int is_amh_exception_holder = this->is_amh_exception_holder (iface);
      AST_Decl::NodeType nt = iface->node_type ();

      if (nt != AST_Decl::NT_valuetype || is_amh_exception_holder)
        {
          *os << be_nl << throw_spec_open;
          *os << be_idt_nl << "CORBA::SystemException";

          if (node->exceptions ())
            {
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

                  *os << be_nl << ", ";
                  *os << excp->name ();
                }
            }

          *os << be_uidt_nl << throw_spec_close << be_uidt;
        }
    }
  /*******************************************************************************/
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
      env_decl = "ACE_ENV_SINGLE_ARG_DECL";
      this->ctx_->sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);
    }
  else if (argument_emitted || node->argument_count () > 0)
    {
      env_decl = "ACE_ENV_ARG_DECL";
    }

  TAO_CodeGen::CG_STATE cgs = this->ctx_->state ();

  if (node->argument_count () > 0
      || cgs == TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH
      || cgs == TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS)
    {
      *os << be_nl;
    }

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      // Last argument is always CORBA::Environment.
      *os << env_decl << "_WITH_DEFAULTS";
      break;
    default:
      *os << env_decl;
      break;
    }

  return 0;
}

// Method that returns the appropriate CORBA::Environment variable.
const char *
be_visitor_operation::gen_environment_var (void)
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
      *os << ");";

      return 0;
    }

  *os << ",";

  // Non-void return type.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_raise_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");";

  return 0;
}

int
be_visitor_operation::gen_check_exception (be_type *return_type)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (return_type == 0 || this->void_return_type (return_type))
    {
      *os << "ACE_CHECK;" << be_nl;
      return 0;
    }

  // Non-void return type....
  *os << "ACE_CHECK_RETURN (";
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_check_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");" << be_nl;
  return 0;
}

int
be_visitor_operation::gen_check_interceptor_exception (be_type *return_type)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (return_type == 0 || this->void_return_type (return_type))
    {
      *os << "TAO_INTERCEPTOR_CHECK;\n";
      return 0;
    }

  // Non-void return type.
  *os << "TAO_INTERCEPTOR_CHECK_RETURN (";
  be_visitor_context ctx (*this->ctx_);
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
be_visitor_operation::gen_stub_operation_body (
    be_operation *node,
    be_type *return_type
  )
{
  be_interface *intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  *os << be_nl << "{" << be_idt_nl;

  if (node->has_native ()) // native exists => no stub
    {
      if (this->gen_raise_exception (return_type,
                                     "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_cs::"
              "visit_operation - "
              "codegen for native exception failed\n"
            ),
            -1
          );
        }

      *os << be_uidt_nl << "}";

      return 0;
    }

  if (!node->is_abstract ())
    {
      // If the object is lazily evaluated the proxy brker might well
      // be null.  Initialize it now.
      *os << "if (!this->is_evaluated ())" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NESTED_CLASS (CORBA, Object)::tao_object_initialize (this);"
          << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "if (this->the" << intf->base_proxy_broker_name () << "_ == 0)"
          << be_idt_nl
          << "{" << be_idt_nl
          << intf->flat_name () << "_setup_collocation ("
          << be_idt << be_idt_nl
          << "this->ACE_NESTED_CLASS (CORBA, Object)::_is_collocated ()"
          << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
    }

  const char *env = this->gen_environment_var ();

  if (ACE_OS::strcmp ("", env) != 0)
    {
      *os << env << be_nl;
    }

  // Declare return type helper class.

  *os << "TAO::Arg_Traits<";

  this->gen_arg_template_param_name (return_type,
                                     os);

  *os << ">::stub_ret_val _tao_retval;";

  // Declare the argument helper classes.

  AST_Argument *arg = 0;

  for (UTL_ScopeActiveIterator arg_decl_iter (node, UTL_Scope::IK_decls);
       ! arg_decl_iter.is_done ();
       arg_decl_iter.next ())
    {
      arg = AST_Argument::narrow_from_decl (arg_decl_iter.item ());

      *os << be_nl
          << "TAO::Arg_Traits<";

      this->gen_arg_template_param_name (arg->field_type (),
                                         os);

      *os << ">::";

      switch (arg->direction ())
        {
          case AST_Argument::dir_IN:
            *os << "in";
            break;
          case AST_Argument::dir_INOUT:
            *os << "inout";
            break;
          case AST_Argument::dir_OUT:
            *os << "out";
          default:
            break;
        }

      *os << "_arg_val _tao_" << arg->local_name () << " ("
          << arg->local_name () << ");";
    }

  *os << be_nl << be_nl
      << "TAO::Argument *_tao_signature [] =" << be_idt_nl
      << "{" << be_idt_nl
      << "&_tao_retval";

  for (UTL_ScopeActiveIterator arg_list_iter (node, UTL_Scope::IK_decls);
       ! arg_list_iter.is_done ();
       arg_list_iter.next ())
    {
      arg = AST_Argument::narrow_from_decl (arg_list_iter.item ());

      *os << "," << be_nl
          << "&_tao_" << arg->local_name ();
    }

  *os << be_uidt_nl
      << "};" << be_uidt;

  if (this->gen_pre_stub_info (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for exceptiondata failed\n"),
                        -1);
    }


  long tmp_len =
    ACE_OS::strlen (node->local_name ()->get_string ());

  *os << be_nl << be_nl
      << "TAO::Invocation_Adapter _tao_call (" << be_idt << be_idt_nl
      << "this," << be_nl
      << "_tao_signature," << be_nl
      << node->argument_count () + 1 << "," << be_nl
      << "\"";

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // If we are a attribute node, add th elength of the operation
      // name.
      tmp_len += 5;

      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "_set_";
        }
      else
        {
          *os << "_get_";
        }
    }
  *os << node->local_name () << "\"," << be_nl
      << tmp_len << "," << be_nl
      << "this->the" << intf->base_proxy_broker_name () << "_";

  if (node->flags () == AST_Operation::OP_oneway)
    {
      *os << "," << be_nl
          << "TAO::TAO_ONEWAY_INVOCATION";
    }

  if (be_global->ami_call_back ())
    {
      *os << "," << be_nl
          << "TAO::TAO_ASYNCHRONOUS_CALLBACK_INVOCATION";
    }

  *os << be_uidt_nl
      << ");" << be_uidt;

  *os << be_nl << be_nl;

  // Since oneways cannot raise user exceptions, we have that
  // case covered as well.
  if (node->exceptions ())
    {
      *os << "_tao_call.invoke (" << be_idt << be_idt_nl
          << "_tao_" << node->flat_name ()
          << "_exceptiondata," << be_nl
          << node->exceptions ()->length () << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt;
    }
  else
    {
      *os << "_tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);";
    }

  *os << be_nl;

  if (this->void_return_type (return_type))
    {
      *os << "ACE_CHECK;";
    }
  else
    {
      *os << "ACE_CHECK_RETURN (_tao_retval.excp ());";
    }

  if (!this->void_return_type (return_type))
    {
      *os << be_nl << be_nl
          << "return _tao_retval.retn ();";
    }

  *os << be_uidt_nl << "}";

  return 0;
}

int
be_visitor_operation::gen_pre_stub_info (
    be_operation *node
  )
{
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_exceptlist_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_cs::"
                         "gen_pre_stub_info - "
                         "Exceptionlist generation error\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation::gen_raise_interceptor_exception (
    be_type *bt,
    const char *excep,
    const char *completion_status
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->void_return_type (bt))
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw " << excep << "(" << completion_status << ");";
        }
      else
        {
          *os << "TAO_INTERCEPTOR_THROW (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")" << be_uidt << be_uidt_nl
              << ");" << be_uidt;
        }
    }
  else
    {
      if (bt->size_type () == AST_Type::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")," << be_uidt_nl
              <<  "0" << be_uidt_nl
              << ");" << be_uidt;
        }
      else
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")," << be_uidt_nl
              <<  "_tao_retval" << be_uidt_nl
              << ");" << be_uidt;
        }
    }

  return 0;
}

void
be_visitor_operation::gen_arg_template_param_name (AST_Type *bt,
                                                   TAO_OutStream *os)
{
  AST_Decl::NodeType nt = bt->node_type ();

  if (nt == AST_Decl::NT_typedef)
    {
      be_typedef *td = be_typedef::narrow_from_decl (bt);
      this->ctx_->alias (td);
      this->gen_arg_template_param_name (td->primitive_base_type (),
                                         os);
      this->ctx_->alias (0);
      return;
    }

  if (nt == AST_Decl::NT_string)
    {
      AST_String *s = AST_String::narrow_from_decl (bt);
      unsigned long bound = s->max_size ()->ev ()->u.ulval;
      AST_Typedef *alias = this->ctx_->alias ();

      if (bound > 0)
        {
          *os << "TAO::" << alias->local_name () << "_" << bound;
          return;
        }
    }

  if (nt == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (bt);

      switch (pdt->pt ())
        {
          case AST_PredefinedType::PT_boolean:
            *os << "ACE_InputCDR::to_boolean";
            return;
          case AST_PredefinedType::PT_octet:
            *os << "ACE_InputCDR::to_octet";
            return;
          case AST_PredefinedType::PT_char:
            *os << "ACE_InputCDR::to_char";
            return;
          case AST_PredefinedType::PT_wchar:
            *os << "ACE_InputCDR::to_wchar";
            return;
          default:
            break;
        }
    }

  *os << bt->name ();
}
