
//=============================================================================
/**
 *  @file    be_home.cpp
 *
 *  Extension of class AST_Home that provides additional means for C++
 *  mapping of a component home.
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
  if (managed_component == nullptr)
    {
      idl_global->set_err_count (idl_global->err_count () + 1);
      return;
    }

  be_component *bt =
     dynamic_cast<be_component*> (managed_component);

  bt->seen_in_operation (true);

  idl_global->object_arg_seen_ = true;
}

be_home::~be_home ()
{
}

void
be_home::scan (UTL_Scope *s)
{
  if (s == nullptr)
    {
      return;
    }

  for (UTL_ScopeActiveIterator i (s, UTL_Scope::IK_both);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Attribute *attr =
        dynamic_cast<AST_Attribute*> (d);

      if (attr != nullptr && ! attr->readonly ())
        {
          this->has_rw_attributes_ = true;
          return;
        }
    }

  AST_Home *h = dynamic_cast<AST_Home*> (s);

  if (h != nullptr)
    {
      this->scan (h->base_home ());
    }
}

void
be_home::destroy ()
{
  delete [] this->full_skel_name_;
  this->full_skel_name_ = nullptr;

  delete [] this->full_coll_name_;
  this->full_coll_name_ = nullptr;

  delete [] this->local_coll_name_;
  this->local_coll_name_ = nullptr;

  delete [] this->relative_skel_name_;
  this->relative_skel_name_ = nullptr;

  delete [] this->direct_proxy_impl_name_;
  this->direct_proxy_impl_name_ = nullptr;

  delete [] this->full_direct_proxy_impl_name_;
  this->full_direct_proxy_impl_name_ = nullptr;

  delete [] this->client_scope_;
  this->client_scope_ = nullptr;

  delete [] this->flat_client_scope_;
  this->flat_client_scope_ = nullptr;

  delete [] this->server_scope_;
  this->server_scope_ = nullptr;

  delete [] this->flat_server_scope_;
  this->flat_server_scope_ = nullptr;

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
