
//=============================================================================
/**
 *  @file    be_structure.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Structure that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_structure.h"
#include "be_field.h"
#include "be_codegen.h"
#include "be_helper.h"
#include "be_visitor.h"
#include "be_extern.h"

#include "utl_identifier.h"
#include "idl_defines.h"
#include "global_extern.h"

be_structure::be_structure (UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_struct,
              n),
    AST_Type (AST_Decl::NT_struct,
              n),
    AST_ConcreteType (AST_Decl::NT_struct,
                      n),
    UTL_Scope (AST_Decl::NT_struct),
    AST_Structure (n,
                   local,
                   abstract),
    be_scope (AST_Decl::NT_struct),
    be_decl (AST_Decl::NT_struct,
             n),
    be_type (AST_Decl::NT_struct,
             n)
{
  if (!this->imported ())
    {
      idl_global->aggregate_seen_ = true;
    }
}

be_structure::be_structure (AST_Decl::NodeType nt,
                            UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (nt,
              n),
    AST_Type (nt,
              n),
    AST_ConcreteType (nt,
                      n),
    UTL_Scope (nt),
    AST_Structure (nt,
                   n,
                   local,
                   abstract),
    be_scope (nt),
    be_decl (nt,
             n),
    be_type (nt,
             n)
{
  if (!this->imported ())
    {
      idl_global->aggregate_seen_ = true;
    }
}

void
be_structure::redefine (AST_Structure *from)
{
  be_structure *bs = be_structure::narrow_from_decl (from);
  this->common_varout_gen_ = bs->common_varout_gen_;
  this->AST_Structure::redefine (from);
}

// Overridden method
void
be_structure::gen_ostream_operator (TAO_OutStream *os,
                                    bool /* use_underscore */)
{
  *os << be_nl
      << "std::ostream& operator<< (" << be_idt << be_idt_nl
      << "std::ostream &strm," << be_nl
      << "const " << this->name () << " &";

  long n = this->pd_decls_used;

  // be_exception is a subclass and could be empty.
  if (n > 0)
    {
      *os <<  "_tao_aggregate";
    }

  *os << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "strm << \"" << this->name () << "(\"";

  for (long i = 0; i < n; ++i)
    {
      be_field *f = be_field::narrow_from_decl (this->pd_decls[i]);

      // We don't want any decls, just members.
      if (f == 0)
        {
          continue;
        }

      if (i != 0)
        {
          *os << " << \", \"";
        }

      *os << be_nl
          << "     << ";

      ACE_CString instance_name ("_tao_aggregate.");
      instance_name += f->local_name ()->get_string ();
      AST_Decl::NodeType nt = f->field_type ()->node_type ();
      bool member_use_underscore =
        nt == AST_Decl::NT_array || nt == AST_Decl::NT_sequence;
      f->gen_member_ostream_operator (os,
                                      instance_name.c_str (),
                                      member_use_underscore,
                                      false);
    }

  *os << be_nl
      << "     << \")\";" << be_nl << be_nl
      << "return strm;" << be_uidt_nl
      << "}" << be_nl;
}

void
be_structure::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_type::destroy ();
  this->AST_Structure::destroy ();
}

int
be_structure::accept (be_visitor *visitor)
{
  return visitor->visit_structure (this);
}

IMPL_NARROW_FROM_DECL (be_structure)
IMPL_NARROW_FROM_SCOPE (be_structure)
