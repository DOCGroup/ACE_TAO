// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    NS_CosNaming.C
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "NS_CosNaming.h"

NS_IntId::NS_IntId (void)
  : type_ (CosNaming::nobject)
{
  ref_ = CORBA::Object::_nil ();
}

NS_IntId::NS_IntId (CORBA::Object_ptr obj,
		    CosNaming::BindingType type)
  : type_ (type)
{
  ref_ = CORBA::Object::_duplicate (obj);
}

NS_IntId::NS_IntId (const NS_IntId &rhs)
{
  type_ = rhs.type_;
  ref_ = CORBA::Object::_duplicate (rhs.ref_);
}

NS_IntId::~NS_IntId (void)
{
  CORBA::release (ref_);
}

void
NS_IntId::operator= (const NS_IntId &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;

  CORBA::release (ref_);
  ref_ = CORBA::Object::_duplicate (rhs.ref_);
}

NS_ExtId::NS_ExtId (void)
  : kind_ (),
    id_ ()
{
}

NS_ExtId::NS_ExtId (const char *id,
		    const char *kind)
  : kind_ (kind),
    id_ (id)
{
}

NS_ExtId::NS_ExtId (const NS_ExtId &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

NS_ExtId::~NS_ExtId (void)
{
}

void
NS_ExtId::operator= (const NS_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

int
NS_ExtId::operator== (const NS_ExtId &rhs) const
{
  return id_ == rhs.id_ && kind_ == rhs.kind_;
}

int
NS_ExtId::operator!= (const NS_ExtId &rhs) const
{
  return id_ != rhs.id_ || kind_ != rhs.kind_;
}

u_long
NS_ExtId::hash (void) const
{
  ACE_CString temp (id_);
  temp += kind_;

  return temp.hash ();
}
