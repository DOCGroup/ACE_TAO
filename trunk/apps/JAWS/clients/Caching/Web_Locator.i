/* -*- C++ -*- */

// $Id$

ACE_INLINE
ACE_URL_Property::ACE_URL_Property (LPCTSTR name, LPCTSTR value)
  : name_ (0),
    value_ (0)
{
  this->name (name);
  this->value (value);
}

ACE_INLINE
ACE_URL_Property::ACE_URL_Property (ACE_URL_Property &p)
  : name_ (0),
    value_ (0)
{
  this->name (p.name ());
  this->value (p.value ());
}
  
ACE_INLINE
ACE_URL_Property::~ACE_URL_Property (void)
{
  delete[] this->name_;
  delete[] this->value_;
}

ACE_INLINE ACE_URL_Property &
ACE_URL_Property::operator= (ACE_URL_Property &rhs)
{
  if (this != &rhs)
    {
      this->name (rhs.name ());
      this->value (rhs.value ());
    }
  return *this;
}

ACE_INLINE int
ACE_URL_Property::operator== (ACE_URL_Property &rhs) const
{
  if (this == &rhs)
    {
      if (ACE_OS::strcmp (this->name (), rhs.name ()) ||
	  ACE_OS::strcmp (this->value (), rhs.value ()))
	return 0;
    }
  return 1;
}

ACE_INLINE int
ACE_URL_Property::operator!= (ACE_URL_Property &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE LPCTSTR
ACE_URL_Property::name (void) const
{
  return this->name_;
}

ACE_INLINE void
ACE_URL_Property::name (LPCTSTR name)
{
  delete [] this->name_;

  if (name)
    {
      ACE_NEW (this->name_, TCHAR[ACE_OS::strlen (name) + 1]);
      ACE_OS::strcpy (this->name_, name);
    }
  else
    this->name_ = 0;
}

ACE_INLINE LPCTSTR
ACE_URL_Property::value (void) const
{
  return this->value_;
}

ACE_INLINE void
ACE_URL_Property::value (LPCTSTR value)
{
  delete [] this->value_;

  if (value)
    {
      ACE_NEW (this->value_, TCHAR[ACE_OS::strlen (value) + 1]);
      ACE_OS::strcpy (this->value_, value);
    }
  else
    this->value_ = 0;
}

ACE_INLINE
ACE_URL_Offer::ACE_URL_Offer (LPCTSTR url, ACE_URL_Property_Seq &prop_seq)
  : url_ (0),
    prop_ (prop_seq)
{
  this->url (url);
}

ACE_INLINE
ACE_URL_Offer::~ACE_URL_Offer (void)
{
  delete [] this->url_;
}

ACE_INLINE ACE_URL_Offer &
ACE_URL_Offer::operator= (ACE_URL_Offer &rhs)
{
  if (this != &rhs)
    {
      this->url (rhs.url ());
      this->url_properties (rhs.url_properties ());
    }
  return *this;
}

ACE_INLINE int
ACE_URL_Offer::operator== (ACE_URL_Offer &rhs) const
{
  // @@ Nanbor, I reformatted this expression.  Please make sure it is
  // still correct!

  if (this != &rhs
      && (ACE_OS::strcmp (this->url (), rhs.url ()) 
      || this->prop_ != rhs.url_properties ()))
      return 0;
  else
    return 1;
}

ACE_INLINE int
ACE_URL_Offer::operator!= (ACE_URL_Offer &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE LPCTSTR
ACE_URL_Offer::url (void) const
{
  return this->url_;
}

ACE_INLINE void
ACE_URL_Offer::url (LPCTSTR url)
{
  delete [] this->url_;

  if (url != 0)
    {
      ACE_NEW (this->url_, TCHAR[ACE_OS::strlen (url) + 1]);
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
ACE_URL_Offer::url_properties (ACE_URL_Property_Seq &prop)
{
  if (&this->prop_ != &prop)
    this->prop_ = prop;
}

ACE_INLINE void
ACE_URL_Locator::set_timeout (const ACE_Time_Value &timeout)
{
  this->timeout_ = timeout;
}

ACE_INLINE ACE_Time_Value 
ACE_URL_Locator::get_timeout (void) const
{
  return this->timeout_;
}

ACE_INLINE void
ACE_URL_Locator::set_handle (ACE_HANDLE handle)
{
  this->handle_ = handle ;
}

ACE_INLINE ACE_HANDLE
ACE_URL_Locator::get_handle (void) const
{
  return this->handle_;
}

ACE_INLINE
ACE_URL_Locator_Query::ACE_URL_Locator_Query (ACE_HANDLE handle,
					      const ACE_Time_Value &timeout)
  : ACE_URL_Locator (handle, timeout)
{
}

ACE_INLINE
ACE_URL_Locator_Query::~ACE_URL_Locator_Query (void)
{
}

ACE_INLINE
ACE_URL_Locator_Register::ACE_URL_Locator_Register (ACE_HANDLE handle,
						    const ACE_Time_Value &timeout)
  : ACE_URL_Locator (handle, timeout)
{
}

ACE_INLINE
ACE_URL_Locator_Register::~ACE_URL_Locator_Register (void)
{
}
