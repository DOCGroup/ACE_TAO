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

int
be_visitor_operation_upcall_command_ss::visit_operation (be_operation * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

  os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the operation-specific TAO::Upcall_Command concrete class.

  be_interface * const intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_upcall_command_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Generate an operation-specific concrete TAO::Upcall_Command
  // class, an instance of which will be invoked by the
  // TAO::Upcall_Wrapper object.

  os << "class Upcall_Command" << be_nl
     << "  : public TAO::Upcall_Command" << be_nl
     << "{" << be_nl
     << "public:" << be_idt_nl << be_nl;

  // Generate constructor
  os << "inline Upcall_Command (" << be_idt_nl
     << intf->full_skel_name () << " * servant";

  // No need to accept an argument array parameter if the operation
  // has no arguments.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      os << "," << be_nl
         << "TAO::Argument * const args[])" << be_nl;
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
      os << be_nl
         << ", args_ (args)";
    }

  os << be_uidt_nl;

  os << "{" << be_nl
     << "}" << be_nl << be_nl;

  // Generate execute() method.
  os << "virtual void execute (ACE_ENV_SINGLE_ARG_DECL)" << be_nl
     << "{" << be_idt_nl;

  if (!node->void_return_type ())
    {
      os << "static_cast<TAO::SArg_Traits< ";

      this->gen_arg_template_param_name (node,
                                         node->return_type (),
                                         &os);

      os << ">::ret_val *> (this->args_[0])->arg () =" << be_idt_nl;
    }

  if (this->gen_upcall (node) == -1)
    {
      return -1;
    }

  if (!node->void_return_type ())
    {
      os << be_uidt;
    }

  os << be_nl
     << "ACE_CHECK;" << be_uidt_nl
     << "}" << be_uidt_nl << be_nl;

  // Prevent copying/assignment and generation of default copy
  // constructor and assignment operator.
  os << "private:" << be_idt_nl << be_nl
     << "Upcall_Command ("
     << "Upcall_Command const &);" << be_nl
     << "void operator= (Upcall_Command const &);"
     << be_uidt_nl << be_nl;

  // Generate class attributes.
  os << "private:" << be_idt_nl << be_nl
     << intf->full_skel_name () << " * const servant_;";

  // Don't bother generating an argument array attribute if the
  // operation has no arguments.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      os << be_nl
         << "TAO::Argument * const * const args_;";
    }

  os << be_uidt_nl
     << "};" << be_nl;

  return 0;
}

int
be_visitor_operation_upcall_command_ss::gen_upcall (be_operation * node)
{
  // @@ Based on
  //    be_visitor_operation_thru_poa_proxy_impl_ss::gen_invoke().

  TAO_OutStream & os = *this->ctx_->stream ();

  os << "this->servant_->" << node->local_name () << " ("
      << be_idt;

  UTL_ScopeActiveIterator si (node,
                              UTL_Scope::IK_decls);

  if (si.is_done ())
    {
      os << be_nl
         << "ACE_ENV_SINGLE_ARG_PARAMETER" << be_uidt_nl
         << ");";

      return 0;
    }

  int index = 1;

  for (; !si.is_done (); si.next (), ++index)
    {
      AST_Argument * const arg =
        AST_Argument::narrow_from_decl (si.item ());

      os << (index == 1 ? "" : ",") << be_nl
         << "static_cast<TAO::SArg_Traits< ";

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

      os << "_arg_val *> (this->args_[" << index << "])->arg ()";
    }

  // End the upcall
  os << be_nl
     << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
     << ");";

  return 0;
}
