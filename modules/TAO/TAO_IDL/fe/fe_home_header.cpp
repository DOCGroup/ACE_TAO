// $Id$

#include "fe_home_header.h"
#include "ast_home.h"
#include "ast_valuetype.h"
#include "utl_err.h"
#include "fe_extern.h"
#include "global_extern.h"

ACE_RCSID (fe,
           fe_home_header,
           "$Id$")

FE_HomeHeader::FE_HomeHeader (UTL_ScopedName *n,
                              UTL_ScopedName *base_home,
                              UTL_NameList *supports,
                              UTL_ScopedName *managed_component,
                              UTL_ScopedName *primary_key)
  : FE_ComponentHeader (n,
                        0,
                        supports,
                        false),
    base_home_ (0),
    primary_key_ (0)
{
  // No need to call compile_supports(), it got done in
  // the call to the base class FE_ComponentHeader.
  this->compile_inheritance (base_home);
  this->compile_managed_component (managed_component);
  this->compile_primary_key (primary_key);
}

FE_HomeHeader::~FE_HomeHeader (void)
{
}

AST_Home *
FE_HomeHeader::base_home (void) const
{
  return this->base_home_;
}

AST_Component *
FE_HomeHeader::managed_component (void) const
{
  return this->managed_component_;
}

AST_ValueType *
FE_HomeHeader::primary_key (void) const
{
  return this->primary_key_;
}

void
FE_HomeHeader::compile_inheritance (UTL_ScopedName *base_home)
{
  if (base_home == 0)
    {
      return;
    }

  UTL_Scope *s = idl_global->scopes ().top_non_null ();
  AST_Decl *d = s->lookup_by_name (base_home,
                                   true);

  if (d == 0)
    {
      idl_global->err ()->lookup_error (base_home);

      // This is probably the result of bad IDL.
      // We will crash if we continue from here.
      throw Bailout ();
    }

  if (d->node_type () == AST_Decl::NT_typedef)
    {
      d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
    }

  this->base_home_ = AST_Home::narrow_from_decl (d);

  if (this->base_home_ == 0)
    {
      idl_global->err ()->inheritance_error (this->name (),
                                             d);
    }
}

void
FE_HomeHeader::compile_managed_component (UTL_ScopedName *managed_component)
{
  if (managed_component == 0)
    {
      return;
    }

  UTL_Scope *s = idl_global->scopes ().top_non_null ();
  AST_Decl *d = s->lookup_by_name (managed_component,
                                   true);

  if (d == 0)
    {
      idl_global->err ()->lookup_error (managed_component);

      // This is probably the result of bad IDL.
      // We will crash if we continue from here.
      throw Bailout ();
    }

  if (d->node_type () == AST_Decl::NT_typedef)
    {
      d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
    }

  this->managed_component_ = AST_Component::narrow_from_decl (d);

  if (this->managed_component_ == 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_USE,
                                  d);
    }
}

void
FE_HomeHeader::compile_primary_key (UTL_ScopedName *primary_key)
{
  if (primary_key == 0)
    {
      return;
    }

  UTL_Scope *s = idl_global->scopes ().top_non_null ();
  AST_Decl *d = s->lookup_by_name (primary_key,
                                   true);

  if (d == 0)
    {
      idl_global->err ()->lookup_error (primary_key);

      // This is probably the result of bad IDL.
      // We will crash if we continue from here.
      throw Bailout ();
    }

  AST_Decl::NodeType nt = d->node_type ();

  if (nt == AST_Decl::NT_typedef)
    {
      d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
    }

  this->primary_key_ = AST_ValueType::narrow_from_decl (d);

  if (this->primary_key_ == 0 || nt != AST_Decl::NT_valuetype)
    {
      idl_global->err ()->valuetype_expected (d);
    }
}

