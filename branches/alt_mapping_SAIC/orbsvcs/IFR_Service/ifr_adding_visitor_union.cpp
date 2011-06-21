/* -*- c++ -*- */
// $Id$

#include "ast_enum.h"
#include "ast_expression.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_union_label.h"
#include "utl_identifier.h"

#include "ifr_adding_visitor_union.h"
#include "ifr_adding_visitor_structure.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/CDR.h"

ifr_adding_visitor_union::ifr_adding_visitor_union (AST_Decl *scope)
  : ifr_adding_visitor (scope)
{
}

ifr_adding_visitor_union::~ifr_adding_visitor_union (void)
{
}

// Specialized visit_scope method for unions only.
int
ifr_adding_visitor_union::visit_scope (UTL_Scope *node)
{
  // If the union has members that are scopes but not unions,
  // the regular visit_scope method should be called instead.
  if (node->scope_node_type () != AST_Decl::NT_union)
    {
      return ifr_adding_visitor::visit_scope (node);
    }

  AST_Union *u = AST_Union::narrow_from_scope (node);

  CORBA::ULong nfields = static_cast<CORBA::ULong> (u->nfields ());

  this->members_.length (nfields);

  AST_Field **f = 0;

  // Index into members_.
  CORBA::ULong index = 0;

  try
    {
      // Visit each field.
      for (CORBA::ULong i = 0; i < nfields; ++i)
        {
          if (u->field (f, i) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                  ACE_TEXT ("visit_scope -")
                  ACE_TEXT (" field node access failed\n")
                ),
                -1
              );
            }

          AST_Type *ft = (*f)->field_type ();

          bool defined_here = ft->is_child (this->scope_);

          // If the union member is defined in the union, we have to
          // do some visiting - otherwise we can just look up the entry.
          if (defined_here)
            {
              if (ft->node_type () == AST_Decl::NT_union)
                {
                  // Since the enclosing scope hasn't been created yet,
                  // we make a special visitor to create this member
                  // at global scope and move it into the union later.
                  ifr_adding_visitor_union visitor (ft);

                  if (ft->ast_accept (&visitor) == -1)
                    {
                      ACE_ERROR_RETURN ((
                        LM_ERROR,
                        ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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
                        ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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

          // Get the case label(s).

          AST_UnionLabel *case_label = 0;
          AST_UnionBranch *ub = AST_UnionBranch::narrow_from_decl (*f);
          unsigned long len = ub->label_list_length ();

          // If there are multiple case labels, we will have an element
          // in the UnionMemberSeq for each label, not just for each member,
          // so the list length and the loop terminator must both be
          // increased accordingly.
          if (len > 1)
            {
              this->members_.length (this->members_.length () + len - 1);
            }

          for (unsigned long j = 0; j < len; ++j)
            {
              case_label = ub->label (j);

              // Is this a regular label or default label?
              if (case_label->label_kind () == AST_UnionLabel::UL_label)
                {
                  AST_Expression::AST_ExprValue *ev =
                    case_label->label_val ()->ev ();

                  // If the discriminator is an enum, we can't just insert
                  // a ulong into the Any member of struct UnionMember.
                  if (u->disc_type ()->node_type () == AST_Decl::NT_enum)
                    {
                      TAO_OutputCDR cdr;
                      cdr.write_ulong (ev->u.ulval);
                      TAO_InputCDR in_cdr (cdr);
                      TAO::Unknown_IDL_Type *unk = 0;
                      ACE_NEW_RETURN (unk,
                                      TAO::Unknown_IDL_Type (
                                        this->disc_tc_.in (),
                                        in_cdr),
                                       -1);
                      this->members_[index].label.replace (unk);
                    }
                  else
                    {
                      this->load_any (ev,
                                      this->members_[index].label);
                    }
                }
              else      // Default case label.
                {
                  this->members_[index].label <<= CORBA::Any::from_octet (0);
                }

              this->members_[index].name =
                CORBA::string_dup ((*f)->local_name ()->get_string ());

              // IfR method create_union does not use this - it just needs
              // to be non-zero for marshaling.
              this->members_[index].type =
                CORBA::TypeCode::_duplicate (CORBA::_tc_void);

              this->members_[index++].type_def =
                CORBA::IDLType::_duplicate (this->ir_current_.in ());
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_union::visit_scope"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_union::visit_structure (AST_Structure *node)
{
  try
    {
      // Is this struct already in the respository?
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
          "ifr_adding_visitor_union::visit_structure"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_union::visit_enum (AST_Enum *node)
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
                  member_name->last_component ()->get_string ());
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
          "ifr_adding_visitor_union::visit_enum"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_union::visit_union (AST_Union *node)
{
  try
    {
      // This will put the discriminator in ir_current_.
      if (node->disc_type ()->ast_accept (this) == -1)
        {
          ACE_ERROR_RETURN ((
            LM_ERROR,
            ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
            ACE_TEXT ("visit_union -")
            ACE_TEXT (" visit of discriminator failed\n")),
           -1);
        }

      this->disc_tc_ = this->ir_current_->type ();

      CORBA::UnionDef_var union_def;
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
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                ACE_TEXT ("visit_union -")
                ACE_TEXT (" scope stack is empty\n")),
               -1);
            }

          // First create the named union without any members.
          union_def =
            current_scope->create_union (
                node->repoID (),
                node->local_name ()->get_string (),
                node->version (),
                this->ir_current_.in (),
                dummyMembers
              );

          if (be_global->ifr_scopes ().push (union_def.in ()) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                  ACE_TEXT ("visit_union -")
                  ACE_TEXT (" scope push failed\n")
                ),
                -1
              );
            }

          // Then add the real union members (which corrupts ir_current_).
          if (this->add_members (node, union_def.in ()) == -1)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::visit_union -")
                ACE_TEXT (" visit_scope failed\n")),
               -1);
            }

          this->ir_current_ = CORBA::IDLType::_narrow (union_def.in ());

          CORBA::Container_ptr used_scope =
            CORBA::Container::_nil ();

          // Pop the new IR object back off the scope stack.
          if (be_global->ifr_scopes ().pop (used_scope) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                  ACE_TEXT ("visit_union -")
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
          union_def = CORBA::UnionDef::_narrow (prev_def.in ());
          union_def->discriminator_type_def (this->ir_current_.in ());

          if (be_global->ifr_scopes ().push (union_def.in ()) != 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                  ACE_TEXT ("visit_union -")
                  ACE_TEXT (" scope push failed\n")
                ),
                -1
              );
            }

          if (this->add_members (node, union_def.in ()) == -1)
            {
              ACE_ERROR_RETURN ((
                LM_ERROR,
                ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                ACE_TEXT ("visit_union -")
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
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                  ACE_TEXT ("visit_union -")
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
          "ifr_adding_visitor_union::visit_union"));

      return -1;
    }

  return 0;
}

CORBA::IDLType_ptr
ifr_adding_visitor_union::ir_current (void) const
{
  return this->ir_current_.in ();
}

int
ifr_adding_visitor_union::add_members (AST_Union *node,
                                       CORBA::UnionDef_ptr union_def)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((
        LM_ERROR,
        ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::visit_union -")
        ACE_TEXT (" visit_scope failed\n")),
       -1);
    }

  // Correct ir_current_ and move the real union members into the union.
  this->ir_current_= CORBA::UnionDef::_duplicate (union_def);
  union_def->members (this->members_);

  node->ifr_added (true);
  return 0;
}
