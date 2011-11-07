
//=============================================================================
/**
 *  @file    be_home.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Home that provides additional means for C++
 *  mapping of a component home.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_home.h"
#include "be_component.h"
#include "be_visitor.h"

#include "ast_attribute.h"

#include "global_extern.h"
#include "utl_err.h"

be_home::be_home (UTL_ScopedName *n,
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
    AST_Home (n,
              base_home,
              managed_component,
              primary_key,
              supports,
              n_supports,
              supports_flat,
              n_supports_flat),
    be_scope (AST_Decl::NT_home),
    be_decl (AST_Decl::NT_home,
             n),
    be_type (AST_Decl::NT_home,
             n),
    be_interface (n,
                  supports,
                  n_supports,
                  supports_flat,
                  n_supports_flat,
                  false,
                  false)
{
  this->size_type (AST_Type::VARIABLE);

  // Some previous error may have caused a lookup failure, in which
  // case we'll crash if we do the narrow below.
  if (managed_component == 0)
    {
      idl_global->set_err_count (idl_global->err_count () + 1);
      return;
    }

  be_component *bt =
     be_component::narrow_from_decl (managed_component);

  bt->seen_in_operation (true);

  idl_global->object_arg_seen_ = true;
}

be_home::~be_home (void)
{
}

void
be_home::scan (UTL_Scope *s)
{
  if (s == 0)
    {
      return;
    }

  for (UTL_ScopeActiveIterator i (s, UTL_Scope::IK_both);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Attribute *attr =
        AST_Attribute::narrow_from_decl (d);

      if (attr != 0 && ! attr->readonly ())
        {
          this->has_rw_attributes_ = true;
          return;
        }
    }

  AST_Home *h = AST_Home::narrow_from_scope (s);

  if (h != 0)
    {
      this->scan (h->base_home ());
    }
}

void
be_home::destroy (void)
{
  delete [] this->full_skel_name_;
  this->full_skel_name_ = 0;

  delete [] this->full_coll_name_;
  this->full_coll_name_ = 0;

  delete [] this->local_coll_name_;
  this->local_coll_name_ = 0;

  delete [] this->relative_skel_name_;
  this->relative_skel_name_ = 0;

  delete [] this->direct_proxy_impl_name_;
  this->direct_proxy_impl_name_ = 0;

  delete [] this->full_direct_proxy_impl_name_;
  this->full_direct_proxy_impl_name_ = 0;

  delete [] this->client_scope_;
  this->client_scope_ = 0;

  delete [] this->flat_client_scope_;
  this->flat_client_scope_ = 0;

  delete [] this->server_scope_;
  this->server_scope_ = 0;

  delete [] this->flat_server_scope_;
  this->flat_server_scope_ = 0;

  this->be_scope::destroy ();
  this->be_type::destroy ();

  // This skips AST_Interface, for the reason above.
  this->AST_Home::destroy ();
}

int
be_home::accept (be_visitor *visitor)
{
  return (idl_global->ignore_idl3 ()
            ? 0
            : visitor->visit_home (this));
}

IMPL_NARROW_FROM_DECL (be_home)
IMPL_NARROW_FROM_SCOPE (be_home)
