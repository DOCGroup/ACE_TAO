// $Id$

// ============================================================================
//
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    be_global.cpp
//
// = DESCRIPTION
//    Stores global data specific to the compiler back end.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "be_global.h"

ACE_RCSID (IFR_Service,
           be_global,
           "$Id$")

TAO_IFR_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : removing_ (0),
    holding_scope_name_ (CORBA::string_dup ("TAO_IFR_holding_scope_module")),
    enable_locking_ (0),
    do_included_files_ (1)
{
}

BE_GlobalData::~BE_GlobalData (void)
{
}

idl_bool
BE_GlobalData::removing (void) const
{
  return this->removing_;
}

void
BE_GlobalData::removing (idl_bool value)
{
  this->removing_ = value;
}

CORBA::ORB_ptr
BE_GlobalData::orb (void) const
{
  return this->orb_.in ();
}

void
BE_GlobalData::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = orb;
}

CORBA::Repository_ptr
BE_GlobalData::repository (void) const
{
  return this->repository_.in ();
}

void
BE_GlobalData::repository (CORBA::Repository_ptr repo)
{
  this->repository_ = repo;
}

CORBA::ModuleDef_ptr
BE_GlobalData::holding_scope (void) const
{
  return this->holding_scope_.in ();
}

void
BE_GlobalData::holding_scope (CORBA::ModuleDef_ptr scope)
{
  this->holding_scope_ = scope;
}

const char *
BE_GlobalData::holding_scope_name (void) const
{
  return this->holding_scope_name_.in ();
}

ACE_Unbounded_Stack<CORBA::Container_ptr> &
BE_GlobalData::ifr_scopes (void)
{
  return this->ifr_scopes_;
}

const char *
BE_GlobalData::filename (void) const
{
  return this->filename_;
}

void
BE_GlobalData::filename (char *fname)
{
  this->filename_ = fname;
}

idl_bool
BE_GlobalData::enable_locking (void) const
{
  return this->enable_locking_;
}

void
BE_GlobalData::enable_locking (idl_bool value)
{
  this->enable_locking_ = value;
}

idl_bool
BE_GlobalData::do_included_files (void) const
{
  return this->do_included_files_;
}

void
BE_GlobalData::do_included_files (idl_bool val)
{
  this->do_included_files_ = val;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Stack<CORBA::Container_ptr>;
template class ACE_Node<CORBA::Container_ptr>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Stack<CORBA::Container_ptr>
#pragma instantiate ACE_Node<CORBA::Container_ptr>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
