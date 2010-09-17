/**
 *  @file   ast_visitor_reifying.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "ast_visitor_reifying.h"
#include "ast_visitor_context.h"
#include "ast_generator.h"

#include "ast_interface.h"
#include "ast_valuetype.h"
#include "ast_valuebox.h"
#include "ast_eventtype.h"
#include "ast_connector.h"
#include "ast_home.h"
#include "ast_exception.h"
#include "ast_typedef.h"
#include "ast_array.h"
#include "ast_sequence.h"
#include "ast_union.h"
#include "ast_enum.h"
#include "ast_predefined_type.h"
#include "ast_string.h"
#include "ast_constant.h"
#include "ast_native.h"
#include "ast_param_holder.h"
#include "ast_template_module.h"

#include "utl_identifier.h"
#include "utl_exprlist.h"
#include "nr_extern.h"

ast_visitor_reifying::ast_visitor_reifying (
      ast_visitor_context *ctx)
  : ast_visitor (),
    ctx_ (ctx),
    reified_node_ (0)
{
}

ast_visitor_reifying::~ast_visitor_reifying (void)
{
}

AST_Decl *
ast_visitor_reifying::reified_node (void) const
{
  return this->reified_node_;
}

int ast_visitor_reifying::visit_decl (AST_Decl *)
{
  return 0;
}

int
ast_visitor_reifying::visit_scope (UTL_Scope *)
{
  return 0;
}

int
ast_visitor_reifying::visit_type (AST_Type *)
{
  return 0;
}

int
ast_visitor_reifying::visit_module (AST_Module *)
{
  return 0;
}

int
ast_visitor_reifying::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int
ast_visitor_reifying::visit_valuebox (AST_ValueBox *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

int
ast_visitor_reifying::visit_eventtype_fwd (AST_EventTypeFwd *)
{
  return 0;
}

int
ast_visitor_reifying::visit_component_fwd (AST_ComponentFwd *)
{
  return 0;
}

int
ast_visitor_reifying::visit_template_module (AST_Template_Module *)
{
  return 0;
}

int
ast_visitor_reifying::visit_template_module_inst (AST_Template_Module_Inst *)
{
  return 0;
}

int
ast_visitor_reifying::visit_template_module_ref (AST_Template_Module_Ref *)
{
  return 0;
}

int
ast_visitor_reifying::visit_porttype (AST_PortType *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_provides (AST_Provides *)
{
  return 0;
}

int
ast_visitor_reifying::visit_uses (AST_Uses *)
{
  return 0;
}

int
ast_visitor_reifying::visit_publishes (AST_Publishes *)
{
  return 0;
}

int
ast_visitor_reifying::visit_emits (AST_Emits *)
{
  return 0;
}

int
ast_visitor_reifying::visit_consumes (AST_Consumes *)
{
  return 0;
}

int
ast_visitor_reifying::visit_extended_port (AST_Extended_Port *)
{
  return 0;
}

int
ast_visitor_reifying::visit_mirror_port (AST_Mirror_Port *)
{
  return 0;
}

int
ast_visitor_reifying::visit_connector (AST_Connector *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_home (AST_Home *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_factory (AST_Factory *)
{
  return 0;
}

int
ast_visitor_reifying::visit_finder (AST_Finder *)
{
  return 0;
}

int
ast_visitor_reifying::visit_structure (AST_Structure *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

int
ast_visitor_reifying::visit_expression (AST_Expression *)
{
  return 0;
}

int
ast_visitor_reifying::visit_enum (AST_Enum *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_operation (AST_Operation *)
{
  return 0;
}

int
ast_visitor_reifying::visit_field (AST_Field *)
{
  return 0;
}

int
ast_visitor_reifying::visit_argument (AST_Argument *)
{
  return 0;
}

int
ast_visitor_reifying::visit_attribute (AST_Attribute *)
{
  return 0;
}

int
ast_visitor_reifying::visit_union (AST_Union *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_union_fwd (AST_UnionFwd *)
{
  return 0;
}

int
ast_visitor_reifying::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int
ast_visitor_reifying::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
ast_visitor_reifying::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int
ast_visitor_reifying::visit_root (AST_Root *)
{
  return 0;
}

int
ast_visitor_reifying::visit_native (AST_Native *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_interface (AST_Interface *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_valuetype (AST_ValueType *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_eventtype (AST_EventType *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_component (AST_Component *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_exception (AST_Exception *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_typedef (AST_Typedef *node)
{
  this->check_and_store (node);
  return 0;
}

int
ast_visitor_reifying::visit_array (AST_Array *node)
{
  AST_Type *bt = node->base_type ();

  if (bt->ast_accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_reifying::")
                         ACE_TEXT ("visit_array - ")
                         ACE_TEXT ("visit of base type failed\n")),
                        -1);
    }

  bt = AST_Type::narrow_from_decl (this->reified_node_);

  AST_Expression **dims = node->dims ();
  AST_Expression *v = 0;
  UTL_ExprList *v_list = 0;

  for (ACE_CDR::ULong i = 0; i < node->n_dims (); ++i)
    {
      AST_Param_Holder *ph = dims[i]->param_holder ();

      if (ph != 0)
        {
          if (this->visit_param_holder (ph) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ast_visitor_reifying::")
                                 ACE_TEXT ("visit_array - ")
                                 ACE_TEXT ("visit_param_holder() ")
                                 ACE_TEXT ("failed\n")),
                                -1);
            }

          AST_Constant *c =
            AST_Constant::narrow_from_decl (this->reified_node_);

          ACE_NEW_RETURN (v,
                          AST_Expression (c->constant_value (),
                                          AST_Expression::EV_ulong),
                          -1);
        }
      else
        {
          ACE_NEW_RETURN (v,
                          AST_Expression (dims[i],
                                          AST_Expression::EV_ulong),
                          -1);
        }

      UTL_ExprList *el = 0;
      ACE_NEW_RETURN (el,
                      UTL_ExprList (v, 0),
                      -1);

      if (v_list == 0)
        {
          v_list = el;
        }
      else
        {
          v_list->nconc (el);
        }
    }

  UTL_ScopedName sn (node->local_name (), 0);

  AST_Array *arr =
    idl_global->gen ()->create_array (&sn,
                                      node->n_dims (),
                                      v_list,
                                      false,
                                      false);

  // No need to add this new node to any scope - it's anonymous
  // and owned by the node that references it.

  v_list->destroy ();
  delete v_list;
  v_list = 0;

  arr->set_base_type (bt);
  this->reified_node_ = arr;

  return 0;
}

int
ast_visitor_reifying::visit_sequence (AST_Sequence *node)
{
  AST_Type *bt = node->base_type ();

  if (bt->ast_accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_reifying::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("visit of base type failed\n")),
                        -1);
    }

  bt = AST_Type::narrow_from_decl (this->reified_node_);

  AST_Expression *v = node->max_size ();
  AST_Param_Holder *ph = v->param_holder ();

  if (ph != 0)
    {
      if (this->visit_param_holder (ph) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_reifying::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("visit_param_holder() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }

      AST_Constant *c =
        AST_Constant::narrow_from_decl (this->reified_node_);

      v = c->constant_value ();
    }

  AST_Expression *bound =
    idl_global->gen ()->create_expr (v,
                                     AST_Expression::EV_ulong);
  Identifier id ("sequence");
  UTL_ScopedName sn (&id, 0);

  this->reified_node_ =
    idl_global->gen ()->create_sequence (bound,
                                         bt,
                                         &sn,
                                         false,
                                         false);

  // No need to add this new node to any scope - it's anonymous
  // and owned by the node that references it.

  return 0;
}

int
ast_visitor_reifying::visit_predefined_type (AST_PredefinedType *node)
{
  this->reified_node_ = node;
  return 0;
}

int
ast_visitor_reifying::visit_string (AST_String *node)
{
  AST_Expression *b = node->max_size ();
  AST_Param_Holder *ph = b->param_holder ();

  if (ph != 0)
    {
      if (this->visit_param_holder (ph) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_reifying::")
                             ACE_TEXT ("visit_string - ")
                             ACE_TEXT ("visit_param_holder() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }

      AST_Constant *c =
        AST_Constant::narrow_from_decl (this->reified_node_);

      b = c->constant_value ();
    }
  else if (b->ev ()->u.ulval == 0)
    {
      this->reified_node_ = node;
      return 0;
    }

  AST_Expression *bound = 0;
  ACE_NEW_RETURN (bound,
                  AST_Expression (b,
                                  AST_Expression::EV_ulong),
                  -1);

  Identifier id ("string");
  UTL_ScopedName sn (&id, 0);

  ACE_NEW_RETURN (this->reified_node_,
                  AST_String (AST_Decl::NT_string,
                              &sn,
                              bound,
                              node->width ()),
                  -1);

  return 0;
}

int
ast_visitor_reifying::visit_constant (AST_Constant *node)
{
  // AFAICT, this is called only on the member constant of a param
  // holder, in which case nothing further is needed.
  this->reified_node_ = node;
  return 0;
}

int
ast_visitor_reifying::visit_param_holder (AST_Param_Holder *node)
{
  size_t i = 0;
  FE_Utils::T_ARGLIST *t_args =
    this->ctx_->template_args ();

  for (FE_Utils::T_PARAMLIST_INFO::ITERATOR iter (
         *this->ctx_->template_params ());
       !iter.done ();
       iter.advance (), ++i)
    {
      FE_Utils::T_Param_Info *item = 0;
      iter.next (item);

      if (item->name_ == node->info ()->name_)
        {
          AST_Decl **ret_ptr = 0;

          if (t_args->get (ret_ptr, i) == 0)
            {
              AST_Decl *candidate = *ret_ptr;

              return candidate->ast_accept (this);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ast_visitor_reifying::")
                                 ACE_TEXT ("visit_param_holder() - access of ")
                                 ACE_TEXT ("current template arglist failed - ")
                                 ACE_TEXT ("param=%C scope=%C index=%d\n"),
                                 item->name_.c_str (),
                                 ScopeAsDecl (idl_global->scopes ().top ())->full_name (),
                                 i),
                                -1);
            }
        }
    }

  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("ast_visitor_reifying::")
                     ACE_TEXT ("visit_param_holder() - no match for ")
                     ACE_TEXT ("template param %C in %C\n"),
                     node->info ()->name_.c_str (),
                     ScopeAsDecl (idl_global->scopes ().top ())->full_name ()),
                    -1);
}

void
ast_visitor_reifying::check_and_store (AST_Decl *node)
{
  UTL_ScopedName *tmpl_tail =
    this->template_module_rel_name (node);

  if (tmpl_tail != 0)
    {
      AST_Decl *d =
        idl_global->scopes ().top ()->lookup_by_name (
          tmpl_tail,
          true,
          false);

      this->reified_node_ = d;

      tmpl_tail->destroy ();
      delete tmpl_tail;
      tmpl_tail = 0;
    }
  else
    {
      this->reified_node_ = node;
    }
}

UTL_ScopedName *
ast_visitor_reifying::template_module_rel_name (AST_Decl *d)
{
  AST_Decl *tmp = d;
  ACE_CString name (d->full_name ());

  while (tmp != 0)
    {
      if (AST_Template_Module::narrow_from_decl (tmp) != 0)
        {
          ACE_CString head (tmp->local_name ()->get_string ());

          ACE_CString::size_type start = name.find (head) + 2;

          ACE_CString tail (name.substr (start + head.length ()));

          return FE_Utils::string_to_scoped_name (tail.c_str ());
        }

      tmp = ScopeAsDecl (tmp->defined_in ());
    }

  return 0;
}

