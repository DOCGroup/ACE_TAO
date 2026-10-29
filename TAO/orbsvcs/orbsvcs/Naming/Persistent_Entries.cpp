//=============================================================================
/**
 *  @file    Persistent_Entries.cpp
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#include "orbsvcs/Naming/Persistent_Entries.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Persistent_IntId::TAO_Persistent_IntId ()
  : ref_ (nullptr),
    type_ (CosNaming::nobject)
{
}

TAO_Persistent_IntId::TAO_Persistent_IntId (char * obj_ref,
                                            CosNaming::BindingType type)
  : ref_ (obj_ref),
    type_ (type)
{
}

TAO_Persistent_IntId::TAO_Persistent_IntId (TAO_Persistent_IntId const &rhs)
{
  ref_ = rhs.ref_;
  type_ = rhs.type_;
}

TAO_Persistent_IntId::~TAO_Persistent_IntId ()
{
}

void
TAO_Persistent_IntId::operator= (TAO_Persistent_IntId const &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;
  ref_ = rhs.ref_;
}

TAO_Persistent_ExtId::TAO_Persistent_ExtId ()
  : id_ (nullptr),
    kind_ (nullptr)
{
}

TAO_Persistent_ExtId::TAO_Persistent_ExtId (char const *id,
                                            char const *kind)
  : id_ (id),
    kind_ (kind)

{
}

TAO_Persistent_ExtId::TAO_Persistent_ExtId (TAO_Persistent_ExtId const &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

TAO_Persistent_ExtId::~TAO_Persistent_ExtId ()
{
}

void
TAO_Persistent_ExtId::operator= (TAO_Persistent_ExtId const &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

bool
TAO_Persistent_ExtId::operator== (TAO_Persistent_ExtId const &rhs) const
{
  return (ACE_OS::strcmp (id_, rhs.id_) == 0)
    && (ACE_OS::strcmp (kind_, rhs.kind_) == 0);
}

bool
TAO_Persistent_ExtId::operator!= (TAO_Persistent_ExtId const &rhs) const
{
  return (ACE_OS::strcmp (id_, rhs.id_) != 0)
    || (ACE_OS::strcmp (kind_, rhs.kind_) != 0);
}

u_long
TAO_Persistent_ExtId::hash () const
{
  ACE_CString temp (id_);
  temp += kind_;

  return temp.hash ();
}

char const *
TAO_Persistent_ExtId::id ()
{
  return id_;
}

char const *
TAO_Persistent_ExtId::kind ()
{
  return kind_;
}

TAO_Persistent_Index_IntId::TAO_Persistent_Index_IntId ()
  : counter_ (nullptr),
    hash_map_ (nullptr)
{
}

TAO_Persistent_Index_IntId::TAO_Persistent_Index_IntId (ACE_UINT32 *counter,
                                                        ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
                                                        TAO_Persistent_IntId> * hash_map)
  : counter_ (counter),
    hash_map_ (hash_map)
{
}

TAO_Persistent_Index_IntId::TAO_Persistent_Index_IntId (TAO_Persistent_Index_IntId const &rhs)
{
  counter_ = rhs.counter_;
  hash_map_ = rhs.hash_map_;
}

TAO_Persistent_Index_IntId::~TAO_Persistent_Index_IntId ()
{
}

void
TAO_Persistent_Index_IntId::operator= (TAO_Persistent_Index_IntId const &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  counter_ = rhs.counter_;
  hash_map_ = rhs.hash_map_;
}

TAO_Persistent_Index_ExtId::TAO_Persistent_Index_ExtId ()
  : poa_id_ (nullptr)
{
}

TAO_Persistent_Index_ExtId::TAO_Persistent_Index_ExtId (char const *poa_id)
  : poa_id_ (poa_id)
{
}

TAO_Persistent_Index_ExtId::TAO_Persistent_Index_ExtId (TAO_Persistent_Index_ExtId const &rhs)
{
  poa_id_ = rhs.poa_id_;
}

TAO_Persistent_Index_ExtId::~TAO_Persistent_Index_ExtId ()
{
}

void
TAO_Persistent_Index_ExtId::operator= (TAO_Persistent_Index_ExtId const &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  poa_id_ = rhs.poa_id_;
}

bool
TAO_Persistent_Index_ExtId::operator== (TAO_Persistent_Index_ExtId const &rhs) const
{
  return (ACE_OS::strcmp (poa_id_, rhs.poa_id_) == 0);
}

bool
TAO_Persistent_Index_ExtId::operator!= (TAO_Persistent_Index_ExtId const &rhs) const
{
  return (ACE_OS::strcmp (poa_id_, rhs.poa_id_) != 0);
}

u_long
TAO_Persistent_Index_ExtId::hash () const
{
  ACE_CString temp (poa_id_);
  return temp.hash ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
