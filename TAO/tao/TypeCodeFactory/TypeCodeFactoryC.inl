/* -*- C++ -*- */
//$Id$

#if !defined (_CORBA_TYPECODEFACTORY___VAR_CI_)
#define _CORBA_TYPECODEFACTORY___VAR_CI_

// *************************************************************
// Inline operations for class CORBA_TypeCodeFactory_var
// *************************************************************

ACE_INLINE
CORBA_TypeCodeFactory_var::CORBA_TypeCodeFactory_var (void) // default constructor
  : ptr_ (CORBA_TypeCodeFactory::_nil ())
{}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr
CORBA_TypeCodeFactory_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_TypeCodeFactory_var::CORBA_TypeCodeFactory_var (const ::CORBA_TypeCodeFactory_var &p) // copy constructor
  : TAO_Base_var (),
    ptr_ (CORBA_TypeCodeFactory::_duplicate (p.ptr ()))
{}

ACE_INLINE
CORBA_TypeCodeFactory_var::~CORBA_TypeCodeFactory_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_TypeCodeFactory_var &
CORBA_TypeCodeFactory_var::operator= (CORBA_TypeCodeFactory_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_TypeCodeFactory_var &
CORBA_TypeCodeFactory_var::operator= (const ::CORBA_TypeCodeFactory_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::CORBA_TypeCodeFactory::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_TypeCodeFactory_var::operator const ::CORBA_TypeCodeFactory_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_TypeCodeFactory_var::operator ::CORBA_TypeCodeFactory_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr
CORBA_TypeCodeFactory_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr
CORBA_TypeCodeFactory_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr &
CORBA_TypeCodeFactory_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr &
CORBA_TypeCodeFactory_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::CORBA_TypeCodeFactory::_nil ();
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr
CORBA_TypeCodeFactory_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::CORBA_TypeCodeFactory_ptr val = this->ptr_;
  this->ptr_ = ::CORBA_TypeCodeFactory::_nil ();
  return val;
}


#endif /* end #if !defined */


#if !defined (_CORBA_TYPECODEFACTORY___OUT_CI_)
#define _CORBA_TYPECODEFACTORY___OUT_CI_

// *************************************************************
// Inline operations for class CORBA_TypeCodeFactory_out
// *************************************************************

ACE_INLINE
CORBA_TypeCodeFactory_out::CORBA_TypeCodeFactory_out (CORBA_TypeCodeFactory_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = ::CORBA_TypeCodeFactory::_nil ();
}

ACE_INLINE
CORBA_TypeCodeFactory_out::CORBA_TypeCodeFactory_out (CORBA_TypeCodeFactory_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::CORBA_TypeCodeFactory::_nil ();
}

ACE_INLINE
CORBA_TypeCodeFactory_out::CORBA_TypeCodeFactory_out (const ::CORBA_TypeCodeFactory_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CORBA_TypeCodeFactory_out &, p).ptr_)
{}

ACE_INLINE ::CORBA_TypeCodeFactory_out &
CORBA_TypeCodeFactory_out::operator= (const ::CORBA_TypeCodeFactory_out &p)
{
  this->ptr_ = ACE_const_cast (CORBA_TypeCodeFactory_out&, p).ptr_;
  return *this;
}

ACE_INLINE CORBA_TypeCodeFactory_out &
CORBA_TypeCodeFactory_out::operator= (const ::CORBA_TypeCodeFactory_var &p)
{
  this->ptr_ = ::CORBA_TypeCodeFactory::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_TypeCodeFactory_out &
CORBA_TypeCodeFactory_out::operator= (CORBA_TypeCodeFactory_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_TypeCodeFactory_out::operator ::CORBA_TypeCodeFactory_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr &
CORBA_TypeCodeFactory_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::CORBA_TypeCodeFactory_ptr
CORBA_TypeCodeFactory_out::operator-> (void)
{
  return this->ptr_;
}


#endif /* end #if !defined */

