// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "ast_valuetype.h"
#include "ast_factory.h"
#include "ast_visitor.h"
#include "ast_extern.h"
#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"
#include "nr_extern.h"

ACE_RCSID (ast, 
           ast_valuetype, 
           "$Id$")

AST_ValueType::AST_ValueType (void)
  : pd_supports (0),
    pd_n_supports (0),
    pd_inherits_concrete (0),
    pd_supports_concrete (0),
    pd_truncatable (I_FALSE)
{
}

AST_ValueType::AST_ValueType (UTL_ScopedName *n,
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
  : AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   I_FALSE,
                   abstract),
    AST_Type (AST_Decl::NT_valuetype,
              n),
    AST_Decl (AST_Decl::NT_valuetype,
              n),
    UTL_Scope (AST_Decl::NT_valuetype),
    COMMON_Base (I_FALSE,
                 abstract),
    pd_supports (supports),
    pd_n_supports (n_supports),
    pd_inherits_concrete (inherits_concrete),
    pd_supports_concrete (supports_concrete),
    pd_truncatable (truncatable)           
{
}

AST_ValueType::~AST_ValueType (void)
{
}

void
AST_ValueType::redefine (AST_Interface *from)
{
  AST_ValueType *vt = AST_ValueType::narrow_from_decl (from);

  if (vt == 0)
    {
      idl_global->err ()->redef_error (from->local_name ()->get_string (),
                                       this->local_name ()->get_string ());
      return;
    }

  // Copy over all the base class members.
  this->AST_Interface::redefine (from);

  this->pd_inherits_concrete = vt->pd_inherits_concrete;
  this->pd_supports_concrete = vt->pd_supports_concrete;
  this->pd_truncatable = vt->pd_truncatable;
}

AST_Interface **
AST_ValueType::supports (void) const
{
  return this->pd_supports;
}

long
AST_ValueType::n_supports (void) const
{
  return this->pd_n_supports;
}

AST_ValueType *
AST_ValueType::inherits_concrete (void) const
{
  return this->pd_inherits_concrete;
}

AST_Interface *
AST_ValueType::supports_concrete (void) const
{
  return this->pd_supports_concrete;
}

idl_bool
AST_ValueType::truncatable (void) const
{
  return this->pd_truncatable;
}

void
AST_ValueType::destroy (void)
{
  this->AST_Interface::destroy ();
}

void
AST_ValueType::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_abstract ())
    {
      o << "abstract ";
    }
  else if (this->pd_truncatable)
    {
      o << "truncatable ";
    }

  o << "valuetype ";

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
AST_ValueType::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_valuetype (this);
}

AST_Factory *
AST_ValueType::fe_add_factory (AST_Factory *f)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  f);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (f, I_FALSE)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      f,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, f->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      f,
                                      this,
                                      d);
          return 0;
        }

      if (f->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope (f,
                                                     d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (f);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (f,
                           I_FALSE,
                           f->local_name ());

  return f;
}

  // Narrowing.
IMPL_NARROW_METHODS1(AST_ValueType, AST_Interface)
IMPL_NARROW_FROM_DECL(AST_ValueType)
IMPL_NARROW_FROM_SCOPE(AST_ValueType)

