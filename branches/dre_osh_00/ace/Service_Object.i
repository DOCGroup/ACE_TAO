/* -*- C++ -*- */
// $Id$

// Service_Object.i

ACE_INLINE ACE_Service_Object_Ptr::ACE_Service_Object_Ptr (ACE_Service_Object *so)
  : service_object_ (so)
{
}

ACE_INLINE ACE_Service_Object_Ptr::~ACE_Service_Object_Ptr (void)
{
  this->service_object_->fini ();
  delete this->service_object_;
}

ACE_INLINE ACE_Service_Object *
ACE_Service_Object_Ptr::operator-> ()
{
  return this->service_object_;
}

ACE_INLINE const ACE_TCHAR *
ACE_Service_Type::name (void) const
{
  ACE_TRACE ("ACE_Service_Type::name");
  return this->name_;
}

ACE_INLINE const ACE_Service_Type_Impl *
ACE_Service_Type::type (void) const
{
  ACE_TRACE ("ACE_Service_Type::type");
  return this->type_;
}

ACE_INLINE void
ACE_Service_Type::type (const ACE_Service_Type_Impl *o, int enabled)
{
  ACE_TRACE ("ACE_Service_Type::type");
  this->type_ = o;
  ((ACE_Service_Type *) this)->active_ = enabled;
}

ACE_INLINE int
ACE_Service_Type::active (void) const
{
  ACE_TRACE ("ACE_Service_Type::active");
  return this->active_ != 0;
}

ACE_INLINE void
ACE_Service_Type::active (int turnon)
{
  ACE_TRACE ("ACE_Service_Type::active");
  this->active_ = turnon;
}

ACE_INLINE int
ACE_Service_Type::fini_called (void) const
{
  ACE_TRACE ("ACE_Service_Type::fini_called");
  return this->fini_already_called_;
}
