
//=============================================================================
/**
 *  @file    be_decl.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Decl that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

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
#include "be_finder.h"
#include "be_sequence.h"
#include "be_visitor.h"

#include "ast_structure_fwd.h"
#include "ast_typedef.h"
#include "ast_string.h"

#include "utl_identifier.h"

#include "global_extern.h"
#include "ace/Log_Msg.h"
#include "ace/String_Base.h"

be_decl::be_decl (AST_Decl::NodeType type,
                  UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (type,
              n),
    cli_hdr_gen_ (false),
    cli_stub_gen_ (false),
    cli_inline_gen_ (false),
    srv_hdr_gen_ (false),
    impl_hdr_gen_ (false),
    srv_skel_gen_ (false),
    impl_skel_gen_ (false),
    srv_inline_gen_ (false),
    tie_skel_gen_ (false),
    cli_hdr_any_op_gen_ (false),
    cli_stub_any_op_gen_ (false),
    cli_hdr_cdr_op_gen_ (false),
    cli_stub_cdr_op_gen_ (false),
    cli_inline_cdr_op_gen_ (false),
    cli_inline_cdr_decl_gen_ (false),
    cli_traits_gen_ (false),
    cli_arg_traits_gen_ (false),
    srv_arg_traits_gen_ (false),
    srv_sarg_traits_gen_ (false),
    cli_pragma_inst_gen_ (false),
    cli_inarg_tmpl_class_gen_ (false),
    cli_inarg_pragma_inst_gen_ (false),
    cli_inoutarg_tmpl_class_gen_ (false),
    cli_inoutarg_pragma_inst_gen_ (false),
    cli_outarg_tmpl_class_gen_ (false),
    cli_outarg_pragma_inst_gen_ (false),
    cli_retarg_tmpl_class_gen_ (false),
    cli_retarg_pragma_inst_gen_ (false),
    srv_tmpl_class_gen_ (false),
    srv_pragma_inst_gen_ (false),
    srv_inarg_tmpl_class_gen_ (false),
    srv_inarg_pragma_inst_gen_ (false),
    srv_inoutarg_tmpl_class_gen_ (false),
    srv_inoutarg_pragma_inst_gen_ (false),
    srv_outarg_tmpl_class_gen_ (false),
    srv_outarg_pragma_inst_gen_ (false),
    srv_retarg_tmpl_class_gen_ (false),
    srv_retarg_pragma_inst_gen_ (false),
    ccm_pre_proc_gen_ (false),
    ex_idl_facet_gen_ (false),
    svnt_hdr_facet_gen_ (false),
    svnt_src_facet_gen_ (false),
    exec_hdr_facet_gen_ (false),
    exec_src_facet_gen_ (false),
    ami4ccm_ex_idl_gen_ (false)
{
}

be_decl::~be_decl (void)
{
}

void
be_decl::compute_full_name  (const char *prefix,
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
      be_decl * const parent =
        be_scope::narrow_from_scope (this->defined_in ())->decl ();

      if (parent == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_decl::"
                      "compute_full_name - "
                      "scope name is nil\n"));

          return;
        }

      // Parent name.
      result_str = ACE_CString (parent->full_name ());

      // _
      if (ACE_OS::strcmp (parent->full_name (), "") != 0)
        {
          result_str += ACE_CString ("::");
        }

      // Prefix.
      result_str += prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  name = ACE_OS::strdup (result_str.fast_rep ());
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

      // Local name. Leave out _cxx_ prefix, if any.
      result_str +=
        ACE_CString (
          this->original_local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }
  else
    {
      // Get scope name.
      be_decl * const parent =
        be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (parent == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_decl::"
                      "compute_flat_name - "
                      "scope name is nil\n"));

          return;
        }

      // Parent name.
      result_str = ACE_CString (parent->flat_name ());

      // _
      if (ACE_OS::strcmp (parent->flat_name (), "") != 0)
        result_str += "_";

      // Prefix.
      result_str += prefix_str;

      // Local name. Leave out _cxx_ prefix, if any.
      result_str +=
        ACE_CString (
          this->original_local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  name = ACE_OS::strdup (result_str.c_str ());
}

void
be_decl::destroy (void)
{
}

void
be_decl::set_local (bool val)
{
  this->is_local_ = val;
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
    case AST_Decl::NT_finder:
      return be_finder::narrow_from_decl (d);
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
bool
be_decl::cli_hdr_gen (void)
{
  return this->cli_hdr_gen_;
}

bool
be_decl::cli_stub_gen (void)
{
  return this->cli_stub_gen_;
}

bool
be_decl::cli_hdr_any_op_gen (void)
{
  return this->cli_hdr_any_op_gen_;
}

bool
be_decl::cli_stub_any_op_gen (void)
{
  return this->cli_stub_any_op_gen_;
}

bool
be_decl::cli_hdr_cdr_op_gen (void)
{
  return this->cli_hdr_cdr_op_gen_;
}

bool
be_decl::cli_stub_cdr_op_gen (void)
{
  return this->cli_stub_cdr_op_gen_;
}

bool
be_decl::cli_inline_cdr_op_gen (void)
{
  return this->cli_inline_cdr_op_gen_;
}

bool
be_decl::cli_inline_cdr_decl_gen (void)
{
  return this->cli_inline_cdr_decl_gen_;
}

bool
be_decl::cli_traits_gen (void)
{
  return this->cli_traits_gen_;
}

bool
be_decl::cli_arg_traits_gen (void)
{
  return this->cli_arg_traits_gen_;
}

bool
be_decl::srv_arg_traits_gen (void)
{
  return this->srv_arg_traits_gen_;
}

bool
be_decl::srv_sarg_traits_gen (void)
{
  return this->srv_sarg_traits_gen_;
}

bool
be_decl::cli_pragma_inst_gen (void)
{
  return this->cli_pragma_inst_gen_;
}

bool
be_decl::cli_inarg_tmpl_class_gen (void)
{
  return this->cli_inarg_tmpl_class_gen_;
}

bool
be_decl::cli_inarg_pragma_inst_gen (void)
{
  return this->cli_inarg_pragma_inst_gen_;
}

bool
be_decl::cli_inoutarg_tmpl_class_gen (void)
{
  return this->cli_inoutarg_tmpl_class_gen_;
}

bool
be_decl::cli_inoutarg_pragma_inst_gen (void)
{
  return this->cli_inoutarg_pragma_inst_gen_;
}

bool
be_decl::cli_outarg_tmpl_class_gen (void)
{
  return this->cli_outarg_tmpl_class_gen_;
}

bool
be_decl::cli_outarg_pragma_inst_gen (void)
{
  return this->cli_outarg_pragma_inst_gen_;
}

bool
be_decl::cli_retarg_tmpl_class_gen (void)
{
  return this->cli_retarg_tmpl_class_gen_;
}

bool
be_decl::cli_retarg_pragma_inst_gen (void)
{
  return this->cli_retarg_pragma_inst_gen_;
}

bool
be_decl::srv_tmpl_class_gen (void)
{
  return this->srv_tmpl_class_gen_;
}

bool
be_decl::srv_pragma_inst_gen (void)
{
  return this->srv_pragma_inst_gen_;
}

bool
be_decl::srv_inarg_tmpl_class_gen (void)
{
  return this->srv_inarg_tmpl_class_gen_;
}

bool
be_decl::srv_inarg_pragma_inst_gen (void)
{
  return this->srv_inarg_pragma_inst_gen_;
}

bool
be_decl::srv_inoutarg_tmpl_class_gen (void)
{
  return this->srv_inoutarg_tmpl_class_gen_;
}

bool
be_decl::srv_inoutarg_pragma_inst_gen (void)
{
  return this->srv_inoutarg_pragma_inst_gen_;
}

bool
be_decl::srv_outarg_tmpl_class_gen (void)
{
  return this->srv_outarg_tmpl_class_gen_;
}

bool
be_decl::srv_outarg_pragma_inst_gen (void)
{
  return this->srv_outarg_pragma_inst_gen_;
}

bool
be_decl::srv_retarg_tmpl_class_gen (void)
{
  return this->srv_retarg_tmpl_class_gen_;
}

bool
be_decl::srv_retarg_pragma_inst_gen (void)
{
  return this->cli_retarg_pragma_inst_gen_;
}

bool
be_decl::cli_inline_gen (void)
{
  return this->cli_inline_gen_;
}

bool
be_decl::srv_hdr_gen (void)
{
  return this->srv_hdr_gen_;
}

bool
be_decl::impl_hdr_gen (void)
{
  return this->impl_hdr_gen_;
}

bool
be_decl::srv_skel_gen (void)
{
  return this->srv_skel_gen_;
}

bool
be_decl::impl_skel_gen (void)
{
  return this->impl_skel_gen_;
}

bool
be_decl::srv_inline_gen (void)
{
  return this->srv_inline_gen_;
}

bool
be_decl::tie_skel_gen (void)
{
  return this->tie_skel_gen_;
}

bool
be_decl::ccm_pre_proc_gen (void)
{
  return this->ccm_pre_proc_gen_;
}

bool
be_decl::ex_idl_facet_gen (void)
{
  return this->ex_idl_facet_gen_;
}

bool
be_decl::svnt_hdr_facet_gen (void)
{
  return this->svnt_hdr_facet_gen_;
}

bool
be_decl::svnt_src_facet_gen (void)
{
  return this->svnt_src_facet_gen_;
}

bool
be_decl::exec_hdr_facet_gen (void)
{
  return this->exec_hdr_facet_gen_;
}

bool
be_decl::exec_src_facet_gen (void)
{
  return this->exec_src_facet_gen_;
}

bool
be_decl::ami4ccm_ex_idl_gen (void)
{
  return this->ami4ccm_ex_idl_gen_;
}

// Set the flag indicating that code generation is done.
void
be_decl::cli_hdr_gen (bool val)
{
  this->cli_hdr_gen_ = val;
}

void
be_decl::cli_stub_gen (bool val)
{
  this->cli_stub_gen_ = val;
}

void
be_decl::cli_hdr_any_op_gen (bool val)
{
  this->cli_hdr_any_op_gen_ = val;
}

void
be_decl::cli_stub_any_op_gen (bool val)
{
  this->cli_stub_any_op_gen_ = val;
}

void
be_decl::cli_hdr_cdr_op_gen (bool val)
{
  this->cli_hdr_cdr_op_gen_ = val;
}

void
be_decl::cli_stub_cdr_op_gen (bool val)
{
  this->cli_stub_cdr_op_gen_ = val;
}

void
be_decl::cli_inline_cdr_op_gen (bool val)
{
  this->cli_inline_cdr_op_gen_ = val;
}

void
be_decl::cli_inline_cdr_decl_gen (bool val)
{
  this->cli_inline_cdr_decl_gen_ = val;
}

void
be_decl::cli_inline_gen (bool val)
{
  this->cli_inline_gen_ = val;
}

void
be_decl::cli_traits_gen (bool val)
{
  this->cli_traits_gen_ = val;
}

void
be_decl::cli_arg_traits_gen (bool val)
{
  this->cli_arg_traits_gen_ = val;
}

void
be_decl::srv_arg_traits_gen (bool val)
{
  this->srv_arg_traits_gen_ = val;
}

void
be_decl::srv_sarg_traits_gen (bool val)
{
  this->srv_sarg_traits_gen_ = val;
}

void
be_decl::cli_pragma_inst_gen (bool val)
{
  this->cli_pragma_inst_gen_ = val;
}

void
be_decl::cli_inarg_tmpl_class_gen (bool val)
{
  this->cli_inarg_tmpl_class_gen_ = val;
}

void
be_decl::cli_inarg_pragma_inst_gen (bool val)
{
  this->cli_inarg_pragma_inst_gen_ = val;
}

void
be_decl::cli_inoutarg_tmpl_class_gen (bool val)
{
  this->cli_inoutarg_tmpl_class_gen_ = val;
}

void
be_decl::cli_inoutarg_pragma_inst_gen (bool val)
{
  this->cli_inoutarg_pragma_inst_gen_ = val;
}

void
be_decl::cli_outarg_tmpl_class_gen (bool val)
{
  this->cli_outarg_tmpl_class_gen_ = val;
}

void
be_decl::cli_outarg_pragma_inst_gen (bool val)
{
  this->cli_outarg_pragma_inst_gen_ = val;
}

void
be_decl::cli_retarg_tmpl_class_gen (bool val)
{
  this->cli_retarg_tmpl_class_gen_ = val;
}

void
be_decl::cli_retarg_pragma_inst_gen (bool val)
{
  this->cli_retarg_pragma_inst_gen_ = val;
}

void
be_decl::srv_tmpl_class_gen (bool val)
{
  this->srv_tmpl_class_gen_ = val;
}

void
be_decl::srv_pragma_inst_gen (bool val)
{
  this->srv_pragma_inst_gen_ = val;
}

void
be_decl::srv_inarg_tmpl_class_gen (bool val)
{
  this->srv_inarg_tmpl_class_gen_ = val;
}

void
be_decl::srv_inarg_pragma_inst_gen (bool val)
{
  this->srv_inarg_pragma_inst_gen_ = val;
}

void
be_decl::srv_inoutarg_tmpl_class_gen (bool val)
{
  this->srv_inoutarg_tmpl_class_gen_ = val;
}

void
be_decl::srv_inoutarg_pragma_inst_gen (bool val)
{
  this->srv_inoutarg_pragma_inst_gen_ = val;
}

void
be_decl::srv_outarg_tmpl_class_gen (bool val)
{
  this->srv_outarg_tmpl_class_gen_ = val;
}

void
be_decl::srv_outarg_pragma_inst_gen (bool val)
{
  this->srv_outarg_pragma_inst_gen_ = val;
}

void
be_decl::srv_retarg_tmpl_class_gen (bool val)
{
  this->srv_retarg_tmpl_class_gen_ = val;
}

void
be_decl::srv_retarg_pragma_inst_gen (bool val)
{
  this->srv_retarg_pragma_inst_gen_ = val;
}

void
be_decl::srv_hdr_gen (bool val)
{
  this->srv_hdr_gen_ = val;
}

void
be_decl::impl_hdr_gen (bool val)
{
  this->impl_hdr_gen_ = val;
}


void
be_decl::srv_skel_gen (bool val)
{
  this->srv_skel_gen_ = val;
}

void
be_decl::srv_inline_gen (bool val)
{
  this->srv_inline_gen_ = val;
}

void
be_decl::tie_skel_gen (bool val)
{
  this->tie_skel_gen_ = val;
}

void
be_decl::ccm_pre_proc_gen (bool val)
{
  this->ccm_pre_proc_gen_ = val;
}

void
be_decl::ex_idl_facet_gen (bool val)
{
  this->ex_idl_facet_gen_ = val;
}

void
be_decl::svnt_hdr_facet_gen (bool val)
{
  this->svnt_hdr_facet_gen_ = val;
}

void
be_decl::svnt_src_facet_gen (bool val)
{
  this->svnt_src_facet_gen_ = val;
}

void
be_decl::exec_hdr_facet_gen (bool val)
{
  this->exec_hdr_facet_gen_ = val;
}

void
be_decl::exec_src_facet_gen (bool val)
{
  this->exec_src_facet_gen_ = val;
}

void
be_decl::ami4ccm_ex_idl_gen (bool val)
{
  this->ami4ccm_ex_idl_gen_ = val;
}

//==========================================

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
        idl_global->object_arg_seen_ = true;
        break;
      case NT_union:
      case NT_struct:
        if (bt->size_type () == AST_Type::FIXED)
          {
            idl_global->fixed_size_arg_seen_ = true;
          }
        else
          {
            idl_global->var_size_arg_seen_ = true;
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
        idl_global->basic_arg_seen_ = true;
        break;
      case NT_string:
      case NT_wstring:
        {
          AST_String *str = AST_String::narrow_from_decl (bt);

          if (str->max_size ()->ev ()->u.ulval == 0)
            {
              idl_global->ub_string_arg_seen_ = true;
            }
          else
            {
              idl_global->bd_string_arg_seen_ = true;
            }

          break;
        }
      case NT_array:
        if (bt->size_type () == AST_Type::FIXED)
          {
            idl_global->fixed_array_arg_seen_ = true;
          }
        else
          {
            idl_global->var_array_arg_seen_ = true;
          }

        break;
      case NT_sequence:
        idl_global->var_size_arg_seen_ = true;
        break;
      case NT_pre_defined:
        {
          AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (bt);

          switch (pdt->pt ())
            {
              case AST_PredefinedType::PT_object:
              case AST_PredefinedType::PT_pseudo:
              case AST_PredefinedType::PT_value:
              case AST_PredefinedType::PT_abstract:
                idl_global->object_arg_seen_ = true;
                break;
              case AST_PredefinedType::PT_any:
                idl_global->var_size_arg_seen_ = true;
                idl_global->any_arg_seen_ = true;
                break;
              case AST_PredefinedType::PT_char:
              case AST_PredefinedType::PT_wchar:
              case AST_PredefinedType::PT_octet:
              case AST_PredefinedType::PT_boolean:
                idl_global->special_basic_arg_seen_ = true;
                break;
             default:
                idl_global->basic_arg_seen_ = true;
                break;
            }
        }
      default:
        break;
    }
}



IMPL_NARROW_FROM_DECL (be_decl)
