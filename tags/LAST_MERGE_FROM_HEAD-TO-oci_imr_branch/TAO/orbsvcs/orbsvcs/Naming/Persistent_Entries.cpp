// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Persistent_Entries.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "Persistent_Entries.h"
#include "ace/SString.h"

TAO_Persistent_IntId::TAO_Persistent_IntId (void)
  : ref_ (0),
    type_ (CosNaming::nobject)
{
}

TAO_Persistent_IntId::TAO_Persistent_IntId (char * obj_ref,
                                            CosNaming::BindingType type)
  : ref_ (obj_ref),
    type_ (type)
{
}

TAO_Persistent_IntId::TAO_Persistent_IntId (const TAO_Persistent_IntId &rhs)
{
  ref_ = rhs.ref_;
  type_ = rhs.type_;
}

TAO_Persistent_IntId::~TAO_Persistent_IntId (void)
{
}

void
TAO_Persistent_IntId::operator= (const TAO_Persistent_IntId &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;
  ref_ = rhs.ref_;
}

TAO_Persistent_ExtId::TAO_Persistent_ExtId (void)
  : id_ (0),
    kind_ (0)
{
}

TAO_Persistent_ExtId::TAO_Persistent_ExtId (const char *id,
                                            const char *kind)
  : id_ (id),
    kind_ (kind)

{
}

TAO_Persistent_ExtId::TAO_Persistent_ExtId (const TAO_Persistent_ExtId &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

TAO_Persistent_ExtId::~TAO_Persistent_ExtId (void)
{
}

void
TAO_Persistent_ExtId::operator= (const TAO_Persistent_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

int
TAO_Persistent_ExtId::operator== (const TAO_Persistent_ExtId &rhs) const
{
  return (ACE_OS::strcmp (id_, rhs.id_) == 0)
    && (ACE_OS::strcmp (kind_, rhs.kind_) == 0);
}

int
TAO_Persistent_ExtId::operator!= (const TAO_Persistent_ExtId &rhs) const
{
  return (ACE_OS::strcmp (id_, rhs.id_) != 0)
    || (ACE_OS::strcmp (kind_, rhs.kind_) != 0);
}

u_long
TAO_Persistent_ExtId::hash (void) const
{
  ACE_CString temp (id_);
  temp += kind_;

  return temp.hash ();
}

const char *
TAO_Persistent_ExtId::id (void)
{
  return id_;
}

const char *
TAO_Persistent_ExtId::kind (void)
{
  return kind_;
}

TAO_Persistent_Index_IntId::TAO_Persistent_Index_IntId (void)
  : counter_ (0),
    hash_map_ (0)
{
}

TAO_Persistent_Index_IntId::TAO_Persistent_Index_IntId (ACE_UINT32 *counter,
                                                        ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
                                                        TAO_Persistent_IntId> * hash_map)
  : counter_ (counter),
    hash_map_ (hash_map)
{
}

TAO_Persistent_Index_IntId::TAO_Persistent_Index_IntId (const TAO_Persistent_Index_IntId &rhs)
{
  counter_ = rhs.counter_;
  hash_map_ = rhs.hash_map_;
}

TAO_Persistent_Index_IntId::~TAO_Persistent_Index_IntId (void)
{
}

void
TAO_Persistent_Index_IntId::operator= (const TAO_Persistent_Index_IntId &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  counter_ = rhs.counter_;
  hash_map_ = rhs.hash_map_;
}

TAO_Persistent_Index_ExtId::TAO_Persistent_Index_ExtId (void)
  : poa_id_ (0)
{
}

TAO_Persistent_Index_ExtId::TAO_Persistent_Index_ExtId (const char *poa_id)
  : poa_id_ (poa_id)
{
}

TAO_Persistent_Index_ExtId::TAO_Persistent_Index_ExtId (const TAO_Persistent_Index_ExtId &rhs)
{
  poa_id_ = rhs.poa_id_;
}

TAO_Persistent_Index_ExtId::~TAO_Persistent_Index_ExtId (void)
{
}

void
TAO_Persistent_Index_ExtId::operator= (const TAO_Persistent_Index_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  poa_id_ = rhs.poa_id_;
}

int
TAO_Persistent_Index_ExtId::operator== (const TAO_Persistent_Index_ExtId &rhs) const
{
  return (ACE_OS::strcmp (poa_id_, rhs.poa_id_) == 0);
}

int
TAO_Persistent_Index_ExtId::operator!= (const TAO_Persistent_Index_ExtId &rhs) const
{
  return (ACE_OS::strcmp (poa_id_, rhs.poa_id_) != 0);
}

u_long
TAO_Persistent_Index_ExtId::hash (void) const
{
  ACE_CString temp (poa_id_);
  return temp.hash ();
}
