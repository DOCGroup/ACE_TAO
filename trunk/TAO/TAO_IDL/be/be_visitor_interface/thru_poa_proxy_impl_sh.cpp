#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, thru_poa_proxy_broker_impl_sh, "$Id$")

be_visitor_interface_thru_poa_proxy_impl_sh::be_visitor_interface_thru_poa_proxy_impl_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_thru_poa_proxy_impl_sh::~be_visitor_interface_thru_poa_proxy_impl_sh (void)
{
  // No-Op.
}

int
be_visitor_interface_thru_poa_proxy_impl_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  //  os->gen_ifdef_macro (node->flat_name (), "THRU_POA_PROXY_IMPL_");
  os->decr_indent (0);
  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//                    ThruPOA  Impl. Declaration" << be_nl 
      << "//" << be_nl << be_nl;
  // Generate Class Declaration.
  *os << "class " << be_global->skel_export_macro ()
      << " " << node->thru_poa_proxy_impl_name ();
  *os << " : " << be_idt_nl << "public virtual " << "::" << node->full_base_proxy_impl_name ()
      << "," << be_nl << "public virtual " << "TAO_ThruPOA_Object_Proxy_Impl";

  if (node->n_inherits () > 0)
    {
      *os << "," << be_nl;
      for (int i = 0; i < node->n_inherits (); i++)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (node->inherits ()[i]);
          be_decl *scope = 0;
          if (inherited->is_nested ())
            {
              // inherited node is used in the scope of "node" node
              scope =
                be_scope::narrow_from_scope (node->defined_in ())->decl ();
            }

          *os << "public virtual ";
          *os << "::" <<  inherited->full_thru_poa_proxy_impl_name ();
          if (i < node->n_inherits () - 1) // node is the case of multiple
            // inheritance, so put a comma
            {
              *os << ", ";
            }
          *os << be_nl;
        }  // end of for loop
    }

  *os << be_uidt_nl;
    *os << "{" << be_nl << "public:" << be_idt_nl;
    
    // Dtor
    *os << "~" << node->thru_poa_proxy_impl_name () << " (void);" << be_nl << be_nl;
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) thru_poa_proxy_impl_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  *os << "};" << be_uidt << be_nl << be_nl;
  *os << be_nl
      << "//" << be_nl
      << "//                ThruPOA  Proxy Impl. Declaration" << be_nl 
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;
  //  os->gen_endif ();
  return 0;

}
