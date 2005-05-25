// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "ast_valuetype.h"
#include "ast_factory.h"
#include "ast_visitor.h"
#include "ast_extern.h"
#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "ace/streams.h"

ACE_RCSID (ast, 
           ast_valuetype, 
           "$Id$")

AST_ValueType::AST_ValueType (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    pd_supports (0),
    pd_n_supports (0),
    pd_inherits_concrete (0),
    pd_supports_concrete (0),
    pd_truncatable (I_FALSE),
    pd_custom (I_FALSE)
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
                              idl_bool truncatable,
                              idl_bool custom)
  : COMMON_Base (I_FALSE,
                 abstract),
    AST_Decl (AST_Decl::NT_valuetype,
              n),
    AST_Type (AST_Decl::NT_valuetype,
              n),
    UTL_Scope (AST_Decl::NT_valuetype),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   I_FALSE,
                   abstract),
    pd_supports (supports),
    pd_n_supports (n_supports),
    pd_inherits_concrete (inherits_concrete),
    pd_supports_concrete (supports_concrete),
    pd_truncatable (truncatable),
    pd_custom (custom)
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

idl_bool
AST_ValueType::custom (void) const
{
  return this->pd_custom;
}

idl_bool
AST_ValueType::will_have_factory (void)
{
  return I_FALSE;
}

// Look through supported interface list.
AST_Decl *
AST_ValueType::look_in_supported (UTL_ScopedName *e,
                                  idl_bool treat_as_ref)
{
  AST_Decl *d = 0;
  AST_Decl *d_before = 0;
  AST_Interface **is = 0;
  long nis = -1;

  // Can't look in an interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->fwd_decl_lookup (this,
                                           e);
      return 0;
    }

  // OK, loop through supported interfaces.

  // (Don't leave the inheritance hierarchy, no module or global ...)
  // Find all and report ambiguous results as error.

  for (nis = this->n_supports (), is = this->supports ();
       nis > 0;
       nis--, is++)
    {
      d = (*is)->lookup_by_name (e,
                                 treat_as_ref,
                                 0 /* not in parent */);
      if (d != 0)
        {
          if (d_before == 0)
            {
              // First result found.
              d_before = d;
            }
          else
            {
              // Conflict against further results?
              if (d != d_before)
                {
                  ACE_ERROR ((LM_ERROR,
                              "warning in %s line %d: ",
                              idl_global->filename ()->get_string (),
                              idl_global->lineno ()));

                  e->dump (*ACE_DEFAULT_LOG_STREAM);

                  ACE_ERROR ((LM_ERROR,
                              " is ambiguous in scope.\n"
                              "Found "));

                  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);

                  ACE_ERROR ((LM_ERROR,
                              " and "));

                  d_before->name ()->dump (*ACE_DEFAULT_LOG_STREAM);

                  ACE_ERROR ((LM_ERROR,
                              ".\n"));
                }
            }
        }
    }

  return d_before;
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
      this->dump_i (o, "abstract ");
    }
  else if (this->pd_truncatable)
    {
      this->dump_i (o, "truncatable ");
    }

  this->dump_i (o, "valuetype ");

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

