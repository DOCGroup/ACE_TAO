//
//$Id$
//

#include "interface.h"

be_visitor_interface_direct_proxy_impl_ss::
be_visitor_interface_direct_proxy_impl_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_direct_proxy_impl_ss::
~be_visitor_interface_direct_proxy_impl_ss (void)
{
}

int
be_visitor_interface_direct_proxy_impl_ss::visit_interface (
    be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);

  *os << be_nl_2
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl
      << "//                 Direct Proxy  Implementation" << be_nl
      << "//" << be_nl_2;

  // Destructor Implementation
  *os << node->full_direct_proxy_impl_name () << "::~"
      << node->direct_proxy_impl_name () << " (void)"
      << be_nl << "{" << be_nl << "}" << be_nl_2;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_direct_collocated_ss::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl_2
      << "//" << be_nl
      << "//           End Direct Proxy Implementation" << be_nl
      << "///////////////////////////////////////////////////////////////////////";

  return 0;
}

int
be_visitor_interface_direct_proxy_impl_ss::gen_abstract_ops_helper (
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
  ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_direct_proxy"
                             "_impl_ss::gen_abstract_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      AST_Decl::NodeType nt = d->node_type ();

      UTL_ScopedName *item_new_name = 0;
      UTL_ScopedName *new_name = 0;

      if (AST_Decl::NT_op == nt || AST_Decl::NT_attr == nt)
        {
          ACE_NEW_RETURN (item_new_name,
                          UTL_ScopedName (d->local_name ()->copy (),
                                          0),
                          -1);

          new_name = (UTL_ScopedName *) node->name ()->copy ();
          new_name->nconc (item_new_name);
        }
      else
        {
          continue;
        }

      // We pass the node's is_abstract flag to the operation
      // constructor so we will get the right generated operation
      // body if we are regenerating an operation from an
      // abstract interface in a concrete interface or component.
      if (d->node_type () == AST_Decl::NT_op)
        {
          be_operation *op = be_operation::narrow_from_decl (d);
          UTL_ScopedName *old_name =
            (UTL_ScopedName *) op->name ()->copy ();
          op->set_name (new_name);
          op->set_defined_in (node);
          op->is_abstract (node->is_abstract ());

          be_visitor_operation_direct_proxy_impl_ss op_visitor (&ctx);
          op_visitor.visit_operation (op);

          op->set_name (old_name);
          op->set_defined_in (base);
          op->is_abstract (base->is_abstract ());
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
          new_attr.set_name (new_name);

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

int be_visitor_interface_direct_proxy_impl_ss::visit_component (
    be_component *node)
{
  return this->visit_interface (node);
}

int be_visitor_interface_direct_proxy_impl_ss::visit_connector (
    be_connector *node)
{
  return this->visit_component (node);
}

