//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           thru_poa_proxy_impl_ss, 
           "$Id$")

be_visitor_interface_thru_poa_proxy_impl_ss::
be_visitor_interface_thru_poa_proxy_impl_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_thru_poa_proxy_impl_ss::
~be_visitor_interface_thru_poa_proxy_impl_ss (void)
{
  // No-Op.
}

int
be_visitor_interface_thru_poa_proxy_impl_ss::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl
      << "//                 ThruPOA Proxy  Implementation\n"
      << "//";

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Ctor Implementation
  *os << node->full_thru_poa_proxy_impl_name () << "::"
      << node->thru_poa_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;

  // Generate the code for the ThruPOA Proxy Impl.
  // operations
  *os << "// ThruPOA Implementation of the IDL interface methods";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for ThruPOA Proxy Impl failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "//\n"
      << "//           End ThruPOA Proxy Implementation\n"
      << "///////////////////////////////////////////////////////////////////////";

  return 0;

}

int 
be_visitor_interface_thru_poa_proxy_impl_ss::gen_abstract_ops_helper (
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
  ctx.state (TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_thru_poa_proxy"
                             "_impl_ss::abstract_base_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      UTL_ScopedName *item_new_name  = 0;
      ACE_NEW_RETURN (item_new_name,
                      UTL_ScopedName (d->local_name ()->copy (),
                                      0),
                      -1);

      UTL_ScopedName *base = (UTL_ScopedName *)node->name ()->copy ();
      base->nconc (item_new_name);

      if (d->node_type () == AST_Decl::NT_op)
        {
          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               0,
                               op->is_local (),
                               op->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          new_op.set_name (base);
          be_visitor_operation_thru_poa_proxy_impl_ss op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);

          base->destroy ();
          delete base;
          base = 0;
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 0,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);
          new_attr.set_name (base);
          new_attr.be_add_get_exceptions (attr->get_get_exceptions ());
          new_attr.be_add_set_exceptions (attr->get_set_exceptions ());
          be_visitor_attribute attr_visitor (&ctx);
          attr_visitor.visit_attribute (&new_attr);
          ctx.attribute (0);
        }
    }

  return 0;
}

int be_visitor_interface_thru_poa_proxy_impl_ss::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}


