// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "ast_eventtype.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast, 
           ast_eventtype, 
           "$Id$")

AST_EventType::AST_EventType (void)
{
}

AST_EventType::AST_EventType (UTL_ScopedName *n,
                              AST_Interface **inherits,
                              long n_inherits,
                              AST_ValueType *inherits_concrete,
                              AST_Interface **inherits_flat,
                              long n_inherits_flat,
                              AST_Interface **supports,
                              long n_supports,
                              AST_Interface *supports_concrete,
                              idl_bool abstract,
                              idl_bool truncatable)
  : AST_ValueType (n,
                   inherits,
                   n_inherits,
                   inherits_concrete,
                   inherits_flat,
                   n_inherits_flat,
                   supports,
                   n_supports,
                   supports_concrete,
                   abstract,
                   truncatable),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   I_FALSE,
                   abstract),
    AST_Type (AST_Decl::NT_eventtype,
              n),
    AST_Decl (AST_Decl::NT_eventtype,
              n),
    UTL_Scope (AST_Decl::NT_eventtype),
    COMMON_Base (I_FALSE,
                 abstract)
{
}

AST_EventType::~AST_EventType (void)
{
}

void
AST_EventType::destroy (void)
{
  this->AST_ValueType::destroy ();
}

void
AST_EventType::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_abstract ())
    {
      o << "abstract ";
    }
  else if (this->pd_truncatable)
    {
      o << "truncatable ";
    }

  o << "eventtype ";

  this->local_name ()->dump (o);
  o << " ";

  if (this->pd_n_inherits > 0)
    {
      o << ": ";

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              o << ", ";
            }
        }
    }

  o << "\n\n";

  if (this->pd_n_supports > 0)
    {
      o << "supports ";

      for (long i = 0; i < this->pd_n_supports; ++i)
        {
          this->pd_supports[i]->local_name ()->dump (o);

          if (i < this->pd_n_supports - 1)
            {
              o << ", ";
            }
        }
    }

  o << " {\n";

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  o << "}";
}

int
AST_EventType::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_eventtype (this);
}

  // Narrowing.
IMPL_NARROW_METHODS1(AST_EventType, AST_ValueType)
IMPL_NARROW_FROM_DECL(AST_EventType)
IMPL_NARROW_FROM_SCOPE(AST_EventType)

