
//=============================================================================
/**
 *  @file    Entries.cpp
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Naming/Entries.h"




TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IntId::TAO_IntId (void)
  : type_ (CosNaming::nobject)
{
  ref_ = CORBA::Object::_nil ();
}

TAO_IntId::TAO_IntId (CORBA::Object_ptr obj,
                      CosNaming::BindingType type)
  : type_ (type)
{
  ref_ = CORBA::Object::_duplicate (obj);
}

TAO_IntId::TAO_IntId (const TAO_IntId &rhs)
{
  type_ = rhs.type_;
  ref_ = CORBA::Object::_duplicate (rhs.ref_);
}

TAO_IntId::~TAO_IntId (void)
{
  CORBA::release (ref_);
}

void
TAO_IntId::operator= (const TAO_IntId &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;

  CORBA::release (ref_);
  ref_ = CORBA::Object::_duplicate (rhs.ref_);
}

TAO_ExtId::TAO_ExtId (void)
  : kind_ (),
    id_ ()
{
}

TAO_ExtId::TAO_ExtId (const char *id,
                    const char *kind)
  : kind_ (kind),
    id_ (id)
{
}

TAO_ExtId::TAO_ExtId (const TAO_ExtId &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

TAO_ExtId::~TAO_ExtId (void)
{
}

const char *
TAO_ExtId::id (void)
{
  return id_.fast_rep ();
}

const char *
TAO_ExtId::kind (void)
{
  return kind_.fast_rep ();
}

void
TAO_ExtId::operator= (const TAO_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

bool
TAO_ExtId::operator== (const TAO_ExtId &rhs) const
{
  return id_ == rhs.id_ && kind_ == rhs.kind_;
}

bool
TAO_ExtId::operator!= (const TAO_ExtId &rhs) const
{
  return id_ != rhs.id_ || kind_ != rhs.kind_;
}

u_long
TAO_ExtId::hash (void) const
{
  ACE_CString temp (id_);
  temp += kind_;

  return temp.hash ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
