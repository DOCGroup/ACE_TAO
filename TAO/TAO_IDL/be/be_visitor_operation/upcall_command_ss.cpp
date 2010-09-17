
//=============================================================================
/**
 *  @file    upcall_command_ss.cpp
 *
 *  $Id$
 *
 *  Visitor that generates operation-specific TAO::Upcall_Command
 *  objects in skeletons.
 *
 *
 *  @author Ossama Othman
 */
//=============================================================================

be_visitor_operation_upcall_command_ss::be_visitor_operation_upcall_command_ss (
      be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_upcall_command_ss::~be_visitor_operation_upcall_command_ss (
  void)
{
}

int
be_visitor_operation_upcall_command_ss::visit (
  be_operation * node,
  char const * full_skel_name,
  char const * upcall_command_name)
{
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  be_interface * const intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_upcall_command_ss::")
                         ACE_TEXT ("visit - ")
                         ACE_TEXT ("bad interface scope\n")),
                        -1);
    }

  be_module *module = 0;

  TAO_OutStream & os = *this->ctx_->stream ();

  // Is our enclosing scope a module? We need this check because for
  // platforms that support namespaces, the typecode must be declared
  // extern.
  if (intf->is_nested () &&
      intf->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      module = be_module::narrow_from_scope (intf->defined_in ());

      if (module == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_")
                             ACE_TEXT ("upcall_command_ss::visit - ")
                             ACE_TEXT ("Error parsing nested name\n")),
                            -1);
        }

      be_util::gen_nested_namespace_begin (&os, module, true);
    }

  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

  os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the operation-specific TAO::Upcall_Command concrete
  // class.

  // Generate an operation-specific concrete TAO::Upcall_Command
  // class, an instance of which will be invoked by the
  // TAO::Upcall_Wrapper object.

  os << "class " << upcall_command_name  << be_nl
     << "  : public TAO::Upcall_Command" << be_nl
     << "{" << be_nl
     << "public:" << be_idt_nl ;

  // Generate constructor
  os << "inline " << upcall_command_name
     << " (" << be_idt_nl
     << full_skel_name << " * servant";

  // No need to accept an argument array parameter if the operation
  // has no arguments.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      os << "," << be_nl;

      if (be_global->gen_thru_poa_collocation ())
        os << "TAO_Operation_Details const * operation_details," << be_nl;

      os << "TAO::Argument * const args[])" << be_nl;
    }
  else
    {
      os << ")" << be_nl;
    }

  os << ": servant_ (servant)";

  // If the operation has no arguments don't generate a member
  // initializer for the class argument array member/attribute.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      os << be_idt_nl;

      if (be_global->gen_thru_poa_collocation ())
        {
          os << ", operation_details_ (operation_details)" << be_nl;
        }

      os << ", args_ (args)" << be_uidt;
    }

  os << be_uidt_nl;

  os << "{" << be_nl
     << "}" << be_nl << be_nl;

  // Generate execute() method.
  os << "virtual void execute (void)" << be_nl
     << "{" << be_idt_nl;

  if (!node->void_return_type ())
    {
      os << "TAO::SArg_Traits< ";


      this->gen_arg_template_param_name (node,
                                         node->return_type (),
                                         &os);

      os << ">::ret_arg_type retval =" << be_idt_nl;

      if (be_global->gen_thru_poa_collocation ())
        {
          os << "TAO::Portable_Server::get_ret_arg< ";

          this->gen_arg_template_param_name (node,
                                             node->return_type (),
                                             &os);

          os << "> (" << be_idt_nl
             << "this->operation_details_," << be_nl
             << "this->args_);" << be_uidt;
        }
      else
        {
          os << "static_cast<TAO::SArg_Traits< ";

          this->gen_arg_template_param_name (node,
                                             node->return_type (),
                                             &os);

          os << ">::ret_val *> (this->args_[0])->arg ();";
        }

      os << be_uidt_nl << be_nl;
    }

  if (this->gen_upcall (node) == -1)
    {
      return -1;
    }

  os << "}" << be_uidt_nl << be_nl;

  // Generate class attributes.
  os << "private:" << be_idt_nl
     << full_skel_name << " * const servant_;";

  // Don't bother generating an argument array attribute if the
  // operation has no arguments.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      os << be_nl;

      if (be_global->gen_thru_poa_collocation ())
        os << "TAO_Operation_Details const * const operation_details_;" << be_nl;

      os << "TAO::Argument * const * const args_;";
    }

  os << be_uidt_nl
     << "};";

  if (module != 0)
    {
      be_util::gen_nested_namespace_end (&os, module);
    }

  return 0;
}

int
be_visitor_operation_upcall_command_ss::gen_upcall (
  be_operation *node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  UTL_ScopeActiveIterator si (node,
                              UTL_Scope::IK_decls);

  unsigned int index = 1;
  const char *op_name = node->flat_name ();
  static const char *excep_suffix = "_excep";
  static const size_t excep_suffix_len = ACE_OS::strlen (excep_suffix);
  bool excep_method = ((ACE_OS::strstr (op_name, excep_suffix) +
                        excep_suffix_len) ==
                       (op_name + ACE_OS::strlen (op_name)));

  for (; !si.is_done (); si.next (), ++index)
    {
      AST_Argument * const arg =
        AST_Argument::narrow_from_decl (si.item ());

      // Finish the check for the _excep method
      if (excep_method)
        {
          excep_method = false;
          be_argument *argument =
            be_argument::narrow_from_decl (si.item ());
          be_valuetype *value_type =
            be_valuetype::narrow_from_decl (argument->field_type ());

          if (value_type != 0)
            {
              static const char *excepholder = "ExceptionHolder";
              static const size_t excepholder_len =
                                         ACE_OS::strlen (excepholder);
              const char *param_name = value_type->full_name ();
              excep_method =
                 ((ACE_OS::strstr (param_name, excepholder) +
                   excepholder_len) ==
                  (param_name + ACE_OS::strlen (param_name)));
            }
        }

      os << "TAO::SArg_Traits< ";

      this->gen_arg_template_param_name (arg,
                                         arg->field_type (),
                                         &os);

      os << ">::";

      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          os << "in";
          break;
        case AST_Argument::dir_INOUT:
          os << "inout";
          break;
        case AST_Argument::dir_OUT:
          os << "out";
        default:
          break;
        }

      os << "_arg_type arg_" << index << " =" << be_idt_nl;

      if (be_global->gen_thru_poa_collocation ())
        {
          os << "TAO::Portable_Server::get_";

          switch (arg->direction ())
            {
            case AST_Argument::dir_IN:
              os << "in";
              break;
            case AST_Argument::dir_INOUT:
              os << "inout";
              break;
            case AST_Argument::dir_OUT:
              os << "out";
            default:
              break;
            }

          os << "_arg< ";

          this->gen_arg_template_param_name (arg,
                                             arg->field_type (),
                                             &os);

          os << "> (" << be_idt_nl
             << "this->operation_details_," << be_nl
             << "this->args_," << be_nl
             << index << ");" << be_uidt_nl;
        }
      else
        {
          os << "static_cast<TAO::SArg_Traits< ";

          this->gen_arg_template_param_name (arg,
                                             arg->field_type (),
                                             &os);

          os << ">::";

          switch (arg->direction ())
            {
            case AST_Argument::dir_IN:
              os << "in";
              break;
            case AST_Argument::dir_INOUT:
              os << "inout";
              break;
            case AST_Argument::dir_OUT:
              os << "out";
            default:
              break;
            }

          os << "_arg_val *> (this->args_[" << index << "])->arg ();"
             << be_nl;
        }

      os << be_uidt_nl;

    }

  --index;

  // We have determined that this is an "_excep" method, there is exactly
  // one argument.  Now, if the node has exceptions, we're in business.
  if (excep_method && index == 1 && node->exceptions())
    {
      be_visitor_operation_exceptlist_cs exceplist (this->ctx ());
      exceplist.visit_operation (node);

      unsigned int exceptions_count = 0;

      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done (); ei.next ())
        {
          ++exceptions_count;
        }

      os << be_nl
         << "TAO::ExceptionHolder *tao_excepholder = " << be_idt_nl
         << "dynamic_cast<TAO::ExceptionHolder *> (arg_" << index
         << ");" << be_uidt_nl
         << "if (tao_excepholder != 0)" << be_idt_nl
         << "{" << be_idt_nl
         << "tao_excepholder->set_exception_data "
            "(_tao_" << op_name << "_exceptiondata, " << exceptions_count << ");" << be_uidt_nl
         << "}" << be_uidt_nl
         << be_nl;
    }

  if (!node->void_return_type ())
    {
      os << "retval =" << be_idt_nl;
    }

  os << "this->servant_->" << node->local_name () << " ("
     << be_idt;

  size_t const count = node->argument_count ();

  for (unsigned int i = 0; i < count; ++i)
    {
      os << be_nl
         << (i == 0 ? "" : ", ") << "arg_" << i + 1;
    }

  os << ");";

  if (!node->void_return_type ())
    {
      os << be_uidt;
    }

  os << be_uidt
     << be_uidt_nl;

  return 0;
}
