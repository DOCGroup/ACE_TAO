// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable.inl
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 */
//=============================================================================


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_NS_Persistence_Header::size (unsigned int size)
{
  this->size_ = size;
}

ACE_INLINE unsigned int
TAO_NS_Persistence_Header::size () const
{
  return this->size_;
}

ACE_INLINE void
TAO_NS_Persistence_Header::destroyed (int flag)
{
  this->destroyed_ = flag;
}

ACE_INLINE int
TAO_NS_Persistence_Header::destroyed () const
{
  return this->destroyed_;
}

ACE_INLINE
TAO_NS_Persistence_Record::TAO_NS_Persistence_Record (
   TAO_NS_Persistence_Record::Record_Type type)
  : type_ (type)
{
}

ACE_INLINE TAO_NS_Persistence_Record::Record_Type
TAO_NS_Persistence_Record::type () const
{
  return this->type_;
}

ACE_INLINE void
TAO_NS_Persistence_Record::type (Record_Type type)
{
  this->type_ = type;
}

ACE_INLINE ACE_CString
TAO_NS_Persistence_Record::id () const
{
  return this->id_;
}

ACE_INLINE void
TAO_NS_Persistence_Record::id (const ACE_CString& id)
{
  this->id_ = id;
}

ACE_INLINE ACE_CString
TAO_NS_Persistence_Record::kind () const
{
  return this->kind_;
}

ACE_INLINE void
TAO_NS_Persistence_Record::kind (const ACE_CString& kind)
{
  this->kind_ = kind;
}

ACE_INLINE ACE_CString
TAO_NS_Persistence_Record::ref () const
{
  return this->ref_;
}

ACE_INLINE void
TAO_NS_Persistence_Record::ref (const ACE_CString& ref)
{
  this->ref_ = ref;
}

ACE_INLINE void
TAO_NS_Persistence_Global::counter (unsigned int counter)
{
  this->counter_ = counter;
}

ACE_INLINE unsigned int
TAO_NS_Persistence_Global::counter () const
{
  return this->counter_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
