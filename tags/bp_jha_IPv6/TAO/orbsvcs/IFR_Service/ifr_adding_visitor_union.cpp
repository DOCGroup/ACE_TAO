/* -*- c++ -*- */
// $Id$

#include "ifr_adding_visitor_union.h"
#include "ifr_adding_visitor_structure.h"

ACE_RCSID(IFR_Service, ifr_adding_visitor_union, "$Id$")

ifr_adding_visitor_union::ifr_adding_visitor_union (
    CORBA::Environment &ACE_TRY_ENV,
    CORBA::Boolean is_nested
  )
  : ifr_adding_visitor (ACE_TRY_ENV),
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
  // If one of the struct's members is a referenced interface,
  // there's no need to do anything here. The forward declaration
  // (if any) will create a repository entry, and the full
  // definition will take care of the interface's scope. Trying 
  // to take care of the interface's scope at this point could
  // cause problems, if the types of all its members have not yet
  // been declared.
  if (node->scope_node_type () == AST_Decl::NT_interface)
    {
      return 0;
    }

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

      if (ft->node_type () == AST_Decl::NT_union)
        {
          IR_Contained_var prev_def =
            be_global->repository ()->lookup_id (ft->repoID (),
                                                 this->env_);
          TAO_IFR_CHECK_RETURN (-1);

          if (CORBA::is_nil (prev_def.in ()))
            {
              ifr_adding_visitor_union visitor (this->env_,
                                                1);

              if ((*f)->ast_accept (&visitor) == -1)
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
                IR_IDLType::_duplicate (visitor.ir_current ());

              IR_Contained_ptr tmp = 
                IR_Contained::_narrow (visitor.ir_current (),
                                       this->env_);
              TAO_IFR_CHECK_RETURN (-1);

              this->move_queue_.enqueue_tail (tmp);
            }
          else
            {
              this->ir_current_ = 
                IR_IDLType::_narrow (prev_def.in (),
                                     this->env_);
              TAO_IFR_CHECK_RETURN (-1);
            }
        }
      else
        {
          if ((*f)->ast_accept (this) == -1)
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
              AST_Union::DefaultValue dv;

              if (u->default_value (dv) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) ifr_adding_visitor_union::"
                                     "visit_scope - "
                                     "computing default value failed\n"),
                                    -1);
                }

              this->load_any_with_default (dv,
                                           u,
                                           this->members_[index].label);
            }

          this->members_[index].name = 
            CORBA::string_dup ((*f)->local_name ()->get_string ());

          // IfR method create_union does not use this - it just needs
          // to be non-null for marshaling.
          this->members_[index].type = 
            CORBA::TypeCode::_duplicate (CORBA::_tc_null);

          this->members_[index++].type_def = 
            IR_IDLType::_duplicate (this->ir_current_.in ());
        }
    }

  return 0;
}

int
ifr_adding_visitor_union::visit_structure (AST_Structure *node)
{
  // Is this struct already in the respository?
  IR_Contained_var prev_def = 
    be_global->repository ()->lookup_id (node->repoID (),
                                         this->env_);
  TAO_IFR_CHECK_RETURN (-1);

  // If not, create a new entry.
  if (CORBA::is_nil (prev_def.in ()))
    {
      ifr_adding_visitor_structure visitor (this->env_,
                                            1);

      int retval = visitor.visit_structure (node);

      if (retval == 0)
        {
          // Get the result of the visit.
          this->ir_current_ =
            IR_IDLType::_duplicate (visitor.ir_current ());

          IR_Contained_ptr tmp = 
            IR_Contained::_narrow (visitor.ir_current (),
                                   this->env_);
          TAO_IFR_CHECK_RETURN (-1);

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
      // There is already an entry in the repository, so just update
      // the current IR object holder.
      this->ir_current_ =
        IR_IDLType::_narrow (prev_def.in (),
                             this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      // Nothing prevents this struct's repo id from already being
      // in the repository as another type, if it came from another
      // IDL file whose generated code is not linked to the generated
      // code from this IDL file. So we check here before the repository
      // gets corrupted.
      if (CORBA::is_nil (this->ir_current_.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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

  return 0;
}

int
ifr_adding_visitor_union::visit_enum (AST_Enum *node)
{
  // Is this enum already in the respository?
  IR_Contained_var prev_def =
    be_global->repository ()->lookup_id (node->repoID (),
                                         this->env_);
  TAO_IFR_CHECK_RETURN (-1);

  // If not, create a new entry.
  if (CORBA::is_nil (prev_def.in ()))
    {
      CORBA::ULong member_count = ACE_static_cast (CORBA::ULong, 
                                                   node->member_count ());

      IR_EnumMemberSeq members (member_count);
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
                                      this->gen_version (node),
                                      members,
                                      this->env_
                                    );
      TAO_IFR_CHECK_RETURN (-1);

      IR_Contained_ptr tmp = 
        IR_Contained::_narrow (this->ir_current_.in (),
                               this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      // Since the enclosing UnionDef hasn't been created
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
        IR_IDLType::_narrow (prev_def.in (),
                             this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      // Nothing prevents this enum's repo id from already being
      // in the repository as another type, if it came from another
      // IDL file whose generated code is not linked to the generated
      // code from this IDL file. So we check here before the repository
      // gets corrupted.
      if (CORBA::is_nil (this->ir_current_.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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

  return 0;
}

int
ifr_adding_visitor_union::visit_union (AST_Union *node)
{
  IR_Contained_var prev_def =
    be_global->repository ()->lookup_id (node->repoID (),
                                         this->env_);
  TAO_IFR_CHECK_RETURN (-1);

  if (CORBA::is_nil (prev_def.in ()))
    {
      // Get the discrimintor node.
      AST_ConcreteType *disc_type = node->disc_type ();

      // Since the IDL compiler stores enum label values as ulongs,
      // we need to get the typecode to insert the label value into
      // an Any.
      if (disc_type->node_type () == AST_Decl::NT_enum)
        {
          IR_Contained_var disc_def =
            be_global->repository ()->lookup_id (disc_type->repoID (),
                                                 this->env_);
          TAO_IFR_CHECK_RETURN (-1);

          if (CORBA::is_nil (disc_def.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::visit_union -")
                  ACE_TEXT (" discriminator not found in repository\n")
                ),  
                -1
              );
            }

          IR_IDLType_var idl_def =
            IR_IDLType::_narrow (disc_def.in (),
                                 this->env_);
          TAO_IFR_CHECK_RETURN (-1);

          this->disc_tc_ = idl_def->type (this->env_);
          TAO_IFR_CHECK_RETURN (-1);
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
            be_global->repository ()->create_union (
                                          node->repoID (),
                                          node->local_name ()->get_string (),
                                          this->gen_version (node),
                                          this->ir_current_.in (),
                                          this->members_,
                                          this->env_
                                        );
        }
      else
        {
          IR_Container_ptr current_scope = IR_Container::_nil ();

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
                               this->gen_version (node),
                               this->ir_current_.in (),
                               this->members_,
                               this->env_
                            );
        }

      TAO_IFR_CHECK_RETURN (-1);

      size_t size = this->move_queue_.size ();

      if (size > 0)
        {
          IR_Contained_var traveller;

          IR_Container_var new_container =
            IR_Container::_narrow (this->ir_current_.in (),
                                   this->env_);
          TAO_IFR_CHECK_RETURN (-1);

          for (size_t i = 0; i < size; ++i)
            {
              this->move_queue_.dequeue_head (traveller);

              CORBA::String_var name = traveller->name (this->env_);
              TAO_IFR_CHECK_RETURN (-1);

              CORBA::String_var version = traveller->version (this->env_);
              TAO_IFR_CHECK_RETURN (-1);

              traveller->move (new_container.in (),
                               name.in (),
                               version.in (),
                               this->env_);
              TAO_IFR_CHECK_RETURN (-1);
            }
        }
    } // if (CORBA::is_nil (...))
  else
    {
      this->ir_current_ =
        IR_IDLType::_narrow (prev_def.in (),
                             this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      // Nothing prevents this union's repo id from already being
      // in the repository as another type, if it came from another
      // IDL file whose generated code is not linked to the generated
      // code from this IDL file. So we check here before the repository
      // gets corrupted.
      if (CORBA::is_nil (this->ir_current_.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
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

  return 0;
}

IR_IDLType_ptr 
ifr_adding_visitor_union::ir_current (void) const
{
  return this->ir_current_.in ();
}

void 
ifr_adding_visitor_union::load_any_with_default (AST_Union::DefaultValue &dv,
                                                 AST_Union *u,
                                                 CORBA::Any &any)
{
  switch (u->udisc_type ())
  {
    case AST_Expression::EV_char:
      any <<= CORBA::Any::from_char (dv.u.char_val);
      break;
    case AST_Expression::EV_bool:
      any <<= CORBA::Any::from_boolean (ACE_static_cast (CORBA::Boolean,
                                                         dv.u.bool_val));
      break;
    case AST_Expression::EV_wchar:
      any <<= CORBA::Any::from_wchar (dv.u.wchar_val);
      break;
    case AST_Expression::EV_short:
      any <<= dv.u.short_val;
      break;
    case AST_Expression::EV_ushort:
      any <<= dv.u.ushort_val;
      break;
    case AST_Expression::EV_long:
      any <<= dv.u.long_val;
      break;
    case AST_Expression::EV_ulong:
      any <<= dv.u.ulong_val;
      break;
    case AST_Expression::EV_any:
    {
      TAO_OutputCDR cdr;
      cdr.write_ulong (dv.u.enum_val);
      any._tao_replace (this->disc_tc_.in (),
                        TAO_ENCAP_BYTE_ORDER,
                        cdr.begin ());
      break;
    }
    case AST_Expression::EV_ulonglong:
    case AST_Expression::EV_longlong:
      // As yet unimplemented.
    default:
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor_union::")
                  ACE_TEXT ("load_any_with_default - ")
                  ACE_TEXT ("type (%d) is invalid\n"), 
                  u->udisc_type ()));
  }
}
