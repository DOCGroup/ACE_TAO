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

#include <string>


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
  TAO_OutStream * os = this->ctx_->stream ();

  be_type * bt = 0;
  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
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

  std::string class_name;
  class_name = "TAO_" + std::string (node->flat_name ());

  // We need the interface node in which this operation was
  // defined.  However, if this operation node was an attribute node
  // in disguise, we get this information from the context and add a
  // "_get"/"_set" to the flat name to get around the problem of
  // overloaded methods which are generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_upcall_command_ss::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          class_name += "_get";
        }
      else
        {
          class_name += "_set";
        }
    }

  class_name += "_Upcall_Command";

  // Generate an operation-specific concrete TAO::Upcall_Command
  // class, an instance of which will be invoked by the
  // TAO::Upcall_Wrapper object.

  *os << "class " << class_name.c_str () << be_nl
      <<"  : public TAO::Upcall_Command" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl << be_nl;

  // Generate constructor
  *os << "inline " << class_name.c_str ()
      << " (" << be_idt_nl
      << intf->full_skel_name () << " * servant";

  // No need to accept an argument array parameter if the operation
  // has no arguments.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      *os << "," << be_nl
          << "TAO::Argument * const args[])" << be_nl;
    }
  else
    {
      *os << ")" << be_nl;
    }

  *os << ": servant_ (servant)";

  // If the operation has no arguments don't generate a member
  // initializer for the class argument array member/attribute.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      *os << be_nl
          << ", args_ (args)";
    }

  *os << be_uidt_nl;

  *os << "{" << be_nl
      << "}" << be_nl << be_nl;

  // Generate execute() method.
  *os << "virtual void execute (ACE_ENV_SINGLE_ARG_DECL)" << be_nl
      << "{" << be_idt_nl;

  if (!node->void_return_type ())
    {
      *os << "static_cast<TAO::SArg_Traits< ";

      this->gen_arg_template_param_name (node,
                                         node->return_type (),
                                         os);

      *os << ">::ret_val *> (this->args_[0])->arg () =" << be_idt_nl;
    }

  if (this->gen_upcall (node) == -1)
    {
      return -1;
    }

  if (!node->void_return_type ())
    {
      *os << be_uidt;
    }

  *os << be_nl
      << "ACE_CHECK;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl << be_nl;

  // Prevent copying/assignment and generation of default copy
  // constructor and assignment operator.
  *os << "private:" << be_idt_nl << be_nl
      << class_name.c_str () << " ("
      << class_name.c_str () << " const &);" << be_nl
      << "void operator= (" << class_name.c_str () << " const &);"
      << be_uidt_nl << be_nl;

  // Generate class attributes.
  *os << "private:" << be_idt_nl << be_nl
      << intf->full_skel_name () << " * const servant_;";

  // Don't bother generating an argument array attribute if the
  // operation has no arguments.
  if (!node->void_return_type () || node->argument_count () > 0)
    {
      *os << be_nl
          << "TAO::Argument * const * const args_;";
    }

  *os << be_uidt_nl;

  *os << be_uidt_nl << "};" << be_nl;

  return 0;
}

int
be_visitor_operation_upcall_command_ss::gen_upcall (be_operation * node)
{
  // @@ Based on
  //    be_visitor_operation_thru_poa_proxy_impl_ss::gen_invoke().

  TAO_OutStream * const os = this->ctx_->stream ();

  *os << "this->servant_->" << node->local_name () << " ("
      << be_idt;

  UTL_ScopeActiveIterator si (node,
                              UTL_Scope::IK_decls);

  if (si.is_done ())
    {
      *os << be_nl
          << "ACE_ENV_SINGLE_ARG_PARAMETER" << be_uidt_nl
          << ");";

      return 0;
    }

  int index = 1;

  for (; !si.is_done (); si.next (), ++index)
    {
      AST_Argument * const arg =
        AST_Argument::narrow_from_decl (si.item ());

      *os << (index == 1 ? "" : ",") << be_nl
          << "static_cast<TAO::SArg_Traits< ";

      this->gen_arg_template_param_name (arg,
                                         arg->field_type (),
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

      *os << "_arg_val *> (this->args_[" << index << "])->arg ()";
    }

  // End the upcall
  *os << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");";

  return 0;
}
