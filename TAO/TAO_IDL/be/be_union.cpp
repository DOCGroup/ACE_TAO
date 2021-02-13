
//=============================================================================
/**
 *  @file    be_union.cpp
 *
 *  Extension of class AST_Union that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_union.h"
#include "be_union_branch.h"
#include "be_union_label.h"
#include "be_visitor.h"
#include "be_codegen.h"
#include "be_helper.h"
#include "be_extern.h"

#include "ast_union_branch.h"
#include "ast_enum.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "global_extern.h"

be_union::be_union (AST_ConcreteType *dt,
                    UTL_ScopedName *n,
                    bool local,
                    bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_union,
              n),
    AST_Type (AST_Decl::NT_union,
              n),
    AST_ConcreteType (AST_Decl::NT_union,
                      n),
    UTL_Scope (AST_Decl::NT_union),
    AST_Structure (AST_Decl::NT_union,
                   n,
                   local,
                   abstract),
    AST_Union (dt,
               n,
               local,
               abstract),
    be_scope (AST_Decl::NT_union),
    be_decl (AST_Decl::NT_union,
             n),
    be_type (AST_Decl::NT_union,
             n)
{
  // Always the case.
  this->has_constructor (true);

  if (!this->imported ())
    {
      idl_global->aggregate_seen_ = true;

      idl_global->union_seen_ = true;
    }
}

void
be_union::redefine (AST_Structure *from)
{
  be_union *bu = dynamic_cast<be_union*> (from);
  this->common_varout_gen_ = bu->common_varout_gen_;
  AST_Union::redefine (from);
}

bool
be_union::has_duplicate_case_labels ()
{
  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_UnionBranch *ub =
        dynamic_cast<AST_UnionBranch*> (d);

      if (ub->label_list_length () > 1)
        {
          return true;
        }
    }

  return false;
}

void
be_union::gen_ostream_operator (TAO_OutStream *os,
                                bool /*use_underscore*/)
{
  *os << be_nl
      << "std::ostream& operator<< (" << be_idt << be_idt_nl
      << "std::ostream &strm," << be_nl
      << "const " << this->name () << " &_tao_union" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "strm << \"" << this->name () << "(\";" << be_nl_2
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt;

  for (long i = 0; i < this->pd_decls_used; ++i)
    {
      be_union_branch *ub =
        dynamic_cast<be_union_branch*> (this->pd_decls[i]);

      // We don't want any decls, just members.
      if (ub == nullptr)
        {
          continue;
        }

      *os << be_nl;

      unsigned long ll_len = ub->label_list_length ();

      for (unsigned long j = 0; j < ll_len; ++j)
        {
          // Check if we are printing the default case.
          if (ub->label (j)->label_kind () == AST_UnionLabel::UL_default)
            {
              *os << "default:";
            }
          else
            {
              *os << "case ";

              ub->gen_label_value (os, j);

              *os << ":";
            }

          if (j == ll_len - 1)
            {
              *os << be_idt_nl;
            }
          else
            {
              *os << be_nl;
            }
        }

      ACE_CString instance_name ("_tao_union.");
      instance_name += ub->local_name ()->get_string ();

      *os << "strm << ";

      be_type *ub_ft = dynamic_cast<be_type*> (ub->field_type ());
      AST_Decl::NodeType ub_nt = ub_ft->node_type ();
      // catch anonymous Array member types
      bool ub_use_underscore = ub_nt == AST_Decl::NT_array;

      ub->gen_member_ostream_operator (os,
                                       instance_name.c_str (),
                                       ub_use_underscore,
                                       true);

      *os << ";" << be_nl
          << "break;" << be_uidt;
    }

  // Some compilers complain unless this is present, but only
  // if not all values are covered in case statements.
  if (this->gen_empty_default_label ())
    {
      *os << be_nl
          << "default:" << be_idt_nl
          << "break;" << be_uidt;
    }

  *os << be_uidt_nl
      << "}" << be_nl_2
      << "return strm << \")\";" << be_uidt_nl
      << "}" << be_nl;
}

void
be_union::destroy ()
{
  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_type::destroy ();
  this->AST_Union::destroy ();

}

// Visitor method.
int
be_union::accept (be_visitor *visitor)
{
  return visitor->visit_union (this);
}

bool
be_union::gen_empty_default_label ()
{
  // A non-empty explicit default label will be generated.
  if (this->default_index () != -1)
    {
      return false;
    }

  AST_ConcreteType *disc = this->disc_type ();
  if (disc == nullptr)
    {
      return true; // In reality this is an error.
    }

  AST_Decl::NodeType nt = disc->node_type ();
  ACE_UINT64 n_labels = this->nlabels ();

  if (nt == AST_Decl::NT_enum)
    {
      // Enums in CORBA are always 32bits in size, so unless
      // there are that many enum labels in the set, it is
      // incomplete (reguardless as to the actual member_count).
      return (n_labels <= ACE_UINT32_MAX);
    }

  AST_PredefinedType *pdt = dynamic_cast<AST_PredefinedType*> (disc);
  if (pdt == nullptr)
    {
      return true; // In reality this is an error.
    }

  switch (pdt->pt ())
    {
    case AST_PredefinedType::PT_boolean:
      return (n_labels < 2);

    case AST_PredefinedType::PT_char:
      return (n_labels <= ACE_OCTET_MAX);

    case AST_PredefinedType::PT_short:
    case AST_PredefinedType::PT_ushort:
      return (n_labels <= ACE_UINT16_MAX);

    case AST_PredefinedType::PT_long:
    case AST_PredefinedType::PT_ulong:
      return (n_labels <= ACE_UINT32_MAX);

    case AST_PredefinedType::PT_longlong:
    case AST_PredefinedType::PT_ulonglong:
      // We would wrap to 0 here - we are using a 64 bit count
      // this case is so marginal as to always be incomplete.
      return true;

    // Keep fussy compilers happy.
    default:
      break;
    }

  return true;
}

AST_UnionBranch *
be_union::be_add_union_branch (AST_UnionBranch *b)
{
  return this->fe_add_union_branch (b);
}

ACE_UINT64
be_union::nlabels ()
{
  ACE_UINT64 retval = 0;

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_UnionBranch *ub =
        dynamic_cast<AST_UnionBranch*> (d);

      if (ub != nullptr)
        {
          retval += ub->label_list_length ();
        }
    }

  return retval;
}
