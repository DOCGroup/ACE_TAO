/* -*- C++ -*- */
// $Id$

// Service_Object.i

ACE_INLINE const void *
ACE_Service_Type::object (void) const
{
  ACE_TRACE ("ACE_Service_Type::object");
  return this->obj_;
}

ACE_INLINE const char *
ACE_Service_Type::name (void) const
{
  ACE_TRACE ("ACE_Service_Type::name");
  return this->name_;
}

ACE_INLINE void
ACE_Service_Type::name (const char *n)
{
  ACE_TRACE ("ACE_Service_Type::name");

  delete [] (char *) this->name_;
  ACE_NEW (this->name_, char[::strlen (n) + 1]);
  ACE_OS::strcpy ((char *) this->name_, n);
}
