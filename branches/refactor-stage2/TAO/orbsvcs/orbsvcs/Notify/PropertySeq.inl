// $Id$

ACE_INLINE int
TAO_NS_PropertySeq::find (const ACE_CString& name, CosNotification::PropertyValue& value) const
{
  return this->property_map_.find (name, value);
}
