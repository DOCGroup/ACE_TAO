// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Storable.i
//
// = AUTHOR
//    Bruce Trask <trask_b@ociweb.com>
//    Chanaka Liyanaarachchi <chanaka@ociweb.com>
//
// ============================================================================

ACE_INLINE
TAO_Storable_Base::TAO_Storable_Base()
{
}

ACE_INLINE
TAO_Storable_Base::TAO_Storable_Base (TAO_Reader_Base &)
{
}

ACE_INLINE
TAO_Storable_Base::~TAO_Storable_Base()
{
}

ACE_INLINE
TAO_Writer_Base::TAO_Writer_Base()
  : delete_bindings_(0)
{
}

ACE_INLINE
TAO_Writer_Base::~TAO_Writer_Base()
{
}

ACE_INLINE
TAO_Reader_Base::TAO_Reader_Base()
{
}

ACE_INLINE
TAO_Reader_Base::~TAO_Reader_Base()
{
}



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
TAO_NS_Persistence_Header::context_name (const ACE_CString& context_name)
{
  this->context_name_ = context_name;
}

ACE_INLINE ACE_CString
TAO_NS_Persistence_Header::context_name () const
{
  return this->context_name_;
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
TAO_NS_Persistence_Record::ior () const
{
  return this->ior_;
}

ACE_INLINE void
TAO_NS_Persistence_Record::ior (const ACE_CString& ior)
{
  this->ior_ = ior;
}


ACE_INLINE ACE_CString
TAO_NS_Persistence_Record::context_binding () const
{
  return this->context_binding_;
}

ACE_INLINE void
TAO_NS_Persistence_Record::context_binding (const ACE_CString& context_binding)
{
  this->context_binding_ = context_binding;
}

ACE_INLINE
TAO_Naming_Service_Persistence_Factory::TAO_Naming_Service_Persistence_Factory()
{
}

ACE_INLINE
TAO_Naming_Service_Persistence_Factory::~TAO_Naming_Service_Persistence_Factory()
{
}
