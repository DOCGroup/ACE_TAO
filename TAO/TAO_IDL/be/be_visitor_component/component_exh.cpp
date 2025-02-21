
//=============================================================================
/**
 *  @file    component_exh.cpp
 *
 *  Visitor generating code for Components in the exec impl header.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "component.h"

be_visitor_component_exh::be_visitor_component_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->exec_export_macro ();
}

be_visitor_component_exh::~be_visitor_component_exh ()
{
}

int
be_visitor_component_exh::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->node_ = node;

  /// Use 'CIAO_' + component's flat name.
  os_ << be_nl_2
      << "/// Namespace for implementation of " << node->full_name () << " component" << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  be_visitor_facet_exh facet_visitor (this->ctx_);
  facet_visitor.node (node);

  if (facet_visitor.visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("facet visitor failed\n")),
                        -1);
    }

  be_visitor_executor_exh exec_visitor (this->ctx_);

  if (exec_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("exec visitor failed\n")),
                        -1);
    }

  /// If this list has members, then we can assume that the
  /// artifacts from -GC exist.
  for (ACE_Unbounded_Queue<char *>::CONST_ITERATOR i (
         idl_global->ciao_ami_recep_names ());
       ! i.done ();
       i.advance ())
    {
      char **item = nullptr;
      i.next (item);

      UTL_ScopedName *sn =
        FE_Utils::string_to_scoped_name (*item);

      UTL_Scope *s =
        idl_global->scopes ().top_non_null ();

      AST_Decl *d = s->lookup_by_name (sn, true);

      if (d == nullptr)
        {
          idl_global->err ()->lookup_error (sn);

          sn->destroy ();
          delete sn;
          sn = nullptr;

          continue;
        }

      sn->destroy ();
      delete sn;
      sn = nullptr;

      be_uses *u = dynamic_cast<be_uses*> (d);

      if (u == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_exh")
                             ACE_TEXT ("::visit_component - ")
                             ACE_TEXT ("narrow to AMI receptacle ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }

      be_component *c =
        dynamic_cast<be_component*> (
          ScopeAsDecl (u->defined_in ()));

      if (c == node)
        {
          be_visitor_context ctx (*this->ctx_);
          be_visitor_component_ami_rh_exh ami_rh_visitor (&ctx);

          if (ami_rh_visitor.visit_uses (u) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_component_exh")
                                 ACE_TEXT ("::visit_component - ")
                                 ACE_TEXT ("AMI reply handler generation ")
                                 ACE_TEXT ("failed\n")),
                                -1);
            }
        }
    }

  this->gen_exec_entrypoint_decl ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

