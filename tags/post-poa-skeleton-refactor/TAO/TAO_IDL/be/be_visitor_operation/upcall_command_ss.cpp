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
  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

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

  return this->visit (node, intf->full_skel_name ());
}

int
be_visitor_operation_upcall_command_ss::visit (be_operation * node,
                                               char const * full_skel_name)
{
  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

  TAO_OutStream & os = *this->ctx_->stream ();

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
                         "visit - "
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
      os << be_nl;

      if (be_global->gen_thru_poa_collocation ())
        os << ", operation_details_ (operation_details)" << be_nl;

      os << ", args_ (args)";
    }

  os << be_uidt_nl;

  os << "{" << be_nl
     << "}" << be_nl << be_nl;

  // Generate execute() method.
  os << "virtual void execute (ACE_ENV_SINGLE_ARG_DECL)" << be_nl
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
          os << "get_ret_arg< ";

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

  if (!node->void_return_type ())
    {
      os << be_uidt;
    }

  os << be_uidt_nl
     << "}" << be_uidt_nl << be_nl;

  // Generate class attributes.
  os << "private:" << be_idt_nl << be_nl
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
     << "};" << be_nl;

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
          os << "get_";

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
      os << be_nl
         << "retval =" << be_idt_nl;
    }

  os << "this->servant_->" << node->local_name () << " ("
     << be_idt_nl;

  size_t const count = node->argument_count ();

  for (unsigned int i = 0; i < count; ++i)
    os << (i == 0 ? "" : ", ") << "arg_" << i + 1 << be_nl;

  if (count > 0)
    os << "ACE_ENV_ARG_PARAMETER);" << be_uidt_nl;
  else
    os << "ACE_ENV_SINGLE_ARG_PARAMETER);" << be_uidt_nl;

//   UTL_ScopeActiveIterator si (node,
//                               UTL_Scope::IK_decls);

//   if (si.is_done ())
//     {
//       os << be_nl
//          << "ACE_ENV_SINGLE_ARG_PARAMETER" << be_uidt_nl
//          << ");";

//       return 0;
//     }

//   int index = 1;

//   for (; !si.is_done (); si.next (), ++index)
//     {
//       AST_Argument * const arg =
//         AST_Argument::narrow_from_decl (si.item ());

//       os << (index == 1 ? "" : ",") << be_nl
//          << "static_cast<TAO::SArg_Traits< ";

//       this->gen_arg_template_param_name (arg,
//                                          arg->field_type (),
//                                          &os);

//       os << ">::";

//       switch (arg->direction ())
//         {
//         case AST_Argument::dir_IN:
//           os << "in";
//           break;
//         case AST_Argument::dir_INOUT:
//           os << "inout";
//           break;
//         case AST_Argument::dir_OUT:
//           os << "out";
//         default:
//           break;
//         }

//       os << "_arg_val *> (this->args_[" << index << "])->arg ()";
//     }

  return 0;
}
