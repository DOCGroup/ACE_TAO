/* -*- c++ -*- */
// $Id$

#include "ast_enum.h"
#include "ast_expression.h"
#include "ast_field.h"
#include "ast_union.h"
#include "utl_identifier.h"

#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_union.h"

ifr_adding_visitor_structure::ifr_adding_visitor_structure (AST_Decl *scope)
  : ifr_adding_visitor (scope)
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
  CORBA::ULong nfields = static_cast<CORBA::ULong> (s->nfields ());
  this->members_.length (nfields);
  AST_Field **f = 0;

  try
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
                ACE_TEXT (" field node access failed\n")),
               -1);
            }

          AST_Type *ft = (*f)->field_type ();
          bool defined_here = ft->is_child (this->scope_);

          // If the struct member is defined in the struct, we have to
          // do some visiting - otherwise we can just look up the entry.
          if (defined_here)
            {
              if (ft->node_type () == AST_Decl::NT_struct)
                {
                  // Since the enclosing scope hasn't been created yet,
                  // we make a special visitor to create this member
                  // at global scope and move it into the struct later.
                  ifr_adding_visitor_structure visitor (ft);

                  if (ft->ast_accept (&visitor) == -1)
                    {
                      ACE_ERROR_RETURN ((
                        LM_ERROR,
                        ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                        ACE_TEXT ("visit_scope -")
                        ACE_TEXT (" failed to accept visitor\n")),
                       -1);
                    }

                  this->ir_current_ =
                    CORBA::IDLType::_duplicate (visitor.ir_current ());
                }
              else
                {
                  if (ft->ast_accept (this) == -1)
                    {
                      ACE_ERROR_RETURN ((
                        LM_ERROR,
                        ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                        ACE_TEXT ("visit_scope -")
                        ACE_TEXT (" failed to accept visitor\n")),
                       -1);
                    }
                }
            }
          else
            {
              // Updates ir_current_.
              this->get_referenced_type (ft);
            }

          this->members_[i].name =
            CORBA::string_dup ((*f)->local_name ()->get_string ());

          // IfR method create_struct does not use this - it just needs
          // to be non-zero for marshaling.
          this->members_[i].type =
            CORBA::TypeCode::_duplicate (CORBA::_tc_void);

          this->members_[i].type_def =
            CORBA::IDLType::_duplicate (this->ir_current_.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_structure::visit_scope"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_structure::visit_structure (AST_Structure *node)
{
  try
    {
      CORBA::StructDef_var struct_def;
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::StructDef_var struct_def;
          CORBA::StructMemberSeq dummyMembers;
          dummyMembers.length (0);

          CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                ACE_TEXT ("visit_structure -")
                ACE_TEXT (" scope stack is empty\n")),
               -1);
            }

          // First create the named structure without any members.
          struct_def =
            current_scope->create_struct (
                node->repoID (),
                node->local_name ()->get_string (),
                node->version (),
                dummyMembers
              );

          if (be_global->ifr_scopes ().push (struct_def.in ()) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_structure -")
                  ACE_TEXT (" scope push failed\n")
                ),
                -1
              );
            }

          // Then add the real structure members (which corrupts ir_current_).
          if (this->add_members (node, struct_def.in ()) == -1)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                ACE_TEXT ("visit_structure -")
                ACE_TEXT (" visit_scope failed\n")),
               -1);
            }

          this->ir_current_ = CORBA::IDLType::_narrow (struct_def.in ());

          CORBA::Container_ptr used_scope =
            CORBA::Container::_nil ();

          // Pop the new IR object back off the scope stack.
          if (be_global->ifr_scopes ().pop (used_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_structure -")
                  ACE_TEXT (" scope pop failed\n")
                ),
                -1
              );
            }
        } // if (CORBA::is_nil (...))
      else
        {
          // We are seeing the full definition of a forward
          // declaration - just add the members so repo
          // entries referencing the UnionDef will stay valid.
          // Also we know node->ifr_fwd_added_ is true.
          struct_def = CORBA::StructDef::_narrow (prev_def.in ());

          if (be_global->ifr_scopes ().push (struct_def.in ()) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_structure -")
                  ACE_TEXT (" scope push failed\n")
                ),
                -1
              );
            }

          if (this->add_members (node, struct_def.in ()) == -1)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                ACE_TEXT ("visit_structure -")
                ACE_TEXT (" visit_scope failed\n")),
               -1);
            }

          this->ir_current_ = CORBA::IDLType::_narrow (prev_def.in ());

          CORBA::Container_ptr used_scope =
            CORBA::Container::_nil ();

          // Pop the new IR object back off the scope stack.
          if (be_global->ifr_scopes ().pop (used_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
                  ACE_TEXT ("visit_structure -")
                  ACE_TEXT (" scope pop failed\n")
                ),
                -1
              );
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_structure::visit_structure"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_structure::visit_enum (AST_Enum *node)
{
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

          this->ir_current_ =
            be_global->repository ()->create_enum (
                                          node->repoID (),
                                          node->local_name ()->get_string (),
                                          node->version (),
                                          members
                                        );

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
              return this->visit_enum (node);
            }

          this->ir_current_ =
            CORBA::IDLType::_narrow (prev_def.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_structure::visit_enum"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_structure::visit_union (AST_Union *node)
{
  try
    {
      // Is this union already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_union visitor (node);
          int retval = visitor.visit_union (node);

          if (retval == 0)
            {
              // Get the result of the visit.
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
          if (!node->ifr_added ())
            {
              prev_def->destroy ();

              // This call will take the other branch.
              return this->visit_union (node);
            }

          this->ir_current_ =
            CORBA::IDLType::_narrow (prev_def.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_structure::visit_union"));

      return -1;
    }

  return 0;
}

CORBA::IDLType_ptr
ifr_adding_visitor_structure::ir_current (void) const
{
  return this->ir_current_.in ();
}

int
ifr_adding_visitor_structure::add_members (AST_Structure *node,
                                           CORBA::StructDef_ptr struct_def)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((
        LM_ERROR,
        ACE_TEXT ("(%N:%l) ifr_adding_visitor_structure::")
        ACE_TEXT ("visit_structure -")
        ACE_TEXT (" visit_scope failed\n")),
       -1);
    }

  // Correct ir_current_ and move the real members into the struct.
  this->ir_current_= CORBA::StructDef::_duplicate (struct_def);
  struct_def->members (this->members_);

  node->ifr_added (true);
  return 0;
}
