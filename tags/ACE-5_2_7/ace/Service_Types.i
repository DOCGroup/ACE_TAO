/* -*- C++ -*- */
// $Id$

ACE_INLINE void *
ACE_Service_Type_Impl::object (void) const
{
  ACE_TRACE ("ACE_Service_Type_Impl::object");
  return this->obj_;
}

ACE_INLINE const ACE_TCHAR *
ACE_Service_Type_Impl::name (void) const
{
  ACE_TRACE ("ACE_Service_Type_Impl::name");
  return this->name_;
}

ACE_INLINE void
ACE_Service_Type_Impl::name (const ACE_TCHAR *n)
{
  ACE_TRACE ("ACE_Service_Type_Impl::name");

  delete [] (ACE_TCHAR *) this->name_;
  this->name_ = ACE::strnew (n);
}

ACE_INLINE
ACE_Service_Object_Type::~ACE_Service_Object_Type (void)
{
  ACE_TRACE ("ACE_Service_Object_Type::~ACE_Service_Object_Type");
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
ACE_Service_Object_Type::info (ACE_TCHAR **str, size_t len) const
{
  ACE_TRACE ("ACE_Service_Object_Type::info");
  return ((ACE_Service_Object *) this->object ())->info (str, len);
}

ACE_INLINE
ACE_Module_Type::~ACE_Module_Type (void)
{
  ACE_TRACE ("ACE_Module_Type::~ACE_Module_Type");
}

ACE_INLINE
ACE_Stream_Type::~ACE_Stream_Type (void)
{
  ACE_TRACE ("ACE_Stream_Type::~ACE_Stream_Type");
}
