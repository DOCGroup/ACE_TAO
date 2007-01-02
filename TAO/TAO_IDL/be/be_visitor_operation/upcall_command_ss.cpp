// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    upcall_command_ss.cpp
//
// = DESCRIPTION
//    Visitor that generates operation-specific TAO::Upcall_Command
//    objects in skeletons.
//
// = AUTHOR
//    Ossama Othman
//
// ============================================================================


ACE_RCSID (be_visitor_operation,
           upcall_command_ss,
           "$Id$")

be_visitor_operation_upcall_command_ss
::be_visitor_operation_upcall_command_ss (
    be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_upcall_command_ss
::~be_visitor_operation_upcall_command_ss (void)
{
}

// The following needs to be done to deal until the MSVC compiler's broken
// handling of namespaces is fixed (hopefully forthcoming in version 7).
int
be_visitor_operation_upcall_command_ss
::gen_nested_namespace_begin (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char *item_name = 0;
  bool first_level = true;

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      item_name = i.item ()->get_string ();

      if (ACE_OS::strcmp (item_name, "") != 0)
        {
          // Leave the outermost root scope.
          *os << "namespace ";

          if (first_level)
            {
              // We are outermost module.
              *os << "POA_";
              first_level = false;
            }

          *os << item_name << be_nl
              << "{" << be_idt_nl;
        }
    }

  return 0;
}

// The following needs to be done to deal until the MSVC compiler's broken
// handling of namespaces is fixed (hopefully forthcoming in version 7).
int
be_visitor_operation_upcall_command_ss
::gen_nested_namespace_end (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      if (ACE_OS::strcmp (i.item ()->get_string (), "") != 0)
        {
          // Leave the outermost root scope.
          *os << be_uidt_nl << "}" << be_nl;
        }
    }

  return 0;
}

int
be_visitor_operation_upcall_command_ss::visit (be_operation * node,
                                               char const * full_skel_name,
                                               char const * upcall_command_name)
{
  be_interface * const intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  be_module *module = 0;

  // Is our enclosing scope a module? We need this check because for
  // platforms that support namespaces, the typecode must be declared
  // extern.
  if (intf->is_nested () &&
      intf->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      module = be_module::narrow_from_scope (intf->defined_in ());

      if (!module || (this->gen_nested_namespace_begin (module) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_operation_upcall_command_ss::visit - "
                             "Error parsing nested name\n"),
                            -1);
        }
    }

  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the operation-specific TAO::Upcall_Command concrete class.

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_upcall_command_ss::"
                         "visit - "
                         "bad interface scope\n"),
                        -1);
    }

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
      if (this->gen_nested_namespace_end (module) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_operation_upcall_command_ss::visit - "
                             "Error parsing nested name\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_operation_upcall_command_ss::gen_upcall (be_operation * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  UTL_ScopeActiveIterator si (node,
                              UTL_Scope::IK_decls);

  unsigned int index = 1;

  for (; !si.is_done (); si.next (), ++index)
    {
      AST_Argument * const arg =
        AST_Argument::narrow_from_decl (si.item ());

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
