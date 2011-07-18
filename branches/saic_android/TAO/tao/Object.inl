// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

template<>
ACE_INLINE
CORBA::Boolean
CORBA::is_nil<> (CORBA::Object_ptr obj)
{
  if (obj == 0)
    {
      return true;
    }

  return CORBA::Object::is_nil_i (obj);
}

// ****************************************************************

ACE_INLINE
CORBA::Object::Object (int)
  : refcount_ (1),
    is_local_ (true),
    is_evaluated_ (true),
    ior_ (),
    orb_core_ (0),
    protocol_proxy_ (0)
{
}

ACE_INLINE CORBA::Object_ptr
CORBA::Object::_duplicate (CORBA::Object_ptr obj)
{
  if (obj)
    {
      obj->_add_ref ();
    }

  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE
CORBA::Object_ptr
CORBA::Object::_nil (void)
{
  return 0;
}

ACE_INLINE
CORBA::Object_ptr
CORBA::Object::_narrow (CORBA::Object_ptr obj)
{
  return CORBA::Object::_duplicate (obj);
}

ACE_INLINE CORBA::Boolean
CORBA::Object::is_evaluated (void) const
{
  return this->is_evaluated_;
}

ACE_INLINE TAO_ORB_Core *
CORBA::Object::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE IOP::IOR *
CORBA::Object::steal_ior (void)
{
  return this->ior_._retn ();
}

ACE_INLINE const IOP::IOR &
CORBA::Object::ior (void) const
{
  return this->ior_.in ();
}

ACE_INLINE void
CORBA::Object::_decr_refcount (void)
{
  this->_remove_ref ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
