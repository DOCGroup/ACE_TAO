//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           thru_poa_proxy_impl_sh, 
           "$Id$")

be_visitor_interface_thru_poa_proxy_impl_sh::
be_visitor_interface_thru_poa_proxy_impl_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_thru_poa_proxy_impl_sh::
~be_visitor_interface_thru_poa_proxy_impl_sh (void)
{
  // No-Op.
}

int
be_visitor_interface_thru_poa_proxy_impl_sh::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//                    ThruPOA  Impl. Declaration" << be_nl
      << "//" << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate Class Declaration.
  *os << "class " << be_global->skel_export_macro ()
      << " " << node->thru_poa_proxy_impl_name ();
  *os << " : " << be_idt_nl << "public virtual " 
      << "::" << node->full_base_proxy_impl_name ()
      << "," << be_nl << "public virtual " << "TAO_ThruPOA_Object_Proxy_Impl";

  if (node->n_inherits () > 0)
    {
      AST_Interface *parent = 0;

      for (int i = 0; i < node->n_inherits (); i++)
        {
          parent = node->inherits ()[i];

          if (parent->is_abstract ())
            {
              continue;
            }

          *os << "," << be_nl;

          be_interface *inherited =
            be_interface::narrow_from_decl (parent);

          *os << "public virtual ";
          *os << "::" <<  inherited->full_thru_poa_proxy_impl_name ();
        }
    }

  if (node->node_type () == AST_Decl::NT_component)
    {
      AST_Component *base = 
        AST_Component::narrow_from_decl (node)->base_component ();

      if (base != 0)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (base);

          *os << "," << be_nl
              << "public virtual "
              << "::" << inherited->full_thru_poa_proxy_impl_name ();
        }
    }

  *os << be_uidt_nl;
  *os << "{" << be_nl << "public:" << be_idt_nl;

  // Ctor
  *os << node->thru_poa_proxy_impl_name () << " (void);" << be_nl << be_nl;

  // Dtor
  *os << "virtual ~" << node->thru_poa_proxy_impl_name ()
      << " (void) {}";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) thru_poa_proxy_impl_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << be_uidt_nl << "};" << be_nl;
  *os << be_nl
      << "//" << be_nl
      << "//                ThruPOA  Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////";

  return 0;

}

int 
be_visitor_interface_thru_poa_proxy_impl_sh::gen_abstract_ops_helper (
    be_interface *node,
    be_interface *base,
    TAO_OutStream *os
  )
{
  if (node == base)
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_thru_poa_proxy_"
                             "impl_sh::abstract_base_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {
          UTL_ScopedName item_new_name (d->local_name (),
                                        0);

          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               &item_new_name,
                               op->is_local (),
                               op->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          be_visitor_operation_proxy_impl_xh op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
    }

  return 0;
}

int be_visitor_interface_thru_poa_proxy_impl_sh::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}


