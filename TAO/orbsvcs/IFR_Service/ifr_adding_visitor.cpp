/* -*- c++ -*- */
// $Id$

#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_constant.h"
#include "ast_enum.h"
#include "ast_exception.h"
#include "ast_expression.h"
#include "ast_field.h"
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
#include "ast_union.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "utl_identifier.h"
#include "utl_string.h"

#include "ifr_adding_visitor.h"
#include "ifr_adding_visitor_operation.h"
#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_exception.h"
#include "ifr_adding_visitor_union.h"

#include "tao/IFR_Client/IFR_ComponentsC.h"

ACE_RCSID (IFR_Service,
           ifr_adding_visitor,
           "$Id$")

ifr_adding_visitor::ifr_adding_visitor (AST_Decl *scope,
                                        CORBA::Boolean in_reopened)
  : scope_ (scope),
    in_reopened_ (in_reopened)
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->ir_current_ =
      be_global->repository ()->get_primitive (
                                    this->predefined_type_to_pkind (node)
                                    ACE_ENV_ARG_PARAMETER
                                  );
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_predefined_type"));

      return -1;
    }
  ACE_ENDTRY;

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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // If this module been opened before, it will already be in
      // the repository.
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          // New module.
          CORBA::Container_ptr container = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (container) == 0)
            {
              new_def = container->create_module (
                                       node->repoID (),
                                       node->local_name ()->get_string (),
                                       node->version ()
                                       ACE_ENV_ARG_PARAMETER
                                     );
              ACE_TRY_CHECK;
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
          CORBA::DefinitionKind kind = prev_def->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // If the line below is true, we are clobbering a previous
          // entry from another IDL file. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (kind != CORBA::dk_Module)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the brach where prev_def.in() is 0.
              return this->visit_module (node);
            }
          else
            {
              // We are either in a reopened module, are processing an IDL
              // IDL file for the second time, or are in a module whose
              // name already exists by coincidence - there is no way to
              // tell the difference. So any members whose repository ID
              // already exists in this case will be skipped.
              this->in_reopened_ = 1;

              new_def =
                CORBA::Container::_narrow (prev_def.in ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
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

      this->in_reopened_ = 0;
      CORBA::Container_ptr tmp = CORBA::Container::_nil ();

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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_module"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_interface (AST_Interface *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_interface_def (node
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined ()
              && node->ifr_added () == 0
              && this->in_reopened_ == 0)
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declared interface
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of an
              //    interface or of some other type.
              // 3. We are inside a module that has a previous entry.
              // If prev_def would narrow successfully to an InterfaceDef, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT an interface, we can
              // safely destroy it, since we know we are not redefining a
              // previous entry, forward declared or not.
              // If we are inside a module that was seen before, we could be
              // just processing an IDL file a second time, in which case we
              // again just update ir_current_.
              if (node->ifr_fwd_added () == 0)
                {
                  CORBA::DefinitionKind kind =
                    prev_def->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  if (kind == CORBA::dk_Interface)
                    {
                      CORBA::InterfaceDef_var iface =
                        CORBA::InterfaceDef::_narrow (prev_def.in ()
                                                      ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      CORBA::ContainedSeq_var contents =
                        iface->contents (CORBA::dk_all,
                                         1
                                         ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      CORBA::ULong length = contents->length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          contents[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
                          ACE_TRY_CHECK;
                        }
                    }
                  else
                    {
                      prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      int status = this->create_interface_def (node
                                                               ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      return status;
                    }
                }

              CORBA::ULong n_parents = ACE_static_cast (CORBA::ULong,
                                                        node->n_inherits ());

              CORBA::InterfaceDefSeq bases (n_parents);
              bases.length (n_parents);
              CORBA::Contained_var result;

              AST_Interface **parents = node->inherits ();

              // Construct a list of the parents.
              for (CORBA::ULong i = 0; i < n_parents; ++i)
                {
                  result =
                    be_global->repository ()->lookup_id (parents[i]->repoID ()
                                                         ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  // If one of our interface's parents is not in the repository,
                  // that means that it has not yet been seen (even as a
                  // forward declaration) in the IDL file, and we will have to
                  // postpone the populating of our interface until they are all
                  // added.
                  if (CORBA::is_nil (result.in ()))
                    {
                      this->ir_current_ =
                        CORBA::IDLType::_narrow (prev_def.in ()
                                                ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK

                      return 0;
                    }

                  bases[i] = CORBA::InterfaceDef::_narrow (result.in ()
                                                          ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  if (CORBA::is_nil (bases[i].in ()))
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
                CORBA::InterfaceDef::_narrow (prev_def. in ()
                                              ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              extant_def->base_interfaces (bases
                                           ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK

              node->ifr_added (1);

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

              CORBA::Container_ptr used_scope = CORBA::Container::_nil ();

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
                CORBA::IDLType::_narrow (prev_def.in ()
                                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_interface"));

      return -1;
    }
  ACE_ENDTRY;

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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (i->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          // If our full definition is found in this IDL file, we go
          // ahead and create the full entry now.
          // The forward declared interface is not defined anywhere
          // in this IDL file, so we just create an empty entry to
          // be replaced by a full definition in some other IDL file.
          CORBA::InterfaceDefSeq bases (0);
          bases.length (0);

          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

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
                                       ACE_ENV_ARG_PARAMETER
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
                                       ACE_ENV_ARG_PARAMETER
                                     );
                }

              ACE_TRY_CHECK;
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

          node->ifr_added (1);
          i->ifr_fwd_added (1);
        }

   }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_interface_fwd"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_valuetype (AST_ValueType *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          int status = this->create_value_def (node
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          return status;
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object
          // holder.
          if (node->is_defined ()
              && node->ifr_added () == 0
              && this->in_reopened_ == 0)
            {
              // If we are here and the line below is true, then either
              // 1. We are defining an undefined forward declared interface
              //    from a previously processed IDL file, or
              // 2. We are clobbering a previous definition, either of an
              //    interface or of some other type.
              // 3. We are inside a module that has a previous entry.
              // If prev_def would narrow successfully to an InterfaceDef, we
              // have NO WAY of knowing if we are defining or clobbering. So
              // we destroy the contents of the previous entry (we don't want
              // to destroy the entry itself, since it may have already been
              // made a member of some other entry, and destroying it would
              // make the containing entry's section key invalid) and repopulate.
              // On the other hand, if prev_def is NOT an interface, we can
              // safely destroy it, since we know we are not redefining a
              // previous entry, forward declared or not.
              // If we are inside a module that was seen before, we could be
              // just processing an IDL file a second time, in which case we
              // again just update ir_current_.
              if (node->ifr_fwd_added () == 0)
                {
                  CORBA::DefinitionKind kind =
                    prev_def->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  if (kind == CORBA::dk_Value)
                    {
                      CORBA::ValueDef_var value =
                        CORBA::ValueDef::_narrow (prev_def.in ()
                                                  ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      CORBA::ContainedSeq_var contents =
                        value->contents (CORBA::dk_all,
                                         1
                                         ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      CORBA::ULong length = contents->length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          contents[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
                          ACE_TRY_CHECK;
                        }
                    }
                  else
                    {
                      prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      int status = this->create_value_def (node
                                                           ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      return status;
                    }
                }

              // Our previous definition is a valuetype, so narrow it here, then
              // populate it.
              CORBA::ValueDef_var extant_def =
                CORBA::ValueDef::_narrow (prev_def. in ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

             // TODO - Add everything except visit_scope() info here.

              node->ifr_added (1);

              // Push the new IR object onto the scope stack before visiting
              // the new object's scope.
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

              CORBA::Container_ptr used_scope = CORBA::Container::_nil ();

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
              // not defined/not added - visit_valuetype_fwd will have
              //                         detected a clobber.
              // not defined/added - not possible.
              // defined/not added - takes the other branch.
              // defined/added - we're ok.
              this->ir_current_ = 
                CORBA::IDLType::_narrow (prev_def.in ()
                                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_interface"));

      return -1;
    }
  ACE_ENDTRY;

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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this interface already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (v->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_value (
                                   v->repoID (),
                                   v->local_name ()->get_string (),
                                   v->version (),
                                   0, // 'custom' not handled yet
                                   v->is_abstract (),
                                   CORBA::ValueDef::_nil (),
                                   0, // 'truncatable' not handled yet
                                   abstract_bases,
                                   supported,
                                   initializers
                                   ACE_ENV_ARG_PARAMETER
                                 );

              ACE_TRY_CHECK;
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

          node->ifr_added (1);
          v->ifr_fwd_added (1);
        }

   }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_interface_fwd"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_structure (AST_Structure *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_structure visitor (node,
                                                0);

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
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          // Unless we are in a module that has been seen before, in
          // which case we might be just processing and IDL file a
          // second time and we want to just update ir_current_.
          if (node->ifr_added () == 0 && this->in_reopened_ == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              return this->visit_structure (node);
            }

          this->ir_current_ =
            CORBA::IDLType::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_adding_visitor::visit_structure"));

      return -1;
    }
  ACE_ENDTRY;

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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this enum already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::ULong member_count =
            ACE_static_cast (CORBA::ULong,
                             node->member_count ());

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

          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_enum (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version (),
                    members
                    ACE_ENV_ARG_PARAMETER
                  );
              ACE_TRY_CHECK;
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

          node->ifr_added (1);
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          // Unless we are in a module that has been seen before, in
          // which case we might be just processing and IDL file a
          // second time and we want to just update ir_current_.
          if (node->ifr_added () == 0 && this->in_reopened_ == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              return this->visit_enum (node);
            }

          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ =
            CORBA::EnumDef::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_adding_visitor::visit_enum"));

      return -1;
    }
  ACE_ENDTRY;

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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      AST_Type *type = node->field_type ();

      // Updates ir_current_.
      this->get_referenced_type (type
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::AttributeMode mode =
        node->readonly () ? CORBA::ATTR_READONLY : CORBA::ATTR_NORMAL;

      CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          CORBA::InterfaceDef_var iface =
            CORBA::InterfaceDef::_narrow (current_scope
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::AttributeDef_var new_def =
            iface->create_attribute (node->repoID (),
                                     node->local_name ()->get_string (),
                                     node->version (),
                                     this->ir_current_.in (),
                                     mode
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_attribute"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_union (AST_Union *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_union visitor (node,
                                            0);

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
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          // Unless we are in a module that has been seen before, in
          // which case we might be just processing and IDL file a
          // second time and we want to just update ir_current_.
          if (node->ifr_added () == 0 && this->in_reopened_ == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              return this->visit_union (node);
            }

          this->ir_current_ =
            CORBA::UnionDef::_narrow (prev_def.in ()
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_adding_visitor::visit_union"));

      return -1;
    }
  ACE_ENDTRY;

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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (id
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
          // Unless we are in a module that has been seen before, in
          // which case we might be just processing and IDL file a
          // second time and we want to just update ir_current_.
          if (node->ifr_added () == 0 && this->in_reopened_ == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              // The node is referenced in an array size, string bound
              // or sequence bound - no action needed in the IFR.
              return 0;
            }
        }

      AST_Expression::AST_ExprValue *ev = node->constant_value ()->ev ();
      AST_Decl *td = node->constant_value ()->get_tdef ();

      if (td != 0 && td->node_type () == AST_Decl::NT_typedef)
        {
          // This constant's type is a typedef - look up the typedef to
          // pass to create_constant().
          CORBA::Contained_var contained =
            be_global->repository ()->lookup_id (td->repoID ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->ir_current_ = CORBA::IDLType::_narrow (contained.in ()
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          CORBA::PrimitiveKind pkind = this->expr_type_to_pkind (ev->et);
          this->ir_current_ =
            be_global->repository ()->get_primitive (pkind
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      CORBA::Any any;
      this->load_any (ev,
                      any);

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
                ACE_ENV_ARG_PARAMETER
              );
          ACE_TRY_CHECK;
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_constant"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_array (AST_Array *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->element_type (node->base_type ()
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      AST_Expression **dims = node->dims ();

      for (unsigned long i = node->n_dims (); i > 0; --i)
        {
          this->ir_current_ =
            be_global->repository ()->create_array (
                dims[i - 1]->ev ()->u.ulval,
                this->ir_current_.in ()
                ACE_ENV_ARG_PARAMETER
              );
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_array"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_sequence (AST_Sequence *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->element_type (node->base_type ()
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->ir_current_ =
        be_global->repository ()->create_sequence (
                                      node->max_size ()->ev ()->u.ulval,
                                      this->ir_current_.in ()
                                      ACE_ENV_ARG_PARAMETER
                                    );
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_sequence"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_string (AST_String *node)
{
  AST_Expression *ex = node->max_size ();

  AST_Expression::AST_ExprValue *ev = ex->ev ();

  CORBA::ULong bound = ACE_static_cast (CORBA::ULong, ev->u.ulval);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (node->node_type () == AST_Decl::NT_string)
        {
          this->ir_current_ =
            be_global->repository ()->create_string (bound
                                                     ACE_ENV_ARG_PARAMETER);
        }
      else
        {
          this->ir_current_ =
            be_global->repository ()->create_wstring (bound
                                                      ACE_ENV_ARG_PARAMETER);
        }

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_string"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_typedef (AST_Typedef *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this typedef already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          this->element_type (node->base_type ()
                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_alias (
                   node->repoID (),
                   node->local_name ()->get_string (),
                   node->version (),
                   this->ir_current_.in ()
                   ACE_ENV_ARG_PARAMETER
                 );
              ACE_TRY_CHECK;
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

          node->ifr_added (1);
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          // Unless we are in a module that has been seen before, in
          // which case we might be just processing and IDL file a
          // second time and we want to just update ir_current_.
          if (node->ifr_added () == 0 && this->in_reopened_ == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_typedef (node);
            }

          this->ir_current_ =
            CORBA::TypedefDef::_narrow (prev_def.in ()
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_typedef"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_root (AST_Root *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (be_global->repository ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
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

      CORBA::Container_ptr tmp = CORBA::Container::_nil ();

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
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_root"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor::visit_native (AST_Native *node)
{
  if (node->imported () && !be_global->do_included_files ())
    {
      return 0;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_native (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version ()
                    ACE_ENV_ARG_PARAMETER
                  );
              ACE_TRY_CHECK;
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

          node->ifr_added (1);
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          // Unless we are in a module that has been seen before, in
          // which case we might be just processing and IDL file a
          // second time and we want to just update ir_current_.
          if (node->ifr_added () == 0 && this->in_reopened_ == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_native (node);
            }

          this->ir_current_ =
            CORBA::NativeDef::_narrow (prev_def.in ()
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_native"));

      return -1;
    }
  ACE_ENDTRY;

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
      any <<= ACE_static_cast (CORBA::Long, ev->u.lval);
      break;
    case AST_Expression::EV_ulong:
      any <<= ACE_static_cast (CORBA::ULong, ev->u.ulval);
      break;
#if !defined (ACE_LACKS_LONGLONG_T)
    case AST_Expression::EV_longlong:
      any <<= ev->u.llval;
      break;
    case AST_Expression::EV_ulonglong:
      any <<= ev->u.ullval;
      break;
#endif /* !defined (ACE_LACKS_LONGLONG_T) */
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
            wstr[i] = ACE_static_cast (CORBA::WChar, str[i]);
          }

        wstr[len] = 0;
        any <<= wstr;
        delete wstr;
        break;
      }
    default:
      break;
    }
}

void
ifr_adding_visitor::element_type (AST_Type *base_type
                                  ACE_ENV_ARG_DECL)
{
  if (base_type->anonymous ())
    {
      if (base_type->ast_accept (this) == -1)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::element_type -")
              ACE_TEXT (" failed to accept visitor\n")
            ));
        }
    }
  else
    {
      CORBA::Contained_var contained =
        be_global->repository ()->lookup_id (base_type->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (contained.in ()))
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::element_type -")
              ACE_TEXT (" lookup_id failed\n")
            ));
        }

      this->ir_current_ = CORBA::IDLType::_narrow (contained.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

int
ifr_adding_visitor::create_interface_def (AST_Interface *node
                                          ACE_ENV_ARG_DECL)
{
  CORBA::ULong n_parents = ACE_static_cast (CORBA::ULong,
                                            node->n_inherits ());

  CORBA::InterfaceDefSeq bases (n_parents);
  bases.length (n_parents);
  CORBA::Contained_var result;

  AST_Interface **parents = node->inherits ();

  // Construct a list of the parents.
  for (CORBA::ULong i = 0; i < n_parents; ++i)
    {
      // If we got to visit_interface() from a forward declared interface,
      // this node may not yet be in the repository. If it is, this
      // call will merely update ir_current_.
      if (this->visit_interface (parents[i]) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
              ACE_TEXT ("visit_interface - call for parent node failed\n")
            ),
            -1
          );
        }

      result =
        be_global->repository ()->lookup_id (parents[i]->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      bases[i] = CORBA::InterfaceDef::_narrow (result.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (bases[i].in ()))
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

  CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

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
                               ACE_ENV_ARG_PARAMETER
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
                               ACE_ENV_ARG_PARAMETER
                             );
        }

      ACE_CHECK_RETURN (-1);

      node->ifr_added (1);

      // Push the new IR object onto the scope stack.
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (new_def.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
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
      // the info in available anywhere. So it's a good idea to
      // update the current IR object holder now. This will
      // consume the objref pointer.
      this->ir_current_ =
        CORBA::IDLType::_duplicate (new_def.in ());

      CORBA::Container_ptr used_scope = CORBA::Container::_nil ();

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
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_interface -")
          ACE_TEXT (" scope stack is empty\n")
        ),
        -1
      );
    }

  return 0;
}

int
ifr_adding_visitor::create_value_def (AST_ValueType *
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

void
ifr_adding_visitor::get_referenced_type (AST_Type *node
                                         ACE_ENV_ARG_DECL)
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
          be_global->repository ()->lookup_id (node->repoID ()
                                               ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        this->ir_current_ =
          CORBA::IDLType::_narrow (prev_def.in ()
                                  ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        break;
      }
  }
}
