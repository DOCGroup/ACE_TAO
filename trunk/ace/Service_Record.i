/* -*- C++ -*- */
// $Id$

// Service_Record.i

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

ACE_INLINE const char *
ACE_Service_Record::name (void) const
{
  ACE_TRACE ("ACE_Service_Record::name");
  return this->name_;
}

ACE_INLINE const ACE_Service_Type *
ACE_Service_Record::type (void) const
{
  ACE_TRACE ("ACE_Service_Record::type");
  return this->type_;
}

ACE_INLINE const void *
ACE_Service_Record::handle (void) const
{
  ACE_TRACE ("ACE_Service_Record::handle");
  return this->handle_;
}

ACE_INLINE void 
ACE_Service_Record::name (const char *n)
{
  ACE_TRACE ("ACE_Service_Record::name");
  this->name_ = ACE_OS::strcpy (new char [::strlen (n) + 1], n);
}

ACE_INLINE void 
ACE_Service_Record::type (const ACE_Service_Type *o, int enabled)
{
  ACE_TRACE ("ACE_Service_Record::type");
  this->type_ = o;
  ((ACE_Service_Record *) this)->active_ = enabled;
}

ACE_INLINE void 
ACE_Service_Record::handle (const void *h)
{
  ACE_TRACE ("ACE_Service_Record::handle");
  this->handle_ = h;
}

ACE_INLINE int  
ACE_Service_Record::active (void) const
{
  ACE_TRACE ("ACE_Service_Record::active");
  return this->active_ != 0;
}

ACE_INLINE void
ACE_Service_Record::active (int turnon)
{
  ACE_TRACE ("ACE_Service_Record::active");
  this->active_ = turnon;
}
