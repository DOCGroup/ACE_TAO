// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Entries.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "Entries.h"

ACE_RCSID(Naming, Entries, "$Id$")

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

void
TAO_ExtId::operator= (const TAO_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

int
TAO_ExtId::operator== (const TAO_ExtId &rhs) const
{
  return id_ == rhs.id_ && kind_ == rhs.kind_;
}

int
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
