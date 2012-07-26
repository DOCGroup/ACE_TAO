/* -*- c++ -*- */
// $Id$

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_constant.h"
#include "ast_enum.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_exception.h"
#include "ast_expression.h"
#include "ast_finder.h"
#include "ast_field.h"
#include "ast_home.h"
#include "ast_interface.h"
#include "ast_interface_fwd.h"
#include "ast_module.h"
#include "ast_native.h"
#include "ast_operation.h"
#include "ast_predefined_type.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure.h"
#include "ast_typedef.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "utl_identifier.h"
#include "utl_string.h"
#include "utl_exceptlist.h"
#include "fe_extern.h"
#include "nr_extern.h"

#include "ifr_adding_visitor.h"
#include "ifr_adding_visitor_operation.h"
#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_exception.h"
#include "ifr_adding_visitor_union.h"

#include "tao/IFR_Client/IFR_ComponentsC.h"

#include "ace/Vector_T.h"

ifr_adding_visitor::ifr_adding_visitor (
      AST_Decl *scope,
      bool in_reopened,
      bool allow_duplicate_typedefs)
  : scope_ (scope),
    in_reopened_ (in_reopened),
    allow_duplicate_typedefs_ (allow_duplicate_typedefs)
{
}

ifr_adding_visitor::~ifr_adding_visitor (void)
{
}

int
ifr_adding_visitor::visit_scope (UTL_Scope *node)
{
  // Proceed if the number of members in our scope is greater than 0.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate over our scope.
      UTL_ScopeActiveIterator si (node,
                                  UTL_Scope::IK_decls);

      AST_Decl *d = 0;

      // Continue until each element is visited.
      while (!si.is_done ())
        {
          d = si.item ();

          if (d == 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_scope -")
                  ACE_TEXT (" bad node in this scope\n")
                ),
                -1
              );
            }

          // These are created at startup in the repository, and
          // need not be dealt with here.
          if (d->node_type () == AST_Decl::NT_pre_defined)
            {
              si.next ();
              continue;
            }

          if (d->ast_accept (this) == -1)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_scope -")
                  ACE_TEXT (" failed to accept visitor\n")
                ),
                -1
              );
            }

          si.next ();
        }
    }

  return 0;
}

int
ifr_adding_visitor::visit_predefined_type (AST_PredefinedType *node)
{
  try
    {
      this->ir_current_ =
      be_global->repository ()->get_primitive (
                                    this->predefined_type_to_pkind (node)
                                  );
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_predefined_type"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_module (AST_Module *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  CORBA::Container_var new_def;

  try
    {
      // If this module been opened before, it will already be in
      // the repository.
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          // New module.
          CORBA::Container_ptr container =
            CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (container) == 0)
            {
              new_def = container->create_module (
                                       node->repoID (),
                                       node->local_name ()->get_string (),
                                       node->version ()
                                     );
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }
        }
      else
        {
          CORBA::DefinitionKind kind =
            prev_def->def_kind ();

          if (kind == CORBA::dk_Module)
            {
              // We are either in a reopened module, are processing an IDL
              // IDL file for the second time, or are in a module whose
              // name already exists by coincidence - there is no way to
              // tell the difference. So any members whose repository ID
              // already exists in this case will throw BAD_PARAM
              // when we attempt to add them to the repository.
              this->in_reopened_ = true;

              new_def =
                CORBA::ComponentIR::Container::_narrow (prev_def.in ());
            }
        }

      if (be_global->ifr_scopes ().push (new_def.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      this->in_reopened_ = false;
      CORBA::Container_ptr tmp =
        CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().pop (tmp) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_module"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_interface (AST_Interface *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_interface_def (node);

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined () && !node->ifr_added ())
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declaration
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of the
              //    node type or of some other type.
              // If prev_def would narrow successfully to the node type, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT the node type, we go
              // ahead an attempt to create an interface, which will get an
              // exception from the IFR, as the spec requires.
              if (!node->ifr_fwd_added () && !node->imported ())
                {
                  CORBA::DefinitionKind kind = prev_def->def_kind ();

                  if (kind == CORBA::dk_Interface)
                    {
                      CORBA::InterfaceDef_var iface =
                        CORBA::InterfaceDef::_narrow (prev_def.in ());

                      CORBA::ContainedSeq_var contents =
                        iface->contents (CORBA::dk_all, true);

                      for (CORBA::ULong i = 0; i < contents->length (); ++i)
                        {
                          contents[i]->destroy ();
                        }
                    }
                  else
                    {
                      // This will cause the IFR to throw an exception,
                      // as it should.
                      return this->create_interface_def (node);
                    }
                }

              CORBA::ULong n_parents = static_cast<CORBA::ULong> (node->n_inherits ());

              CORBA::InterfaceDefSeq bases (n_parents);
              bases.length (n_parents);
              CORBA::Contained_var result;

              AST_Type **parents = node->inherits ();

              // Construct a list of the parents.
              for (CORBA::ULong i = 0; i < n_parents; ++i)
                {
                  result =
                    be_global->repository ()->lookup_id (parents[i]->repoID ());

                  // If one of our interface's parents is not in the repository,
                  // that means that it has not yet been seen (even as a
                  // forward declaration) in the IDL file, and we will have to
                  // postpone the populating of our interface until they are all
                  // added.
                  if (CORBA::is_nil (result.in ()))
                    {
                      this->ir_current_ =
                        CORBA::IDLType::_narrow (prev_def.in ());

                      return 0;
                    }

                  bases[i] =
                    CORBA::InterfaceDef::_narrow (result.in ());

                  if (CORBA::is_nil (bases[i]))
                    {
                      ACE_ERROR_RETURN ((
                          LM_ERROR,
                          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                          ACE_TEXT ("visit_interface -")
                          ACE_TEXT (" CORBA::InterfaceDef::_narrow failed\n")
                        ),
                        -1
                      );
                    }
                }

              CORBA::InterfaceDef_var extant_def =
                CORBA::InterfaceDef::_narrow (prev_def. in ());

              extant_def->base_interfaces (bases);

              node->ifr_added (true);

              // Push the new IR object onto the scope stack.
              if (be_global->ifr_scopes ().push (extant_def.in ()) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope push failed\n")
                    ),
                    -1
                  );
                }

              // Visit the members, if any.
              if (this->visit_scope (node) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" visit_scope failed\n")
                    ),
                    -1
                  );
                }

              // This spot in the AST doesn't necessarily have to be the
              // interface definition - it could be any reference to it.
              // The front end will already have fully defined it, so all
              // the info is available anywhere. So it's a good idea to
              // update the current IR object holder now.
              this->ir_current_ =
                CORBA::IDLType::_duplicate (extant_def.in ());

              CORBA::Container_ptr used_scope =
                CORBA::Container::_nil ();

              // Pop the new IR object back off the scope stack.
              if (be_global->ifr_scopes ().pop (used_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope pop failed\n")
                    ),
                    -1
                  );
                }
            }
          else
            {
              // @@ (JP) I think we're ok here without a check:
              // not defined/not added - visit_interface_fwd will have
              //                         detected a clobber.
              // not defined/added - not possible.
              // defined/not added - takes the other branch.
              // defined/added - we're ok.
              this->ir_current_ =
                CORBA::IDLType::_narrow (prev_def.in ());
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_interface"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  AST_Interface *i = node->full_definition ();

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (i->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          // If our full definition is found in this IDL file, we go
          // ahead and create the full entry now.
          // The forward declared interface is not defined anywhere
          // in this IDL file, so we just create an empty entry to
          // be replaced by a full definition in some other IDL file.
          CORBA::InterfaceDefSeq bases (0);
          bases.length (0);

          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              if (i->is_local ())
                {
                  this->ir_current_ =
                    current_scope->create_local_interface (
                                       i->repoID (),
                                       i->local_name ()->get_string (),
                                       i->version (),
                                       bases
                                     );
                }
              else
                {
                  this->ir_current_ =
                    current_scope->create_interface (
                                       i->repoID (),
                                       i->local_name ()->get_string (),
                                       i->version (),
                                       bases
                                     );
                }

            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_interface_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          node->ifr_added (true);
          i->ifr_fwd_added (true);
        }

   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_interface_fwd"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_valuebox (AST_ValueBox *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      this->element_type (node->boxed_type ());

      CORBA::Container_ptr current_scope =
        CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          this->ir_current_ =
            current_scope->create_value_box (
                node->repoID (),
                node->local_name ()->get_string (),
                node->version (),
                this->ir_current_.in ()
              );
        }
      else
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_valuebox -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }

      node->ifr_added (true);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_valuebox"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_valuetype (AST_ValueType *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      // Is this valuetype already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_value_def (node);

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined () && !node->ifr_added ())
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declaration
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of the
              //    node type or of some other type.
              // If prev_def would narrow successfully to the node type, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT the node type, we go
              // ahead an attempt to create an interface, which will get an
              // exception from the IFR, as the spec requires.
              if (!node->ifr_fwd_added ())
                {
                  CORBA::DefinitionKind kind =
                    prev_def->def_kind ();

                  if (kind == CORBA::dk_Value)
                    {
                      CORBA::ValueDef_var value =
                        CORBA::ValueDef::_narrow (prev_def.in ());

                      CORBA::ContainedSeq_var contents =
                        value->contents (CORBA::dk_all,
                                         1);

                      CORBA::ULong length = contents->length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          contents[i]->destroy ();
                        }
                    }
                  else
                    {
                      prev_def->destroy ();

                      int status =
                        this->create_value_def (node);

                      return status;
                    }
                }

              // Our previous definition is a valuetype, so narrow it here,
              // then populate it.
              CORBA::ExtValueDef_var extant_def =
                CORBA::ExtValueDef::_narrow (prev_def. in ());

              // Concrete base value.

              CORBA::ValueDef_var base_vt;
              this->fill_base_value (base_vt.out (),
                                     node);

              extant_def->base_value (base_vt.in ());

              // Abstract base values.

              CORBA::ValueDefSeq abstract_base_values;
              this->fill_abstract_base_values (abstract_base_values,
                                               node);

              extant_def->abstract_base_values (abstract_base_values);

              // Supported interfaces.

              CORBA::InterfaceDefSeq supported;
              this->fill_supported_interfaces (supported,
                                               node);

              extant_def->supported_interfaces (supported);

              // Intializers.

              CORBA::ExtInitializerSeq initializers;
              this->fill_initializers (initializers,
                                       node);

              extant_def->ext_initializers (initializers);

              // Truncatable, abstract, custom.

              extant_def->is_abstract (static_cast<CORBA::Boolean> (node->is_abstract ()));

              extant_def->is_truncatable (static_cast<CORBA::Boolean> (node->truncatable ()));

              extant_def->is_custom (static_cast<CORBA::Boolean> (node->custom ()));

              node->ifr_added (true);

              // Push the new IR object onto the scope stack before visiting
              // the new object's scope.
              if (be_global->ifr_scopes ().push (extant_def.in ()) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_valuetype -")
                      ACE_TEXT (" scope push failed\n")
                    ),
                    -1
                  );
                }

              // Visit the members, if any.
              if (this->visit_scope (node) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_valuetype -")
                      ACE_TEXT (" visit_scope failed\n")
                    ),
                    -1
                  );
                }

              // This spot in the AST doesn't necessarily have to be the
              // interface definition - it could be any reference to it.
              // The front end will already have fully defined it, so all
              // the info is available anywhere. So it's a good idea to
              // update the current IR object holder now.
              this->ir_current_ =
                CORBA::IDLType::_duplicate (extant_def.in ());

              CORBA::Container_ptr used_scope =
                CORBA::Container::_nil ();

              // Pop the new IR object back off the scope stack.
              if (be_global->ifr_scopes ().pop (used_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_valuetype -")
                      ACE_TEXT (" scope pop failed\n")
                    ),
                    -1
                  );
                }
            }
          else
            {
              // @@ (JP) I think we're ok here without a check:
              // not defined/not added - visit_valuetype_fwd will have
              //                         detected a clobber.
              // not defined/added - not possible.
              // defined/not added - takes the other branch.
              // defined/added - we're ok.
              this->ir_current_ =
                CORBA::IDLType::_narrow (prev_def.in ());
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_valuetype"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  AST_Interface *v = node->full_definition ();

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (v->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          // If our full definition is found in this IDL file, we go
          // ahead and create the full entry now.
          // The forward declared valuetype is not defined anywhere
          // in this IDL file, so we just create an empty entry to
          // be replaced by a full definition in some other IDL file.
          CORBA::ValueDefSeq abstract_bases (0);
          abstract_bases.length (0);
          CORBA::InterfaceDefSeq supported (0);
          supported.length (0);
          CORBA::InitializerSeq initializers (0);
          initializers.length (0);

          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

          CORBA::Boolean abstract =
            static_cast<CORBA::Boolean> (v->is_abstract ());

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_value (
                                   v->repoID (),
                                   v->local_name ()->get_string (),
                                   v->version (),
                                   0, // 'custom' not handled yet
                                   abstract,
                                   CORBA::ValueDef::_nil (),
                                   0, // 'truncatable' not handled yet
                                   abstract_bases,
                                   supported,
                                   initializers
                                 );
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_valuetype_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          node->ifr_added (true);
          v->ifr_fwd_added (true);
        }
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_valuetype_fwd"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_component (AST_Component *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_component_def (node);

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined () && !node->ifr_added ())
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declaration
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of the
              //    node type or of some other type.
              // If prev_def would narrow successfully to the node type, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT the node type, we go
              // ahead an attempt to create an interface, which will get an
              // exception from the IFR, as the spec requires.
              if (!node->ifr_fwd_added ())
                {
                  CORBA::DefinitionKind kind =
                    prev_def->def_kind ();

                  if (kind == CORBA::dk_Component)
                    {
                      CORBA::ComponentIR::ComponentDef_var value =
                        CORBA::ComponentIR::ComponentDef::_narrow (
                            prev_def.in ()
                          );

                      CORBA::ContainedSeq_var contents =
                        value->contents (CORBA::dk_all,
                                         1);

                      CORBA::ULong length = contents->length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          contents[i]->destroy ();
                        }
                    }
                  else
                    {
                      prev_def->destroy ();

                      int status =
                        this->create_component_def (node);

                      return status;
                    }
                }

              // Re-populate the repository entry.

              CORBA::ComponentIR::ComponentDef_var extant_def =
                CORBA::ComponentIR::ComponentDef::_narrow (
                                                      prev_def.in ()
                                                    );

              CORBA::InterfaceDefSeq supported_interfaces;
              this->fill_supported_interfaces (supported_interfaces,
                                               node);

              extant_def->supported_interfaces (supported_interfaces);

              CORBA::ComponentIR::ComponentDef_var base_component;
              this->fill_base_component (base_component.out (),
                                         node);

              extant_def->base_component (base_component.in ());

              node->ifr_added (true);

              // Push the new IR object onto the scope stack before visiting
              // the new object's scope.
              if (be_global->ifr_scopes ().push (extant_def.in ()) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_component -")
                      ACE_TEXT (" scope push failed\n")
                    ),
                    -1
                  );
                }

              // This spot in the AST doesn't necessarily have to be the
              // interface definition - it could be any reference to it.
              // The front end will already have fully defined it, so all
              // the info is available anywhere. So it's a good idea to
              // update the current IR object holder now.
              this->ir_current_ =
                CORBA::IDLType::_duplicate (extant_def.in ());

              // Visit the members, if any.
              if (this->visit_scope (node) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_component -")
                      ACE_TEXT (" visit_scope failed\n")
                    ),
                    -1
                  );
                }

              CORBA::Container_ptr used_scope =
                CORBA::Container::_nil ();

              // Pop the new IR object back off the scope stack.
              if (be_global->ifr_scopes ().pop (used_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope pop failed\n")
                    ),
                    -1
                  );
                }
             }
          else
            {
              // @@ (JP) I think we're ok here without a check:
              // not defined/not added - visit_interface_fwd will have
              //                         detected a clobber.
              // not defined/added - not possible.
              // defined/not added - takes the other branch.
              // defined/added - we're ok.
              this->ir_current_ =
                CORBA::IDLType::_narrow (prev_def.in ());
            }
        }
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_component"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_component_fwd (AST_ComponentFwd *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  AST_Component *c =
    AST_Component::narrow_from_decl (node->full_definition ());

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (c->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              CORBA::ComponentIR::Container_var ccm_scope =
                CORBA::ComponentIR::Container::_narrow (
                                                   current_scope
                                                 );

              // If our full definition is found in this IDL file, we go
              // ahead and create the full entry now.
              // The forward declared component is not defined anywhere
              // in this IDL file, so we just create an empty entry to
              // be replaced by a full definition later.
              CORBA::InterfaceDefSeq supported_interfaces;
              supported_interfaces.length (0);
              CORBA::ComponentIR::ComponentDef_var base_component;


              if (node->is_defined ())
                {
                  this->fill_supported_interfaces (supported_interfaces,
                                                   c);

                  this->fill_base_component (base_component.out (),
                                             c);
                }

              this->ir_current_ =
                ccm_scope->create_component (
                               c->repoID (),
                               c->local_name ()->get_string (),
                               c->version (),
                               base_component.in (),
                               supported_interfaces
                             );
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_component_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          node->ifr_added (true);
          c->ifr_fwd_added (true);
        }
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_component_fwd"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_provides (AST_Provides *node)
{
  ::CORBA::Contained_var contained =
    be_global->repository ()->lookup_id (
      node->provides_type ()->repoID ());

  ::CORBA::InterfaceDef_var interface_type =
    ::CORBA::InterfaceDef::_narrow (contained.in ());

  ::CORBA::ComponentIR::ComponentDef_var c =
    ::CORBA::ComponentIR::ComponentDef::_narrow (
      this->ir_current_.in ());

  ACE_CString comp_str (c->_interface_repository_id ());
  char *local_name = node->local_name ()->get_string ();
  this->expand_id (comp_str, local_name);

  ::CORBA::ComponentIR::ProvidesDef_var new_def =
    c->create_provides (comp_str.fast_rep (),
                        local_name,
                        node->provides_type ()->version (),
                        interface_type.in ());

  return 0;
}

int
ifr_adding_visitor::visit_uses (AST_Uses *node)
{
  ::CORBA::Contained_var contained =
    be_global->repository ()->lookup_id (
      node->uses_type ()->repoID ());

  ::CORBA::InterfaceDef_var interface_type =
    ::CORBA::InterfaceDef::_narrow (contained.in ());

  ::CORBA::ComponentIR::ComponentDef_var c =
    ::CORBA::ComponentIR::ComponentDef::_narrow (
      this->ir_current_.in ());

  ACE_CString comp_str (c->_interface_repository_id ());
  char *local_name = node->local_name ()->get_string ();
  this->expand_id (comp_str, local_name);

  ::CORBA::ComponentIR::UsesDef_var new_def =
    c->create_uses (comp_str.fast_rep (),
                    local_name,
                    node->uses_type ()->version (),
                    interface_type.in (),
                    static_cast<CORBA::Boolean> (node->is_multiple ()));

  return 0;
}

int
ifr_adding_visitor::visit_publishes (AST_Publishes *node)
{
  ::CORBA::Contained_var contained =
    be_global->repository ()->lookup_id (
      node->publishes_type ()->repoID ());

  ::CORBA::ComponentIR::EventDef_var event_type =
    ::CORBA::ComponentIR::EventDef::_narrow (contained.in ());

  ::CORBA::ComponentIR::ComponentDef_var c =
    ::CORBA::ComponentIR::ComponentDef::_narrow (
      this->ir_current_.in ());

  ACE_CString comp_str (c->_interface_repository_id ());
  char *local_name = node->local_name ()->get_string ();
  this->expand_id (comp_str, local_name);

  ::CORBA::ComponentIR::PublishesDef_var new_def =
    c->create_publishes (comp_str.fast_rep (),
                         local_name,
                         node->publishes_type ()->version (),
                         event_type.in ());

  return 0;
}

int
ifr_adding_visitor::visit_emits (AST_Emits *node)
{
  ::CORBA::Contained_var contained =
    be_global->repository ()->lookup_id (
      node->emits_type ()->repoID ());

  ::CORBA::ComponentIR::EventDef_var event_type =
    ::CORBA::ComponentIR::EventDef::_narrow (contained.in ());

  ::CORBA::ComponentIR::ComponentDef_var c =
    ::CORBA::ComponentIR::ComponentDef::_narrow (
      this->ir_current_.in ());

  ACE_CString comp_str (c->_interface_repository_id ());
  char *local_name = node->local_name ()->get_string ();
  this->expand_id (comp_str, local_name);

  ::CORBA::ComponentIR::EmitsDef_var new_def =
    c->create_emits (comp_str.fast_rep (),
                     local_name,
                     node->emits_type ()->version (),
                     event_type.in ());

  return 0;
}

int
ifr_adding_visitor::visit_consumes (AST_Consumes *node)
{
  ::CORBA::Contained_var contained =
    be_global->repository ()->lookup_id (
      node->consumes_type ()->repoID ());

  ::CORBA::ComponentIR::EventDef_var event_type =
    ::CORBA::ComponentIR::EventDef::_narrow (contained.in ());

  ::CORBA::ComponentIR::ComponentDef_var c =
    ::CORBA::ComponentIR::ComponentDef::_narrow (
      this->ir_current_.in ());

  ACE_CString comp_str (c->_interface_repository_id ());
  char *local_name = node->local_name ()->get_string ();
  this->expand_id (comp_str, local_name);

  ::CORBA::ComponentIR::ConsumesDef_var new_def =
    c->create_consumes (comp_str.fast_rep (),
                        local_name,
                        node->consumes_type ()->version (),
                        event_type.in ());

  return 0;
}

int
ifr_adding_visitor::visit_eventtype (AST_EventType *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_event_def (node);

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined () && !node->ifr_added ())
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declaration
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of the
              //    node type or of some other type.
              // If prev_def would narrow successfully to the node type, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT the node type, we go
              // ahead an attempt to create an interface, which will get an
              // exception from the IFR, as the spec requires.
              if (!node->ifr_fwd_added ())
                {
                  CORBA::DefinitionKind kind =
                    prev_def->def_kind ();

                  if (kind == CORBA::dk_Value)
                    {
                      CORBA::ComponentIR::EventDef_var event =
                        CORBA::ComponentIR::EventDef::_narrow (
                            prev_def.in ()
                          );

                      CORBA::ContainedSeq_var contents =
                        event->contents (CORBA::dk_all,
                                         1);

                      CORBA::ULong length = contents->length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          contents[i]->destroy ();
                        }
                    }
                  else
                    {
                      prev_def->destroy ();

                      int status =
                        this->create_event_def (node);

                      return status;
                    }
                }

              // Our previous definition is a valuetype, so narrow it here,
              // then populate it.
              CORBA::ComponentIR::EventDef_var extant_def =
                CORBA::ComponentIR::EventDef::_narrow (prev_def. in ());

              // Concrete base value.

              CORBA::ValueDef_var base_vt;
              this->fill_base_value (base_vt.out (),
                                     node);

              extant_def->base_value (base_vt.in ());

              // Abstract base values.

              CORBA::ValueDefSeq abstract_base_values;
              this->fill_abstract_base_values (abstract_base_values,
                                               node);

              extant_def->abstract_base_values (abstract_base_values);

              // Supported interfaces.

              CORBA::InterfaceDefSeq supported;
              this->fill_supported_interfaces (supported,
                                               node);

              extant_def->supported_interfaces (supported);

              // Intializers.

              CORBA::ExtInitializerSeq initializers;
              this->fill_initializers (initializers,
                                       node);

              extant_def->ext_initializers (initializers);

              // Truncatable, abstract, custom.

              extant_def->is_abstract (static_cast<CORBA::Boolean> (node->is_abstract ()));

              extant_def->is_truncatable (static_cast<CORBA::Boolean> (node->truncatable ()));

              extant_def->is_custom (static_cast<CORBA::Boolean> (node->custom ()));

              node->ifr_added (true);

              // Push the new IR object onto the scope stack before visiting
              // the new object's scope.
              if (be_global->ifr_scopes ().push (extant_def.in ()) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_eventtype -")
                      ACE_TEXT (" scope push failed\n")
                    ),
                    -1
                  );
                }

              // Visit the members, if any.
              if (this->visit_scope (node) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_eventtype -")
                      ACE_TEXT (" visit_scope failed\n")
                    ),
                    -1
                  );
                }

              // This spot in the AST doesn't necessarily have to be the
              // interface definition - it could be any reference to it.
              // The front end will already have fully defined it, so all
              // the info is available anywhere. So it's a good idea to
              // update the current IR object holder now.
              this->ir_current_ =
                CORBA::IDLType::_duplicate (extant_def.in ());

              CORBA::Container_ptr used_scope =
                CORBA::Container::_nil ();

              // Pop the new IR object back off the scope stack.
              if (be_global->ifr_scopes ().pop (used_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_eventtype -")
                      ACE_TEXT (" scope pop failed\n")
                    ),
                    -1
                  );
                }
            }
          else
            {
              // @@ (JP) I think we're ok here without a check:
              // not defined/not added - visit_valuetype_fwd will have
              //                         detected a clobber.
              // not defined/added - not possible.
              // defined/not added - takes the other branch.
              // defined/added - we're ok.
              this->ir_current_ =
                CORBA::IDLType::_narrow (prev_def.in ());
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_eventtype"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  AST_Interface *v = node->full_definition ();

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (v->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          // If our full definition is found in this IDL file, we go
          // ahead and create the full entry now.
          // The forward declared valuetype is not defined anywhere
          // in this IDL file, so we just create an empty entry to
          // be replaced by a full definition in some other IDL file.
          CORBA::ValueDefSeq abstract_bases (0);
          abstract_bases.length (0);
          CORBA::InterfaceDefSeq supported (0);
          supported.length (0);
          CORBA::ExtInitializerSeq initializers (0);
          initializers.length (0);

          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              CORBA::ComponentIR::Container_var ccm_scope =
                CORBA::ComponentIR::Container::_narrow (
                                                   current_scope
                                                 );

              CORBA::Boolean abstract =
                static_cast<CORBA::Boolean> (v->is_abstract ());

              this->ir_current_ =
                ccm_scope->create_event (
                               v->repoID (),
                               v->local_name ()->get_string (),
                               v->version (),
                               0, // 'custom' not handled yet
                               abstract,
                               CORBA::ValueDef::_nil (),
                               0, // 'truncatable' not handled yet
                               abstract_bases,
                               supported,
                               initializers
                             );
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_eventtype_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          node->ifr_added (true);
          v->ifr_fwd_added (true);
        }
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_eventtype_fwd"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_home (AST_Home *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_home_def (node);

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined () && !node->ifr_added ())
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declaration
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of the
              //    node type or of some other type.
              // If prev_def would narrow successfully to the node type, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT the node type, we go
              // ahead an attempt to create an interface, which will get an
              // exception from the IFR, as the spec requires.
              if (!node->ifr_fwd_added ())
                {
                  CORBA::DefinitionKind kind =
                    prev_def->def_kind ();

                  if (kind == CORBA::dk_Component)
                    {
                      CORBA::ComponentIR::HomeDef_var value =
                        CORBA::ComponentIR::HomeDef::_narrow (
                            prev_def.in ()
                          );

                      CORBA::ContainedSeq_var contents =
                        value->contents (CORBA::dk_all,
                                         1);

                      CORBA::ULong length = contents->length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          contents[i]->destroy ();
                        }
                    }
                  else
                    {
                      prev_def->destroy ();

                      int status =
                        this->create_home_def (node);

                      return status;
                    }
                }

              // Re-populate the repository entry.

            }
          else
            {
              // @@ (JP) I think we're ok here without a check:
              // not defined/not added - visit_interface_fwd will have
              //                         detected a clobber.
              // not defined/added - not possible.
              // defined/not added - takes the other branch.
              // defined/added - we're ok.
              this->ir_current_ =
                CORBA::IDLType::_narrow (prev_def.in ());
            }
        }
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_home"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_structure (AST_Structure *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_structure visitor (node);
          int retval = visitor.visit_structure (node);

          if (retval == 0)
            {
              this->ir_current_ =
                CORBA::IDLType::_duplicate (visitor.ir_current ());
            }

          return retval;
        }
      else
        {
          if (node->ifr_added ())
            {
              // We have visited this node before in the same IDL
              // compilation unit - just update ir_current_.
              this->ir_current_ =
                CORBA::StructDef::_narrow (prev_def.in ());
            }
          else if (node->ifr_fwd_added ())
            {
              // We are seeing the full definition of a forward
              // declaration - just visit the scope to populate
              // the IFR entry while keeping the same section key
              // so as not to invalidate existing references.
              ifr_adding_visitor_structure visitor (node);
              int retval = visitor.visit_structure (node);

              if (retval == 0)
                {
                  this->ir_current_ =
                    CORBA::IDLType::_duplicate (visitor.ir_current ());
                }

              return retval;
            }
          else
            {
              // We are clobbering a previous entry
              // from another IDL file. In this case we do what
              // other ORB vendors do - destroy the original
              // entry, create a new one, and let the user beware.
              prev_def->destroy ();
              return this->visit_structure (node);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor::visit_structure"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_structure_fwd (AST_StructureFwd *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::StructMemberSeq dummyMembers;
          dummyMembers.length (0);
          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_structure_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          CORBA::StructDef_var struct_def =
            current_scope->create_struct (
                node->repoID (),
                node->local_name ()->get_string (),
                node->version (),
                dummyMembers
              );

          node->full_definition ()->ifr_fwd_added (true);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("ifr_adding_visitor::")
                               ACE_TEXT ("visit_structure_fwd"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_exception (AST_Exception *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ifr_adding_visitor_exception visitor (node,
                                        this->in_reopened_);

  // No point in updating ir_current_ here because
  // ExceptionDef is not an IDLType.

  return visitor.visit_exception (node);
}

int
ifr_adding_visitor::visit_enum (AST_Enum *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      // Is this enum already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::ULong member_count =
            static_cast<CORBA::ULong> (node->member_count ());

          CORBA::EnumMemberSeq members (member_count);
          members.length (member_count);

          UTL_ScopedName *member_name = 0;

          // Get a list of the member names.
          for (CORBA::ULong i = 0; i < member_count; ++i)
            {
              member_name = node->value_to_name (i);

              members[i] =
                CORBA::string_dup (
                    member_name->last_component ()->get_string ()
                  );
            }

          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_enum (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version (),
                    members
                  );
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_enum -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          node->ifr_added (true);
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (!node->ifr_added ())
            {
              prev_def->destroy ();

              return this->visit_enum (node);
            }

          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ =
            CORBA::EnumDef::_narrow (prev_def.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("ifr_adding_visitor::visit_enum"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_operation (AST_Operation *node)
{
  ifr_adding_visitor_operation visitor (node);

  return visitor.visit_operation (node);
}

int
ifr_adding_visitor::visit_field (AST_Field *node)
{
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  AST_Decl::NodeType nt = scope->node_type ();

  if (nt == AST_Decl::NT_valuetype || nt == AST_Decl::NT_eventtype)
    {
      return this->create_value_member (node);
    }

  AST_Type *ft = AST_Type::narrow_from_decl (node->field_type ());

  if (ft == 0)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
          ACE_TEXT ("visit_field - ")
          ACE_TEXT ("Bad field type\n")
        ),
        -1
      );
    }

  if (ft->ast_accept (this) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
          ACE_TEXT ("visit_field - ")
          ACE_TEXT ("failed to accept visitor\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::visit_attribute (AST_Attribute *node)
{
  ::CORBA::IDLType_var holder;

  try
    {
      AST_Type *type = node->field_type ();

      // Save to be replaced later.
      holder =
        ::CORBA::IDLType::_duplicate (this->ir_current_.in ());

      // Updates ir_current_.
      this->get_referenced_type (type);

      CORBA::AttributeMode mode =
        node->readonly () ? CORBA::ATTR_READONLY : CORBA::ATTR_NORMAL;

      CORBA::ExceptionDefSeq get_exceptions;
      this->fill_get_exceptions (get_exceptions,
                                 node);

      CORBA::ExceptionDefSeq set_exceptions;
      this->fill_set_exceptions (set_exceptions,
                                 node);

      CORBA::Container_ptr current_scope =
        CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          CORBA::DefinitionKind kind =
            current_scope->def_kind ();

          if (kind == CORBA::dk_Value || kind == CORBA::dk_Event)
            {
              CORBA::ExtValueDef_var value =
                CORBA::ExtValueDef::_narrow (current_scope);

              CORBA::ExtAttributeDef_var new_def =
                value->create_ext_attribute (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version (),
                    this->ir_current_.in (),
                    mode,
                    get_exceptions,
                    set_exceptions
                  );
            }
          else
            {
              // We are an interface, a local interface, an abstract
              // interface or a component. This narrow covers them all.
              CORBA::InterfaceAttrExtension_var iface =
                CORBA::InterfaceAttrExtension::_narrow (current_scope);

              CORBA::ExtAttributeDef_var new_def =
                iface->create_ext_attribute (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version (),
                    this->ir_current_.in (),
                    mode,
                    get_exceptions,
                    set_exceptions
                  );
            }
        }
      else
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_attribute -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_attribute"));

      return -1;
    }

  // Restore entry of component or interface.
  this->ir_current_ =
    ::CORBA::IDLType::_duplicate (holder.in ());

  return 0;
}

int
ifr_adding_visitor::visit_union (AST_Union *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_union visitor (node);
          int retval = visitor.visit_union (node);

          if (retval == 0)
            {
              this->ir_current_ =
                CORBA::IDLType::_duplicate (visitor.ir_current ());
            }

          return retval;
        }
      else
        {
          if (node->ifr_added ())
            {
              // We have visited this node before in the same IDL
              // compilation unit - just update ir_current_.
              this->ir_current_ =
                CORBA::UnionDef::_narrow (prev_def.in ());
            }
          else if (node->ifr_fwd_added ())
            {
              // We are seeing the full definition of a forward
              // declaration - just visit the scope to populate
              // the IFR entry while keeping the same section key
              // so as not to invalidate existing references.
              ifr_adding_visitor_union visitor (node);
              int retval = visitor.visit_union (node);

              if (retval == 0)
                {
                  this->ir_current_ =
                    CORBA::IDLType::_duplicate (visitor.ir_current ());
                }

              return retval;
            }
          else
            {
              // We are clobbering a previous entry
              // from another IDL file. In this case we do what
              // other ORB vendors do - destroy the original
              // entry, create a new one, and let the user beware.
              prev_def->destroy ();
              return this->visit_union (node);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("ifr_adding_visitor::visit_union"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_union_fwd (AST_UnionFwd *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::UnionMemberSeq dummyMembers;
          dummyMembers.length (0);
          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_union_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          CORBA::UnionDef_var union_def =
            current_scope->create_union (
                node->repoID (),
                node->local_name ()->get_string (),
                node->version (),
                CORBA::IDLType::_nil (),
                dummyMembers
              );

          node->full_definition ()->ifr_fwd_added (true);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("ifr_adding_visitor::")
                               ACE_TEXT ("visit_union_fwd"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_constant (AST_Constant *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  const char *id = node->repoID ();

  try
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (id);

      // Nothing prevents this constant's repo id from already being
      // in the repository as another type, if it came from another
      // IDL file whose generated code is not linked to the generated
      // code from this IDL file. So we check here before we make a
      // call on ir_current_.
      if (!CORBA::is_nil (prev_def.in ()))
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (!node->ifr_added ())
            {
              prev_def->destroy ();
            }
          else
            {
              // The node is referenced in an array size, string bound
              // or sequence bound - no action needed in the IFR.
              return 0;
            }
        }

      AST_Expression::ExprType et = node->et ();
      AST_Expression *cv = node->constant_value ();

      /// @@@ (JP 2007-12-12) I've removed code to check
      /// for constants of typedefs. The CORBA document
      /// formal/2004-03-12 (CORBA 3.0.3) says in section
      /// 10.5.8.1 that the type of a constant in the IFR must be
      /// 'one of the primitive types allowed in constant
      /// declarations'. So for constants of typedefs we store
      /// the underlying type in the IFR.

      if (et == AST_Expression::EV_enum)
        {
          // This constant's type is an enum - look up the enum member
          // representing the value, then get the enclosing enum to pass
          // to create_constant().
          AST_Decl *enum_val =
            node->defined_in ()->lookup_by_name (cv->n (), true);
          AST_Decl *d = ScopeAsDecl (enum_val->defined_in ());

          CORBA::Contained_var contained =
            be_global->repository ()->lookup_id (d->repoID ());

          this->ir_current_ = CORBA::IDLType::_narrow (contained.in ());
        }
      else
        {
          // This constant's type is a primitive type - fetch it from the
          // repo and pass it to create_constant().
          CORBA::PrimitiveKind pkind = this->expr_type_to_pkind (et);

          this->ir_current_ =
            be_global->repository ()->get_primitive (pkind);
        }

      CORBA::Any any;
      this->load_any (cv->ev (), any);

      CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          CORBA::ConstantDef_var new_def =
            current_scope->create_constant (
                id,
                node->local_name ()->get_string (),
                node->version (),
                this->ir_current_.in (),
                any
              );
        }
      else
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_constant -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_constant"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_array (AST_Array *node)
{
  try
    {
      bool owned = false;
      AST_Type *bt = node->base_type ();
      UTL_Scope *bts = bt->defined_in ();
      UTL_Scope *ns = node->defined_in ();

      if (bts == ns && !bt->ifr_added ())
        {
          // What we most likely have if we get here is an
          // anonymous array member whose base type is
          // defined as part of the array declaration.
          // Setting the boolean to TRUE and passing it
          // to element_type() will force the base type
          // to be added to the IFR before anything else
          // happens.
          owned = true;
        }

      this->element_type (bt, owned);

      AST_Expression **dims = node->dims ();

      for (unsigned long i = node->n_dims (); i > 0; --i)
        {
          this->ir_current_ =
            be_global->repository ()->create_array (
                dims[i - 1]->ev ()->u.ulval,
                this->ir_current_.in ()
              );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_array"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_sequence (AST_Sequence *node)
{
  try
    {
      this->element_type (node->base_type ());

      this->ir_current_ =
        be_global->repository ()->create_sequence (
                                      node->max_size ()->ev ()->u.ulval,
                                      this->ir_current_.in ()
                                    );
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_sequence"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_string (AST_String *node)
{
  AST_Expression *ex = node->max_size ();

  AST_Expression::AST_ExprValue *ev = ex->ev ();

  CORBA::ULong bound = static_cast<CORBA::ULong> (ev->u.ulval);

  try
    {
      if (node->node_type () == AST_Decl::NT_string)
        {
          this->ir_current_ =
            be_global->repository ()->create_string (bound);
        }
      else
        {
          this->ir_current_ =
            be_global->repository ()->create_wstring (bound);
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_string"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor::visit_typedef (AST_Typedef *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      this->element_type (node->base_type (), node->owns_base_type ());

      CORBA::Container_ptr current_scope =
        CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          this->ir_current_ =
            current_scope->create_alias (
                node->repoID (),
                node->local_name ()->get_string (),
                node->version (),
                this->ir_current_.in ()
              );
        }
      else
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_typedef -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }

      node->ifr_added (true);
    }
  catch (const CORBA::Exception& ex)
    {
      if (!this->allow_duplicate_typedefs_)
        {
          ex._tao_print_exception (ACE_TEXT ("visit_typedef"));

          return -1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              ACE_TEXT ("ifr_adding_visitor::visit_typedef - ignoring duplicate typedef"),
              node->local_name ()->get_string ()));
        }
    }

  return 0;
}

int
ifr_adding_visitor::visit_root (AST_Root *node)
{
  if (be_global->ifr_scopes ().push (be_global->repository ()) != 0)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_root -")
          ACE_TEXT (" scope push failed\n")
        ),
        -1
      );
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_root -")
          ACE_TEXT (" visit_scope failed\n")
        ),
        -1
      );
    }

  CORBA::Container_ptr tmp =
    CORBA::Container::_nil ();

  if (be_global->ifr_scopes ().pop (tmp) != 0)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_root -")
          ACE_TEXT (" scope pop failed\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::visit_native (AST_Native *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  try
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_native (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version ()
                  );
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_native -")
                  ACE_TEXT (" scope stack is empty\n")
                ),
                -1
              );
            }

          node->ifr_added (true);
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (!node->ifr_added ())
            {
              prev_def->destroy ();

              // This call will take the other branch.
              return this->visit_native (node);
            }

          this->ir_current_ =
            CORBA::NativeDef::_narrow (prev_def.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_native"));

      return -1;
    }

  return 0;
}

CORBA::PrimitiveKind
ifr_adding_visitor::expr_type_to_pkind (AST_Expression::ExprType et)
{
  switch (et)
    {
    case AST_Expression::EV_short:
      return CORBA::pk_short;
    case AST_Expression::EV_ushort:
      return CORBA::pk_ushort;
    case AST_Expression::EV_long:
      return CORBA::pk_long;
    case AST_Expression::EV_ulong:
      return CORBA::pk_ulong;
    case AST_Expression::EV_longlong:
      return CORBA::pk_longlong;
    case AST_Expression::EV_ulonglong:
      return CORBA::pk_ulonglong;
    case AST_Expression::EV_float:
      return CORBA::pk_float;
    case AST_Expression::EV_double:
      return CORBA::pk_double;
    case AST_Expression::EV_longdouble:
      return CORBA::pk_longdouble;
    case AST_Expression::EV_char:
      return CORBA::pk_char;
    case AST_Expression::EV_wchar:
      return CORBA::pk_wchar;
    case AST_Expression::EV_octet:
      return CORBA::pk_octet;
    case AST_Expression::EV_bool:
      return CORBA::pk_boolean;
    case AST_Expression::EV_string:
      return CORBA::pk_string;
    case AST_Expression::EV_wstring:
      return CORBA::pk_wstring;
    case AST_Expression::EV_any:
      return CORBA::pk_any;
    case AST_Expression::EV_void:
      return CORBA::pk_void;
    case AST_Expression::EV_none:
      return CORBA::pk_null;
    default:
      return CORBA::pk_null;
    }
}

CORBA::PrimitiveKind
ifr_adding_visitor::predefined_type_to_pkind (AST_PredefinedType *node)
{
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_short:
      return CORBA::pk_short;
    case AST_PredefinedType::PT_ushort:
      return CORBA::pk_ushort;
    case AST_PredefinedType::PT_long:
      return CORBA::pk_long;
    case AST_PredefinedType::PT_ulong:
      return CORBA::pk_ulong;
    case AST_PredefinedType::PT_longlong:
      return CORBA::pk_longlong;
    case AST_PredefinedType::PT_ulonglong:
      return CORBA::pk_ulonglong;
    case AST_PredefinedType::PT_float:
      return CORBA::pk_float;
    case AST_PredefinedType::PT_double:
      return CORBA::pk_double;
    case AST_PredefinedType::PT_longdouble:
      return CORBA::pk_longdouble;
    case AST_PredefinedType::PT_char:
      return CORBA::pk_char;
    case AST_PredefinedType::PT_wchar:
      return CORBA::pk_wchar;
    case AST_PredefinedType::PT_octet:
      return CORBA::pk_octet;
    case AST_PredefinedType::PT_boolean:
      return CORBA::pk_boolean;
    case AST_PredefinedType::PT_any:
      return CORBA::pk_any;
    case AST_PredefinedType::PT_void:
      return CORBA::pk_void;
    case AST_PredefinedType::PT_object:
      return CORBA::pk_objref;
    case AST_PredefinedType::PT_value:
      return CORBA::pk_value_base;
    case AST_PredefinedType::PT_pseudo:
      {
        const char *local_name = node->local_name ()->get_string ();

        if (!ACE_OS::strcmp (local_name, "Principal"))
          {
            return CORBA::pk_Principal;
          }
        else
          {
            return CORBA::pk_TypeCode;
          }
      }
    default:
      return CORBA::pk_null;
    }
}

void
ifr_adding_visitor::load_any (AST_Expression::AST_ExprValue *ev,
                              CORBA::Any &any)
{
  switch (ev->et)
    {
    case AST_Expression::EV_short:
      any <<= ev->u.sval;
      break;
    case AST_Expression::EV_ushort:
      any <<= ev->u.usval;
      break;
    case AST_Expression::EV_long:
      any <<= static_cast<CORBA::Long> (ev->u.lval);
      break;
    case AST_Expression::EV_ulong:
      any <<= static_cast<CORBA::ULong> (ev->u.ulval);
      break;
    case AST_Expression::EV_longlong:
      any <<= ev->u.llval;
      break;
    case AST_Expression::EV_ulonglong:
      any <<= ev->u.ullval;
      break;
    case AST_Expression::EV_float:
      any <<= ev->u.fval;
      break;
    case AST_Expression::EV_double:
      any <<= ev->u.dval;
      break;
#if 0
    case AST_Expression::EV_longdouble:
    // Not implemented in IDL compiler.
#endif
    case AST_Expression::EV_char:
      any <<= CORBA::Any::from_char (ev->u.cval);
      break;
    case AST_Expression::EV_wchar:
      any <<= CORBA::Any::from_wchar (ev->u.wcval);
      break;
    case AST_Expression::EV_octet:
      any <<= CORBA::Any::from_octet (ev->u.oval);
      break;
    case AST_Expression::EV_bool:
      any <<= CORBA::Any::from_boolean ((CORBA::Boolean) ev->u.bval);
      break;
    case AST_Expression::EV_string:
      any <<= ev->u.strval->get_string ();
      break;
    case AST_Expression::EV_wstring:
      {
        char *str = ev->u.wstrval;
        size_t len = ACE_OS::strlen (str);
        CORBA::WChar *wstr = 0;
        ACE_NEW (wstr,
                 CORBA::WChar[len + 1]);

        for (size_t i = 0; i < len; ++i)
          {
            wstr[i] = static_cast<CORBA::WChar> (str[i]);
          }

        wstr[len] = 0;
        any <<= wstr;
        delete [] wstr;
        break;
      }
    case AST_Expression::EV_enum:
      any <<= static_cast<CORBA::ULong> (ev->u.eval);
      break;
    default:
      break;
    }
}

void
ifr_adding_visitor::element_type (AST_Type *base_type, bool owned)
{
  AST_Decl::NodeType nt = base_type->node_type ();
  bool no_repo_id = nt == AST_Decl::NT_array
                    || nt == AST_Decl::NT_sequence
                    || base_type->anonymous ();

  if (no_repo_id || owned)
    {
      if (base_type->ast_accept (this) == -1)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::element_type -")
              ACE_TEXT (" failed to accept visitor\n")
            ));

          throw Bailout ();
        }
    }
  else
    {
      CORBA::Contained_var contained =
        be_global->repository ()->lookup_id (base_type->repoID ());

      if (CORBA::is_nil (contained.in ()))
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::element_type -")
              ACE_TEXT (" lookup_id failed\n")
            ));

          throw Bailout ();
        }

      this->ir_current_ = CORBA::IDLType::_narrow (contained.in ());
    }
}

int
ifr_adding_visitor::create_interface_def (AST_Interface *node)
{
  CORBA::ULong n_parents = static_cast<CORBA::ULong> (node->n_inherits ());
  AST_Type **parents = node->inherits ();
  CORBA::Contained_var result;
  CORBA::AbstractInterfaceDefSeq abs_bases;
  CORBA::InterfaceDefSeq bases;

  if (node->is_abstract ())
    {
      abs_bases.length (n_parents);

      // Construct a list of the parents.
      for (CORBA::ULong i = 0; i < n_parents; ++i)
        {
          AST_Interface *intf =
            AST_Interface::narrow_from_decl (parents[i]);

          if (intf == 0)
            {
              ACE_ERROR_RETURN ((
                 LM_ERROR,
                 ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                 ACE_TEXT ("create_interface_def -")
                 ACE_TEXT (" parent %s is not an interface\n"),
                 parents[i]->full_name ()),
                -1);
            }

          result =
            be_global->repository ()->lookup_id (intf->repoID ());

          // If we got to visit_interface() from a forward declared interface,
          // this node may not yet be in the repository.
          if (CORBA::is_nil (result.in ()))
            {
              int status = this->create_interface_def (intf);

              if (status != 0)
                {
                  ACE_ERROR_RETURN ((
                     LM_ERROR,
                     ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                     ACE_TEXT ("create_interface_def -")
                     ACE_TEXT (" parent interfacedef creation failed\n")),
                    -1);
                }

              bases[i] =
                CORBA::AbstractInterfaceDef::_narrow (this->ir_current_.in ());
            }
          else
            {
              abs_bases[i] =
                CORBA::AbstractInterfaceDef::_narrow (result.in ());
            }

          if (CORBA::is_nil (abs_bases[i]))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("create_interface_def -")
                  ACE_TEXT (" CORBA::InterfaceDef::_narrow failed\n")
                ),
                -1
              );
            }
        }
    }
  else
    {
      bases.length (n_parents);

      // Construct a list of the parents.
      for (CORBA::ULong i = 0; i < n_parents; ++i)
        {
          AST_Interface *intf =
            AST_Interface::narrow_from_decl (parents[i]);

          if (intf == 0)
            {
              ACE_ERROR_RETURN ((
                 LM_ERROR,
                 ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                 ACE_TEXT ("create_interface_def -")
                 ACE_TEXT (" parent %s is not an interface\n"),
                 parents[i]->full_name ()),
                -1);
            }

          result =
            be_global->repository ()->lookup_id (intf->repoID ());

          // If we got to visit_interface() from a forward declared interface,
          // this node may not yet be in the repository.
          if (CORBA::is_nil (result.in ()))
            {
              int status = this->create_interface_def (intf);

              if (status != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("create_interface_def -")
                      ACE_TEXT (" parent interfacedef creation failed\n")
                    ),
                    -1
                  );
                }

              bases[i] = CORBA::InterfaceDef::_narrow (this->ir_current_.in ());
            }
          else
            {
              bases[i] = CORBA::InterfaceDef::_narrow (result.in ());
            }

          if (CORBA::is_nil (bases[i]))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("create_interface_def -")
                  ACE_TEXT (" CORBA::InterfaceDef::_narrow failed\n")
                ),
                -1
              );
            }
        }
    }

  CORBA::Container_ptr current_scope =
    CORBA::Container::_nil ();

  if (be_global->ifr_scopes ().top (current_scope) == 0)
    {
      CORBA::InterfaceDef_var new_def;

      if (node->is_local ())
        {
          new_def =
            current_scope->create_local_interface (
                               node->repoID (),
                               node->local_name ()->get_string (),
                               node->version (),
                               bases
                             );
        }
      else if (node->is_abstract ())
        {
          new_def =
            current_scope->create_abstract_interface (
                               node->repoID (),
                               node->local_name ()->get_string (),
                               node->version (),
                               abs_bases
                             );
        }
      else
        {
          new_def =
            current_scope->create_interface (
                               node->repoID (),
                               node->local_name ()->get_string (),
                               node->version (),
                               bases
                             );
        }


      node->ifr_added (true);

      // Push the new IR object onto the scope stack.
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (new_def.in ());

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_interface_def -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      // Visit the members, if any.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_interface_def -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      // This spot in the AST doesn't necessarily have to be the
      // interface definition - it could be any reference to it.
      // The front end will already have fully defined it, so all
      // the info is available anywhere. So it's a good idea to
      // update the current IR object holder now. This will
      // consume the objref pointer.
      this->ir_current_ =
        CORBA::IDLType::_duplicate (new_def.in ());

      CORBA::Container_ptr used_scope =
        CORBA::Container::_nil ();

      // Pop the new IR object back off the scope stack.
      if (be_global->ifr_scopes ().pop (used_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_interface_def -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::create_interface_def -")
          ACE_TEXT (" scope stack is empty\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::create_value_def (AST_ValueType *node)
{
  CORBA::Container_ptr current_scope =
    CORBA::Container::_nil ();

  if (be_global->ifr_scopes ().top (current_scope) == 0)
    {
      CORBA::ValueDef_var base_value;
      this->fill_base_value (base_value.out (),
                             node);

      CORBA::ValueDefSeq abstract_base_values;
      this->fill_abstract_base_values (abstract_base_values,
                                       node);

      CORBA::InterfaceDefSeq supported_interfaces;
      this->fill_supported_interfaces (supported_interfaces,
                                       node);

      CORBA::ExtInitializerSeq initializers;
      this->fill_initializers (initializers,
                               node);

      CORBA::ExtValueDef_var new_def =
        current_scope->create_ext_value (
                           node->repoID (),
                           node->local_name ()->get_string (),
                           node->version (),
                           static_cast<CORBA::Boolean> (node->custom ()),
                           static_cast<CORBA::Boolean> (node->is_abstract ()),
                           base_value.in (),
                           static_cast<CORBA::Boolean> (node->truncatable ()),
                           abstract_base_values,
                           supported_interfaces,
                           initializers
                         );

      node->ifr_added (true);

      // Push the new IR object onto the scope stack.
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (new_def.in ());

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_value_def -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      // Visit the members, if any.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_value_def -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      // This spot in the AST doesn't necessarily have to be the
      // interface definition - it could be any reference to it.
      // The front end will already have fully defined it, so all
      // the info is available anywhere. So it's a good idea to
      // update the current IR object holder now. This will
      // consume the objref pointer.
      this->ir_current_ =
        CORBA::IDLType::_duplicate (new_def.in ());

      CORBA::Container_ptr used_scope =
        CORBA::Container::_nil ();

      // Pop the new IR object back off the scope stack.
      if (be_global->ifr_scopes ().pop (used_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_value_def -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::create_value_def -")
          ACE_TEXT (" scope stack is empty\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::create_component_def (AST_Component *node)
{
  CORBA::Container_ptr current_scope =
    CORBA::Container::_nil ();

  if (be_global->ifr_scopes ().top (current_scope) == 0)
    {
      CORBA::ComponentIR::ComponentDef_var base_component;
      this->fill_base_component (base_component.out (),
                                 node);

      CORBA::InterfaceDefSeq supported_interfaces;
      this->fill_supported_interfaces (supported_interfaces,
                                       node);

      CORBA::ComponentIR::Container_var ccm_scope =
        CORBA::ComponentIR::Container::_narrow (current_scope);

      CORBA::ComponentIR::ComponentDef_var new_def =
        ccm_scope->create_component (node->repoID (),
                                     node->local_name ()->get_string (),
                                     node->version (),
                                     base_component.in (),
                                     supported_interfaces);

      node->ifr_added (true);

      if (be_global->ifr_scopes ().push (new_def.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_component_def -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      // This spot in the AST doesn't necessarily have to be the
      // interface definition - it could be any reference to it.
      // The front end will already have fully defined it, so all
      // the info is available anywhere. So it's a good idea to
      // update the current IR object holder now. This will
      // consume the objref pointer.
      this->ir_current_ =
        CORBA::IDLType::_duplicate (new_def.in ());

      // Visit the members, if any.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_component_def -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      CORBA::Container_ptr used_scope =
        CORBA::Container::_nil ();

      // Pop the new IR object back off the scope stack.
      if (be_global->ifr_scopes ().pop (used_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_component_def -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::create_component_def -")
          ACE_TEXT (" scope stack is empty\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::create_home_def (AST_Home *node)
{
  CORBA::Container_ptr current_scope =
    CORBA::Container::_nil ();

  if (be_global->ifr_scopes ().top (current_scope) == 0)
    {
      CORBA::ComponentIR::HomeDef_var base_home;
      this->fill_base_home (base_home.out (),
                            node);

      CORBA::ComponentIR::ComponentDef_var managed_component;
      this->fill_managed_component (managed_component.out (),
                                    node);

      CORBA::InterfaceDefSeq supported_interfaces;
      this->fill_supported_interfaces (supported_interfaces,
                                       node);

      CORBA::ValueDef_var primary_key;
      this->fill_primary_key (primary_key.out (),
                              node);

      CORBA::ComponentIR::Container_var ccm_scope =
        CORBA::ComponentIR::Container::_narrow (current_scope);

      CORBA::ComponentIR::HomeDef_var new_def =
        ccm_scope->create_home (node->repoID (),
                                node->local_name ()->get_string (),
                                node->version (),
                                base_home.in (),
                                managed_component.in (),
                                supported_interfaces,
                                primary_key.in ());

      node->ifr_added (true);

      // Push the new IR object onto the scope stack.
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (new_def.in ());

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_home_def -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      // Visit the members, if any.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_home_def -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      // Get the contents of these lists into the repository.

      this->visit_all_factories (node,
                                 new_def.in ());

      this->visit_all_finders (node,
                               new_def.in ());

      // This spot in the AST doesn't necessarily have to be the
      // interface definition - it could be any reference to it.
      // The front end will already have fully defined it, so all
      // the info is available anywhere. So it's a good idea to
      // update the current IR object holder now. This will
      // consume the objref pointer.
      this->ir_current_ =
        CORBA::IDLType::_duplicate (new_def.in ());

      CORBA::Container_ptr used_scope =
        CORBA::Container::_nil ();

      // Pop the new IR object back off the scope stack.
      if (be_global->ifr_scopes ().pop (used_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_home_def -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::create_home_def -")
          ACE_TEXT (" scope stack is empty\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::create_event_def (AST_EventType *node)
{
  CORBA::Container_ptr current_scope =
    CORBA::Container::_nil ();

  if (be_global->ifr_scopes ().top (current_scope) == 0)
    {
      CORBA::ValueDef_var base_value;
      this->fill_base_value (base_value.out (),
                             node);

      CORBA::ValueDefSeq abstract_base_values;
      this->fill_abstract_base_values (abstract_base_values,
                                       node);

      CORBA::InterfaceDefSeq supported_interfaces;
      this->fill_supported_interfaces (supported_interfaces,
                                       node);

      CORBA::ExtInitializerSeq initializers;
      this->fill_initializers (initializers,
                               node);

      CORBA::ComponentIR::Container_var ccm_scope =
        CORBA::ComponentIR::Container::_narrow (current_scope);

      CORBA::ExtValueDef_var new_def =
        ccm_scope->create_event (
                       node->repoID (),
                       node->local_name ()->get_string (),
                       node->version (),
                       static_cast<CORBA::Boolean> (node->custom ()),
                       static_cast<CORBA::Boolean> (node->is_abstract ()),
                       base_value.in (),
                       static_cast<CORBA::Boolean> (node->truncatable ()),
                       abstract_base_values,
                       supported_interfaces,
                       initializers
                     );

      node->ifr_added (true);

      // Push the new IR object onto the scope stack.
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (new_def.in ());

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_event_def -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      // Visit the members, if any.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_event_def -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      // This spot in the AST doesn't necessarily have to be the
      // interface definition - it could be any reference to it.
      // The front end will already have fully defined it, so all
      // the info is available anywhere. So it's a good idea to
      // update the current IR object holder now. This will
      // consume the objref pointer.
      this->ir_current_ =
        CORBA::IDLType::_duplicate (new_def.in ());

      CORBA::Container_ptr used_scope =
        CORBA::Container::_nil ();

      // Pop the new IR object back off the scope stack.
      if (be_global->ifr_scopes ().pop (used_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_event_def -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::create_event_def -")
          ACE_TEXT (" scope stack is empty\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::create_value_member (AST_Field *node)
{
  try
    {
      AST_Type *bt = node->field_type ();
      AST_Decl::NodeType nt = bt->node_type ();

      // We can't use lookup_id() on these, because
      // they don't inherit from Contained.
      if (nt == AST_Decl::NT_pre_defined
          || nt == AST_Decl::NT_string
          || nt == AST_Decl::NT_wstring
          || nt == AST_Decl::NT_array
          || nt == AST_Decl::NT_sequence)
        {
          /// This will put the repo entry into ir_current_.
          if (bt->ast_accept (this) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("create_value_member -")
                  ACE_TEXT (" visit base type failed\n")
                ),
                -1
              );
            }
        }
      else
        {
          // If the IDL is legal, this will succeed.
          CORBA::Contained_var holder =
                be_global->repository ()->lookup_id (bt->repoID ());

          this->ir_current_ =
            CORBA::IDLType::_narrow (holder.in ());
        }

      CORBA::Visibility vis = CORBA::PUBLIC_MEMBER;

      switch (node->visibility ())
        {
          case AST_Field::vis_PUBLIC:
            break;
          case AST_Field::vis_PRIVATE:
            vis = CORBA::PRIVATE_MEMBER;
            break;
          default:
            ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor::create_value_member -")
                ACE_TEXT (" bad visibility value in node\n")
              ),
              -1
            );
        };

      CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("create_value_member -")
              ACE_TEXT (" scope stack empty\n")
            ),
            -1
          );
        }

      CORBA::ValueDef_var vt =
        CORBA::ValueDef::_narrow (current_scope);

      CORBA::ValueMemberDef_var vm =
        vt->create_value_member (node->repoID (),
                                 node->local_name ()->get_string (),
                                 node->version (),
                                 this->ir_current_.in (),
                                 vis);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("create_value_member"));

      return -1;
    }

  return 0;
}

void
ifr_adding_visitor::get_referenced_type (AST_Type *node)
{
  switch (node->node_type ())
  {
    // For anonymous types, a new IR object is be created each
    // time, so we just visit the node, which get the object and
    // updates ir_current_.
    case AST_Decl::NT_pre_defined:
    case AST_Decl::NT_string:
    case AST_Decl::NT_wstring:
    case AST_Decl::NT_array:
    case AST_Decl::NT_sequence:
      if (node->ast_accept (this) == -1)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("get_scope_member -")
              ACE_TEXT (" failed to accept visitor\n")
            ));
        }

      break;
    // For names types, we can just look up the entry and update
    // ir_current_.
    default:
      {
        CORBA::Contained_var prev_def =
          be_global->repository ()->lookup_id (node->repoID ());

        this->ir_current_ =
          CORBA::IDLType::_narrow (prev_def.in ());
        break;
      }
  }
}

void
ifr_adding_visitor::fill_base_value (CORBA::ValueDef_ptr &result,
                                     AST_ValueType *node)
{
  result = CORBA::ValueDef::_nil ();
  AST_Type *base_value = node->inherits_concrete ();

  if (base_value == 0)
    {
      return;
    }

  CORBA::Contained_var holder =
    be_global->repository ()->lookup_id (
                                  base_value->repoID ()
                                );

  if (!CORBA::is_nil (holder.in ()))
    {
      result =
        CORBA::ValueDef::_narrow (holder.in ());
    }
}

void
ifr_adding_visitor::fill_base_component (
    CORBA::ComponentIR::ComponentDef_ptr &result,
    AST_Component *node
  )
{
  result = CORBA::ComponentIR::ComponentDef::_nil ();
  AST_Component *base_component = node->base_component ();

  if (base_component == 0)
    {
      return;
    }

  CORBA::Contained_var holder =
    be_global->repository ()->lookup_id (
                                  base_component->repoID ()
                                );

  if (!CORBA::is_nil (holder.in ()))
    {
      result =
        CORBA::ComponentIR::ComponentDef::_narrow (holder.in ());
    }
}

void
ifr_adding_visitor::fill_base_home (CORBA::ComponentIR::HomeDef_ptr &result,
                                    AST_Home *node)
{
  result = CORBA::ComponentIR::HomeDef::_nil ();
  AST_Home *base_home = node->base_home ();

  if (base_home == 0)
    {
      return;
    }

  CORBA::Contained_var holder =
    be_global->repository ()->lookup_id (
                                  base_home->repoID ()
                                );

  if (!CORBA::is_nil (holder.in ()))
    {
      result =
        CORBA::ComponentIR::HomeDef::_narrow (holder.in ());
    }
  else
    {
      /// Maybe the base home is in an included IDL file - put it in
      /// the repository and go again.
      (void) base_home->ast_accept (this);
      this->fill_base_home (result,
                            node);
    }
}

void
ifr_adding_visitor::fill_managed_component (
    CORBA::ComponentIR::ComponentDef_ptr &result,
    AST_Home *node
  )
{
  result = CORBA::ComponentIR::ComponentDef::_nil ();
  AST_Component *managed_component = node->managed_component ();

  if (managed_component == 0)
    {
      return;
    }

  CORBA::Contained_var holder =
    be_global->repository ()->lookup_id (managed_component->repoID ());

  if (!CORBA::is_nil (holder.in ()))
    {
      result =
        CORBA::ComponentIR::ComponentDef::_narrow (holder.in ());
    }
  else
    {
      /// Maybe the managed component is in an included IDL file - put it in
      /// the repository and go again.
      (void) managed_component->ast_accept (this);
      this->fill_managed_component (result,
                                    node);
    }
}

void
ifr_adding_visitor::fill_primary_key (CORBA::ValueDef_ptr &result,
                                      AST_Home *node)
{
  result = CORBA::ValueDef::_nil ();
  AST_Type *primary_key = node->primary_key ();

  if (primary_key == 0)
    {
      return;
    }

  CORBA::Contained_var holder =
    be_global->repository ()->lookup_id (primary_key->repoID ());

  if (!CORBA::is_nil (holder.in ()))
    {
      result =
        CORBA::ValueDef::_narrow (holder.in ());
    }
  else
    {
      /// Maybe the primary key is in an included IDL file - put it in
      /// the repository and go again.
      (void) primary_key->ast_accept (this);
      this->fill_primary_key (result,
                              node);
    }
}

void
ifr_adding_visitor::fill_abstract_base_values (CORBA::ValueDefSeq &result,
                                               AST_ValueType *node)
{
  CORBA::Long s_length = node->n_inherits ();
  result.length (0);

  // Not sure if this could be negative in some default case or
  // not. If it's 0, we should make the call anyway to clear
  // existing entries, if any,  from the repository.
  if (s_length > 0)
    {
      AST_Type **list = node->inherits ();
      CORBA::ULong u_length = static_cast<CORBA::ULong> (s_length);
      bool first_abs = list[0]->is_abstract ();
      result.length (first_abs ? u_length : u_length - 1);

      for (CORBA::ULong i = 0; i < u_length; ++i)
        {
          if (i == 0 && ! first_abs)
            {
              continue;
            }

          // Get list[i] into ir_current_.
          (void) list[i]->ast_accept (this);

          result[first_abs ? i : i - 1] =
            CORBA::ValueDef::_narrow (this->ir_current_.in ());
        }
    }
}

void
ifr_adding_visitor::fill_inherited_interfaces (CORBA::InterfaceDefSeq &result,
                                               AST_Interface *node)
{
  result.length (0);
  this->fill_interfaces (result,
                         node->inherits (),
                         node->n_inherits ());
}

void
ifr_adding_visitor::fill_supported_interfaces (CORBA::InterfaceDefSeq &result,
                                               AST_Interface *node)
{
  result.length (0);
  CORBA::Long s_length = 0;
  AST_Type **list = 0;

  switch (node->node_type ())
    {
      case AST_Decl::NT_valuetype:
      case AST_Decl::NT_eventtype:
        {
          AST_ValueType *v = AST_ValueType::narrow_from_decl (node);
          s_length = v->n_supports ();
          list = v->supports ();
          break;
        }
      case AST_Decl::NT_component:
        {
          AST_Component *c = AST_Component::narrow_from_decl (node);
          s_length = c->n_supports ();
          list = c->supports ();
          break;
        }
      case AST_Decl::NT_home:
        {
          AST_Home *h = AST_Home::narrow_from_decl (node);
          s_length = h->n_supports ();
          list = h->supports ();
          break;
        }
      default:
        return;
    }

  this->fill_interfaces (result,
                         list,
                         s_length);
}

void
ifr_adding_visitor::fill_interfaces (CORBA::InterfaceDefSeq &result,
                                     AST_Type **list,
                                     CORBA::Long length)
{
  // Not sure if this could be negative in some default case or
  // not. If it's 0, we should make the call anyway to clear
  // existing entries, if any,  from the repository.
  if (length > 0)
    {
      CORBA::ULong u_length = static_cast<CORBA::ULong> (length);
      result.length (u_length);

      for (CORBA::ULong i = 0; i < u_length; ++i)
        {
          // Get list[i] into ir_current_.
          (void) list[i]->ast_accept (this);

          result[i] =
            CORBA::InterfaceDef::_narrow (this->ir_current_.in ());
        }
    }
}

void
ifr_adding_visitor::fill_initializers (CORBA::ExtInitializerSeq &result,
                                       AST_ValueType *node)
{
  result.length (0);
  AST_Decl *item = 0;
  ACE_Vector<AST_Factory *> factories;

  for (UTL_ScopeActiveIterator v_iter (node,
                                       UTL_Scope::IK_decls);
       !v_iter.is_done ();
       v_iter.next ())
    {
      item = v_iter.item ();

      if (item->node_type () == AST_Decl::NT_factory)
        {
          factories.push_back (AST_Factory::narrow_from_decl (item));
        }
    }

  CORBA::ULong n_factories = factories.size ();

  if (n_factories == 0)
    {
      return;
    }

  result.length (n_factories);
  CORBA::ULong n_args = 0;
  AST_Argument *arg = 0;
  CORBA::ULong index = 0;
  AST_Exception *excp = 0;
  CORBA::Contained_var holder;

  for (CORBA::ULong i = 0; i < n_factories; ++i)
    {
      result[i].name =
        CORBA::string_dup (factories[i]->local_name ()->get_string ());
      n_args = static_cast<CORBA::ULong> (factories[i]->argument_count ());
      result[i].members.length (n_args);

      // The factory should have nothing in its scope but args.
      for (UTL_ScopeActiveIterator f_iter (factories[i],
                                           UTL_Scope::IK_decls);
           !f_iter.is_done ();
           f_iter.next (), ++index)
        {
          arg = AST_Argument::narrow_from_decl (f_iter.item ());
          result[i].members[index].name =
            CORBA::string_dup (arg->local_name ()->get_string ());
          result[i].members[index].type =
            CORBA::TypeCode::_duplicate (CORBA::_tc_void);

          /// This will put the field type in ir_current_, and also add it
          /// to the repository if it's not already there.
          if (arg->field_type ()->ast_accept (this) != 0)
            {
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("fill_initializers -")
                  ACE_TEXT (" failed to accept arg type visitor\n")
                ));
            }

          result[i].members[index].type_def =
            CORBA::IDLType::_duplicate (this->ir_current_.in ());
        }

      CORBA::ULong n_exceptions =
        static_cast<CORBA::ULong> (factories[i]->n_exceptions ());
      result[i].exceptions.length (n_exceptions);
      index = 0;

      for (UTL_ExceptlistActiveIterator ei (factories[i]->exceptions ());
           !ei.is_done ();
           ei.next ())
        {
          excp = AST_Exception::narrow_from_decl (ei.item ());
          result[i].exceptions[index].name =
            CORBA::string_dup (excp->local_name ()->get_string ());
          result[i].exceptions[index].id = excp->repoID ();
          result[i].exceptions[index].defined_in =
            ScopeAsDecl (excp->defined_in ())->repoID ();
          result[i].exceptions[index].version = excp->version ();
          result[i].exceptions[index++].type =
            CORBA::TypeCode::_duplicate (CORBA::_tc_void);
        }
    }
}

void
ifr_adding_visitor::fill_get_exceptions (CORBA::ExceptionDefSeq &result,
                                         AST_Attribute *node)
{
  this->fill_exceptions (result,
                         node->get_get_exceptions ());
}

void
ifr_adding_visitor::fill_set_exceptions (CORBA::ExceptionDefSeq &result,
                                         AST_Attribute *node)
{
  this->fill_exceptions (result,
                         node->get_set_exceptions ());
}

void
ifr_adding_visitor::fill_exceptions (CORBA::ExceptionDefSeq &result,
                                     AST_Decl *node)
{
  switch (node->node_type ())
    {
      case AST_Decl::NT_op:
        {
          AST_Operation *op = AST_Operation::narrow_from_decl (node);
          this->fill_exceptions (result,
                                 op->exceptions ());
          return;
        }
      case AST_Decl::NT_factory:
      case AST_Decl::NT_finder:
        {
          AST_Factory *f = AST_Factory::narrow_from_decl (node);
          this->fill_exceptions (result,
                                 f->exceptions ());
          return;
        }
      default:
        result.length (0);
        return;
    }
}

void
ifr_adding_visitor::fill_exceptions (CORBA::ExceptionDefSeq &result,
                                     UTL_ExceptList *list)
{
  if (list == 0)
    {
      result.length (0);
      return;
    }

  result.length (static_cast<CORBA::ULong> (list->length ()));
  CORBA::ULong index = 0;
  AST_Decl *d = 0;
  CORBA::Contained_var holder;

  for (UTL_ExceptlistActiveIterator ei (list);
       !ei.is_done ();
       ei.next (), ++index)
    {
      d = ei.item ();

      // Just to make sure. The call will return quickly if d has already
      // been visited. Can't use ir_current_ because ExceptionDef doesn't
      // inherit from IDLType.
      (void) d->ast_accept (this);

      holder = be_global->repository ()->lookup_id (d->repoID ());

      result[index] =
        CORBA::ExceptionDef::_narrow (holder.in ());
    }
}

void
ifr_adding_visitor::fill_params (CORBA::ParDescriptionSeq &result,
                                 UTL_Scope *node)
{
  AST_Argument *arg = 0;
  CORBA::ULong n_args = static_cast<CORBA::ULong> (node->nmembers ());
  result.length (n_args);
  CORBA::ULong index = 0;
  CORBA::Contained_var holder;

  for (UTL_ScopeActiveIterator iter (node,
                                     UTL_Scope::IK_decls);
       ! iter.is_done ();
       iter.next (), ++index)
    {
      arg = AST_Argument::narrow_from_decl (iter.item ());
      result[index].name =
        CORBA::string_dup (arg->local_name ()->get_string ());
      result[index].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

      // Get the arg type into ir_current_.
      (void) arg->ast_accept (this);

      result[index].type_def =
        CORBA::IDLType::_duplicate (this->ir_current_.in ());

      result[index].mode = CORBA::PARAM_IN;
    }
}

void
ifr_adding_visitor::visit_all_factories (AST_Home *node,
                                         CORBA::ComponentIR::HomeDef_ptr h)
{
  CORBA::Contained_var contained;
  CORBA::ComponentIR::FactoryDef_var new_def;

  for (UTL_ScopeActiveIterator h_iter (node,
                                       UTL_Scope::IK_decls);
       !h_iter.is_done ();
       h_iter.next ())
    {
      AST_Decl *d = h_iter.item ();
      AST_Decl::NodeType nt = d->node_type ();

      if (nt != AST_Decl::NT_factory)
        {
          continue;
        }

      AST_Factory *f = AST_Factory::narrow_from_decl (d);

      CORBA::ParDescriptionSeq params;
      this->fill_params (params, f);

      CORBA::ExceptionDefSeq exceptions;
      this->fill_exceptions (exceptions, f);

      new_def = h->create_factory (f->repoID (),
                                   f->local_name ()->get_string (),
                                   f->version (),
                                   params,
                                   exceptions);
    }
}

void
ifr_adding_visitor::visit_all_finders (AST_Home *node,
                                       CORBA::ComponentIR::HomeDef_ptr h)
{
  AST_Finder *f = 0;
  CORBA::Contained_var contained;
  CORBA::ComponentIR::FinderDef_var new_def;

  for (UTL_ScopeActiveIterator h_iter (node,
                                       UTL_Scope::IK_decls);
       !h_iter.is_done ();
       h_iter.next ())
    {
      f = AST_Finder::narrow_from_decl (h_iter.item ());

      if (f == 0)
        {
          continue;
        }

      CORBA::ParDescriptionSeq params;
      this->fill_params (params, f);

      CORBA::ExceptionDefSeq exceptions;
      this->fill_exceptions (exceptions, f);

      new_def = h->create_finder (f->repoID (),
                                  f->local_name ()->get_string (),
                                  f->version (),
                                  params,
                                  exceptions);
    }
}

void
ifr_adding_visitor::expand_id (ACE_CString &str,
                               const char *local_name)
{
  ssize_t pos = str.rfind (':');
  str = str.substr (0, pos) + '/' + local_name + str.substr (pos);
}
