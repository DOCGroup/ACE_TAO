/**
 *  @file   ast_visitor_tmpl_module_inst.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "ast_visitor_tmpl_module_inst.h"
#include "ast_visitor_reifying.h"
#include "ast_visitor_context.h"
#include "ast_generator.h"

#include "ast_root.h"
#include "ast_module.h"
#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_template_module_ref.h"
#include "ast_eventtype.h"
#include "ast_interface.h"
#include "ast_home.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_mirror_port.h"
#include "ast_connector.h"
#include "ast_attribute.h"
#include "ast_operation.h"
#include "ast_argument.h"
#include "ast_typedef.h"
#include "ast_constant.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_finder.h"
#include "ast_param_holder.h"
#include "ast_expression.h"

#include "ast_param_holder.h"

#include "utl_namelist.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "utl_labellist.h"

#include "fe_obv_header.h"
#include "fe_home_header.h"

#include "nr_extern.h"

ast_visitor_tmpl_module_inst::ast_visitor_tmpl_module_inst (
      ast_visitor_context *ctx,
      bool ref_only)
  : ast_visitor (),
    ctx_ (ctx),
    for_eventtype_ (false),
    for_finder_ (false),
    ref_only_ (ref_only)
{
}

ast_visitor_tmpl_module_inst::~ast_visitor_tmpl_module_inst (void)
{
}

int ast_visitor_tmpl_module_inst::visit_decl (AST_Decl *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                             ACE_TEXT ("visit_scope - ")
                             ACE_TEXT ("bad node in this scope\n")),
                            -1);
        }

      // Send the visitor.
      if (d == 0 || d->ast_accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                             ACE_TEXT ("visit_scope - ")
                             ACE_TEXT ("codegen for scope failed\n")),
                            -1);
        }
    }

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_type (AST_Type *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_valuebox (AST_ValueBox *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_component (AST_Component *node)
{
  UTL_ScopedName *base_name = 0;
  AST_Decl *parent =
    this->reify_type (node->base_component ());

  if (parent != 0)
    {
      base_name = parent->name ();
    }

  UTL_NameList *supports_names =
    this->create_name_list (node->supports (),
                            node->n_supports ());

  Identifier *node_id = 0;
  ACE_NEW_RETURN (node_id,
                  Identifier (node->local_name ()->get_string ()),
                  -1);

  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);

  FE_ComponentHeader header (local_name,
                             base_name,
                             supports_names,
                             false);

  AST_Component *added_comp =
    idl_global->gen ()->create_component (header.name (),
                                          header.base_component (),
                                          header.supports (),
                                          header.n_supports (),
                                          header.supports_flat (),
                                          header.n_supports_flat ());

  if (supports_names != 0)
    {
      supports_names->destroy ();
      delete supports_names;
      supports_names = 0;
    }

  idl_global->scopes ().top ()->add_to_scope (added_comp);

  // Update the scope management.
  idl_global->scopes ().push (added_comp);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_eventtype_fwd (AST_EventTypeFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_component_fwd (AST_ComponentFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_template_module_ref (
  AST_Template_Module_Ref *)
{
  // A subclass of this visitor is launched when an 'alias'
  // construct is parsed, which creates the necessary implied IDL
  // in the template module. This visitor skips the alias and
  // processes the implied IDL. It's a bit inefficient to have
  // the implied IDL in both the template module and its
  // instantiations, but otherwise the lookup issues are
  // extremely complicated. This approach allows
  // lookup_by_name_local() to just skip over the alias and
  // match the module of the same name occurring later in the
  // template module scope. From that vantage point, the
  // reifying visitor uses its established mechanism to jump
  // to the module of the same name in the instantiated template
  // module scope (see ast_visitor_reifying::check_and_store()
  // and ast_visitor_reifying::template_module_rel_name()).
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_porttype (AST_PortType *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_PortType *added_porttype =
    idl_global->gen ()->create_porttype (&sn);

  idl_global->scopes ().top ()->add_to_scope (added_porttype);

  idl_global->scopes ().push (added_porttype);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_porttype - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_provides (AST_Provides *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Type *p_type =
    AST_Type::narrow_from_decl (
      this->reify_type (node->provides_type ()));

  AST_Provides *added_provides =
    idl_global->gen ()->create_provides (&sn,
                                         p_type);

  idl_global->scopes ().top ()->add_to_scope (added_provides);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_uses (AST_Uses *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Type *u_type =
    AST_Type::narrow_from_decl (
      this->reify_type (node->uses_type ()));

  AST_Uses *added_uses =
    idl_global->gen ()->create_uses (&sn,
                                     u_type,
                                     node->is_multiple ());

  idl_global->scopes ().top ()->add_to_scope (added_uses);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_publishes (AST_Publishes *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Type *p_type =
    AST_Type::narrow_from_decl (
      this->reify_type (node->publishes_type ()));

  AST_Publishes *added_publishes =
    idl_global->gen ()->create_publishes (&sn,
                                          p_type);

  idl_global->scopes ().top ()->add_to_scope (added_publishes);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_emits (AST_Emits *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Type *e_type =
    AST_Type::narrow_from_decl (
      this->reify_type (node->emits_type ()));

  AST_Emits *added_emits =
    idl_global->gen ()->create_emits (&sn, e_type);

  idl_global->scopes ().top ()->add_to_scope (added_emits);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_consumes (AST_Consumes *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Type *c_type =
    AST_Type::narrow_from_decl (
      this->reify_type (node->consumes_type ()));

  AST_Consumes *added_consumes =
    idl_global->gen ()->create_consumes (&sn, c_type);

  idl_global->scopes ().top ()->add_to_scope (added_consumes);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_extended_port (
  AST_Extended_Port *node)
{
  AST_PortType *pt =
    AST_PortType::narrow_from_decl (
      this->reify_type (node->port_type ()));

  UTL_ScopedName sn (node->local_name (), 0);

  AST_Extended_Port *added_ep =
    idl_global->gen ()->create_extended_port (&sn, pt);

  idl_global->scopes ().top ()->add_to_scope (added_ep);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_mirror_port (
  AST_Mirror_Port *node)
{
  AST_PortType *pt =
    AST_PortType::narrow_from_decl (
      this->reify_type (node->port_type ()));

  UTL_ScopedName sn (node->local_name (), 0);

  AST_Mirror_Port *added_mp =
    idl_global->gen ()->create_mirror_port (&sn, pt);

  idl_global->scopes ().top ()->add_to_scope (added_mp);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_connector (AST_Connector *node)
{
  AST_Connector *parent =
    AST_Connector::narrow_from_decl (
      this->reify_type (node->base_connector ()));

  UTL_ScopedName sn (node->local_name (), 0);

  AST_Connector *added_connector =
    idl_global->gen ()->create_connector (&sn, parent);

  idl_global->scopes ().top ()->add_to_scope (added_connector);

  idl_global->scopes ().push (added_connector);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_home (AST_Home *node)
{
  UTL_ScopedName *base_name = 0;
  AST_Decl *parent =
    this->reify_type (node->base_home ());

  if (parent != 0)
    {
      base_name = parent->name ();
    }

  UTL_NameList *supports_names =
    this->create_name_list (node->supports (),
                            node->n_supports ());

  UTL_ScopedName *managed_comp_name = 0;

  AST_Component *managed_comp =
    AST_Component::narrow_from_decl (
      this->reify_type (node->managed_component ()));

  if (managed_comp != 0)
    {
      managed_comp_name = managed_comp->name ();
    }

  UTL_ScopedName *p_key_name = 0;

  AST_ValueType *p_key =
    AST_ValueType::narrow_from_decl (
      this->reify_type (node->primary_key ()));

  if (p_key != 0)
    {
      p_key_name = p_key->name ();
    }

  Identifier *node_id = 0;
  ACE_NEW_RETURN (node_id,
                  Identifier (node->local_name ()->get_string ()),
                  -1);

  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);

  FE_HomeHeader header (local_name,
                        base_name,
                        supports_names,
                        managed_comp_name,
                        p_key_name);

  AST_Home *added_home =
    idl_global->gen ()->create_home (header.name (),
                                     header.base_home (),
                                     header.managed_component (),
                                     header.primary_key (),
                                     header.supports (),
                                     header.n_supports (),
                                     header.supports_flat (),
                                     header.n_supports_flat ());

  if (supports_names != 0)
    {
      supports_names->destroy ();
      delete supports_names;
      supports_names = 0;
    }

  idl_global->scopes ().top ()->add_to_scope (added_home);

  // Update the scope management.
  idl_global->scopes ().push (added_home);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_home - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_exception (AST_Exception *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_expression (AST_Expression *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_enum (AST_Enum *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Enum *added_enum =
    idl_global->gen ()->create_enum (&sn,
                                     node->is_local (),
                                     node->is_abstract ());

  idl_global->scopes ().top ()->add_to_scope (added_enum);

  idl_global->scopes ().push (added_enum);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_enum - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union (AST_Union *node)
{
  UTL_ScopedName sn (node->local_name  (), 0);

  AST_Union *added_union =
    idl_global->gen ()->create_union (node->disc_type (),
                                      &sn,
                                      node->is_local (),
                                      node->is_abstract ());

  idl_global->scopes ().top ()->add_to_scope (added_union);

  // Update our scope stack.
  idl_global->scopes ().push (added_union);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_union - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  added_union->member_count ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union_fwd (AST_UnionFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union_branch (AST_UnionBranch *node)
{
  AST_Type *ft =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));

  UTL_ScopedName sn (node->local_name (), 0);

  // The union branch owns its label list so we have to copy it.
  UTL_LabelList *ll = node->labels ()->copy ();

  AST_UnionBranch *added_branch =
    idl_global->gen ()->create_union_branch (ll, ft, &sn);

  // fe_add_union_branch() does necessary things besides calling
  // add_to_scope() so we need to reuse it.

  AST_Union *u =
    AST_Union::narrow_from_scope (idl_global->scopes ().top ());

  u->fe_add_union_branch (added_branch);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_enum_val (AST_EnumVal *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_EnumVal *added_enum_val =
    idl_global->gen ()->create_enum_val (
      node->constant_value ()->ev ()->u.ulval,
      &sn);

  idl_global->scopes ().top ()->add_to_scope (added_enum_val);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_array (AST_Array *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_sequence (AST_Sequence *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_string (AST_String *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_param_holder (AST_Param_Holder *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_root (AST_Root *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_native (AST_Native *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_module (AST_Module *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Module *added_module =
    idl_global->gen ()->create_module (idl_global->scopes ().top (),
                                       &sn);

  AST_Module *m =
    AST_Module::narrow_from_scope (idl_global->scopes ().top ());

  m->fe_add_module (added_module);

  idl_global->scopes ().push (added_module);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }

  // Restore scope stack.
  idl_global->scopes ().pop ();

  return 0;
}

// Not called from ast_template_module::accept() but from
// visit_template_module_inst() in this visitor.
int
ast_visitor_tmpl_module_inst::visit_template_module (
  AST_Template_Module *node)
{
  this->ctx_->template_params (node->template_params ());

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_template_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_template_module_inst (
  AST_Template_Module_Inst *node)
{
  this->ctx_->template_args (node->template_args ());

  AST_Module *instance =
    idl_global->gen ()->create_module (idl_global->scopes ().top (),
                                       node->name ());

  instance->from_inst (node);

  // Add the new module to the scope containing the template
  // module instantiation.
  AST_Module *m =
    AST_Module::narrow_from_scope (idl_global->scopes ().top ());

  m->fe_add_module (instance);

  // Update our scope management.
  idl_global->scopes ().push (instance);

  AST_Template_Module *tm = node->ref ();

  if (this->visit_template_module (tm) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_template_module_inst - ")
                         ACE_TEXT ("visit_template_module failed\n")),
                        -1);
    }

  // By checking this member for a non-zero value, we can tell if
  // we are processing an instantiated template module or not. We
  // need to know this, for example while visiting an IDL module,
  // to decide whether to create an implied IDL module or just
  // visit its scope.
  this->ctx_->template_args (0);

  // Restore the scope stack.
  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_eventtype (AST_EventType *node)
{
  this->for_eventtype_ = true;
  return this->visit_valuetype (node);
}

int
ast_visitor_tmpl_module_inst::visit_valuetype (AST_ValueType *node)
{
  UTL_NameList *parent_names =
    this->create_name_list (node->inherits (),
                            node->n_inherits ());

  UTL_NameList *supports_names =
    this->create_name_list (node->supports (),
                            node->n_supports ());

  Identifier *node_id = 0;
  ACE_NEW_RETURN (node_id,
                  Identifier (node->local_name ()->get_string ()),
                  -1);

  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);

  AST_ValueType *added_vtype = 0;

  FE_OBVHeader header (local_name,
                       parent_names,
                       supports_names,
                       (parent_names != 0
                          ? parent_names->truncatable ()
                          : false),
                       this->for_eventtype_);

  if (this->for_eventtype_)
    {
      added_vtype =
        idl_global->gen ()->create_eventtype (header.name (),
                                              header.inherits (),
                                              header.n_inherits (),
                                              header.inherits_concrete (),
                                              header.inherits_flat (),
                                              header.n_inherits_flat (),
                                              header.supports (),
                                              header.n_supports (),
                                              header.supports_concrete (),
                                              false,
                                              header.truncatable (),
                                              false);
    }
  else
    {
      added_vtype =
        idl_global->gen ()->create_valuetype (header.name (),
                                              header.inherits (),
                                              header.n_inherits (),
                                              header.inherits_concrete (),
                                              header.inherits_flat (),
                                              header.n_inherits_flat (),
                                              header.supports (),
                                              header.n_supports (),
                                              header.supports_concrete (),
                                              false,
                                              header.truncatable (),
                                              false);
    }

  if (parent_names != 0)
    {
      parent_names->destroy ();
      delete parent_names;
      parent_names = 0;
    }

  if (supports_names != 0)
    {
      supports_names->destroy ();
      delete supports_names;
      supports_names = 0;
    }

  idl_global->scopes ().top ()->add_to_scope (added_vtype);

  // Update the scope management.
  idl_global->scopes ().push (added_vtype);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_valuetype - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  // Reset the flag.
  this->for_eventtype_ = false;

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_interface (AST_Interface *node)
{
  UTL_NameList *parent_names =
    this->create_name_list (node->inherits (),
                            node->n_inherits ());

  Identifier *node_id = 0;
  ACE_NEW_RETURN (node_id,
                  Identifier (node->local_name ()->get_string ()),
                  -1);

  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);

  FE_InterfaceHeader header (local_name,
                             parent_names,
                             node->is_local (),
                             node->is_abstract (),
                             true);

  AST_Interface *added_iface =
    idl_global->gen ()->create_interface (header.name (),
                                          header.inherits (),
                                          header.n_inherits (),
                                          header.inherits_flat (),
                                          header.n_inherits_flat (),
                                          header.is_local (),
                                          header.is_abstract ());

  if (parent_names != 0)
    {
      parent_names->destroy ();
      delete parent_names;
      parent_names = 0;
    }

  idl_global->scopes ().top ()->add_to_scope (added_iface);

  // If this interface has both abstract and concrete parents,
  // extra code needs to be generated for it, such as overrides
  // of _is_nil() and release().
  added_iface->analyze_parentage ();

  // Update the scope stack.
  idl_global->scopes ().push (added_iface);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_attribute (AST_Attribute *node)
{
  AST_Type *ft =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));

  AST_Attribute *added_attr =
    idl_global->gen ()->create_attribute (node->readonly (),
                                          ft,
                                          node->name (),
                                          node->is_local (),
                                          node->is_abstract ());

  idl_global->scopes ().top ()->add_to_scope (added_attr);

  // These will work even if the exception lists are null.

  UTL_ExceptList *old_ex = node->get_get_exceptions ();
  UTL_ExceptList *new_ex = this->reify_exception_list (old_ex);
  added_attr->be_add_get_exceptions (new_ex);

  old_ex = node->get_set_exceptions ();
  new_ex = this->reify_exception_list (old_ex);
  added_attr->be_add_set_exceptions (new_ex);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_operation (AST_Operation *node)
{
  AST_Type *rt =
    AST_Type::narrow_from_decl (
      this->reify_type (node->return_type ()));

  Identifier id (node->local_name ()->get_string ());
  UTL_ScopedName sn (&id, 0);

  AST_Operation *added_op =
    idl_global->gen ()->create_operation (rt,
                                          node->flags (),
                                          &sn,
                                          node->is_local (),
                                          node->is_abstract ());

  idl_global->scopes ().top ()->add_to_scope (added_op);

  // Update the scope stack.
  idl_global->scopes ().push (added_op);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  UTL_ExceptList *new_ex =
    this->reify_exception_list (node->exceptions ());

  added_op->be_add_exceptions (new_ex);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_argument (AST_Argument *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));

  if (t == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("reify_type failed\n")),
                        -1);
    }

  AST_Argument *added_arg =
    idl_global->gen ()->create_argument (node->direction (),
                                         t,
                                         node->name ());

  idl_global->scopes ().top ()->add_to_scope (added_arg);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_typedef (AST_Typedef *node)
{
  AST_Type *bt =
    AST_Type::narrow_from_decl (
      this->reify_type (node->base_type ()));

  UTL_ScopedName sn (node->local_name (), 0);

  AST_Typedef *added_td =
    idl_global->gen ()->create_typedef (bt,
                                        &sn,
                                        false,
                                        false);

  //AST_Decl *d = ScopeAsDecl (idl_global->scopes ().top ());

  idl_global->scopes ().top ()->add_to_scope (added_td);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_constant (AST_Constant *node)
{
  AST_Param_Holder *ph =
    node->constant_value ()->param_holder ();

  AST_Expression *v = 0;
  AST_Expression::ExprType et = node->et ();

  if (ph != 0)
    {
      ast_visitor_reifying rv (this->ctx_);

      if (rv.visit_param_holder (ph) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                             ACE_TEXT ("visit_constant - ")
                             ACE_TEXT ("reification of param ")
                             ACE_TEXT ("holder failed\n")),
                            -1);
        }

      AST_Constant *c =
        AST_Constant::narrow_from_decl (rv.reified_node ());

      /// We don't use the reified node's ExprType here, since
      /// it was created from a template arg that (for const
      /// type template args) was a literal. The arg name was
      /// matched with a template param list to check correctness,
      /// but not the type. Thus an integer literal, for example,
      /// will be a 64-bit type usually, but what we want is
      /// the type of the constant we are visiting. When the
      /// new expression and constant are created below, things
      /// will be coerced to the ExprType passed in, and if
      /// there is a mismatch, we'll get an error at that point.
      v = c->constant_value ();
    }
  else
    {
      v = node->constant_value ();
    }

  AST_Expression *new_v =
    idl_global->gen ()->create_expr (v, et);

  AST_Constant *added_const =
    idl_global->gen ()->create_constant (et,
                                         new_v,
                                         node->name ());

  idl_global->scopes ().top ()->add_to_scope (added_const);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_structure (AST_Structure *node)
{
  UTL_ScopedName sn (node->name ()->last_component (), 0);

  AST_Structure *added_struct =
    idl_global->gen ()->create_structure (&sn,
                                          node->is_local (),
                                          node->is_abstract ());

  idl_global->scopes ().top ()->add_to_scope (added_struct);

  // Update our scope stack.
  idl_global->scopes ().push (added_struct);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_field (AST_Field *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));

  AST_Field *added_field =
    idl_global->gen ()->create_field (t,
                                      node->name (),
                                      node->visibility ());

  idl_global->scopes ().top ()->add_to_scope (added_field);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_factory (AST_Factory *node)
{
  Identifier id (node->local_name ()->get_string ());
  UTL_ScopedName sn (&id, 0);

  AST_Factory *added_factory = 0;

  if (this->for_finder_)
    {
      added_factory =
        idl_global->gen ()->create_finder (&sn);
    }
  else
    {
      added_factory =
        idl_global->gen ()->create_factory (&sn);
    }

  idl_global->scopes ().top ()->add_to_scope (added_factory);

  // Update the scope stack.
  idl_global->scopes ().push (added_factory);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_factory - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  // Through with this scope.
  idl_global->scopes ().pop ();

  UTL_ExceptList *reified_exceps =
    this->reify_exception_list (node->exceptions ());

  added_factory->be_add_exceptions (reified_exceps);

  // In case it was set for this call.
  this->for_finder_ = false;

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_finder (AST_Finder *node)
{
  this->for_finder_ = true;
  return this->visit_factory (node);
}

AST_Decl *
ast_visitor_tmpl_module_inst::reify_type (AST_Decl *d)
{
  if (d == 0)
    {
      return 0;
    }

  if (this->ref_only_)
    {
      // If d is a param holder, the lookup will return a heap
      // allocation, which we need since this node's destination
      // owns param holders.
      if (d->node_type () == AST_Decl::NT_param_holder)
        {
          return
            idl_global->scopes ().top ()->lookup_by_name (
              d->name (),
              true);
        }
      else
        {
          return d;
        }
    }

  ast_visitor_reifying rv (this->ctx_);

  if (d->ast_accept (&rv) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                  ACE_TEXT ("reify_type() - reifying ")
                  ACE_TEXT ("visitor failed on %C\n"),
                  d->full_name ()));

      return 0;
    }

  return rv.reified_node ();
}

UTL_ExceptList *
ast_visitor_tmpl_module_inst::reify_exception_list (
  UTL_ExceptList *orig)
{
  if (orig == 0)
    {
      return 0;
    }

  UTL_ExceptList *retval = 0;

  for (UTL_ExceptlistActiveIterator i (orig);
       !i.is_done ();
       i.next ())
    {
      AST_Type *ex =
        AST_Type::narrow_from_decl (this->reify_type (i.item ()));

      UTL_ExceptList *ex_list = 0;
      ACE_NEW_RETURN (ex_list,
                      UTL_ExceptList (ex, 0),
                      0);

      if (retval == 0)
        {
          retval = ex_list;
        }
      else
        {
          retval->nconc (ex_list);
        }
    }

  return retval;
}

UTL_NameList *
ast_visitor_tmpl_module_inst::create_name_list (AST_Type **list,
                                               long length)
{
  UTL_NameList *retval = 0;

  for (long i = 0; i < length; ++i)
    {
      AST_Type *item =
        AST_Type::narrow_from_decl (this->reify_type (list[i]));

      // We copy each name added so we can call destroy() on the
      // list, which disposes of the contents as well as the
      // nested tail pointers.
      UTL_NameList *name_item = 0;
      ACE_NEW_RETURN (name_item,
                      UTL_NameList (item->name ()->copy (), 0),
                      0);

      if (retval == 0)
        {
          retval = name_item;
        }
      else
        {
          retval->nconc (name_item);
        }
    }

  return retval;
}

