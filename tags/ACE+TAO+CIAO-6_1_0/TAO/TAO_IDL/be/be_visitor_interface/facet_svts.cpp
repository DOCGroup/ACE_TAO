
//=============================================================================
/**
 *  @file    facet_svts.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a facet servant class in the
 *  servant source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "interface.h"

be_visitor_facet_svts::be_visitor_facet_svts (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_facet_svts::~be_visitor_facet_svts (void)
{
}

int
be_visitor_facet_svts::visit_interface (be_interface *node)
{
  if (node->imported () ||
      node->svnt_src_facet_gen () ||
      idl_global->ami_connector_seen_ ||
      node->original_interface () ||
      node->is_local ())
    {
      return 0;
    }

  const char *lname = node->local_name ();

  be_decl *scope =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  ACE_CString sname_str (scope->full_name ());

  const char *global = (sname_str == "" ? "" : "::");

  ACE_CString suffix (scope->flat_name ());

  if (suffix != "")
    {
      suffix = ACE_CString ("_") + suffix;
    }

  os_ << be_nl_2
      << "namespace CIAO_FACET" << suffix.c_str () << be_nl
      << "{" << be_idt_nl;

  os_ << "template <typename BASE, typename EXEC, typename CONTEXT>" << be_nl
      << lname << "_Servant_T<BASE, EXEC, CONTEXT>::"
      << lname << "_Servant_T (" << be_idt << be_idt_nl
      << "typename EXEC::_ptr_type executor," << be_nl
      << "::Components::CCMContext_ptr ctx)" << be_uidt_nl
      << ": " << global << "CIAO::Facet_Servant_Base_T<BASE, EXEC, "
      << "CONTEXT> (executor, ctx)"
      << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2 << "template <typename BASE, typename EXEC, typename CONTEXT>" << be_nl
      << lname << "_Servant_T<BASE, EXEC, CONTEXT>::~"
      << lname << "_Servant_T (void)" << be_nl
      << "{" << be_nl
      << "}";

  bool is_intf = node->node_type () == AST_Decl::NT_interface;

  if (is_intf)
    {
      be_interface *op_scope =
        be_interface::narrow_from_decl (node);

      os_ << be_nl_2
          << "// All facet operations and attributes.";

      /// The overload of traverse_inheritance_graph() used here
      /// doesn't automatically prime the queues.
      op_scope->get_insert_queue ().reset ();
      op_scope->get_del_queue ().reset ();
      op_scope->get_insert_queue ().enqueue_tail (op_scope);

      be_facet_op_attr_defn_helper helper (op_scope);

      int status =
        op_scope->traverse_inheritance_graph (helper,
                                              &os_,
                                              false,
                                              false);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_provides::")
                             ACE_TEXT ("gen_facet_svnt_defn - ")
                             ACE_TEXT ("traverse_inheritance_graph() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }
    }

  os_ << be_uidt_nl << "}";

  node->svnt_src_facet_gen (true);
  return 0;
}

// ********************************************

be_facet_op_attr_defn_helper::be_facet_op_attr_defn_helper (
      be_interface *op_scope)
  : op_scope_ (op_scope)
{
}

int
be_facet_op_attr_defn_helper::emit (be_interface * /* derived_interface */,
                                    TAO_OutStream *os,
                                    be_interface *base_interface)
{
  AST_Decl::NodeType nt = base_interface->node_type ();

  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      return 0;
    }

  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_ROOT_SVTS);

  for (UTL_ScopeActiveIterator i (base_interface, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Decl::NodeType nt = d->node_type ();

      switch (nt)
        {
          case AST_Decl::NT_op:
            {
              be_operation *op =
                be_operation::narrow_from_decl (d);

              /// If AMI implied IDL was generated for the
              /// original interface, we don't want those
              /// extra operations in the facet servant.
              if (op->is_sendc_ami ())
                {
                  continue;
                }

              be_visitor_operation_svs v (&ctx);
              v.scope (op_scope_);

              if (v.visit_operation (op) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_facet_op_attr_defn_helper")
                                     ACE_TEXT ("::emit - ")
                                     ACE_TEXT ("visit_operation() failed\n")),
                                    -1);
                }

              break;
            }
          case AST_Decl::NT_attr:
            {
              be_attribute *attr =
                be_attribute::narrow_from_decl (d);

              be_visitor_attribute v (&ctx);
              v.op_scope (op_scope_);

              if (v.visit_attribute (attr) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_facet_op_attr_defn_helper")
                                     ACE_TEXT ("::emit - ")
                                     ACE_TEXT ("visit_attribute() failed\n")),
                                    -1);
                }

              break;
            }
          default:
            continue;
        }
    }

  return 0;
}

// ********************************************
