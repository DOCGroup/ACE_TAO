// $Id$

ACE_INLINE int
TAO_Notify_PropertySeq::find (const char* name, CosNotification::PropertyValue& value) const
{
  ACE_CString str_name (name);
  return this->property_map_.find (str_name, value);
}
