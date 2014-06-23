
//=============================================================================
/**
 *  @file    facet_svth.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a facet servant class in the
 *  servant header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "interface.h"

be_visitor_facet_svth::be_visitor_facet_svth (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_facet_svth::~be_visitor_facet_svth (void)
{
}

int
be_visitor_facet_svth::visit_interface (be_interface *node)
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
  ACE_CString suffix (scope->flat_name ());

  if (suffix != "")
    {
      suffix = ACE_CString ("_") + suffix;
    }

  os_ << be_nl_2
      << "namespace CIAO_FACET" << suffix.c_str () <<  be_nl
      << "{" << be_idt_nl;

  bool is_intf = node->node_type () == AST_Decl::NT_interface;

  os_ << "template <typename BASE, typename EXEC, typename CONTEXT>" << be_nl
      << "class " << lname << "_Servant_T" << be_idt_nl
      << ": public ::CIAO::Facet_Servant_Base_T<BASE, EXEC, "
      << "CONTEXT>" << be_uidt_nl << "{" << be_nl
      << "public:" << be_idt_nl;

  AST_Decl *s = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (s->full_name ());

  os_ << lname << "_Servant_T (" << be_idt_nl
      << "typename EXEC::_ptr_type executor," << be_nl
      << "::Components::CCMContext_ptr ctx);" << be_uidt_nl << be_nl;

  os_ << "virtual ~" << lname << "_Servant_T (void);";

  if (is_intf)
    {
      be_interface *intf =
        be_interface::narrow_from_decl (node);

      be_global->in_facet_servant (true);

      int status =
        intf->traverse_inheritance_graph (
          be_interface::op_attr_decl_helper,
          &os_);

      be_global->in_facet_servant (false);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_provides::gen_facet - ")
                             ACE_TEXT ("traverse_inheritance_graph()")
                             ACE_TEXT (" failed\n")),
                            -1);
        }
    }

  os_ << be_uidt_nl << "};" << be_nl << be_uidt_nl;

  os_ << "}";

  node->svnt_hdr_facet_gen (true);
  return 0;
}
