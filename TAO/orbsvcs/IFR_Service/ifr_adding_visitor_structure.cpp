/* -*- c++ -*- */
// $Id$

#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_union.h"

ACE_RCSID(IFR_Service, ifr_adding_visitor_structure, "$Id$")

ifr_adding_visitor_structure::ifr_adding_visitor_structure (
    AST_Decl *scope,
    CORBA::Boolean is_nested
  )
  : ifr_adding_visitor (scope),
    is_nested_ (is_nested)
{
}

ifr_adding_visitor_structure::~ifr_adding_visitor_structure (void)
{
}

// Specialized visit_scope method for stucts only.
int
ifr_adding_visitor_structure::visit_scope (UTL_Scope *node)
{
  // If the struct has members that are scopes but not structs,
  // the regular visit_scope method should be called instead.
  if (node->scope_node_type () != AST_Decl::NT_struct)
    {
      return ifr_adding_visitor::visit_scope (node);
    }

  AST_Structure *s = AST_Structure::narrow_from_scope (node);

  CORBA::ULong nfields = ACE_static_cast (CORBA::ULong,
                                          s->nfields ());

  this->members_.length (nfields);

  AST_Field **f = 0;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Visit each field.
      for (CORBA::ULong i = 0; i < nfields; ++i)
        {
          if (s->field (f, i) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
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
              if (ft->node_type () == AST_Decl::NT_struct)
                {
                  // Since the enclosing scope hasn't been created yet,
                  // we make a special visitor to create this member
                  // at global scope and move it into the struct later.
                  ifr_adding_visitor_structure visitor (ft,
                                                        1);

                  if (ft->ast_accept (&visitor) == -1)
                    {
                      ACE_ERROR_RETURN ((
                          LM_ERROR,
                          ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                          ACE_TEXT ("visit_scope -")
                          ACE_TEXT (" failed to accept visitor\n")
                        ),
                        -1
                      );
                    }

                  this->ir_current_ =
                    CORBA_IDLType::_duplicate (visitor.ir_current ());

                  CORBA_Contained_ptr tmp =
                    CORBA_Contained::_narrow (visitor.ir_current ()
                                              TAO_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  this->move_queue_.enqueue_tail (tmp);
                }
              else
                {
                  if (ft->ast_accept (this) == -1)
                    {
                      ACE_ERROR_RETURN ((
                          LM_ERROR,
                          ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                          ACE_TEXT ("visit_scope -")
                          ACE_TEXT (" failed to accept visitor\n")
                        ),
                        -1
                      );
                    }
                }
            }
          else
            {
              // Updates ir_current_.
              this->get_referenced_type (ft
                                         TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          this->members_[i].name =
            CORBA::string_dup ((*f)->local_name ()->get_string ());

          // IfR method create_struct does not use this - it just needs
          // to be non-zero for marshaling.
          this->members_[i].type =
            CORBA::TypeCode::_duplicate (CORBA::_tc_void);

          this->members_[i].type_def =
            CORBA_IDLType::_duplicate (this->ir_current_.in ());
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
ifr_adding_visitor_structure::visit_structure (AST_Structure *node)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          if (this->visit_scope (node) == -1)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_structure -")
                  ACE_TEXT (" visit_scope failed\n")
                ),
                -1
              );
            }

          if (this->is_nested_)
            {
              this->ir_current_ =
                be_global->holding_scope ()->create_struct (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version (),
                    this->members_
                    TAO_ENV_ARG_PARAMETER
                  );
            }
          else
            {
              CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

              if (be_global->ifr_scopes ().top (current_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                      ACE_TEXT ("visit_structure -")
                      ACE_TEXT (" scope stack is empty\n")
                    ),
                    -1
                  );
                }

              this->ir_current_ =
                current_scope->create_struct (
                                   node->repoID (),
                                   node->local_name ()->get_string (),
                                   node->version (),
                                   this->members_
                                   TAO_ENV_ARG_PARAMETER
                                );
            }

          ACE_TRY_CHECK;

          size_t size = this->move_queue_.size ();

          if (size > 0)
            {
              CORBA_Contained_var traveller;

              CORBA_Container_var new_container =
                CORBA_Container::_narrow (this->ir_current_.in ()
                                          TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              for (size_t i = 0; i < size; ++i)
                {
                  this->move_queue_.dequeue_head (traveller);

                  CORBA::String_var name = traveller->name (TAO_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  CORBA::String_var version =
                    traveller->version (TAO_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  traveller->move (new_container.in (),
                                   name.in (),
                                   version.in ()
                                   TAO_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }

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
              prev_def->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_structure (node);
            }

          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in ()
                                    TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_structure::visit_structure")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_structure::visit_enum (AST_Enum *node)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Is this enum already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::ULong member_count = ACE_static_cast (CORBA::ULong,
                                                       node->member_count ());

          CORBA_EnumMemberSeq members (member_count);
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
                                          TAO_ENV_ARG_PARAMETER
                                        );
          ACE_TRY_CHECK;

          CORBA_Contained_ptr tmp =
            CORBA_Contained::_narrow (this->ir_current_.in ()
                                      TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Since the enclosing StructDef hasn't been created
          // yet, we don't have a scope, so this nested EnumDef
          // (which was created at global scope) goes on the
          // queue to be moved later.
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
              prev_def->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_enum (node);
            }

          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in ()
                                    TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_structure::visit_enum")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_structure::visit_union (AST_Union *node)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Is this union already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             TAO_ENV_ARG_PARAMETER);
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
                CORBA_IDLType::_duplicate (visitor.ir_current ());

              CORBA_Contained_ptr tmp =
                CORBA_Contained::_narrow (visitor.ir_current ()
                                          TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Since the enclosing StructDef hasn't been created
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
              prev_def->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_union (node);
            }

          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in ()
                                    TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_structure::visit_union")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA_IDLType_ptr
ifr_adding_visitor_structure::ir_current (void) const
{
  return this->ir_current_.in ();
}
