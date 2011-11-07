
//=============================================================================
/**
 *  @file    be_string.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_String that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_string.h"
#include "be_visitor.h"
#include "be_helper.h"

#include "utl_identifier.h"
#include "global_extern.h"

be_string::be_string (AST_Decl::NodeType nt,
                      UTL_ScopedName * n,
                      AST_Expression * v,
                      long width)
  : COMMON_Base (),
    AST_Decl (nt,
              n,
              true),
    AST_Type (nt,
              n),
    AST_ConcreteType (nt,
                      n),
    AST_String (nt,
                n,
                v,
                width),
    be_decl (nt,
             n),
    be_type (nt,
             n)
{
  if (!this->imported ())
    {
      idl_global->string_seen_ = true;

      if (v->ev ()->u.ulval != 0)
        {
          idl_global->bd_string_seen_ = true;
        }
      else
        {
          idl_global->ub_string_seen_ = true;
        }
    }
}

// Overridden method.
void
be_string::gen_member_ostream_operator (TAO_OutStream *os,
                                        const char *instance_name,
                                        bool use_underscore,
                                        bool accessor)
{
  // For wide strings, generate code that outputs the hex values of
  // the individual wchars inside square brackets, otherwise generate
  // code that outputs the string literal bracketed with quotes.
  if (this->width () == 1)
    {
      *os << "\"\\\"\" << ";

      this->be_type::gen_member_ostream_operator (os,
                                                  instance_name,
                                                  use_underscore,
                                                  accessor);

      *os << " << \"\\\"\"";
    }
  else
    {
      *os << "\"[\";" << be_nl_2
          << "for (size_t i = 0; i < " << "ACE_OS::strlen ("
          << instance_name
          << (accessor ? " ()" : ".in ()") << "); ++i)" << be_idt_nl
          << "{" << be_idt_nl
          << "if (i != 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "strm << \", \";" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "strm << ACE_OutputCDR::from_wchar (" << instance_name
          << (accessor ? " ()" : "") << "[i]);" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "strm << \"]\"";
    }
}

// Overriden method.
void
be_string::compute_tc_name (void)
{
  Identifier * id = 0;
  ACE_CDR::ULong val = 0UL;
  AST_Expression zero (val);

  if (*this->max_size () == &zero)
    {
      // If the string is unbounded, use the string TypeCode
      // constants.

      // Start with the head as the CORBA namespace.
      Identifier * corba_id = 0;
      ACE_NEW (corba_id,
               Identifier ("CORBA"));

      ACE_NEW (this->tc_name_,
               UTL_ScopedName (corba_id,
                               0));

      ACE_NEW (id,
               Identifier (this->width () == 1
                           ? "_tc_string"
                           : "_tc_wstring"));
    }
  else
    {
      // We have a bounded string.  Generate a TypeCode name that is
      // meant for internal use alone.

      Identifier * tao_id = 0;
      ACE_NEW (tao_id,
               Identifier ("TAO"));

      ACE_NEW (this->tc_name_,
               UTL_ScopedName (tao_id,
                               0));

      ACE_CString local_tc_name =
        ACE_CString ("tc_")
        + ACE_CString (this->flat_name ());

      Identifier * typecode_scope = 0;
      ACE_NEW (typecode_scope,
               Identifier ("TypeCode"));

      UTL_ScopedName * tc_scope_conc_name = 0;
      ACE_NEW (tc_scope_conc_name,
               UTL_ScopedName (typecode_scope,
                               0));

      this->tc_name_->nconc (tc_scope_conc_name);

      ACE_NEW (id,
               Identifier (local_tc_name.c_str ()));
    }

  zero.destroy ();

  UTL_ScopedName *conc_name = 0;
  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           0));

  this->tc_name_->nconc (conc_name);
}

int
be_string::accept (be_visitor * visitor)
{
  return visitor->visit_string (this);
}

void
be_string::destroy (void)
{
  this->be_type::destroy ();
  this->AST_String::destroy ();
}

IMPL_NARROW_FROM_DECL (be_string)
