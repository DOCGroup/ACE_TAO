#include "operation.h"

be_visitor_operation_proxy_impl_xh::be_visitor_operation_proxy_impl_xh (
    be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_proxy_impl_xh::~be_visitor_operation_proxy_impl_xh () = default;

int be_visitor_operation_proxy_impl_xh::visit_operation (be_operation *node)
{
  /// These implied IDL operations are not to be processed on
  /// the skeleton side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  TAO_OutStream * const os = this->ctx_->stream ();
  this->ctx_->node (node);

  TAO_INSERT_COMMENT (os);

  std::string const attr_prefix = this->ctx_->attribute ()
    ? (node->nmembers () == 1 ? "_set_" : "_get_") : "";
  std::string const method_name = attr_prefix + this->ctx_->port_prefix ()
    + node->local_name ()->get_string ();
  bool const uses_args = !node->void_return_type () || node->nmembers () > 0;
  std::string const signature = "static void " + method_name
    + " (TAO_Abstract_ServantBase *servant, TAO::Argument **"
    + (uses_args ? "args" : "") + ");";

  if (signature.length () <= be_global->max_line_length ())
    {
      *os << signature;
    }
  else
    {
      *os << "static void" << be_nl
          << method_name << " (" << be_idt_nl
          << "TAO_Abstract_ServantBase *servant," << be_nl
          << "TAO::Argument **";
      if (uses_args)
        {
          *os << "args";
        }
      *os << ");" << be_uidt;
    }

  return 0;
}
