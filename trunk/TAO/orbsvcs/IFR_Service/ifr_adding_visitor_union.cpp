/* -*- c++ -*- */
// $Id$

#include "ifr_adding_visitor_union.h"
#include "ifr_adding_visitor_structure.h"

ACE_RCSID(IFR_Service, ifr_adding_visitor_union, "$Id$")

ifr_adding_visitor_union::ifr_adding_visitor_union (
    AST_Decl *scope,
    CORBA::Boolean is_nested
  )
  : ifr_adding_visitor (scope),
    is_nested_ (is_nested)
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

  CORBA::ULong nfields = ACE_static_cast (CORBA::ULong,
                                          u->nfields ());

  this->members_.length (nfields);

  AST_Field **f = 0;

  // Index into members_.
  CORBA::ULong index = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
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

          idl_bool defined_here = ft->is_child (this->scope_);

          // If the union member is defined in the union, we have to
          // do some visiting - otherwise we can just look up the entry.
          if (defined_here)
            {
              if (ft->node_type () == AST_Decl::NT_union)
                {
                  // Since the enclosing scope hasn't been created yet,
                  // we make a special visitor to create this member
                  // at global scope and move it into the union later.
                  ifr_adding_visitor_union visitor (ft,
                                                    1);

                  if (ft->ast_accept (&visitor) == -1)
                    {
                      ACE_ERROR_RETURN ((
                          LM_ERROR,
                          ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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
                                              ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  this->move_queue_.enqueue_tail (tmp);
                }
              else
                {
                  if (ft->ast_accept (this) == -1)
                    {
                      ACE_ERROR_RETURN ((
                          LM_ERROR,
                          ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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
                                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          // Get the case label(s).

          AST_UnionBranch *ub = 0;
          AST_UnionLabel *case_label = 0;

          ub = AST_UnionBranch::narrow_from_decl (*f);

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
                      this->members_[index].label._tao_replace (
                                                      this->disc_tc_.in (),
                                                      TAO_ENCAP_BYTE_ORDER,
                                                      cdr.begin ()
                                                    );
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
                CORBA_IDLType::_duplicate (this->ir_current_.in ());
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_union::visit_scope")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_union::visit_structure (AST_Structure *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this struct already in the respository?
      CORBA_Contained_var prev_def =
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
                CORBA_IDLType::_duplicate (visitor.ir_current ());

              CORBA_Contained_ptr tmp =
                CORBA_Contained::_narrow (visitor.ir_current ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Since the enclosing UnionDef hasn't been created
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
            CORBA_IDLType::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_union::visit_structure")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_union::visit_enum (AST_Enum *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this enum already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
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
                CORBA::string_dup (member_name->last_component ()->get_string ());
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

          CORBA_Contained_ptr tmp =
            CORBA_Contained::_narrow (this->ir_current_.in ()
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Since the enclosing UnionDef hasn't been created
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
              prev_def->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // This call will take the other branch.
              return this->visit_enum (node);
            }

          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_union::visit_enum")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_union::visit_union (AST_Union *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          // Get the discrimintor node.
          AST_ConcreteType *disc_type = node->disc_type ();

          // Since the IDL compiler stores enum label values as ulongs,
          // we need to get the typecode to insert the label value into
          // an Any.
          if (disc_type->node_type () == AST_Decl::NT_enum)
            {
              CORBA_Contained_var disc_def =
                be_global->repository ()->lookup_id (disc_type->repoID ()
                                                     ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (disc_def.in ()))
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                      ACE_TEXT ("visit_union -")
                      ACE_TEXT (" discriminator not found in repository\n")
                    ),
                    -1
                  );
                }

              CORBA_IDLType_var idl_def =
                CORBA_IDLType::_narrow (disc_def.in ()
                                        ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              this->disc_tc_ = idl_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if (this->visit_scope (node) == -1)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::visit_union -")
                  ACE_TEXT (" visit_scope failed\n")
                ),
                -1
              );
            }

          // This will put an IR object for the discriminator in ir_current_.
          if (disc_type->ast_accept (this) == -1)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::visit_union -")
                  ACE_TEXT (" failed to accept visitor\n")
                ),
                -1
              );
            }

          if (this->is_nested_)
            {
              this->ir_current_ =
                be_global->holding_scope ()->create_union (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    node->version (),
                    this->ir_current_.in (),
                    this->members_
                    ACE_ENV_ARG_PARAMETER
                  );
            }
          else
            {
              CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

              if (be_global->ifr_scopes ().top (current_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                      ACE_TEXT ("visit_union -")
                      ACE_TEXT (" scope stack is empty\n")
                    ),
                    -1
                  );
                }

              this->ir_current_ =
                current_scope->create_union (
                                   node->repoID (),
                                   node->local_name ()->get_string (),
                                   node->version (),
                                   this->ir_current_.in (),
                                   this->members_
                                   ACE_ENV_ARG_PARAMETER
                                );
            }

          ACE_TRY_CHECK;

          size_t size = this->move_queue_.size ();

          if (size > 0)
            {
              CORBA_Contained_var traveller;

              CORBA_Container_var new_container =
                CORBA_Container::_narrow (this->ir_current_.in ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              for (size_t i = 0; i < size; ++i)
                {
                  this->move_queue_.dequeue_head (traveller);

                  CORBA::String_var name = traveller->name (ACE_ENV_SINGLE_ARG_PARAMETER);
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
        } // if (CORBA::is_nil (...))
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
            CORBA_IDLType::_narrow (prev_def.in ()
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_union::visit_union")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA_IDLType_ptr
ifr_adding_visitor_union::ir_current (void) const
{
  return this->ir_current_.in ();
}

