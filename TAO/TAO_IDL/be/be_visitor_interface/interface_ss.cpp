
//=============================================================================
/**
 *  @file    interface_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the server skeletons file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "global_extern.h"
#include "ast_generator.h"
#include "ast_string.h"

// ************************************************************
// Interface visitor for server skeletons.
// ************************************************************

be_visitor_interface_ss::be_visitor_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ss::~be_visitor_interface_ss (void)
{
}

int
be_visitor_interface_ss::visit_interface (be_interface *node)
{
  if (node->srv_skel_gen ()
      || node->imported ()
      || node->is_abstract ())
    {
      return 0;
    }

  if (node->is_local ())
    {
      if (this->is_amh_rh_node (node))
        {
          // Create amh_rh_visitors.
          be_visitor_amh_rh_interface_ss amh_rh_ss_intf (this->ctx_);
          amh_rh_ss_intf.visit_interface (node);
        }

      return 0;
    }

  if (this->generate_amh_classes (node) == -1)
    {
      return -1;
    }
  ACE_CString full_skel_name_holder =
    this->generate_full_skel_name (node);

  const char *full_skel_name = full_skel_name_holder.c_str ();

  ACE_CString flat_name_holder =
    this->generate_flat_name (node);

  const char *flat_name = flat_name_holder.c_str ();

  int status =
    node->gen_operation_table (flat_name,
                               full_skel_name);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for operation ")
                         ACE_TEXT ("table failed\n")),
                        -1);
    }

  if (this->generate_proxy_classes (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for proxy classes\n")),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  // Find if we are at the top scope or inside some module,
  // pre-compute the prefix that must be added to the local name in
  // each case.
  const char *local_name_prefix = "";

  if (!node->is_nested ())
    {
      local_name_prefix = "POA_";
    }

  ACE_CString node_local_name_holder =
    this->generate_local_name (node);

  const char *node_local_name = node_local_name_holder.c_str ();

  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name
      << " (void)" << be_idt_nl;

  *os << ": TAO_ServantBase ()" << be_uidt_nl;

  // Default constructor body.
  *os << "{" << be_idt_nl
      << "this->optable_ = &tao_" << flat_name
      << "_optable;" << be_uidt_nl
      << "}" << be_nl_2;

  // find if we are at the top scope or inside some module
  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name << " ("
      << "const " << local_name_prefix
      << node_local_name << "& rhs)";

  *os << be_idt_nl
      << ": TAO_Abstract_ServantBase (rhs)," << be_nl
      << "  TAO_ServantBase (rhs)";

  if (this->generate_copy_ctor (node, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT (" copy ctor generation failed\n")),
                        -1);
    }

  *os << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl_2;

  *os << full_skel_name << "::~"
      << local_name_prefix << node_local_name
      << " (void)" << be_nl;
  *os << "{" << be_nl;
  *os << "}" << be_nl;

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  // Generate code for the _is_a override.
  *os << be_nl_2
      << "::CORBA::Boolean " << full_skel_name
      << "::_is_a (const char* value)" << be_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "(" << be_idt_nl;

  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("traversal of inhertance ")
                         ACE_TEXT ("graph failed\n")),
                        -1);
    }

  *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
      << "value," << be_nl
      << "\"IDL:omg.org/CORBA/Object:1.0\"" << be_uidt_nl
      << ")";

  if (node->has_mixed_parentage ())
    {
      *os << " ||" << be_uidt_nl
          << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
          << "(char *)value," << be_nl
          << "\"IDL:omg.org/CORBA/AbstractBase:1.0\""
          << be_uidt_nl
          << ")";
    }

  *os << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "const char* " << full_skel_name
      << "::_interface_repository_id (void) const"
      << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return \"" << node->repoID () << "\";" << be_uidt_nl;
  *os << "}";

  // Print out dispatch method.
  this->dispatch_method (node);

  this->this_method (node);

  if (be_global->gen_tie_classes () && !node->tie_skel_gen ())
    {
      // Generate the TIE class.
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_ROOT_TIE_SS);
      ctx.stream (tao_cg->server_template_skeletons ());
      be_visitor_interface_tie_ss visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_ss::")
                             ACE_TEXT ("visit_interface - ")
                             ACE_TEXT ("codegen for TIE ")
                             ACE_TEXT ("class failed\n")),
                            -1);
        }

      // AMH generation reuses this visit_interface(), hence the flag.
      node->tie_skel_gen (true);
    }

  return 0;
}

int
be_visitor_interface_ss::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ss::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ss::gen_abstract_ops_helper (
  be_interface *node,
  be_interface *base,
  TAO_OutStream *os)
{
  if (!base->is_abstract ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_ROOT_SS);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_ss::")
                             ACE_TEXT ("gen_abstract_ops_helper - ")
                             ACE_TEXT ("bad node in this scope\n")),
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
      if (AST_Decl::NT_op == nt)
        {
          be_operation *op = be_operation::narrow_from_decl (d);
          UTL_ScopedName *old_name =
            (UTL_ScopedName *) op->name ()->copy ();
          op->set_name (new_name);
          op->set_defined_in (node);
          op->is_abstract (node->is_abstract ());

          be_visitor_operation_ss op_visitor (&ctx);
          op_visitor.visit_operation (op);

          op->set_name (old_name);
          op->set_defined_in (base);
          op->is_abstract (base->is_abstract ());
        }
      else if (AST_Decl::NT_attr == nt)
        {
          AST_Attribute *attr =
            AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 0,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);
          new_attr.set_name (new_name);

          UTL_ExceptList *get_exceptions =
            attr->get_get_exceptions ();

          if (0 != get_exceptions)
            {
              new_attr.be_add_get_exceptions (get_exceptions->copy ());
            }

          UTL_ExceptList *set_exceptions =
            attr->get_set_exceptions ();

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

void
be_visitor_interface_ss::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  // The _this () operation.
  *os << node->full_name () << " *" << be_nl
      << node->full_skel_name ()
      << "::_this (void)" << be_nl
      << "{" << be_idt_nl
      << "TAO_Stub *stub = this->_create_stub ();"
      << be_nl_2
      << "TAO_Stub_Auto_Ptr safe_stub (stub);" << be_nl;

  /* Coverity whines about an unused return value from _nil() when
     initializing tmp.  Just use zero instead. */
  *os << "::CORBA::Object_ptr tmp = CORBA::Object_ptr ();"
      << be_nl_2;

  *os << "::CORBA::Boolean const _tao_opt_colloc ="
      << be_idt_nl
      << "stub->servant_orb_var ()->orb_core ()->"
      << "optimize_collocation_objects ();" << be_uidt_nl << be_nl;

  *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "tmp," << be_nl
      << "::CORBA::Object (stub, ";

  *os << "_tao_opt_colloc";

  *os << ", this)," << be_nl
      << "0);" << be_uidt << be_uidt_nl << be_nl;

  *os << "::CORBA::Object_var obj = tmp;" << be_nl
      << "(void) safe_stub.release ();" << be_nl_2
      << "typedef ::" << node->name () << " STUB_SCOPED_NAME;"
      << be_nl
      << "return" << be_idt_nl
      << "TAO::Narrow_Utils<STUB_SCOPED_NAME>::unchecked_narrow ("
      << be_idt << be_idt_nl
      << "obj.in ());";

  *os << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "}";
}

void
be_visitor_interface_ss::dispatch_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  *os << "void " << node->full_skel_name ()
      << "::_dispatch (" << be_idt_nl
      << "TAO_ServerRequest & req," << be_nl
      << "TAO::Portable_Server::Servant_Upcall* servant_upcall)"
      << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << "this->synchronous_upcall_dispatch (req, servant_upcall, this);"
      << be_uidt_nl;
  *os << "}";
}

int
be_visitor_interface_ss::generate_amh_classes (be_interface *node)
{
  // We have to check for any abstract ancestor until AMH is integrated
  // with abstract interfaces. If the node itself is abstract, this
  // visitor would not be created.
  if (be_global->gen_amh_classes () && !node->has_mixed_parentage ())
    {
      be_visitor_amh_interface_ss amh_intf (this->ctx_);
      return amh_intf.visit_interface (node);
    }

  return 0;
}

int
be_visitor_interface_ss::generate_proxy_classes (be_interface *node)
{
  be_visitor_context ctx = *this->ctx_;

  if (be_global->gen_direct_collocation ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS);
      be_visitor_interface_direct_proxy_impl_ss idpi_visitor (&ctx);

      if (node->accept (&idpi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_cs::")
                             ACE_TEXT ("generate_proxy_classes - ")
                             ACE_TEXT ("codegen for Base Proxy ")
                             ACE_TEXT ("Broker class failed\n")),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_interface_ss::generate_copy_ctor (be_interface *node,
                                             TAO_OutStream *os)
{
  return node->traverse_inheritance_graph (be_interface::copy_ctor_helper,
                                           os);
}

ACE_CString
be_visitor_interface_ss::generate_flat_name (be_interface *node)
{
  return ACE_CString (node->flat_name ());
}

ACE_CString
be_visitor_interface_ss::generate_local_name (be_interface *node)
{
  return ACE_CString (node->local_name ());
}

ACE_CString
be_visitor_interface_ss::generate_full_skel_name  (be_interface *node)
{
  return ACE_CString (node->full_skel_name ());
}
