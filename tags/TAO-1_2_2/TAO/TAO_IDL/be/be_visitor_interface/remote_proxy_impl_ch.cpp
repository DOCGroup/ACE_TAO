#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, remote_proxy_broker_impl_ch, "$Id$")

be_visitor_interface_remote_proxy_impl_ch::be_visitor_interface_remote_proxy_impl_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_remote_proxy_impl_ch::~be_visitor_interface_remote_proxy_impl_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_impl_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl
      << "//                Remote Proxy Impl. Declaration" << be_nl
      << "//" << be_nl << be_nl;

  // Generate Class Declaration.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->remote_proxy_impl_name () << be_idt_nl;
  *os << ": " << "public virtual " << node->base_proxy_impl_name ()
      << "," << be_idt_nl << "public virtual " << "TAO_Remote_Object_Proxy_Impl";

  if (node->n_inherits () > 0)
    {
      *os << "," << be_nl;

      for (int i = 0; i < node->n_inherits (); i++)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (node->inherits ()[i]);

          *os << "public virtual ";
          *os << inherited->full_remote_proxy_impl_name ();

          if (i < node->n_inherits () - 1)
            {
              // Node is the case of multiple
              // inheritance, so put a comma.
              *os << ", ";
              *os << be_nl;
            }
        }
    }

  *os << be_uidt << be_uidt_nl;
  *os << "{" << be_nl << "public:" << be_idt_nl;

  // Constructor Declaration.
  *os << node->remote_proxy_impl_name () << " (void);"
      << be_nl  << be_nl;

  // Destructor Declaration.
  *os << "virtual ~" << node->remote_proxy_impl_name () << " (void) { }"
      << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};" << be_nl;
  *os << be_nl
      << "//" << be_nl
      << "//             End Remote Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
