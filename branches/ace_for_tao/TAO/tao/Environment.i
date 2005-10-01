// -*- C++ -*-
//
// $Id$

ACE_INLINE
CORBA::Boolean
CORBA::is_nil (CORBA::Environment_ptr env)
{
  return env == 0;
}

ACE_INLINE
void
CORBA::release (CORBA::Environment_ptr env)
{
  delete env;
}

ACE_INLINE
CORBA::Exception *
CORBA::Environment::exception (void) const
{
  return this->exception_;
}

ACE_INLINE
CORBA::Environment *
CORBA::Environment::_duplicate (CORBA::Environment *x)
{
  if (x == 0)
    {
      return 0;
    }

  return new CORBA::Environment (*x);
}

ACE_INLINE
CORBA::Environment_ptr
CORBA::Environment::_nil (void)
{
  return static_cast <CORBA::Environment_ptr> (0);
}

