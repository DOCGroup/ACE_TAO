// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_Notify_PropertySeq::find (const char* name,
                              CosNotification::PropertyValue& value) const
{
  ACE_CString str_name (name);
  return this->property_map_.find (str_name, value);
}

ACE_INLINE
size_t
TAO_Notify_PropertySeq::size() const
{
  return this->property_map_.current_size();
}

TAO_END_VERSIONED_NAMESPACE_DECL
