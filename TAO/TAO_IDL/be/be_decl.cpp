
//=============================================================================
/**
 *  @file    be_decl.cpp
 *
 *  Extension of class AST_Decl that provides additional means for C++
 *  mapping.
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

be_decl::~be_decl ()
{
}

void
be_decl::compute_full_name  (const char *prefix,
                             const char *suffix,
                             char *&name)
{
  if (prefix == nullptr || suffix == nullptr)
    {
      return;
    }

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);
  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == nullptr)
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
        dynamic_cast<be_scope*> (this->defined_in ())->decl ();

      if (parent == nullptr)
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
  if (prefix == nullptr || suffix == nullptr)
    {
      return;
    }

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);

  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == nullptr)
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
        dynamic_cast<be_scope*> (this->defined_in ())->decl ();
      if (parent == nullptr)
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
be_decl::destroy ()
{
}

void
be_decl::set_local (bool val)
{
  this->is_local_ = val;
}

// Return the scope created by this node (if one exists, else NULL).
be_scope *
be_decl::scope ()
{
  be_decl *d = this;

  switch (this->node_type ())
  {
    case AST_Decl::NT_interface_fwd:
      // Resolve forward declared interface by looking at full_definition()
      // field and iterating.
      d =
        dynamic_cast<be_interface*> (
            (dynamic_cast<be_interface_fwd*> (this))->full_definition ()
          );
    // Fall through
    case AST_Decl::NT_interface:
      return dynamic_cast<be_interface*> (d);
    case AST_Decl::NT_module:
      return dynamic_cast<be_module*> (d);
    case AST_Decl::NT_root:
      return dynamic_cast<be_root*> (d);
    case AST_Decl::NT_except:
      return dynamic_cast<be_exception*> (d);
    case AST_Decl::NT_union:
      return dynamic_cast<be_union*> (d);
    case AST_Decl::NT_struct:
      return dynamic_cast<be_structure*> (d);
    case AST_Decl::NT_enum:
      return dynamic_cast<be_enum*> (d);
    case AST_Decl::NT_op:
      return dynamic_cast<be_operation*> (d);
    case AST_Decl::NT_factory:
      return dynamic_cast<be_factory*> (d);
    case AST_Decl::NT_finder:
      return dynamic_cast<be_finder*> (d);
    case AST_Decl::NT_sequence:
      return dynamic_cast<be_sequence*> (d);
    case AST_Decl::NT_valuetype:
      return dynamic_cast<be_valuetype*> (d);
    case AST_Decl::NT_component:
      return dynamic_cast<be_component*> (d);
    case AST_Decl::NT_eventtype:
      return dynamic_cast<be_eventtype*> (d);
    case AST_Decl::NT_home:
      return dynamic_cast<be_home*> (d);
    default:
      return nullptr;
  }
}

// Boolean methods to test if code was already generated.
bool
be_decl::cli_hdr_gen ()
{
  return this->cli_hdr_gen_;
}

bool
be_decl::cli_stub_gen ()
{
  return this->cli_stub_gen_;
}

bool
be_decl::cli_hdr_any_op_gen ()
{
  return this->cli_hdr_any_op_gen_;
}

bool
be_decl::cli_stub_any_op_gen ()
{
  return this->cli_stub_any_op_gen_;
}

bool
be_decl::cli_hdr_cdr_op_gen ()
{
  return this->cli_hdr_cdr_op_gen_;
}

bool
be_decl::cli_stub_cdr_op_gen ()
{
  return this->cli_stub_cdr_op_gen_;
}

bool
be_decl::cli_inline_cdr_op_gen ()
{
  return this->cli_inline_cdr_op_gen_;
}

bool
be_decl::cli_inline_cdr_decl_gen ()
{
  return this->cli_inline_cdr_decl_gen_;
}

bool
be_decl::cli_traits_gen ()
{
  return this->cli_traits_gen_;
}

bool
be_decl::cli_arg_traits_gen ()
{
  return this->cli_arg_traits_gen_;
}

bool
be_decl::srv_arg_traits_gen ()
{
  return this->srv_arg_traits_gen_;
}

bool
be_decl::srv_sarg_traits_gen ()
{
  return this->srv_sarg_traits_gen_;
}

bool
be_decl::cli_pragma_inst_gen ()
{
  return this->cli_pragma_inst_gen_;
}

bool
be_decl::cli_inarg_tmpl_class_gen ()
{
  return this->cli_inarg_tmpl_class_gen_;
}

bool
be_decl::cli_inarg_pragma_inst_gen ()
{
  return this->cli_inarg_pragma_inst_gen_;
}

bool
be_decl::cli_inoutarg_tmpl_class_gen ()
{
  return this->cli_inoutarg_tmpl_class_gen_;
}

bool
be_decl::cli_inoutarg_pragma_inst_gen ()
{
  return this->cli_inoutarg_pragma_inst_gen_;
}

bool
be_decl::cli_outarg_tmpl_class_gen ()
{
  return this->cli_outarg_tmpl_class_gen_;
}

bool
be_decl::cli_outarg_pragma_inst_gen ()
{
  return this->cli_outarg_pragma_inst_gen_;
}

bool
be_decl::cli_retarg_tmpl_class_gen ()
{
  return this->cli_retarg_tmpl_class_gen_;
}

bool
be_decl::cli_retarg_pragma_inst_gen ()
{
  return this->cli_retarg_pragma_inst_gen_;
}

bool
be_decl::srv_tmpl_class_gen ()
{
  return this->srv_tmpl_class_gen_;
}

bool
be_decl::srv_pragma_inst_gen ()
{
  return this->srv_pragma_inst_gen_;
}

bool
be_decl::srv_inarg_tmpl_class_gen ()
{
  return this->srv_inarg_tmpl_class_gen_;
}

bool
be_decl::srv_inarg_pragma_inst_gen ()
{
  return this->srv_inarg_pragma_inst_gen_;
}

bool
be_decl::srv_inoutarg_tmpl_class_gen ()
{
  return this->srv_inoutarg_tmpl_class_gen_;
}

bool
be_decl::srv_inoutarg_pragma_inst_gen ()
{
  return this->srv_inoutarg_pragma_inst_gen_;
}

bool
be_decl::srv_outarg_tmpl_class_gen ()
{
  return this->srv_outarg_tmpl_class_gen_;
}

bool
be_decl::srv_outarg_pragma_inst_gen ()
{
  return this->srv_outarg_pragma_inst_gen_;
}

bool
be_decl::srv_retarg_tmpl_class_gen ()
{
  return this->srv_retarg_tmpl_class_gen_;
}

bool
be_decl::srv_retarg_pragma_inst_gen ()
{
  return this->cli_retarg_pragma_inst_gen_;
}

bool
be_decl::cli_inline_gen ()
{
  return this->cli_inline_gen_;
}

bool
be_decl::srv_hdr_gen ()
{
  return this->srv_hdr_gen_;
}

bool
be_decl::impl_hdr_gen ()
{
  return this->impl_hdr_gen_;
}

bool
be_decl::srv_skel_gen ()
{
  return this->srv_skel_gen_;
}

bool
be_decl::impl_skel_gen ()
{
  return this->impl_skel_gen_;
}

bool
be_decl::srv_inline_gen ()
{
  return this->srv_inline_gen_;
}

bool
be_decl::tie_skel_gen ()
{
  return this->tie_skel_gen_;
}

bool
be_decl::ccm_pre_proc_gen ()
{
  return this->ccm_pre_proc_gen_;
}

bool
be_decl::ex_idl_facet_gen ()
{
  return this->ex_idl_facet_gen_;
}

bool
be_decl::svnt_hdr_facet_gen ()
{
  return this->svnt_hdr_facet_gen_;
}

bool
be_decl::svnt_src_facet_gen ()
{
  return this->svnt_src_facet_gen_;
}

bool
be_decl::exec_hdr_facet_gen ()
{
  return this->exec_hdr_facet_gen_;
}

bool
be_decl::exec_src_facet_gen ()
{
  return this->exec_src_facet_gen_;
}

bool
be_decl::ami4ccm_ex_idl_gen ()
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
