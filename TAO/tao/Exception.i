// This may look like C, but it's really -*- C++ -*-

ACE_INLINE void *
CORBA_Exception::operator new (size_t,
                               const void *p)
{ 
  return (void *) p; 
}

ACE_INLINE void *
CORBA_Exception::operator new (size_t s) 
{ 
  return ::operator new (s); 
}

ACE_INLINE void 
CORBA_Exception::operator delete (void *p) 
{ 
  ::operator delete (p); 
}

ACE_INLINE CORBA::ULong 
CORBA_SystemException::minor (void) const 
{ 
  return _minor; 
}

ACE_INLINE void 
CORBA_SystemException::minor (CORBA::ULong m) 
{ 
  _minor = m; 
}

ACE_INLINE CORBA::CompletionStatus 
CORBA_SystemException::completion (void) const 
{ 
  return _completed; 
}

ACE_INLINE void
CORBA_SystemException::completion (CORBA::CompletionStatus c) 
{ 
  _completed = c; 
}

ACE_INLINE 
CORBA_Environment::CORBA_Environment (void)
  : _exception (0) 
{
}

ACE_INLINE void 
CORBA_Environment::clear (void)
{
  if (_exception)
    {
      _exception->Release ();
      _exception = 0;	// XXX
    }
}

ACE_INLINE 
CORBA_Environment::~CORBA_Environment (void) 
{ 
  this->clear (); 
}

ACE_INLINE 
CORBA::Exception_ptr CORBA_Environment::exception (void) const 
{ 
  return _exception; 
}

ACE_INLINE void 
CORBA_Environment::exception (CORBA::Exception *ex)
{
  if (ex != _exception)
    {
      this->clear ();
      _exception = ex;
      _exception->AddRef ();
    }
}
