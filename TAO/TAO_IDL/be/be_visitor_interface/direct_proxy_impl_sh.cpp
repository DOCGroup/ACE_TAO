//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           direct_proxy_impl_sh, 
           "$Id$")

be_visitor_interface_direct_proxy_impl_sh::
be_visitor_interface_direct_proxy_impl_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}

be_visitor_interface_direct_proxy_impl_sh::
~be_visitor_interface_direct_proxy_impl_sh (void)
{
  // No-Op.
}

int
be_visitor_interface_direct_proxy_impl_sh::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->decr_indent (0);

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//                    Direct  Impl. Declaration" << be_nl
      << "//" << be_nl << be_nl;
  // Generate Class Declaration.
  *os << "class " << be_global->skel_export_macro ()
      << " " << node->direct_proxy_impl_name ();
  *os << " : " << be_idt_nl << "public virtual " 
      << node->full_base_proxy_impl_name ()
      << "," << be_nl << "public virtual " << "TAO_Direct_Object_Proxy_Impl";

  if (node->n_inherits () > 0)
    {
      *os << "," << be_nl;

      for (int i = 0; i < node->n_inherits (); i++)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (node->inherits ()[i]);

          *os << "public virtual ";
          *os << inherited->full_direct_proxy_impl_name ();

          if (i < node->n_inherits () - 1)
            {
              *os << ", ";
            }

          *os << be_nl;
        }
    }

  *os << be_uidt_nl;
  *os << "{" << be_nl << "public:" << be_idt_nl;

  // Ctor
  *os << node->direct_proxy_impl_name () << " (void);" << be_nl << be_nl;

  // Dtor
  *os << "virtual ~" << node->direct_proxy_impl_name () << " (void) { }" << be_nl << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) direct_proxy_impl_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << "};" << be_uidt << be_nl << be_nl;
  *os << be_nl
      << "//" << be_nl
      << "//                Direct  Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
