// -*- C++ -*-
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler::_duplicate (TAO_AMH_Locate_ResponseHandler_ptr x)
{
  if (x != 0)
    {
      x->_add_ref ();
    }

  return x;
}


// Inline operations for class TAO_AMH_Locate_ResponseHandler_var
ACE_INLINE
TAO_AMH_Locate_ResponseHandler_var::TAO_AMH_Locate_ResponseHandler_var (void)
  : ptr_ (TAO_AMH_Locate_ResponseHandler::_nil ())
{
}

ACE_INLINE
TAO_AMH_Locate_ResponseHandler_var::TAO_AMH_Locate_ResponseHandler_var (TAO_AMH_Locate_ResponseHandler_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
TAO_AMH_Locate_ResponseHandler_var::~TAO_AMH_Locate_ResponseHandler_var (void)
{
  CORBA::release(ptr_);
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
TAO_AMH_Locate_ResponseHandler_var::TAO_AMH_Locate_ResponseHandler_var (const TAO_AMH_Locate_ResponseHandler_var &p)
  : ptr_ (TAO_AMH_Locate_ResponseHandler::_duplicate (p.ptr ()))
{}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_var &
TAO_AMH_Locate_ResponseHandler_var::operator= (TAO_AMH_Locate_ResponseHandler_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_var &
TAO_AMH_Locate_ResponseHandler_var::operator= (const TAO_AMH_Locate_ResponseHandler_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = TAO_AMH_Locate_ResponseHandler::_duplicate (p.ptr ());
    }
  return *this;
}

ACE_INLINE
TAO_AMH_Locate_ResponseHandler_var::operator const TAO_AMH_Locate_ResponseHandler_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
TAO_AMH_Locate_ResponseHandler_var::operator TAO_AMH_Locate_ResponseHandler_ptr &()
{
  return this->ptr_;
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr &
TAO_AMH_Locate_ResponseHandler_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr &
TAO_AMH_Locate_ResponseHandler_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = TAO_AMH_Locate_ResponseHandler::_nil ();
  return this->ptr_;
}

ACE_INLINE TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler_var::_retn (void)
{
  // yield ownership
  TAO_AMH_Locate_ResponseHandler_ptr val = this->ptr_;
  this->ptr_ = TAO_AMH_Locate_ResponseHandler::_nil ();
  return val;
}
TAO_END_VERSIONED_NAMESPACE_DECL
