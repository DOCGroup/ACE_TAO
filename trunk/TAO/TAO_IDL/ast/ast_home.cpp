// $Id$

#include "ast_home.h"
#include "ast_component.h"
#include "ast_valuetype.h"
#include "ast_param_holder.h"
#include "ast_operation.h"
#include "ast_finder.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "global_extern.h"

AST_Decl::NodeType const
AST_Home::NT = AST_Decl::NT_home;

AST_Home::AST_Home (UTL_ScopedName *n,
                    AST_Home *base_home,
                    AST_Component *managed_component,
                    AST_Type *primary_key,
                    AST_Type **supports,
                    long n_supports,
                    AST_Interface **supports_flat,
                    long n_supports_flat)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_home,
              n),
    AST_Type (AST_Decl::NT_home,
              n),
    UTL_Scope (AST_Decl::NT_home),
    AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   false,
                   false),
    pd_base_home (base_home),
    pd_managed_component (managed_component),
    pd_primary_key (primary_key),
    owns_primary_key_ (false)
{
  AST_ValueType *pk =
    AST_ValueType::narrow_from_decl (primary_key);

  if (pk != 0)
    {
      idl_global->primary_keys ().enqueue_tail (pk);
    }
  else if (primary_key != 0)
    {
      // If we are here, it's a param holder and we must destroy it.
      this->owns_primary_key_ = true;
    }
}

AST_Home::~AST_Home (void)
{
}

AST_Decl *
AST_Home::look_in_inherited (UTL_ScopedName *e,
                             bool full_def_only)
{
  AST_Decl *d = 0;

  if (this->pd_base_home != 0)
    {
      d =
        this->pd_base_home->lookup_by_name (e, full_def_only);
    }

  return d;
}

// Look through supported interface list.
AST_Decl *
AST_Home::look_in_supported (UTL_ScopedName *e,
                             bool full_def_only)
{
  AST_Decl *d = 0;
  AST_Type **is = 0;
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
      if ((*is)->node_type () == AST_Decl::NT_param_holder)
        {
          continue;
        }

      AST_Interface *i =
        AST_Interface::narrow_from_decl (*is);

      d = (i)->lookup_by_name_r (e, full_def_only);

      if (d != 0)
        {
          break;
        }
    }

  return d;
}

AST_Decl *
AST_Home::special_lookup (UTL_ScopedName *e,
                          bool full_def_only)
{
  AST_Decl *d = this->look_in_inherited (e, full_def_only);

  if (d == 0)
    {
      d = this->look_in_supported (e, full_def_only);
    }

  return d;
}

AST_Home *
AST_Home::base_home (void) const
{
  return this->pd_base_home;
}

// These next two look ugly, but it is to keep from having to
// create separate visitors for homes in the back end.

AST_Type **
AST_Home::supports (void) const
{
  return
    this->pd_base_home == 0
      ? this->inherits ()
      : this->inherits () + 1;
}

long
AST_Home::n_supports (void) const
{
  return this->n_inherits ();
}

AST_Component *
AST_Home::managed_component (void) const
{
  return this->pd_managed_component;
}

AST_Type *
AST_Home::primary_key (void) const
{
  return this->pd_primary_key;
}

void
AST_Home::destroy (void)
{
  // If it's a param holder, it was created on the fly.
  if (owns_primary_key_)
    {
      this->pd_primary_key->destroy ();
      delete this->pd_primary_key;
      this->pd_primary_key = 0;
    }

  // We have to go through these conniptions to destroy
  // a home because its decls (for which there are no
  // copy constructors) are assigned to the scope
  // of the equivalent interface, which will destroy
  // them. But we still have to destroy the containers
  // for those references, which may be private or
  // protected.

  delete [] this->inherits ();
  delete [] this->inherits_flat ();

  delete [] this->pd_decls;
  this->pd_decls = 0;
  this->pd_decls_allocated = 0;
  this->pd_decls_used = 0;

  delete [] this->pd_referenced;
  this->pd_referenced = 0;
  this->pd_referenced_allocated = 0;
  this->pd_referenced_used = 0;

  // These are stored by copying the Identifier.
  for (long i = 0; i < this->pd_name_referenced_used; ++i)
    {
      this->pd_name_referenced[i]->destroy ();
      delete this->pd_name_referenced[i];
      this->pd_name_referenced[i] = 0;
    }

  delete [] this->pd_name_referenced;
  this->pd_name_referenced = 0;
  this->pd_name_referenced_allocated = 0;
  this->pd_name_referenced_used = 0;

  // Skip AST_Interface, since the home's decls
  // are added to the equivalent interface, and
  // they should get destroyed there.
  this->AST_Type::destroy ();
}

void
AST_Home::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "home ");

  this->local_name ()->dump (o);

  this->dump_i (o, " ");

  if (this->pd_base_home != 0)
    {
      this->dump_i (o, ": ");
      this->pd_base_home->local_name ()->dump (o);
    }

  if (this->pd_managed_component != 0)
    {
      this->dump_i (o, "\n");
      this->dump_i (o, "manages ");
      this->pd_managed_component->local_name ()->dump (o);
    }

  if (this->pd_primary_key != 0)
    {
      this->dump_i (o, "\n");
      this->dump_i (o, "primary key ");
      this->pd_primary_key->local_name ()->dump (o);
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

int
AST_Home::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_home (this);
}

AST_Factory *
AST_Home::fe_add_factory (AST_Factory *f)
{
  return
    AST_Factory::narrow_from_decl (
      this->fe_add_decl (f));
}

AST_Finder *
AST_Home::fe_add_finder (AST_Finder *f)
{
  return
    AST_Finder::narrow_from_decl (
      this->fe_add_decl (f));
}

  // Narrowing.

IMPL_NARROW_FROM_DECL(AST_Home)
IMPL_NARROW_FROM_SCOPE(AST_Home)

