//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           remote_proxy_impl_cs, 
           "$Id$")

be_visitor_interface_remote_proxy_impl_cs::
be_visitor_interface_remote_proxy_impl_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_remote_proxy_impl_cs::
~be_visitor_interface_remote_proxy_impl_cs (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_impl_cs::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  os->decr_indent (0);

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//                Base & Remote Proxy  Implementation. " << be_nl
      << "//" << be_nl << be_nl;
  // Create the destructor implementation for the base
  // proxy implementation.

  // Ctor Impl.
  *os << node->full_base_proxy_impl_name () << "::"
      << node->base_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;
  /*
  // Dtor Impl.
  *os << node->full_base_proxy_impl_name () << "::~"
      << node->base_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;
  */
  // Create the destructor implementation for the remote
  // proxy implementation.

  // Ctor Impl
  *os << node->full_remote_proxy_impl_name () << "::"
      << node->remote_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;

  /*
  // Dtor Impl
  *os << node->full_remote_proxy_impl_name () << "::~"
      << node->remote_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;
  */

  // Generate the code for the Remote Proxy Impl.
  // operations
  *os << "// Remote Implementation of the IDL interface methods"
      << be_nl << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for Base Proxy Broker class failed\n"),
                        -1);
    }

  *os << be_nl
      << "//" << be_nl
      << "//            End  Base & Remote  Proxy Implemeentation. " << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;

}
