#include "operation.h"

be_visitor_operation_direct_proxy_impl_ss::
be_visitor_operation_direct_proxy_impl_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_direct_proxy_impl_ss::
~be_visitor_operation_direct_proxy_impl_ss () = default;

int
be_visitor_operation_direct_proxy_impl_ss::visit_operation (
  be_operation *node)
{
  /// These implied IDL operations are not to be processed on
  /// the skeleton side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  TAO_OutStream * const os = this->ctx_->stream ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context.
  UTL_Scope *s =
    this->ctx_->attribute ()
      ? this->ctx_->attribute ()->defined_in ()
      : node->defined_in ();

  be_interface *intf = dynamic_cast<be_interface*> (s);

  if (intf == nullptr)
    {
      be_porttype *pt = dynamic_cast<be_porttype*> (s);

      if (pt == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_")
                             ACE_TEXT ("direct_proxy_impl_ss::")
                             ACE_TEXT ("visit_operation - ")
                             ACE_TEXT ("bad scope\n")),
                            -1);
        }
      else
        {
          intf = this->ctx_->interface ();
        }
    }

  TAO_INSERT_COMMENT (os);

  std::string operation_name = intf->full_direct_proxy_impl_name () + "::";

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          operation_name += "_set_";
        }
      else
        {
          operation_name += "_get_";
        }
    }

  operation_name += this->ctx_->port_prefix ();
  operation_name += node->local_name ()->get_string ();

  if (!node->void_return_type () || node->nmembers () > 0)
    {
      // The arguments parameter is named only when the operation uses it.
      std::string signature = operation_name
        + " (TAO_Abstract_ServantBase *servant, TAO::Argument **args)";

      *os << "void" << be_nl;
      if (signature.length () <= be_global->max_line_length ())
        {
          *os << signature << be_nl;
        }
      else
        {
          *os << operation_name << " (" << be_idt_nl
              << "TAO_Abstract_ServantBase *servant," << be_nl
              << "TAO::Argument **args)" << be_uidt_nl;
        }
    }
  else
    {
      std::string signature = operation_name
        + " (TAO_Abstract_ServantBase *servant, TAO::Argument **)";

      *os << "void" << be_nl;
      if (signature.length () <= be_global->max_line_length ())
        {
          *os << signature << be_nl;
        }
      else
        {
          *os << operation_name << " (" << be_idt_nl
              << "TAO_Abstract_ServantBase *servant," << be_nl
              << "TAO::Argument **)" << be_uidt_nl;
        }
    }

  *os << "{" << be_idt_nl;

  std::string const servant_cast = intf->full_skel_name ()
    + "_ptr _tao_ptr = dynamic_cast<" + intf->full_skel_name ()
    + "_ptr> (servant);";
  if (servant_cast.length () + 2 <= be_global->max_line_length ())
    {
      *os << servant_cast << be_nl;
    }
  else
    {
      *os << intf->full_skel_name () << "_ptr _tao_ptr =" << be_idt_nl
          << "dynamic_cast<" << intf->full_skel_name ()
          << "_ptr> (servant);" << be_uidt_nl;
    }

  *os << "if (!_tao_ptr)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt_nl;

  if (!node->void_return_type ())
    {
      *os << "using _tao_retval_type = TAO::Arg_Traits<";

      this->gen_arg_template_param_name (node,
                                         node->return_type (),
                                         os);

      *os << ">::ret_val;" << be_nl
          << "_tao_retval_type * const _tao_retval = "
          << "static_cast<_tao_retval_type *> (args[0]);" << be_nl;
    }

  be_visitor_context ctx;

  if (this->gen_invoke (ctx, node) == -1)
    {
      return -1;
    }

  *os << be_uidt_nl << "}" << be_nl_2;

  return 0;
}


int
be_visitor_operation_direct_proxy_impl_ss::gen_invoke (
    be_visitor_context & /*ctx*/,
    be_operation *node
  )
{
  TAO_OutStream * const os = this->ctx_->stream ();

  UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
  AST_Argument *arg {};
  int index = 1;

  for (; !si.is_done (); si.next (), ++index)
    {
      arg = dynamic_cast<AST_Argument*> (si.item ());

      char const *arg_direction = nullptr;
      switch (arg->direction ())
        {
          case AST_Argument::dir_IN:
            arg_direction = "in";
            break;
          case AST_Argument::dir_INOUT:
            arg_direction = "inout";
            break;
          case AST_Argument::dir_OUT:
            arg_direction = "out";
            break;
          default:
            break;
        }

      *os << "using _tao_arg_" << index << "_type = TAO::Arg_Traits<";

      this->gen_arg_template_param_name (arg,
                                         arg->field_type (),
                                         os);

      *os << ">::" << arg_direction << "_arg_val;" << be_nl
          << "_tao_arg_" << index << "_type * const _tao_arg_" << index
          << " = static_cast<_tao_arg_" << index << "_type *> (args["
          << index << "]);" << be_nl;
    }

  if (index > 1)
    {
      *os << be_nl;
    }

  if (!node->void_return_type ())
    {
      *os << "_tao_retval->arg () = ";
    }

  *os << "_tao_ptr->" << node->local_name () << " (";
  if (index == 1)
    {
      *os << ");";
      return 0;
    }

  *os << be_idt_nl;
  for (int arg_index = 1; arg_index < index; ++arg_index)
    {
      *os << "_tao_arg_" << arg_index << "->arg ()";
      if (arg_index + 1 == index)
        {
          *os << ");";
        }
      else
        {
          *os << "," << be_nl;
        }
    }
  *os << be_uidt;

  return 0;
}
