//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_component_scope.cpp
//
// = DESCRIPTION
//    Abstract visitor providing ancestor scope visitation.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_visitor_component_scope.h"
#include "be_visitor_context.h"

#include "be_mirror_port.h"
#include "be_component.h"
#include "be_provides.h"
#include "be_uses.h"

#include "be_extern.h"

#include "utl_identifier.h"
#include "nr_extern.h"

be_visitor_component_scope::be_visitor_component_scope (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ()),
    swapping_ (be_global->gen_component_swapping ()),
    static_config_ (be_global->gen_ciao_static_config ())
{
  /// All existing CIAO examples set the servant export values in the CIDL
  /// compiler to equal the IDL compiler's skel export values. Below is a
  /// partial effort to decouple them, should be completely decoupled
  /// sometime. See comment in codegen.cpp, line 1173.
  if (export_macro_ == "")
    {
      export_macro_ = be_global->skel_export_macro ();
    }
}

be_visitor_component_scope::~be_visitor_component_scope (
  void)
{
}

int
be_visitor_component_scope::visit_extended_port (
  be_extended_port *node)
{
  
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();
  
  // Store this to prefix to contained provides or uses node name.  
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      this->current_port_name_ = node->local_name ()->get_string ();
    }
    
  return this->visit_porttype_scope (node->port_type ());
}

int
be_visitor_component_scope::visit_mirror_port (
  be_mirror_port *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();
    
  // Store this to prefix to contained provides or uses node name.  
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      this->current_port_name_ = node->local_name ()->get_string ();
    }
    
  return this->visit_porttype_scope_mirror (node->port_type ());
}

int
be_visitor_component_scope::visit_component_scope (
  be_component *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_scope")
                         ACE_TEXT ("::visit_component_scope - ")
                         ACE_TEXT ("visit_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
    
  return this->visit_component_scope (node->base_component ());
}

int
be_visitor_component_scope::visit_porttype_scope (
  be_porttype *node)
{
  return this->visit_scope (node);
}

int
be_visitor_component_scope::visit_porttype_scope_mirror (be_porttype *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      be_decl *d = be_decl::narrow_from_decl (si.item ());
      
      (void) this->pre_process (d);

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            {
              be_provides *p =
                be_provides::narrow_from_decl (d);
                
              be_uses mirror_node (p->name (),
                                   p->provides_type (),
                                   false);
                                   
              if (this->visit_uses (&mirror_node) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_visitor_component_scope")
                                     ACE_TEXT ("::visit_porttype_mirror - ")
                                     ACE_TEXT ("visit_uses() failed\n")),
                                    -1);
                }
                
              mirror_node.destroy ();
              break;
            }
          case AST_Decl::NT_uses:
            {
              be_uses *u =
                be_uses::narrow_from_decl (d);
                
              be_provides mirror_node (u->name (),
                                       u->uses_type ());
                                   
              if (this->visit_provides (&mirror_node) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_visitor_component_scope")
                                     ACE_TEXT ("::visit_porttype_mirror - ")
                                     ACE_TEXT ("visit_provides() failed\n")),
                                    -1);
                }
                
              mirror_node.destroy ();
              break;
            }
          default:
            return d->accept (this);
        }
    }
    
  return 0;
}

int
be_visitor_component_scope::pre_process (be_decl *node)
{
  AST_Decl::NodeType nt = node->node_type ();
  
  if (nt == AST_Decl::NT_provides || nt == AST_Decl::NT_uses)
    {
      AST_Decl *s = ScopeAsDecl (node->defined_in ());
      AST_Decl::NodeType snt = s->node_type ();
      
      if (snt == AST_Decl::NT_porttype)
        {
          ACE_CString new_name = current_port_name_;
          new_name += '_';
          new_name += node->local_name ()->get_string ();
          
          Identifier *i = node->name ()->last_component ();
          i->replace_string (new_name.c_str ());
        }
    }
    
  return 0;
}