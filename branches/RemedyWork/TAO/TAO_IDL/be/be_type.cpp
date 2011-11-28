
//=============================================================================
/**
 *  @file    be_type.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Type that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_type.h"
#include "be_scope.h"
#include "be_visitor.h"
#include "be_codegen.h"
#include "be_helper.h"
#include "be_extern.h"
#include "be_util.h"

#include "ast_valuetype.h"
#include "ast_sequence.h"

#include "utl_identifier.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include <TAO_IDL/be_include/be_helper.h>

be_type::be_type (AST_Decl::NodeType nt,
                  UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (nt,
              n),
    AST_Type (nt,
              n),
    be_decl (nt,
             n),
    tc_name_ (0),
    common_varout_gen_ (false),
    seen_in_sequence_ (false),
    seen_in_operation_ (false)
{
  if (n != 0)
    {
      this->gen_fwd_helper_name ();
    }
}

be_type::~be_type (void)
{
}

// Compute the typecode name. The idea is to use the fully scoped name,
// however, prepend a _tc_ to the last component. A slightly different approach
// is required of the predefined types. Hence this method is overridden for
// predefined types.

void
be_type::compute_tc_name (void)
{
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n = this->name ();

  if (this->tc_name_ != 0)
    {
      this->tc_name_->destroy ();
      delete this->tc_name_;
      this->tc_name_ = 0;
    }

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  while (n->tail () != 0)
    {
      // Does not exist.
      if (this->tc_name_ == 0)
        {
          ACE_NEW (this->tc_name_,
                   UTL_ScopedName (n->head ()->copy (),
                                   0));
        }
      else
        {
          UTL_ScopedName *conc_name = 0;
          ACE_NEW (conc_name,
                   UTL_ScopedName (n->head ()->copy (),
                                   0));

          this->tc_name_->nconc (conc_name);
        }

      n = (UTL_ScopedName *)n->tail ();
    }

  ACE_OS::sprintf (namebuf,
                   "_tc_%s",
                   n->last_component ()->get_string ());

  Identifier *id = 0;
  ACE_NEW (id,
           Identifier (namebuf));

  // Does not exist.
  if (this->tc_name_ == 0)
    {
      ACE_NEW (this->tc_name_,
               UTL_ScopedName (id,
                               0));
    }
  else
    {
      UTL_ScopedName *conc_name = 0;
      ACE_NEW (conc_name,
               UTL_ScopedName (id,
                               0));

      this->tc_name_->nconc (conc_name);
    }
}

// Retrieve typecode name.
UTL_ScopedName *
be_type::tc_name (void)
{
  // Compute and init the member.
  if (this->tc_name_ == 0)
    {
      this->compute_tc_name ();
    }

  return this->tc_name_;
}

// This works for the "special" names generated for smart proxy
// classes. The form of these names is
// scope'TAO_'+flat_name+'_Smart_Proxy_Base'.
//
// We can use the flat_name() method for the local_name parm but have
// to construct the full name ourselves.
const char *
be_type::nested_sp_type_name (be_decl *use_scope,
                              const char *suffix,
                              const char *prefix)
{
  // Our defining scope.
  be_decl *fu_scope = 0;

  char fu_name [NAMEBUFSIZE];
  char fl_name [NAMEBUFSIZE];

  ACE_OS::memset (fu_name,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::memset (fl_name,
                  '\0',
                  NAMEBUFSIZE);

  fu_scope = this->defined_in ()
               ? be_scope::narrow_from_scope (this->defined_in ())->decl ()
               : 0;

  ACE_OS::strcat (fu_name,
                  fu_scope->full_name ());

  ACE_OS::strcat (fu_name,
                  "::TAO_");

  ACE_OS::strcat (fu_name,
                  this->flat_name());

  ACE_OS::strcat (fl_name,
                  "TAO_");

  ACE_OS::strcat (fl_name,
                  this->flat_name());

  return this->nested_name (fl_name,
                            fu_name,
                            use_scope,
                            suffix,
                            prefix);
}

void
be_type::gen_fwd_helper_name (void)
{
  AST_Decl *parent = ScopeAsDecl (this->defined_in ());
  Identifier *segment = 0;
  char *tmp = 0;
  this->fwd_helper_name_.clear (true);

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      for (UTL_IdListActiveIterator i (parent->name ());
           !i.is_done ();
           i.next ())
        {
          segment = i.item ();
          tmp = segment->get_string ();

          if (ACE_OS::strcmp (tmp, "") == 0)
            {
              continue;
            }

          this->fwd_helper_name_ += tmp;
          this->fwd_helper_name_ += "::";
        }
    }
  else
    {
      this->fwd_helper_name_= "";
    }

  this->fwd_helper_name_ += "tao_";
  this->fwd_helper_name_ += this->local_name ()->get_string ();
}

void
be_type::gen_ostream_operator (TAO_OutStream *,
                               bool /* use_underscore */)
{
}

void
be_type::gen_member_ostream_operator (TAO_OutStream *os,
                                      const char *instance_name,
                                      bool /* use_underscore */,
                                      bool accessor)
{
  *os << instance_name << (accessor ? " ()" : "");
}

const char *
be_type::fwd_helper_name (void) const
{
  return this->fwd_helper_name_.fast_rep ();
}

void
be_type::fwd_helper_name (const char *name)
{
  this->fwd_helper_name_ = name;
}

void
be_type::gen_common_varout (TAO_OutStream *os)
{
  if (this->common_varout_gen_)
    {
      return;
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  AST_Type::SIZE_TYPE st = this->size_type ();

  *os << be_nl_2
      << (this->node_type () == AST_Decl::NT_struct ? "struct "
                                                    : "class ")
      << this->local_name () << ";";

  *os << be_nl_2
      << "typedef" << be_idt_nl
      << (st == AST_Type::FIXED ? "::TAO_Fixed_Var_T<"
                                : "::TAO_Var_Var_T<")
      << be_idt << be_idt_nl
      << this->local_name () << be_uidt_nl
      << ">" << be_uidt_nl
      << this->local_name () << "_var;" << be_uidt << be_nl_2;

  if (st == AST_Type::FIXED)
    {
      *os << "typedef" << be_idt_nl
          << this->local_name () << " &" << be_nl
          << this->local_name () << "_out;" << be_uidt;
    }
  else
    {
      *os << "typedef" << be_idt_nl
          << "::TAO_Out_T<" << be_idt << be_idt_nl
          << this->local_name () << be_uidt_nl
          << ">" << be_uidt_nl
          << this->local_name () << "_out;" << be_uidt;
    }

  this->common_varout_gen_ = true;
}

void
be_type::gen_stub_decls (TAO_OutStream *os)
{
  if (this->anonymous ())
    {
      return;
    }

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  AST_Interface *i = AST_Interface::narrow_from_decl (this);
  AST_ValueType *v = AST_ValueType::narrow_from_decl (this);

  if (i != 0)
    {
      *os << "typedef " << this->local_name ()
          << (v == 0 ? "_ptr" : " *") << " _ptr_type;";
    }

  bool skip_varout = false;
  AST_Sequence *s = AST_Sequence::narrow_from_decl (this);

  if (s != 0)
    {
      // _vars and _outs not supported yet by alt mapping.
      if (be_global->alt_mapping () && s->unbounded ())
        {
          skip_varout = true;
        }
    }

  if (!skip_varout)
    {
      *os << be_nl
          << "typedef " << this->local_name ()
          << "_var _var_type;" << be_nl
          << "typedef " << this->local_name ()
          << "_out _out_type;";
    }

  bool gen_any_destructor =
    be_global->any_support ()
    && (!this->is_local ()
        || be_global->gen_local_iface_anyops ());

  if (gen_any_destructor)
    {
      *os << be_nl_2
          << "static void _tao_any_destructor (void *);";
    }
}

bool
be_type::seen_in_sequence (void) const
{
  return this->seen_in_sequence_;
}

void
be_type::seen_in_sequence (bool val)
{
  this->seen_in_sequence_ = val;
}

bool
be_type::seen_in_operation (void) const
{
  return this->seen_in_operation_;
}

void
be_type::seen_in_operation (bool val)
{
  this->seen_in_operation_ = val;
}

AST_Decl::NodeType
be_type::base_node_type (void) const
{
  return const_cast<be_type*> (this)->node_type ();
}

// Cleanup method
void
be_type::destroy (void)
{
  if (this->tc_name_ != 0)
    {
      this->tc_name_->destroy ();
      delete this->tc_name_;
      this->tc_name_ = 0;
    }

  this->be_decl::destroy ();
}

int
be_type::accept (be_visitor *visitor)
{
  return visitor->visit_type (this);
}



IMPL_NARROW_FROM_DECL (be_type)
