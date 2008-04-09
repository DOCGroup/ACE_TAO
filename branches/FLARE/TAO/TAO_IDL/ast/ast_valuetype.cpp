// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "ast_valuetype.h"
#include "ast_factory.h"
#include "ast_visitor.h"
#include "ast_extern.h"
#include "ast_field.h"
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
    pd_truncatable (false),
    pd_custom (false)
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
                              bool abstract,
                              bool truncatable,
                              bool custom)
  : COMMON_Base (false,
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
                   false,
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

bool
AST_ValueType::in_recursion (ACE_Unbounded_Queue<AST_Type *> &list)
{
  // We should calculate this only once. If it has already been
  // done, just return it.
  if (this->in_recursion_ != -1)
    {
      return this->in_recursion_;
    }

  list.enqueue_tail (this);

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::in_recursion - "
                             "bad node in this scope\n"),
                            0);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (field == 0)
        {
          continue;
        }

      AST_Type *type = field->field_type ();

      // A valuetype may contain itself as a member. This will not
      // cause a problem when checking if the valuetype itself is
      // recursive, but if another valuetype contains a recursive
      // one, the string compare below is not sufficient, and we
      // will go into an infinite recursion of calls to in_recursion ;-).
      // The check below will catch that use case.
      if (this == type)
        {
          this->in_recursion_ = 1;
          idl_global->recursive_type_seen_ = true;
          return this->in_recursion_;
        }

      if (type == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::in_recursion - "
                             "bad base type\n"),
                            0);
        }

      // IDL doesn't have such a feature as name reuse so
      // just compare fully qualified names.
      if (this->match_names (this, list))
        {
          this->in_recursion_ = 1;
          idl_global->recursive_type_seen_ = true;
          return this->in_recursion_;
        }

      if (type->node_type () == AST_Decl::NT_typedef)
        {
          AST_Typedef *td = AST_Typedef::narrow_from_decl (type);
          type = td->primitive_base_type ();
        }

      // Now hand over to our field type.
      if (type->in_recursion (list))
        {
          this->in_recursion_ = 1;
          idl_global->recursive_type_seen_ = true;
          return this->in_recursion_;
        }

    } // end of for loop

  this->in_recursion_ = 0;
  return this->in_recursion_;
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

bool
AST_ValueType::truncatable (void) const
{
  return this->pd_truncatable;
}

bool
AST_ValueType::custom (void) const
{
  return this->pd_custom;
}

bool
AST_ValueType::will_have_factory (void)
{
  return false;
}

// Look through supported interface list.
AST_Decl *
AST_ValueType::look_in_supported (UTL_ScopedName *e,
                                  bool treat_as_ref)
{
  AST_Decl *d = 0;
  AST_Decl *d_before = 0;
  AST_Interface **is = 0;
  long nis = -1;

  // Can't look in an interface which was not yet defined.
  if (!this->is_defined ())
    {
//      idl_global->err ()->fwd_decl_lookup (this,
//                                           e);
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

bool
AST_ValueType::legal_for_primary_key (void) const
{
  AST_ValueType *pk_base = this->lookup_primary_key_base ();

  if (!this->derived_from_primary_key_base (this, pk_base))
    {
      return false;
    }

  bool has_public_member = false;
  bool retval = true;

  if (!this->recursing_in_legal_pk_)
    {
      this->recursing_in_legal_pk_ = true;

      for (UTL_ScopeActiveIterator i (const_cast<AST_ValueType *> (this),
                                      UTL_Scope::IK_decls);
          !i.is_done ();
          i.next ())
        {
          AST_Field *f = AST_Field::narrow_from_decl (i.item ());

          // We're not interested in any valuetype decls that aren't fields.
          if (f == 0)
            {
              continue;
            }

          // Private members are not allowed in primary keys.
          if (f->visibility () == AST_Field::vis_PRIVATE)
            {
              retval = false;
              break;
            }
          else
            {
              // Returns false for interfaces, components, homes.
              // Called recursively on valuetypes and on members of
              // structs, unions, sequences, typedefs and arrays. Returns
              // TRUE otherwise.
              if (!f->field_type ()->legal_for_primary_key ())
                {
                  retval = false;
                  break;
                }

              has_public_member = true;
            }
        }

        this->recursing_in_legal_pk_ = false;
    }

  // Must have at least one public member, unless we are
  // short-circuiting the test because we are in recursion.
  return retval && (has_public_member || this->recursing_in_legal_pk_);
}

void
AST_ValueType::destroy (void)
{
  delete [] this->pd_supports;
  this->pd_supports = 0;
  this->pd_n_supports = 0;

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
  if ((d = this->lookup_for_add (f, false)) != 0)
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
                           false,
                           f->local_name ());

  return f;
}

bool
AST_ValueType::derived_from_primary_key_base (const AST_ValueType *node,
                                              const AST_ValueType *pk_base) const
{
  if (0 == node)
    {
      return false;
    }

  if (node == pk_base)
    {
      return true;
    }

  AST_ValueType *concrete_parent = node->inherits_concrete ();

  if (this->derived_from_primary_key_base (concrete_parent, pk_base))
    {
      return true;
    }

  AST_Interface **v = node->pd_inherits;

  for (long i = 0; i < node->pd_n_inherits; ++i)
    {
      AST_ValueType *tmp = AST_ValueType::narrow_from_decl (v[i]);

      if (this->derived_from_primary_key_base (tmp, pk_base))
        {
          return true;
        }
    }

  return false;
}

AST_ValueType *
AST_ValueType::lookup_primary_key_base (void) const
{
  AST_ValueType *retval = idl_global->primary_key_base ();

  if (retval == 0)
    {
      Identifier local_id ("PrimaryKeyBase");
      UTL_ScopedName local_name (&local_id, 0);

      Identifier scope_name ("Components");
      UTL_ScopedName pk_name (&scope_name, &local_name);
      AST_Decl *d =
        const_cast<AST_ValueType *> (this)->lookup_by_name (&pk_name, true);

      local_id.destroy ();
      scope_name.destroy ();

      if (d == 0)
        {
          idl_global->err ()->lookup_error (&pk_name);
          return 0;
        }

      retval = AST_ValueType::narrow_from_decl (d);

      if (retval == 0)
        {
          idl_global->err ()->valuetype_expected (d);
          return 0;
        }

      idl_global->primary_key_base (retval);
    }

  return retval;
}



IMPL_NARROW_FROM_DECL(AST_ValueType)
IMPL_NARROW_FROM_SCOPE(AST_ValueType)

