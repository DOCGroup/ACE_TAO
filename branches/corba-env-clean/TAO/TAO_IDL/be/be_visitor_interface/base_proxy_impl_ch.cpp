//$Id$

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, base_proxy_impl_ch, "$Id$")

be_visitor_interface_base_proxy_impl_ch::be_visitor_interface_base_proxy_impl_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_base_proxy_impl_ch::~be_visitor_interface_base_proxy_impl_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_base_proxy_impl_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  //  os->indent ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//                    Base Proxy Impl. Declaration" << be_nl
      << "//" << be_nl << be_nl;

  // Generate Class Declaration.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->base_proxy_impl_name () << be_idt_nl
      << ": ";

  if (node->n_inherits () > 0)
    {
      *os << be_idt;

      for (int i = 0; i < node->n_inherits (); i++)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (node->inherits ()[i]);

          *os << "public virtual ";
          *os << inherited->full_base_proxy_impl_name ();

          if (i < node->n_inherits () - 1)
            {
               // Node is the case of multiple
               // inheritance, so put a comma.
              *os << ", " << be_nl;
            }
        }

      *os << be_uidt << be_uidt_nl; // idt = 0
    }
  else
    {
      *os << "public virtual TAO_Object_Proxy_Impl" << be_uidt_nl;
    }

  *os << "{" << be_nl << "public:"
      << be_idt_nl; // idt = 1

  // Destructor Declaration.
  *os << "virtual ~" << node->base_proxy_impl_name () << " (void) { }"
      << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_base_proxy_impl_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  *os << be_uidt_nl;

  // Constructor Declaration.
  *os << "protected:" << be_idt_nl // idt = 1
      << node->base_proxy_impl_name () << " (void);"
      << be_uidt_nl;

  *os << "};" << be_nl << be_nl
      << "//" << be_nl
      << "//               End Base Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;
  return 0;
}
