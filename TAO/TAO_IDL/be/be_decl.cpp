// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_decl.cpp
//
// = DESCRIPTION
//    Extension of class AST_Decl that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_decl.h"
#include "be_scope.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_valuetype.h"
#include "be_component.h"
#include "be_eventtype.h"
#include "be_home.h"
#include "be_module.h"
#include "be_root.h"
#include "be_exception.h"
#include "be_structure.h"
#include "be_union.h"
#include "be_enum.h"
#include "be_operation.h"
#include "be_factory.h"
#include "be_sequence.h"
#include "be_visitor.h"
#include "ast_structure_fwd.h"
#include "ast_string.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"
#include "ace/String_Base.h"

ACE_RCSID (be,
           be_decl,
           "$Id$")

// Default Constructor
be_decl::be_decl (void)
  : COMMON_Base (),
    AST_Decl (),
    cli_hdr_gen_ (I_FALSE),
    cli_stub_gen_ (I_FALSE),
    cli_inline_gen_ (I_FALSE),
    srv_hdr_gen_ (I_FALSE),
    impl_hdr_gen_ (I_FALSE),
    srv_skel_gen_ (I_FALSE),
    impl_skel_gen_ (I_FALSE),
    srv_inline_gen_ (I_FALSE),
    cli_hdr_any_op_gen_ (I_FALSE),
    cli_stub_any_op_gen_ (I_FALSE),
    cli_hdr_cdr_op_gen_ (I_FALSE),
    cli_stub_cdr_op_gen_ (I_FALSE),
    cli_inline_cdr_op_gen_ (I_FALSE),
    cli_inline_cdr_decl_gen_ (I_FALSE),
    cli_traits_gen_ (I_FALSE)
{
}

// Constructor
be_decl::be_decl (AST_Decl::NodeType type,
                  UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (type,
              n),
    cli_hdr_gen_ (I_FALSE),
    cli_stub_gen_ (I_FALSE),
    cli_inline_gen_ (I_FALSE),
    srv_hdr_gen_ (I_FALSE),
    impl_hdr_gen_ (I_FALSE),
    srv_skel_gen_ (I_FALSE),
    impl_skel_gen_ (I_FALSE),
    srv_inline_gen_ (I_FALSE),
    cli_hdr_any_op_gen_ (I_FALSE),
    cli_stub_any_op_gen_ (I_FALSE),
    cli_hdr_cdr_op_gen_ (I_FALSE),
    cli_stub_cdr_op_gen_ (I_FALSE),
    cli_inline_cdr_op_gen_ (I_FALSE),
    cli_inline_cdr_decl_gen_ (I_FALSE),
    cli_traits_gen_ (I_FALSE)
{
}

// Destructor
be_decl::~be_decl (void)
{
}

void
be_decl::compute_full_name  (const char *prefix,
                             const char *suffix,
                             char *&name)
{
  if (prefix == 0 || suffix == 0)
    return;

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);

  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == 0)
    {
      // Global scope.

      // Prefix.
      result_str = prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }
  else
    {
      // Get scope name.
      be_decl *parent = be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (parent == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_decl::"
                      "compute_flat_name - "
                      "scope name is nil\n"));
        }

      // Parent name.
      result_str = ACE_CString (parent->full_name ());

      // _
      if (ACE_OS::strcmp (parent->full_name (), "") != 0)
        result_str += ACE_CString ("::");

      // Prefix.
      result_str += prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  name = result_str.rep ();
}

void
be_decl::compute_flat_name  (const char *prefix,
                             const char *suffix,
                             char *&name)
{
  if (prefix == 0 || suffix == 0)
    {
      return;
    }

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);

  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == 0)
    {
      // Global scope.

      // Prefix.
      result_str = prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }
  else
    {
      // Get scope name.
      be_decl *parent =
        be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (parent == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_decl::"
                      "compute_flat_name - "
                      "scope name is nil\n"));
        }

      // Parent name.
      result_str = ACE_CString (parent->flat_name ());

      // _
      if (ACE_OS::strcmp (parent->flat_name (), "") != 0)
        result_str += "_";

      // Prefix.
      result_str += prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  name = result_str.rep ();
}

void
be_decl::destroy (void)
{
  this->AST_Decl::destroy ();
}

// Return the scope created by this node (if one exists, else NULL).
be_scope *
be_decl::scope (void)
{
  be_decl *d = this;

   switch (this->node_type ())
   {
     case AST_Decl::NT_interface_fwd:
        // Resolve forward declared interface by looking at full_definition()
        // field and iterating.
        d =
          be_interface::narrow_from_decl (
              (be_interface_fwd::narrow_from_decl (this))->full_definition ()
            );
     // Fall through
     case AST_Decl::NT_interface:
        return be_interface::narrow_from_decl (d);
     case AST_Decl::NT_module:
        return be_module::narrow_from_decl (d);
     case AST_Decl::NT_root:
        return be_root::narrow_from_decl (d);
     case AST_Decl::NT_except:
        return be_exception::narrow_from_decl (d);
     case AST_Decl::NT_union:
        return be_union::narrow_from_decl (d);
     case AST_Decl::NT_struct:
        return be_structure::narrow_from_decl (d);
     case AST_Decl::NT_enum:
        return be_enum::narrow_from_decl (d);
     case AST_Decl::NT_op:
        return be_operation::narrow_from_decl (d);
     case AST_Decl::NT_factory:
        return be_factory::narrow_from_decl (d);
     case AST_Decl::NT_sequence:
        return be_sequence::narrow_from_decl (d);
     case AST_Decl::NT_valuetype:
        return be_valuetype::narrow_from_decl (d);
     case AST_Decl::NT_component:
        return be_component::narrow_from_decl (d);
     case AST_Decl::NT_eventtype:
        return be_eventtype::narrow_from_decl (d);
     case AST_Decl::NT_home:
        return be_home::narrow_from_decl (d);
     default:
        return (be_scope *)0;
   }
}

// Boolean methods to test if code was already generated.
idl_bool
be_decl::cli_hdr_gen (void)
{
  return this->cli_hdr_gen_;
}

idl_bool
be_decl::cli_stub_gen (void)
{
  return this->cli_stub_gen_;
}

idl_bool
be_decl::cli_hdr_any_op_gen (void)
{
  return this->cli_hdr_any_op_gen_;
}

idl_bool
be_decl::cli_stub_any_op_gen (void)
{
  return this->cli_stub_any_op_gen_;
}

idl_bool
be_decl::cli_hdr_cdr_op_gen (void)
{
  return this->cli_hdr_cdr_op_gen_;
}

idl_bool
be_decl::cli_stub_cdr_op_gen (void)
{
  return this->cli_stub_cdr_op_gen_;
}

idl_bool
be_decl::cli_inline_cdr_op_gen (void)
{
  return this->cli_inline_cdr_op_gen_;
}

idl_bool
be_decl::cli_inline_cdr_decl_gen (void)
{
  return this->cli_inline_cdr_decl_gen_;
}

idl_bool
be_decl::cli_traits_gen (void)
{
  return this->cli_traits_gen_;
}

idl_bool
be_decl::cli_inline_gen (void)
{
  return this->cli_inline_gen_;
}

idl_bool
be_decl::srv_hdr_gen (void)
{
  return this->srv_hdr_gen_;
}

idl_bool
be_decl::impl_hdr_gen (void)
{
  return this->impl_hdr_gen_;
}

idl_bool
be_decl::srv_skel_gen (void)
{
  return this->srv_skel_gen_;
}

idl_bool
be_decl::impl_skel_gen (void)
{
  return this->impl_skel_gen_;
}

idl_bool
be_decl::srv_inline_gen (void)
{
  return this->srv_inline_gen_;
}

// Set the flag indicating that code generation is done.
void
be_decl::cli_hdr_gen (idl_bool val)
{
  this->cli_hdr_gen_ = val;
}

void
be_decl::cli_stub_gen (idl_bool val)
{
  this->cli_stub_gen_ = val;
}

void
be_decl::cli_hdr_any_op_gen (idl_bool val)
{
  this->cli_hdr_any_op_gen_ = val;
}

void
be_decl::cli_stub_any_op_gen (idl_bool val)
{
  this->cli_stub_any_op_gen_ = val;
}

void
be_decl::cli_hdr_cdr_op_gen (idl_bool val)
{
  this->cli_hdr_cdr_op_gen_ = val;
}

void
be_decl::cli_stub_cdr_op_gen (idl_bool val)
{
  this->cli_stub_cdr_op_gen_ = val;
}

void
be_decl::cli_inline_cdr_op_gen (idl_bool val)
{
  this->cli_inline_cdr_op_gen_ = val;
}

void
be_decl::cli_inline_cdr_decl_gen (idl_bool val)
{
  this->cli_inline_cdr_decl_gen_ = val;
}

void
be_decl::cli_inline_gen (idl_bool val)
{
  this->cli_inline_gen_ = val;
}

void
be_decl::cli_traits_gen (idl_bool val)
{
  this->cli_traits_gen_ = val;
}

void
be_decl::srv_hdr_gen (idl_bool val)
{
  this->srv_hdr_gen_ = val;
}

void
be_decl::impl_hdr_gen (idl_bool val)
{
  this->impl_hdr_gen_ = val;
}


void
be_decl::srv_skel_gen (idl_bool val)
{
  this->srv_skel_gen_ = val;
}

void
be_decl::srv_inline_gen (idl_bool val)
{
  this->srv_inline_gen_ = val;
}

int
be_decl::accept (be_visitor *visitor)
{
  return visitor->visit_decl (this);
}

void
be_decl::set_arg_seen_bit (be_type *bt)
{
  if (bt == 0)
    {
      return;
    }

  switch (bt->node_type ())
    {
      case NT_typedef:
        {
          AST_Typedef *td = AST_Typedef::narrow_from_decl (bt);
          this->set_arg_seen_bit (
                    be_type::narrow_from_decl (td->primitive_base_type ())
                  );
          break;
        }
      case NT_interface:
      case NT_interface_fwd:
      case NT_valuetype:
      case NT_valuetype_fwd:
      case NT_component:
      case NT_component_fwd:
      case NT_home:
      case NT_eventtype:
      case NT_eventtype_fwd:
        ACE_SET_BITS (idl_global->decls_seen_info_,
                      idl_global->decls_seen_masks.object_arg_seen_);
        break;        
      case NT_union:
      case NT_struct:
        if (bt->size_type () == AST_Type::FIXED)
          {
            ACE_SET_BITS (idl_global->decls_seen_info_,
                          idl_global->decls_seen_masks.fixed_size_arg_seen_);
          }
        else
          {
            ACE_SET_BITS (idl_global->decls_seen_info_,
                          idl_global->decls_seen_masks.var_size_arg_seen_);
          }

        break;
      case NT_struct_fwd:
      case NT_union_fwd:
        {
          AST_StructureFwd *fwd = AST_StructureFwd::narrow_from_decl (bt);
          be_type *fd = be_type::narrow_from_decl (fwd->full_definition ());
          this->set_arg_seen_bit (fd);
          break;
        }
      case NT_enum:
      case NT_enum_val:
        ACE_SET_BITS (idl_global->decls_seen_info_,
                      idl_global->decls_seen_masks.basic_arg_seen_);
        break;        
      case NT_string:
      case NT_wstring:
        {
          AST_String *str = AST_String::narrow_from_decl (bt);

          if (str->max_size ()->ev ()->u.ulval == 0)
            {
              ACE_SET_BITS (idl_global->decls_seen_info_,
                            idl_global->decls_seen_masks.ub_string_arg_seen_);
            }
          else
            {
              ACE_SET_BITS (idl_global->decls_seen_info_,
                            idl_global->decls_seen_masks.bd_string_arg_seen_);
            }

          break;
        }
      case NT_array:
        if (bt->size_type () == AST_Type::FIXED)
          {
            ACE_SET_BITS (idl_global->decls_seen_info_,
                          idl_global->decls_seen_masks.fixed_array_arg_seen_);
          }
        else
          {
            ACE_SET_BITS (idl_global->decls_seen_info_,
                          idl_global->decls_seen_masks.var_array_arg_seen_);
          }

        break;
      case NT_sequence:
        ACE_SET_BITS (idl_global->decls_seen_info_,
                      idl_global->decls_seen_masks.var_size_arg_seen_);
        break;        
      case NT_pre_defined:
        {
          AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (bt);
          
          switch (pdt->pt ())
            {
              case AST_PredefinedType::PT_object:
              case AST_PredefinedType::PT_pseudo:
              case AST_PredefinedType::PT_value:
                ACE_SET_BITS (idl_global->decls_seen_info_,
                              idl_global->decls_seen_masks.object_arg_seen_);
                break;        
              case AST_PredefinedType::PT_any:
                ACE_SET_BITS (idl_global->decls_seen_info_,
                              idl_global->decls_seen_masks.var_size_arg_seen_);
                break;        
              case AST_PredefinedType::PT_char:
              case AST_PredefinedType::PT_wchar:
              case AST_PredefinedType::PT_octet:
              case AST_PredefinedType::PT_boolean:
                ACE_SET_BITS (
                    idl_global->decls_seen_info_,
                    idl_global->decls_seen_masks.special_basic_arg_seen_
                  );
                break;
             default:
                ACE_SET_BITS (idl_global->decls_seen_info_,
                              idl_global->decls_seen_masks.basic_arg_seen_);
                break;
            }
        }
      default:
        break;
    }
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (be_decl, AST_Decl)
IMPL_NARROW_FROM_DECL (be_decl)
