// $Id$

// This may look like C, but it's really -*- C++ -*-

// Return the target of this request.

ACE_INLINE void
CORBA::release (CORBA::Request_ptr req)
{
  if (req)
    req->_decr_refcnt ();
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::Request_ptr req)
{
  return (CORBA::Boolean) (req == 0);
}

ACE_INLINE CORBA_Request*
CORBA_Request::_duplicate (CORBA_Request* x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_Request*
CORBA_Request::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Object_ptr 
CORBA_Request::target (void) const 
{ 
  return target_; 
}

// Return the operation name for the request.
ACE_INLINE const CORBA::Char *
CORBA_Request::operation (void) const 
{
  return opname_; 
}

// Return the arguments for the request.
ACE_INLINE CORBA::NVList_ptr 
CORBA_Request::arguments (void) 
{ 
  return args_; 
}

// Return the result for the request.
ACE_INLINE CORBA::NamedValue_ptr 
CORBA_Request::result (void) 
{
  return result_; 
}

// Return the exceptions resulting from this request.
ACE_INLINE CORBA::ExceptionList_ptr 
CORBA_Request::exceptions (void) 
{ 
  return &exceptions_; 
}

// Return the <Environment> for this request.
ACE_INLINE CORBA::Environment *
CORBA_Request::env (void) 
{
  return &env_; 
}
