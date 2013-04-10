
//=============================================================================
/**
 *  @file    be_enum.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Enum that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_enum.h"
#include "be_visitor.h"
#include "be_helper.h"

#include "global_extern.h"

be_enum::be_enum (UTL_ScopedName *n,
                  bool local,
                  bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_enum,
              n),
    AST_Type (AST_Decl::NT_enum,
              n),
    AST_ConcreteType (AST_Decl::NT_enum,
                      n),
    UTL_Scope (AST_Decl::NT_enum),
    AST_Enum (n,
              local,
              abstract),
    be_scope (AST_Decl::NT_enum),
    be_decl (AST_Decl::NT_enum,
             n),
    be_type (AST_Decl::NT_enum,
             n)
{
  if (!this->imported ())
    {
      idl_global->enum_seen_ = true;
      idl_global->fixed_size_decl_seen_ = true;
    }
}

void
be_enum::gen_ostream_operator (TAO_OutStream *os,
                               bool /* use_underscore */)
{
  *os << be_nl
      << "std::ostream& operator<< (std::ostream &strm, const "
      << this->name () << " _tao_enumerator)" << be_nl
      << "{" << be_idt_nl
      << "switch (_tao_enumerator)" << be_idt_nl
      << "{" << be_idt_nl;

  // The enum's type name itself is not part of the scope of the
  // enum values. If the enum is defined at global scope, this will
  // produce an empty string when streamed to the output file.
  UTL_ScopedName *s = ScopeAsDecl (this->defined_in ())->name ();

  for (UTL_ScopeActiveIterator i (this, IK_decls); !i.is_done (); i.next ())
    {
      Identifier *id = i.item ()->local_name ();

      *os << "case " << s << "::" << id
          << ": return strm << \"" << s << "::" << id
          << "\";" << be_nl;
    }

  *os << "default: return strm;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl;
}

void
be_enum::destroy (void)
{
  // Call the destroy methods of our base classes.
  // No need to call be_scope::destroy(). It has no
  // allocated members, and AST_Enum::destroy() will
  // call UTL_Scope::destroy().
  this->be_type::destroy ();
  this->be_scope::destroy ();
  this->AST_Enum::destroy ();
}

int
be_enum::accept (be_visitor *visitor)
{
  return visitor->visit_enum (this);
}



IMPL_NARROW_FROM_DECL (be_enum)
IMPL_NARROW_FROM_SCOPE (be_enum)
