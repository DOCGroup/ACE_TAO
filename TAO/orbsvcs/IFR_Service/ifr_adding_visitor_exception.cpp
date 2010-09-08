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

ifr_adding_visitor_exception::ifr_adding_visitor_exception (
    AST_Decl *scope,
    bool in_reopened
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

  CORBA::ULong nfields = static_cast<CORBA::ULong> (e->nfields ());

  this->members_.length (nfields);

  AST_Field **f = 0;

  try
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

          bool defined_here = ft->is_child (this->scope_);

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
              this->get_referenced_type (ft);
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
ifr_adding_visitor_exception::visit_structure (AST_Structure *node)
{
  try
    {
      // Is this union already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_structure visitor (node);
          int retval = visitor.visit_structure (node);

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
              return this->visit_structure (node);
            }

          this->ir_current_ =
            CORBA::IDLType::_narrow (prev_def.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_exception::visit_structure"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_exception::visit_exception (AST_Exception *node)
{
  try
    {
      CORBA::ExceptionDef_var new_def;
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::StructMemberSeq dummyMembers;
          dummyMembers.length (0);
          CORBA::Container_ptr current_scope =
            CORBA::Container::_nil ();

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

          new_def =
            current_scope->create_exception (node->repoID (),
                                            node->local_name ()->get_string (),
                                            node->version (),
                                            dummyMembers);

          if (be_global->ifr_scopes ().push (new_def.in ()) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                  ACE_TEXT ("visit_exception -")
                  ACE_TEXT (" scope push failed\n")
                ),
                -1
              );
            }

          // Then add the real exception members.
          if (this->add_members (node, new_def.in ()) == -1)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                ACE_TEXT ("visit_exception -")
                ACE_TEXT (" visit_scope failed\n")),
                -1);
            }

          CORBA::Container_ptr used_scope =
            CORBA::Container::_nil ();

          // Pop the new IR object back off the scope stack.
          if (be_global->ifr_scopes ().pop (used_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                  ACE_TEXT ("visit_exception -")
                  ACE_TEXT (" scope pop failed\n")
                ),
                -1
              );
            }
        }
      else
      {
          new_def = CORBA::ExceptionDef::_narrow (prev_def.in ());

          if (be_global->ifr_scopes ().push (new_def.in ()) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                  ACE_TEXT ("visit_exception -")
                  ACE_TEXT (" scope push failed\n")
                ),
                -1
              );
            }

          // Then add the real exception members.
          if (this->add_members (node, new_def.in ()) == -1)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                ACE_TEXT ("visit_exception -")
                ACE_TEXT (" visit_scope failed\n")),
                -1);
            }

          CORBA::Container_ptr used_scope =
            CORBA::Container::_nil ();

          // Pop the new IR object back off the scope stack.
          if (be_global->ifr_scopes ().pop (used_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_exception::")
                  ACE_TEXT ("visit_exception -")
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
          "ifr_adding_visitor_exception::visit_exception"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_exception::visit_enum (AST_Enum *node)
{
  try
    {
      // Is this enum already in the respository?
      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::ULong member_count = static_cast<CORBA::ULong> (node->member_count ());

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
          "ifr_adding_visitor_exception::visit_enum"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_exception::visit_union (AST_Union *node)
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
          "ifr_adding_visitor_exception::visit_union"));

      return -1;
    }

  return 0;
}

CORBA::IDLType_ptr
ifr_adding_visitor_exception::ir_current (void) const
{
  return this->ir_current_.in ();
}

int
ifr_adding_visitor_exception::add_members (AST_Exception *node,
                                           CORBA::ExceptionDef_ptr except_def)
{
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

  except_def->members (this->members_);

  node->ifr_added (true);
  return 0;
}
