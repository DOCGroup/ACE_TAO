/* -*- c++ -*- */
// $Id$

#include "ast_enum.h"
#include "ast_exception.h"
#include "ast_expression.h"
#include "ast_field.h"
#include "ast_union.h"
#include "utl_identifier.h"

#include "ifr_adding_visitor_exception.h"
#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_union.h"

ACE_RCSID (IFR_Service,
           ifr_adding_visitor_exception,
           "$Id$")

ifr_adding_visitor_exception::ifr_adding_visitor_exception (
    AST_Decl *scope,
    CORBA::Boolean in_reopened
  )
  : ifr_adding_visitor (scope,
                        in_reopened)
{
}

ifr_adding_visitor_exception::~ifr_adding_visitor_exception (void)
{
}

// Specialized visit_scope method for exceptions only.
int
ifr_adding_visitor_exception::visit_scope (UTL_Scope *node)
{
  // If the exception has members that are scopes but not exceptions,
  // the regular visit_scope method should be called instead.
  if (node->scope_node_type () != AST_Decl::NT_except)
    {
      return ifr_adding_visitor::visit_scope (node);
    }

  AST_Exception *e = AST_Exception::narrow_from_scope (node);

  CORBA::ULong nfields = ACE_static_cast (CORBA::ULong,
                                          e->nfields ());

  this->members_.length (nfields);

  AST_Field **f = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Visit each field.
      for (CORBA::ULong i = 0; i < nfields; ++i)
        {
          if (e->field (f, i) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                  ACE_TEXT ("visit_scope -")
                  ACE_TEXT (" field node access failed\n")
                ),
                -1
              );
            }

          AST_Type *ft = (*f)->field_type ();

          idl_bool defined_here = ft->is_child (this->scope_);

          // If the struct member is defined in the struct, we have to
          // do some visiting - otherwise we can just look up the entry.
          if (defined_here)
            {
              if (ft->ast_accept (this) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                      ACE_TEXT ("visit_scope -")
                      ACE_TEXT (" failed to accept visitor\n")
                    ),
                    -1
                  );
                }
            }
          else
            {
              // Updates ir_current_.
              this->get_referenced_type (ft ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          this->members_[i].name =
            CORBA::string_dup ((*f)->local_name ()->get_string ());

          // IfR method create_exception does not use this - it just needs
          // to be non-zero for marshaling.
          this->members_[i].type =
            CORBA::TypeCode::_duplicate (CORBA::_tc_void);

          this->members_[i].type_def =
            CORBA::IDLType::_duplicate (this->ir_current_.in ());
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_structure::visit_scope")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_exception::visit_structure (AST_Structure *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this union already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_structure visitor (node,
                                                1);

          int retval = visitor.visit_structure (node);

          if (retval == 0)
            {
              // Get the result of the visit.
              this->ir_current_ =
                CORBA::IDLType::_duplicate (visitor.ir_current ());

              CORBA::Contained_ptr tmp =
                CORBA::Contained::_narrow (visitor.ir_current ()
                                           ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Since the enclosing ExceptionDef hasn't been created
              // yet, we don't have a scope, so this nested StructDef
              // (which was created at global scope) goes on the
              // queue to be moved later.
              this->move_queue_.enqueue_tail (tmp);
            }

          return retval;
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (node->ifr_added () == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
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
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_exception::visit_structure")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_exception::visit_exception (AST_Exception *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (prev_def.in ()))
        {
          // If we and our enclosing module are both already in the
          // repository, we are probably processing the same IDL file
          // a second time. If it is just a name clash, there is no
          // way to detect it.
          if (this->in_reopened_ == 1)
            {
              return 0;
            }

          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (node->ifr_added () == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will create a new ExceptionDef entry.
              return this->visit_exception (node);
            }
          else
            {
              // The node is being referenced in an operation, no action.
              return 0;
            }
        }

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
              ACE_TEXT ("visit_exception -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
              ACE_TEXT ("visit_exception -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }

      CORBA::ExceptionDef_var new_def =
        current_scope->create_exception (node->repoID (),
                                         node->local_name ()->get_string (),
                                         node->version (),
                                         this->members_
                                         ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      size_t size = this->move_queue_.size ();

      if (size > 0)
        {
          CORBA::Contained_var traveller;

          CORBA::Container_var new_container =
            CORBA::Container::_narrow (new_def.in ()
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          for (size_t i = 0; i < size; ++i)
            {
              this->move_queue_.dequeue_head (traveller);

              CORBA::String_var name =
                traveller->name (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CORBA::String_var version =
                traveller->version (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              traveller->move (new_container.in (),
                               name.in (),
                               version.in ()
                               ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }

      node->ifr_added (1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_exception::visit_exception")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_exception::visit_enum (AST_Enum *node)
{
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
          CORBA::ULong member_count = ACE_static_cast (CORBA::ULong,
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

          this->ir_current_ =
            be_global->repository ()->create_enum (
                                          node->repoID (),
                                          node->local_name ()->get_string (),
                                          node->version (),
                                          members
                                          ACE_ENV_ARG_PARAMETER
                                        );
          ACE_TRY_CHECK;

          CORBA::Contained_ptr tmp =
            CORBA::Contained::_narrow (this->ir_current_.in ()
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->move_queue_.enqueue_tail (tmp);

          node->ifr_added (1);
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (node->ifr_added () == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_enum (node);
            }

          this->ir_current_ =
            CORBA::IDLType::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_exception::visit_enum")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_exception::visit_union (AST_Union *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this union already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_union visitor (node,
                                            1);

          int retval = visitor.visit_union (node);

          if (retval == 0)
            {
              // Get the result of the visit.
              this->ir_current_ =
                CORBA::IDLType::_duplicate (visitor.ir_current ());

              CORBA::Contained_ptr tmp =
                CORBA::Contained::_narrow (visitor.ir_current ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Since the enclosing ExceptionDef hasn't been created
              // yet, we don't have a scope, so this nested UnionDef
              // (which was created at global scope) goes on the
              // queue to be moved later.
              this->move_queue_.enqueue_tail (tmp);
            }

          return retval;
        }
      else
        {
          // If the line below is true, we are clobbering a previous
          // entry (from another IDL file) of another type. In that
          // case we do what other ORB vendors do, and destroy the
          // original entry, create the new one, and let the user beware.
          if (node->ifr_added () == 0)
            {
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_union (node);
            }

          this->ir_current_ =
            CORBA::IDLType::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_exception::visit_union")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::IDLType_ptr
ifr_adding_visitor_exception::ir_current (void) const
{
  return this->ir_current_.in ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Queue<CORBA::Contained_ptr>;
template class ACE_Unbounded_Queue_Iterator<CORBA::Contained_ptr>;
template class ACE_Node<CORBA::Contained_ptr>;
template class ACE_Write_Guard<ACE_Lock>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Queue<CORBA::Contained_ptr>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::Contained_ptr>
#pragma instantiate ACE_Node<CORBA::Contained_ptr>
#pragma instantiate ACE_Write_Guard<ACE_Lock>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
