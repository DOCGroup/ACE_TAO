// -*- C++ -*-
//
// $Id$

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
CORBA::LocalObject::_narrow (CORBA::Object_ptr obj
                             ACE_ENV_ARG_DECL_NOT_USED)
{
  if (obj->_is_local () != 0)
    {
      return
        ACE_reinterpret_cast (
            CORBA::LocalObject_ptr,
            obj->_tao_QueryInterface (
                ACE_reinterpret_cast (
                    ptrdiff_t,
                    &CORBA::LocalObject::_narrow
                  )
              )
          );
    }

  return 0;
}

ACE_INLINE
CORBA::LocalObject_ptr
CORBA::LocalObject::_unchecked_narrow (CORBA::Object_ptr obj
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
  if (obj->_is_local () != 0)
    {
      return
         ACE_reinterpret_cast (
             CORBA::LocalObject_ptr,
             obj->_tao_QueryInterface (
                 ACE_reinterpret_cast (
                     ptrdiff_t,
                     &CORBA::LocalObject::_narrow
                   )
               )
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
  : refcount_lock_ (),
    refcount_ (1)
{
}
