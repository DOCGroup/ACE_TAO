//=============================================================================
/**
 *  @file    Entries.cpp
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#include "orbsvcs/Naming/Entries.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IntId::TAO_IntId ()
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

TAO_IntId::TAO_IntId (TAO_IntId const &rhs)
{
  type_ = rhs.type_;
  ref_ = CORBA::Object::_duplicate (rhs.ref_);
}

TAO_IntId::~TAO_IntId ()
{
  CORBA::release (ref_);
}

void
TAO_IntId::operator= (TAO_IntId const &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;

  CORBA::release (ref_);
  ref_ = CORBA::Object::_duplicate (rhs.ref_);
}

TAO_ExtId::TAO_ExtId ()
  : kind_ (),
    id_ ()
{
}

TAO_ExtId::TAO_ExtId (char const *id,
                    char const *kind)
  : kind_ (kind),
    id_ (id)
{
}

TAO_ExtId::TAO_ExtId (TAO_ExtId const &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

TAO_ExtId::~TAO_ExtId ()
{
}

char const *
TAO_ExtId::id ()
{
  return id_.fast_rep ();
}

char const *
TAO_ExtId::kind ()
{
  return kind_.fast_rep ();
}

void
TAO_ExtId::operator= (TAO_ExtId const &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

bool
TAO_ExtId::operator== (TAO_ExtId const &rhs) const
{
  return id_ == rhs.id_ && kind_ == rhs.kind_;
}

bool
TAO_ExtId::operator!= (TAO_ExtId const &rhs) const
{
  return id_ != rhs.id_ || kind_ != rhs.kind_;
}

u_long
TAO_ExtId::hash () const
{
  ACE_CString temp (id_);
  temp += kind_;

  return temp.hash ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
