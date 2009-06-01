// $Id$

#ifndef TAO_OBJECT_T_CPP
#define TAO_OBJECT_T_CPP

#include "tao/Object_T.h"
#include "tao/Stub.h"
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T>
  T *
  Narrow_Utils<T>::narrow (CORBA::Object_ptr obj,
                           const char *repo_id,
                           Proxy_Broker_Factory pbf)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    if (obj->_is_a (repo_id) == false)
      {
        return T::_nil ();
      }

    return TAO::Narrow_Utils<T>::unchecked_narrow (obj, repo_id, pbf);
  }

  template<typename T> T *
  Narrow_Utils<T>::unchecked_narrow (CORBA::Object_ptr obj,
                                     Proxy_Broker_Factory pbf)
  {
    T *proxy = 0;
    try
      {
        proxy = TAO::Narrow_Utils<T>::unchecked_narrow (obj, 0, pbf);
      }
    catch (const ::CORBA::Exception&)
      {
        // Swallow the exception
        return T::_nil ();
      }

    return proxy;
  }

  template<typename T> T *
  Narrow_Utils<T>::unchecked_narrow (CORBA::Object_ptr obj,
                                     const char *,
                                     Proxy_Broker_Factory pbf)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    if (obj->_is_local ())
      {
        return T::_duplicate (dynamic_cast<T *> (obj));
      }

    T_ptr proxy = Narrow_Utils<T>::lazy_evaluation (obj);

    if (!CORBA::is_nil (proxy))
      {
        return proxy;
      }

    TAO_Stub* stub = obj->_stubobj ();

    if (stub == 0)
      {
        // If we're here, we have been passed a bogus objref.
        throw ::CORBA::BAD_PARAM ();
      }

    stub->_incr_refcnt ();

    bool const collocated =
      !CORBA::is_nil (stub->servant_orb_var ().in ())
      && stub->optimize_collocation_objects ()
      && obj->_is_collocated ()
      && pbf != 0;

    ACE_NEW_THROW_EX (proxy,
                      T (stub,
                         collocated,
                         obj->_servant ()),
                      CORBA::NO_MEMORY ());
    return proxy;
  }

  template<typename T>
  T *
  Narrow_Utils<T>::lazy_evaluation (CORBA::Object_ptr obj)
  {
    T_ptr default_proxy = T::_nil ();

    // Code for lazily evaluated IORs.
    if (!obj->is_evaluated ())
      {
        ACE_NEW_RETURN (default_proxy,
                        T (obj->steal_ior (),
                           obj->orb_core ()),
                        T::_nil ());
      }

    return default_proxy;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_OBJECT_T_CPP */
