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


ACE_INLINE
TAO_Naming_Service_Persistence_Factory::TAO_Naming_Service_Persistence_Factory()
{
}

ACE_INLINE
TAO_Naming_Service_Persistence_Factory::~TAO_Naming_Service_Persistence_Factory()
{
}

ACE_INLINE
TAO_Storable_Base::TAO_Storable_Base()
 : state_ (goodbit)
{
}

ACE_INLINE
TAO_Storable_Base::~TAO_Storable_Base()
{
}

ACE_INLINE void
TAO_Storable_Base::clear (TAO_Storable_Base::Storable_State state)
{
  this->state_ = state;
}

ACE_INLINE void
TAO_Storable_Base::setstate (TAO_Storable_Base::Storable_State state)
{
  this->clear (static_cast <TAO_Storable_Base::Storable_State> (
               this->rdstate () | state));
}

ACE_INLINE TAO_Storable_Base::Storable_State
TAO_Storable_Base::rdstate (void) const
{
  return this->state_;
}

ACE_INLINE bool
TAO_Storable_Base::good (void) const
{
  return (this->state_ == goodbit);
}

ACE_INLINE bool
TAO_Storable_Base::bad (void) const
{
  return (this->state_ & badbit);
}

ACE_INLINE bool
TAO_Storable_Base::eof (void) const
{
  return (this->state_ & eofbit);
}

ACE_INLINE bool
TAO_Storable_Base::fail (void) const
{
  return (this->state_ & failbit);
}

TAO_END_VERSIONED_NAMESPACE_DECL
