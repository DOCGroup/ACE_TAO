/* -*- c++ -*- */
// $Id$

#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_union.h"

ACE_RCSID(IFR_Service, ifr_adding_visitor_structure, "$Id$")

ifr_adding_visitor_structure::ifr_adding_visitor_structure (
    CORBA::Boolean is_nested
  )
  : is_nested_ (is_nested)
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

  ACE_DECLARE_NEW_CORBA_ENV;
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

          if (ft->node_type () == AST_Decl::NT_struct)
            {
              CORBA_Contained_var prev_def =
                be_global->repository ()->lookup_id (ft->repoID (),
                                                     ACE_TRY_ENV);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (prev_def.in ()))
                {
                  ifr_adding_visitor_structure visitor (1);

                  if ((*f)->ast_accept (&visitor) == -1)
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
                    CORBA_Contained::_narrow (visitor.ir_current (),
                                              ACE_TRY_ENV);
                  ACE_TRY_CHECK;

                  this->move_queue_.enqueue_tail (tmp);
                }
              else
                {
                  this->ir_current_ = 
                    CORBA_IDLType::_narrow (prev_def.in (),
                                            ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
            }
          else
            {
              if ((*f)->ast_accept (this) == -1)
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

          this->members_[i].name = 
            CORBA::string_dup ((*f)->local_name ()->get_string ());

          // IfR method create_struct does not use this - it just needs
          // to be non-null for marshaling.
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
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
                be_global->repository ()->create_struct (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    this->gen_version (node),
                    this->members_,
                    ACE_TRY_ENV
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
                                   this->gen_version (node),
                                   this->members_,
                                   ACE_TRY_ENV
                                );
            }

          ACE_TRY_CHECK;

          size_t size = this->move_queue_.size ();

          if (size > 0)
            {
              CORBA_Contained_var traveller;

              CORBA_Container_var new_container =
                CORBA_Container::_narrow (this->ir_current_.in (),
                                          ACE_TRY_ENV);
              ACE_TRY_CHECK;

              for (size_t i = 0; i < size; ++i)
                {
                  this->move_queue_.dequeue_head (traveller);

                  CORBA::String_var name = traveller->name (ACE_TRY_ENV);
                  ACE_TRY_CHECK;

                  CORBA::String_var version = 
                    traveller->version (ACE_TRY_ENV);
                  ACE_TRY_CHECK;

                  traveller->move (new_container.in (),
                                   name.in (),
                                   version.in (),
                                   ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
            }
        }
      else
        {
          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in (),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this struct's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before the repository
          // gets corrupted.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_structure -")
                  ACE_TEXT (" struct %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this enum already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
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
                                          this->gen_version (node),
                                          members,
                                          ACE_TRY_ENV
                                        );
          ACE_TRY_CHECK;

          CORBA_Contained_ptr tmp = 
            CORBA_Contained::_narrow (this->ir_current_.in (),
                                      ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Since the enclosing StructDef hasn't been created
          // yet, we don't have a scope, so this nested EnumDef
          // (which was created at global scope) goes on the
          // queue to be moved later.
          this->move_queue_.enqueue_tail (tmp);
        }
      else
        {
          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ = 
            CORBA_IDLType::_narrow (prev_def.in (),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this enum's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before the repository
          // gets corrupted.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_enum -")
                  ACE_TEXT (" enum %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this union already in the respository?
      CORBA_Contained_var prev_def = 
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_union visitor (1);

          int retval = visitor.visit_union (node);

          if (retval == 0)
            {
              // Get the result of the visit.
              this->ir_current_ =
                CORBA_IDLType::_duplicate (visitor.ir_current ());

              CORBA_Contained_ptr tmp = 
                CORBA_Contained::_narrow (visitor.ir_current (),
                                          ACE_TRY_ENV);
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
          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in (),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this union's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before the repository
          // gets corrupted.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_union -")
                  ACE_TEXT (" union %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
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
