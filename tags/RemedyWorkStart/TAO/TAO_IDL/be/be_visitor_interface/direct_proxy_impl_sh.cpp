//
// $Id$
//

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

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl
      << "//                    Direct  Impl. Declaration" << be_nl
      << "//" << be_nl_2;

  // Generate Class Declaration.
  *os << "class " << be_global->skel_export_macro ()
      << " " << node->direct_proxy_impl_name ();

  bool first_concrete = true;

  if (node->n_inherits () > 0)
    {
      AST_Type *parent = 0;

      for (int i = 0; i < node->n_inherits (); ++i)
        {
          parent = node->inherits ()[i];

          if (parent->is_abstract ())
            {
              continue;
            }

          be_interface *inherited =
            be_interface::narrow_from_decl (parent);

          if (first_concrete)
            {
              *os << be_nl
                  << "  : " << be_idt << be_idt;
            }
          else
            {
              *os << "," << be_nl;
            }

          first_concrete = false;

          *os << "public virtual ::"
              << inherited->full_direct_proxy_impl_name ();
        }

      if (!first_concrete)
        {
          *os << be_uidt << be_uidt;
        }
    }

  *os << be_nl
      << "{" << be_nl << "public:" << be_idt_nl;

  // Dtor
  *os << "virtual ~" << node->direct_proxy_impl_name () << " (void);";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) direct_proxy_impl_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};" << be_nl_2
      << "//" << be_nl
      << "//                Direct  Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl_2;

  return 0;
}

int
be_visitor_interface_direct_proxy_impl_sh::gen_abstract_ops_helper (
    be_interface *node,
    be_interface *base,
    TAO_OutStream *os
  )
{
  if (!base->is_abstract ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_thru_poa_proxy_"
                             "impl_sh::gen_abstract_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      UTL_ScopedName item_new_name (d->local_name (),
                                    0);

          // We pass the node's is_abstract flag to the operation
          // constructor so we will get the right generated operation
          // body if we are regenerating an operation from an
          // abstract interface in a concrete interface or component.
      if (d->node_type () == AST_Decl::NT_op)
        {
          be_operation *op = be_operation::narrow_from_decl (d);
          be_visitor_operation_proxy_impl_xh op_visitor (&ctx);
          op_visitor.visit_operation (op);
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 &item_new_name,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);

          UTL_ExceptList *get_exceptions = attr->get_get_exceptions ();

          if (0 != get_exceptions)
            {
              new_attr.be_add_get_exceptions (get_exceptions->copy ());
            }

          UTL_ExceptList *set_exceptions = attr->get_set_exceptions ();

          if (0 != set_exceptions)
            {
              new_attr.be_add_set_exceptions (set_exceptions->copy ());
            }

          be_visitor_attribute attr_visitor (&ctx);
          attr_visitor.visit_attribute (&new_attr);
          ctx.attribute (0);
          new_attr.destroy ();
        }
    }

  return 0;
}

int be_visitor_interface_direct_proxy_impl_sh::visit_component (
    be_component *node)
{
  return this->visit_interface (node);
}

int be_visitor_interface_direct_proxy_impl_sh::visit_connector (
    be_connector *node)
{
  return this->visit_component (node);
}

