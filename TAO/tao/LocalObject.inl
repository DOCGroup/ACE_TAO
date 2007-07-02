// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::LocalObject_ptr
CORBA::LocalObject::_duplicate (CORBA::LocalObject_ptr obj)
{
  if (obj)
    {
      obj->_add_ref ();
    }

  return obj;
}

ACE_INLINE
CORBA::LocalObject_ptr
CORBA::LocalObject::_nil (void)
{
  return 0;
}

ACE_INLINE
CORBA::LocalObject_ptr
CORBA::LocalObject::_narrow (CORBA::Object_ptr obj)
{
  if (obj->_is_local () != 0)
    {
      return CORBA::LocalObject::_duplicate (
          dynamic_cast<CORBA::LocalObject_ptr> (obj)
        );
    }

  return 0;
}

ACE_INLINE
CORBA::LocalObject::LocalObject (void)
  : Object (0)
{
}

// =================================================================

ACE_INLINE
TAO_Local_RefCounted_Object::TAO_Local_RefCounted_Object (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
