// This may look like C, but it's really -*- C++ -*-
// $Id$

ACE_INLINE CORBA_Exception*
CORBA_Exception::_narrow (CORBA_Exception* x)
{
  return x;
}

ACE_INLINE
CORBA_UserException::CORBA_UserException (const CORBA_UserException &src)
  : CORBA_Exception (src)
{
}

ACE_INLINE CORBA::ULong
CORBA_SystemException::minor (void) const
{
  return this->minor_;
}

ACE_INLINE void
CORBA_SystemException::minor (CORBA::ULong m)
{
  this->minor_ = m;
}

ACE_INLINE CORBA::CompletionStatus
CORBA_SystemException::completed (void) const
{
  return this->completed_;
}

ACE_INLINE void
CORBA_SystemException::completed (CORBA::CompletionStatus c)
{
  this->completed_ = c;
}

ACE_INLINE
CORBA_ExceptionList::CORBA_ExceptionList (void)
{
}

ACE_INLINE CORBA::ULong
CORBA_ExceptionList::count (void)
{
  return (CORBA::ULong) this->tc_list_.size ();
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList::_nil (void)
{
  return (CORBA_ExceptionList_ptr)0;
}

ACE_INLINE
CORBA_ExceptionList_var::CORBA_ExceptionList_var (void) // default constructor
  : ptr_ (CORBA_ExceptionList::_nil ())
{}

ACE_INLINE
CORBA_ExceptionList_var::CORBA_ExceptionList_var (CORBA_ExceptionList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ExceptionList_var::CORBA_ExceptionList_var (const CORBA_ExceptionList_var &p) // copy constructor
  : ptr_ (p.ptr_->_duplicate ())
{}

ACE_INLINE
CORBA_ExceptionList_var::~CORBA_ExceptionList_var (void) // destructor
{
  this->ptr_->_destroy ();
}

ACE_INLINE CORBA_ExceptionList_var &
CORBA_ExceptionList_var::operator= (CORBA_ExceptionList_ptr p)
{
  this->ptr_->_destroy ();
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ExceptionList_var &
CORBA_ExceptionList_var::operator= (const CORBA_ExceptionList_var &p)
{
  if (this != &p)
    {
      this->ptr_->_destroy ();
      this->ptr_ = p.ptr_->_duplicate ();
    }
  return *this;
}

ACE_INLINE
CORBA_ExceptionList_var::operator const CORBA_ExceptionList_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ExceptionList_var::operator CORBA_ExceptionList_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr &
CORBA_ExceptionList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr &
CORBA_ExceptionList_var::out (void)
{
  this->ptr_->_destroy ();
  this->ptr_ = CORBA_ExceptionList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_ExceptionList_ptr val = this->ptr_;
  this->ptr_ = CORBA_ExceptionList::_nil ();
  return val;
}
