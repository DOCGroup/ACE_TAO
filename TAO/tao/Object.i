// -*- C++ -*-
//
// $Id$

// ****************************************************************

ACE_INLINE
CORBA::Object::Object (int)
  : is_collocated_ (0),
    servant_ (0),
    is_local_ (1),
    proxy_broker_ (0),
    protocol_proxy_ (0),
    refcount_ (1),
    refcount_lock_ (0)
{
}

ACE_INLINE 
CORBA::Object_ptr
CORBA::Object::_duplicate (CORBA::Object_ptr obj)
{
  if (obj)
    {
      obj->_add_ref ();
    }

  return obj;
}

// ************************************************************
// These are in CORBA namespace

ACE_INLINE 
void
CORBA::release (CORBA::Object_ptr obj)
{
  if (obj)
    {
      obj->_remove_ref ();
    }
}

ACE_INLINE 
CORBA::Boolean
CORBA::is_nil (CORBA::Object_ptr obj)
{
  if (obj == 0)
    {
      return 1;
    }

  return CORBA::Object::is_nil_i (obj);
}

// ************************************************************

// Null pointers represent nil objects.

ACE_INLINE 
CORBA::Object_ptr
CORBA::Object::_nil (void)
{
  return 0;
}

ACE_INLINE 
CORBA::Object_ptr
CORBA::Object::_narrow (CORBA::Object_ptr obj
                        ACE_ENV_ARG_DECL)
{
  return CORBA::Object::_unchecked_narrow (obj
                                           ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE 
TAO_Stub *
CORBA::Object::_stubobj (void) const
{
  return this->protocol_proxy_;
}

ACE_INLINE 
CORBA::Boolean
CORBA::Object::marshal (TAO_OutputCDR &cdr)
{
  return (cdr << this);
}

