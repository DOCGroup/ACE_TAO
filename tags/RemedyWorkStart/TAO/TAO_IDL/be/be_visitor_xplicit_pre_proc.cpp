/**
 *  @file   be_visitor_xplicit_pre_proc.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_xplicit_pre_proc.h"
#include "be_home.h"
#include "be_component.h"
#include "be_interface.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_finder.h"
#include "be_exception.h"
#include "be_field.h"
#include "be_valuebox.h"
#include "be_eventtype.h"
#include "be_typedef.h"
#include "be_predefined_type.h"
#include "be_enum.h"
#include "be_enum_val.h"
#include "be_union.h"
#include "be_union_fwd.h"
#include "be_union_branch.h"
#include "be_attribute.h"
#include "be_constant.h"
#include "be_array.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_native.h"

#include "ast_module.h"

#include "utl_namelist.h"
#include "utl_labellist.h"
#include "utl_exprlist.h"

#include "fe_obv_header.h"
#include "nr_extern.h"
#include "global_extern.h"

be_visitor_xplicit_pre_proc::be_visitor_xplicit_pre_proc (
      be_visitor_context *ctx)
  : be_visitor_ccm_pre_proc (ctx),
    xplicit_ (0),
    type_holder_ (0),
    ref_type_ (false)
{
}

be_visitor_xplicit_pre_proc::~be_visitor_xplicit_pre_proc (void)
{
}

int
be_visitor_xplicit_pre_proc::visit_home (be_home *node)
{
  UTL_NameList *parent_list = this->compute_inheritance (node);

  FE_InterfaceHeader header (0,
                             parent_list,
                             false,
                             false,
                             true);

  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  AST_Module *m =
    AST_Module::narrow_from_scope (node->defined_in ());

  idl_global->scopes ().push (m);

  UTL_ScopedName *explicit_name =
    this->create_scoped_name (
      "",
      node->original_local_name ()->get_string (),
      "Explicit",
      m);

  be_interface *i = 0;
  ACE_NEW_RETURN (i,
                  be_interface (explicit_name,
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                false,
                                false),
                  0);

  (void) m->be_add_interface (i);

  i->original_interface (node);
  i->set_imported (node->imported ());

  // So we can generate the proper typecode.
  i->home_equiv (true);

  idl_global->scopes ().push (i);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_home - code generation ")
                         ACE_TEXT ("for home scope failed\n")),
                        0);
    }

  // Through with the explicit interface scope
  idl_global->scopes ().pop ();

  explicit_name->destroy ();
  delete explicit_name;
  explicit_name = 0;

  header.destroy ();

  parent_list->destroy ();
  delete parent_list;
  parent_list = 0;

  // Through with the scope containing the home.
  idl_global->scopes ().pop ();

  this->xplicit_ = i;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_operation (be_operation *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  be_operation *home_op = 0;
  ACE_NEW_RETURN (home_op,
                  be_operation (node->return_type (),
                                node->flags (),
                                &sn,
                                false,
                                false),
                  -1);

  home_op->be_add_exceptions (node->exceptions ());

  idl_global->scopes ().top ()->add_to_scope (home_op);
  idl_global->scopes ().push (home_op);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_operation - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_argument (be_argument *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  this->ref_type_ = true;

  be_type *ft =
    be_type::narrow_from_decl (node->field_type ());

  if (ft->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_argument - code generation ")
                         ACE_TEXT ("for arg type failed\n")),
                        -1);
    }

  this->ref_type_ = false;

  AST_Type *arg_type =
    AST_Type::narrow_from_decl (this->type_holder_);

  be_argument *added_arg = 0;
  ACE_NEW_RETURN (added_arg,
                  be_argument (node->direction (),
                               arg_type,
                               &sn),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_arg);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_factory (be_factory *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Home *f_home =
    AST_Home::narrow_from_scope (node->defined_in ());

  be_operation *added_factory = 0;
  ACE_NEW_RETURN (added_factory,
                  be_operation (f_home->managed_component (),
                                AST_Operation::OP_noflags,
                                &sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_factory);
  idl_global->scopes ().push (added_factory);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_factory - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_finder (be_finder *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Home *f_home =
    AST_Home::narrow_from_scope (node->defined_in ());

  be_operation *added_finder = 0;
  ACE_NEW_RETURN (added_finder,
                  be_operation (f_home->managed_component (),
                                AST_Operation::OP_noflags,
                                &sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_finder);
  idl_global->scopes ().push (added_finder);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_finder - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_interface (be_interface *node)
{
  // Interface can't be declared inside a home, no need to call
  // check_and_store().
  this->type_holder_ = node;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_valuebox (be_valuebox *node)
{
  // Valuebox can't be declared inside a home, no need to call
  // check_and_store().
  this->type_holder_ = node;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_valuetype (be_valuetype *node)
{
  // Valuetype can't be declared inside a home, no need to call
  // check_and_store().
  this->type_holder_ = node;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_eventtype (be_eventtype *node)
{
  // Eventtype can't be declared inside a home, no need to call
  // check_and_store().
  this->type_holder_ = node;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_structure (be_structure *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  UTL_ScopedName sn (node->local_name (), 0);

  be_structure *added_struct = 0;
  ACE_NEW_RETURN (added_struct,
                  be_structure (&sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_struct);
  idl_global->scopes ().push (added_struct);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_structure - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_structure_fwd (
  be_structure_fwd *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  be_structure *dummy = 0;
  ACE_NEW_RETURN (dummy,
                  be_structure (&sn,
                                false,
                                false),
                  -1);

  be_structure_fwd *added_struct_fwd = 0;
  ACE_NEW_RETURN (added_struct_fwd,
                  be_structure_fwd (dummy,
                                    &sn),
                  -1);

  dummy->fwd_decl (added_struct_fwd);

  idl_global->scopes ().top ()->add_to_scope (added_struct_fwd);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_exception (be_exception *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  be_exception *added_excep = 0;
  ACE_NEW_RETURN (added_excep,
                  be_exception (&sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_excep);
  idl_global->scopes ().push (added_excep);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_exception - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_enum (be_enum *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  UTL_ScopedName sn (node->local_name (), 0);

  be_enum *added_enum = 0;
  ACE_NEW_RETURN (added_enum,
                  be_enum (&sn,
                           false,
                           false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_enum);
  idl_global->scopes ().push (added_enum);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_enum - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_field (be_field *node)
{
  this->ref_type_ = true;

  be_type *ft =
    be_type::narrow_from_decl (node->field_type ());

  if (ft->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_field - code generation ")
                         ACE_TEXT ("for field type failed\n")),
                        -1);
    }

  this->ref_type_ = false;

  UTL_ScopedName sn (node->local_name (), 0);
  AST_Type *field_type =
    AST_Type::narrow_from_decl (this->type_holder_);

  be_field *added_field = 0;
  ACE_NEW_RETURN (added_field,
                  be_field (field_type,
                            &sn,
                            node->visibility ()),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_field);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_attribute (be_attribute *node)
{
  be_type *ft =
    be_type::narrow_from_decl (node->field_type ());

  this->ref_type_ = true;

  if (ft->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_attribute - code generation ")
                         ACE_TEXT ("for attribute type failed\n")),
                        -1);
    }

  this->ref_type_ = false;

  AST_Type *attr_type =
    AST_Type::narrow_from_decl (this->type_holder_);

  UTL_ScopedName sn (node->local_name (), 0);

  be_attribute *added_attr = 0;
  ACE_NEW_RETURN (added_attr,
                  be_attribute (node->readonly (),
                                attr_type,
                                &sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_attr);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union (be_union *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  be_type *ud =
    be_type::narrow_from_decl (node->disc_type ());

  this->ref_type_ = true;

  if (ud->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_union - code generation ")
                         ACE_TEXT ("for discriminator type failed\n")),
                        -1);
    }

  this->ref_type_ = false;

  AST_ConcreteType *disc =
    AST_ConcreteType::narrow_from_decl (this->type_holder_);

  UTL_ScopedName sn (node->local_name (), 0);

  be_union *added_union = 0;
  ACE_NEW_RETURN (added_union,
                  be_union (disc,
                            &sn,
                            false,
                            false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_union);
  idl_global->scopes ().push (added_union);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_union - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union_fwd (be_union_fwd *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  be_union *dummy = 0;
  ACE_NEW_RETURN (dummy,
                  be_union (0,
                            &sn,
                            false,
                            false),
                  -1);

  be_union_fwd *added_union_fwd = 0;
  ACE_NEW_RETURN (added_union_fwd,
                  be_union_fwd (dummy,
                                &sn),
                  -1);

  dummy->fwd_decl (added_union_fwd);

  idl_global->scopes ().top ()->add_to_scope (added_union_fwd);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union_branch (
  be_union_branch *node)
{
  be_type *ft =
    be_type::narrow_from_decl (node->field_type ());

  this->ref_type_ = true;

  if (ft->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_union_branch - code generation ")
                         ACE_TEXT ("for branch type failed\n")),
                        -1);
    }

  this->ref_type_ = false;

  AST_Type *bt =
    AST_Type::narrow_from_decl (this->type_holder_);

  UTL_ScopedName sn (node->local_name (), 0);

  // The union branch owns its label list so we have to copy it.
  UTL_LabelList *ll = node->labels ()->copy ();

  be_union_branch *added_branch = 0;
  ACE_NEW_RETURN (added_branch,
                  be_union_branch (ll, bt, &sn),
                  -1);

  // fe_add_union_branch() does necessary things besides calling
  // add_to_scope() so we need to reuse it.

  be_union *u =
    be_union::narrow_from_scope (idl_global->scopes ().top ());

  u->be_add_union_branch (added_branch);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union_label (be_union_label *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_constant (be_constant *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  AST_Expression *new_v = 0;
  ACE_NEW_RETURN (new_v,
                  AST_Expression (node->constant_value (),
                                  node->et ()),
                  -1);

  UTL_ScopedName sn (node->local_name (), 0);

  be_constant *added_const = 0;
  ACE_NEW_RETURN (added_const,
                  be_constant (node->et (),
                               new_v,
                               &sn),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_const);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_enum_val (be_enum_val *node)
{
  if (this->ref_type_)
    {
      this->type_holder_ = node;
      return 0;
    }

  UTL_ScopedName sn (node->local_name (), 0);

  be_enum_val *added_enum_val = 0;
  ACE_NEW_RETURN (added_enum_val,
                  be_enum_val (node->constant_value ()->ev ()->u.ulval,
                               &sn),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_enum_val);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_array (be_array *node)
{
  be_type *bt =
    be_type::narrow_from_decl (node->base_type ());

  bool tmp = this->ref_type_;
  this->ref_type_ = true;

  if (bt->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_array - code generation ")
                         ACE_TEXT ("for base type failed\n")),
                        -1);
    }

  this->ref_type_ = tmp;

  AST_Expression *v = 0;
  UTL_ExprList *v_list = 0;

  for (ACE_CDR::ULong i = 0; i < node->n_dims (); ++i)
    {
      ACE_NEW_RETURN (v,
                      AST_Expression (node->dims ()[i],
                                      AST_Expression::EV_ulong),
                      -1);

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

  be_array *added_array = 0;
  ACE_NEW_RETURN (added_array,
                  be_array (&sn,
                            node->n_dims (),
                            v_list,
                            false,
                            false),
                  -1);

  // No need to add this new node to any scope - it's anonymous
  // and owned by the node that references it.

  v_list->destroy ();
  delete v_list;
  v_list = 0;

  AST_Type *base_type =
    AST_Type::narrow_from_decl (this->type_holder_);

  added_array->set_base_type (base_type);

  this->type_holder_ = added_array;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_sequence (be_sequence *node)
{
  be_type *bt =
    be_type::narrow_from_decl (node->base_type ());

  bool tmp = this->ref_type_;
  this->ref_type_ = true;

  if (bt->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_sequence - code generation ")
                         ACE_TEXT ("for element type failed\n")),
                        -1);
    }

  this->ref_type_ = tmp;

  AST_Expression *v = node->max_size ();

  AST_Expression *bound = 0;
  ACE_NEW_RETURN (bound,
                  AST_Expression (v,
                                  AST_Expression::EV_ulong),
                  -1);

  Identifier id ("sequence");
  UTL_ScopedName sn (&id, 0);

  AST_Type *ft =
    AST_Type::narrow_from_decl (this->type_holder_);

  ACE_NEW_RETURN (this->type_holder_,
                  be_sequence (bound,
                               ft,
                               &sn,
                               false,
                               false),
                  -1);

  // No need to add this new node to any scope - it's anonymous
  // and owned by the node that references it.

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_string (be_string *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  AST_Expression *b = node->max_size ();

  if (b->ev ()->u.ulval == 0)
    {
      this->type_holder_ = node;
      return 0;
    }

  AST_Expression *bound = 0;
  ACE_NEW_RETURN (bound,
                  AST_Expression (b,
                                  AST_Expression::EV_ulong),
                  -1);

  Identifier id ("string");
  UTL_ScopedName sn (&id, 0);

  ACE_NEW_RETURN (this->type_holder_,
                  be_string (AST_Decl::NT_string,
                             &sn,
                             bound,
                             node->width ()),
                  -1);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_typedef (be_typedef *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  be_type *t =
    be_type::narrow_from_decl (node->field_type ());

  this->ref_type_ = true;

  if (t->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_typedef - code generation ")
                         ACE_TEXT ("for base type failed\n")),
                        -1);
    }

  this->ref_type_ = false;

  UTL_ScopedName sn (node->local_name (), 0);
  AST_Type *bt =
    AST_Type::narrow_from_decl (this->type_holder_);

  be_typedef *added_typedef = 0;
  ACE_NEW_RETURN (added_typedef,
                  be_typedef (bt,
                              &sn,
                              false,
                              false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_typedef);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_native (be_native *node)
{
  if (this->ref_type_)
    {
      this->check_and_store (node);
      return 0;
    }

  UTL_ScopedName sn (node->local_name (), 0);

  be_native *added_native = 0;
  ACE_NEW_RETURN (added_native,
                  be_native (&sn),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_native);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_predefined_type (
  be_predefined_type *node)
{
  this->type_holder_ = node;

  return 0;
}

be_interface *
be_visitor_xplicit_pre_proc::xplicit (void) const
{
  return this->xplicit_;
}

void
be_visitor_xplicit_pre_proc::check_and_store (AST_Decl *node)
{
  UTL_ScopedName *tmpl_tail =
    this->xplicit_iface_rel_name (node);

  if (tmpl_tail != 0)
    {
      AST_Decl *d =
        idl_global->scopes ().top ()->lookup_by_name (
          tmpl_tail,
          true);

      this->type_holder_ = d;

      tmpl_tail->destroy ();
      delete tmpl_tail;
      tmpl_tail = 0;
    }
  else
    {
      this->type_holder_ = node;
    }
}

UTL_ScopedName *
be_visitor_xplicit_pre_proc::xplicit_iface_rel_name (AST_Decl *d)
{
  AST_Decl *tmp = d;
  ACE_CString name (d->full_name ());

  while (tmp != 0)
    {
      if (be_home::narrow_from_decl (tmp) != 0)
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

