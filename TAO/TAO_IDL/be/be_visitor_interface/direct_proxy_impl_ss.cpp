//$Id$

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, direct_proxy_impl_ss, "$Id$")

be_visitor_interface_direct_proxy_impl_ss::be_visitor_interface_direct_proxy_impl_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}

be_visitor_interface_direct_proxy_impl_ss::~be_visitor_interface_direct_proxy_impl_ss (void)
{
  // No-Op.
}

int
be_visitor_interface_direct_proxy_impl_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);
  os->indent ();
  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//                 Direct Proxy  Implementation" << be_nl
      << "//" << be_nl << be_nl;


  // Ctor Implementation
  *os << node->full_direct_proxy_impl_name () << "::"
      << node->direct_proxy_impl_name () << " (void)"
      << be_nl << "{}" << be_nl << be_nl;
  /*
  // Dtor Implementation
  *os << node->full_direct_proxy_impl_name () << "::"
      << "~" << node->direct_proxy_impl_name () << " (void)"
      << be_nl << "{}" << be_nl << be_nl;
  */
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_direct_collocated_ss::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl
      << "//" << be_nl
      << "//           End Direct Proxy Implementation" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl;
  return 0;
}
