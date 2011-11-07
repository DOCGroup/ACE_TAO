// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "ast_eventtype.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

AST_Decl::NodeType const
AST_EventType::NT = AST_Decl::NT_eventtype;

AST_EventType::AST_EventType (UTL_ScopedName *n,
                              AST_Type **inherits,
                              long n_inherits,
                              AST_Type *inherits_concrete,
                              AST_Interface **inherits_flat,
                              long n_inherits_flat,
                              AST_Type **supports,
                              long n_supports,
                              AST_Type *supports_concrete,
                              bool abstract,
                              bool truncatable,
                              bool custom)
  : COMMON_Base (false,
                 abstract),
    AST_Decl (AST_Decl::NT_eventtype,
              n),
    AST_Type (AST_Decl::NT_eventtype,
              n),
    UTL_Scope (AST_Decl::NT_eventtype),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   false,
                   abstract),
    AST_ValueType (n,
                   inherits,
                   n_inherits,
                   inherits_concrete,
                   inherits_flat,
                   n_inherits_flat,
                   supports,
                   n_supports,
                   supports_concrete,
                   abstract,
                   truncatable,
                   custom)
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
      this->dump_i (o, "abstract ");
    }
  else if (this->pd_truncatable)
    {
      this->dump_i (o, "truncatable ");
    }

  this->dump_i (o, "eventtype ");

  this->local_name ()->dump (o);
  this->dump_i (o, " ");

  if (this->pd_n_inherits > 0)
    {
      this->dump_i (o, ": ");

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              this->dump_i (o, ", ");
            }
        }
    }

  this->dump_i (o, "\n\n");

  if (this->pd_n_supports > 0)
    {
      this->dump_i (o, "supports ");

      for (long i = 0; i < this->pd_n_supports; ++i)
        {
          this->pd_supports[i]->local_name ()->dump (o);

          if (i < this->pd_n_supports - 1)
            {
              this->dump_i (o, ", ");
            }
        }
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

int
AST_EventType::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_eventtype (this);
}

  // Narrowing.

IMPL_NARROW_FROM_DECL(AST_EventType)
IMPL_NARROW_FROM_SCOPE(AST_EventType)

