// This may look like C, but it's really -*- C++ -*-

ACE_INLINE void *
CORBA_Exception::operator new (size_t,
                               const void *p)
{
  return (void *) p;
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
CORBA_SystemException::completion (void) const
{
  return this->completed_;
}

ACE_INLINE void
CORBA_SystemException::completion (CORBA::CompletionStatus c)
{
  this->completed_ = c;
}

ACE_INLINE
<<<<<<< Exception.i
=======
CORBA_Environment::CORBA_Environment (void)
  : exception_ (0)
{
}

ACE_INLINE void
CORBA_Environment::clear (void)
{
  if (this->exception_)
    {
      this->exception_->Release ();
    }
  this->exception_ = 0;
}

ACE_INLINE
CORBA_Environment::~CORBA_Environment (void)
{
  this->clear ();
}

ACE_INLINE
CORBA::Exception_ptr CORBA_Environment::exception (void) const
{
  return this->exception_;
}

