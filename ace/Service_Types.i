/* -*- C++ -*- */
// $Id$

// Service_Record.i

ACE_INLINE const void *
ACE_Service_Type_Impl::object (void) const
{
  ACE_TRACE ("ACE_Service_Type_Impl::object");
  return this->obj_;
}

ACE_INLINE const char *
ACE_Service_Type_Impl::name (void) const
{
  ACE_TRACE ("ACE_Service_Type_Impl::name");
  return this->name_;
}

ACE_INLINE void
ACE_Service_Type_Impl::name (const char *n)
{
  ACE_TRACE ("ACE_Service_Type_Impl::name");

  delete [] (char *) this->name_;
  ACE_NEW (this->name_, char[::strlen (n) + 1]);
  ACE_OS::strcpy ((char *) this->name_, n);
}

ACE_INLINE int
ACE_Service_Object_Type::suspend (void) const
{
  ACE_TRACE ("ACE_Service_Object_Type::suspend");
  return ((ACE_Service_Object *) this->object ())->suspend ();  
}

ACE_INLINE int
ACE_Service_Object_Type::resume (void) const
{
  ACE_TRACE ("ACE_Service_Object_Type::resume");
  return ((ACE_Service_Object *) this->object ())->resume ();
}

ACE_INLINE int
ACE_Service_Object_Type::info (char **str, size_t len) const
{
  ACE_TRACE ("ACE_Service_Object_Type::info");
  return ((ACE_Service_Object *) this->object ())->info (str, len);  
}

