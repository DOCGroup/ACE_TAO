// -*- c++ -*-
//
// $Id$

// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA::release (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->Release ();
}

ACE_INLINE ULONG
CORBA_ORB::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));

  return refcount_++;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_duplicate (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->AddRef ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::ORB_ptr obj)
{
  return (CORBA::Boolean) (obj == 0);
}

ACE_INLINE CORBA::Boolean
CORBA_ORB::work_pending (void)
{
  // There's ALWAYS work to do ;-)
  return CORBA::B_TRUE;
}

ACE_INLINE void
CORBA_ORB::shutdown (CORBA::Boolean wait_for_completion)
{
  ACE_UNUSED_ARG (wait_for_completion);

  this->should_shutdown_ = CORBA::B_TRUE;
  TAO_ORB_Core_instance ()->reactor ()->wakeup_all_threads ();
  return;
}

// *************************************************************
// Inline operations for class CORBA_ORB_var
// *************************************************************

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (void) // default constructor
	: ptr_ (CORBA_ORB::_nil ())
{}

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (CORBA::ORB_ptr p)
	: ptr_ (p)
{}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (const CORBA_ORB_var &p) // copy constructor
	: ptr_ (CORBA_ORB::_duplicate (p.ptr ()))
{}

ACE_INLINE
CORBA_ORB_var::~CORBA_ORB_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_ORB_var &
CORBA_ORB_var::operator= (CORBA::ORB_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ORB_var &
CORBA_ORB_var::operator= (const CORBA_ORB_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_ORB::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_ORB_var::operator const CORBA::ORB_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ORB_var::operator CORBA::ORB_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_ORB::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA::ORB_ptr val = this->ptr_;
  this->ptr_ = CORBA_ORB::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_ORB_out
// *************************************************************

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA::ORB_ptr &p)
	: ptr_ (p)
{
  this->ptr_ = CORBA_ORB::_nil ();
}

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA_ORB_var &p) // constructor from _var
	: ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_ORB::_nil ();
}

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA_ORB_out &p) // copy constructor
	: ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (CORBA_ORB_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (const CORBA_ORB_var &p)
{
  this->ptr_ = CORBA_ORB::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (CORBA::ORB_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_ORB_out::operator CORBA::ORB_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_out::operator-> (void)
{
  return this->ptr_;
}
