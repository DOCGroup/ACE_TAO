/* -*- C++ -*- */

// $Id$

ACE_INLINE
ACE_URL_Property::ACE_URL_Property (const char *name, const char *value)
  : name_ (0),
    value_ (0)
{
  if (name != 0)
    ACE_NEW (this->name_, ACE_WString (name));
  if (value != 0)
    ACE_NEW (this->value_, ACE_WString (value));
}

ACE_INLINE
ACE_URL_Property::ACE_URL_Property (const ACE_USHORT16 *name,
				    const ACE_USHORT16 *value)
  : name_ (0),
    value_ (0)
{
  if (name != 0)
    ACE_NEW (this->name_, ACE_WString (name));
  if (value != 0)
    ACE_NEW (this->value_, ACE_WString (value));
}

ACE_INLINE
ACE_URL_Property::ACE_URL_Property (const ACE_URL_Property &p)
  : name_ (0),
    value_ (0)
{
  if (p.name_ != 0)
    ACE_NEW (this->name_, ACE_WString (*p.name_));
  if (p.value_ != 0)
    ACE_NEW (this->value_, ACE_WString (*p.value_)); 
}
  
ACE_INLINE
ACE_URL_Property::~ACE_URL_Property (void)
{
  delete this->name_;
  delete this->value_;
}

ACE_INLINE ACE_URL_Property &
ACE_URL_Property::operator= (const ACE_URL_Property &rhs)
{
  if (this != &rhs)
    {
      delete this->name_;
      delete this->value_;
      if (rhs.name_ != 0)
	ACE_NEW_RETURN (this->name_, ACE_WString(*rhs.name_), *this);
      if (rhs.value_ != 0)
	ACE_NEW_RETURN (this->value_, ACE_WString(*rhs.value_), *this);
    }
  return *this;
}

ACE_INLINE int
ACE_URL_Property::operator== (const ACE_URL_Property &rhs) const
{
  if (this == &rhs || *this->name_ != *rhs.name () ||
      *this->value_ != *rhs.value ())
    return 1;
  else
    return 0;
}

ACE_INLINE int
ACE_URL_Property::operator!= (const ACE_URL_Property &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE ACE_WString *
ACE_URL_Property::name_rep (void)
{
  return this->name_;
}

ACE_INLINE const ACE_WString *
ACE_URL_Property::name (void) const
{
  return this->name_;
}

ACE_INLINE void
ACE_URL_Property::name (const char *n)
{
  delete this->name_;
  if (n != 0)
    ACE_NEW (this->name_, ACE_WString (n));
}

ACE_INLINE void
ACE_URL_Property::name (const ACE_USHORT16 *n)
{
  delete this->name_;
  if (n != 0)
    ACE_NEW (this->name_, ACE_WString (n));
}  

ACE_INLINE ACE_WString *
ACE_URL_Property::value_rep (void)
{
  return this->value_;
}

ACE_INLINE const ACE_WString *
ACE_URL_Property::value (void) const
{
  return this->value_;
}

ACE_INLINE void
ACE_URL_Property::value (const char *v)
{
  delete this->value_;
  if (v != 0)
    ACE_NEW (this->value_, ACE_WString (v));
}

ACE_INLINE void
ACE_URL_Property::value (const ACE_USHORT16 *v)
{
  delete this->value_;
  if (v != 0)
    ACE_NEW (this->value_, ACE_WString (v));
}


ACE_INLINE size_t
ACE_URL_Property::bsize (void) const
{
  size_t len = 2;
  if (this->name_ != 0)
    len += this->name_->length ();
  if (this->value_ != 0)
    len += this->value_->length ();
  return len * sizeof (ACE_USHORT16);
}

ACE_INLINE
ACE_URL_Offer::ACE_URL_Offer (const size_t size, const char *url)
  : url_ (0),
    prop_ (size)
{
  this->url (url);
}

ACE_INLINE
ACE_URL_Offer::~ACE_URL_Offer (void)
{
  delete [] this->url_;
}

ACE_INLINE ACE_URL_Offer &
ACE_URL_Offer::operator= (const ACE_URL_Offer &rhs)
{
  if (this != &rhs)
    {
      this->url (rhs.url_);
      this->url_properties (rhs.prop_);
    }
  return *this;
}

ACE_INLINE int
ACE_URL_Offer::operator== (const ACE_URL_Offer &rhs) const
{
  if (this == &rhs
      && ACE_OS::strcmp (this->url (), rhs.url ()) == 0 
      && this->prop_ == rhs.prop_)
    return 1;
  else
    return 0;
}

ACE_INLINE int
ACE_URL_Offer::operator!= (const ACE_URL_Offer &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE const char *
ACE_URL_Offer::url (void) const
{
  return this->url_;
}

ACE_INLINE void
ACE_URL_Offer::url (const char *url)
{
  delete [] this->url_;

  if (url != 0)
    {
      ACE_NEW (this->url_, char[ACE_OS::strlen (url) + 1]);
      ACE_OS::strcpy (this->url_, url);
    }
  else
    this->url_ = 0;
}

ACE_INLINE ACE_URL_Property_Seq &
ACE_URL_Offer::url_properties (void)
{
  return this->prop_;
}
  
ACE_INLINE void
ACE_URL_Offer::url_properties (const ACE_URL_Property_Seq &prop)
{
  this->prop_ = prop;
}
