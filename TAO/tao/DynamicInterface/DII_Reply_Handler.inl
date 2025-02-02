// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler::_duplicate (TAO_DII_Reply_Handler_ptr x)
{
  if (x != 0)
    {
      x->_add_ref ();
    }

  return x;
}

// Inline operations for class TAO_DII_Reply_Handler_var
ACE_INLINE
TAO_DII_Reply_Handler_var::TAO_DII_Reply_Handler_var ()
  : ptr_ (TAO_DII_Reply_Handler::_nil ())
{
}

ACE_INLINE
TAO_DII_Reply_Handler_var::TAO_DII_Reply_Handler_var (
    TAO_DII_Reply_Handler_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
TAO_DII_Reply_Handler_var::~TAO_DII_Reply_Handler_var ()
{
  CORBA::release (this->ptr_);
}

ACE_INLINE TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler_var::ptr () const
{
  return this->ptr_;
}

ACE_INLINE
TAO_DII_Reply_Handler_var::TAO_DII_Reply_Handler_var (
    const TAO_DII_Reply_Handler_var &p)
  : ptr_ (TAO_DII_Reply_Handler::_duplicate (p.ptr ()))
{}

ACE_INLINE TAO_DII_Reply_Handler_var &
TAO_DII_Reply_Handler_var::operator= (TAO_DII_Reply_Handler_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE TAO_DII_Reply_Handler_var &
TAO_DII_Reply_Handler_var::operator= (const TAO_DII_Reply_Handler_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = TAO_DII_Reply_Handler::_duplicate (p.ptr ());
    }
  return *this;
}

ACE_INLINE
TAO_DII_Reply_Handler_var::operator const TAO_DII_Reply_Handler_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
TAO_DII_Reply_Handler_var::operator TAO_DII_Reply_Handler_ptr &()
{
  return this->ptr_;
}

ACE_INLINE TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler_var::operator-> () const
{
  return this->ptr_;
}

ACE_INLINE TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler_var::in () const
{
  return this->ptr_;
}

ACE_INLINE TAO_DII_Reply_Handler_ptr &
TAO_DII_Reply_Handler_var::inout ()
{
  return this->ptr_;
}

ACE_INLINE TAO_DII_Reply_Handler_ptr &
TAO_DII_Reply_Handler_var::out ()
{
  CORBA::release (this->ptr_);
  this->ptr_ = TAO_DII_Reply_Handler::_nil ();
  return this->ptr_;
}

ACE_INLINE TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler_var::_retn ()
{
  // yield ownership
  TAO_DII_Reply_Handler_ptr val = this->ptr_;
  this->ptr_ = TAO_DII_Reply_Handler::_nil ();
  return val;
}
TAO_END_VERSIONED_NAMESPACE_DECL
