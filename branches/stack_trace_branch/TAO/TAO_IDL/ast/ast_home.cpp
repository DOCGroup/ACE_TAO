// $Id$

#include "ast_home.h"
#include "ast_component.h"
#include "ast_valuetype.h"
#include "ast_operation.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_home,
           "$Id$")

AST_Home::AST_Home (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    pd_base_home (0),
    pd_managed_component (0),
    pd_primary_key (0)
{
}

AST_Home::AST_Home (UTL_ScopedName *n,
                    AST_Home *base_home,
                    AST_Component *managed_component,
                    AST_ValueType *primary_key,
                    AST_Interface **supports,
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
    pd_primary_key (primary_key)
{
  if (primary_key != 0)
    {
      idl_global->primary_keys ().enqueue_tail (primary_key);
    }
}

AST_Home::~AST_Home (void)
{
}

AST_Decl *
AST_Home::look_in_inherited (UTL_ScopedName *e,
                             bool treat_as_ref)
{
  AST_Decl *d = 0;

  if (this->pd_base_home != 0)
    {
      d = this->pd_base_home->lookup_by_name (e, treat_as_ref);
    }

  return d;
}

// Look through supported interface list.
AST_Decl *
AST_Home::look_in_supported (UTL_ScopedName *e,
                             bool treat_as_ref)
{
  AST_Decl *d = 0;
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
          break;
        }
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

AST_Interface **
AST_Home::supports (void) const
{
  return this->pd_base_home ? this->inherits () + 1 : this->inherits ();
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

AST_ValueType *
AST_Home::primary_key (void) const
{
  return this->pd_primary_key;
}

ACE_Unbounded_Queue<AST_Operation *> &
AST_Home::factories (void)
{
  return this->pd_factories;
}

ACE_Unbounded_Queue<AST_Operation *> &
AST_Home::finders (void)
{
  return this->pd_finders;
}

void
AST_Home::destroy (void)
{
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

  // Narrowing.

IMPL_NARROW_FROM_DECL(AST_Home)
IMPL_NARROW_FROM_SCOPE(AST_Home)

