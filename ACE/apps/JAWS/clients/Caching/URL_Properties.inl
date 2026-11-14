// -*- C++ -*-
ACE_INLINE size_t
ACE_WString_Helper::size (const ACE_WString &wstr)
{
  return (wstr.length () + 1) * sizeof (ACE_UINT16);
}

ACE_INLINE
ACE_URL_Property::ACE_URL_Property (char const *name, char const *value)
  : name_ (name),
    value_ (value)
{
}

ACE_INLINE bool
ACE_URL_Property::operator== (ACE_URL_Property const &rhs) const
{
  return this->name_ == rhs.name_ && this->value_ == rhs.value_;
}

ACE_INLINE bool
ACE_URL_Property::operator!= (ACE_URL_Property const &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE ACE_WString &
ACE_URL_Property::name_rep ()
{
  return this->name_;
}

ACE_INLINE ACE_WString const &
ACE_URL_Property::name () const
{
  return this->name_;
}

ACE_INLINE void
ACE_URL_Property::name (char const *n)
{
  this->name_ = ACE_NS_WString (n);
}

ACE_INLINE ACE_WString &
ACE_URL_Property::value_rep ()
{
  return this->value_;
}

ACE_INLINE ACE_WString const &
ACE_URL_Property::value () const
{
  return this->value_;
}

ACE_INLINE void
ACE_URL_Property::value (char const *v)
{
  this->value_ = ACE_NS_WString (v);
}
ACE_INLINE size_t
ACE_URL_Property::size () const
{
  size_t len = 2;
    len += this->name_.length () + this->value_.length ();
  return len * sizeof (ACE_UINT16);
}

ACE_INLINE
ACE_URL_Offer::ACE_URL_Offer (size_t size, char const *url)
  : url_ (url),
    prop_ (size)
{
}

ACE_INLINE bool
ACE_URL_Offer::operator== (ACE_URL_Offer const &rhs) const
{
  return this->url_ == rhs.url_ && this->prop_ == rhs.prop_;
}

ACE_INLINE bool
ACE_URL_Offer::operator!= (ACE_URL_Offer const &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE ACE_WString &
ACE_URL_Offer::url_rep ()
{
  return this->url_;
}

ACE_INLINE ACE_WString const &
ACE_URL_Offer::url () const
{
  return this->url_;
}

ACE_INLINE void
ACE_URL_Offer::url (char const *url)
{
  this->url_ = ACE_NS_WString (url);
}

ACE_INLINE ACE_URL_Property_Seq &
ACE_URL_Offer::url_properties ()
{
  return this->prop_;
}

ACE_INLINE void
ACE_URL_Offer::url_properties (ACE_URL_Property_Seq const &prop)
{
  this->prop_ = prop;
}
