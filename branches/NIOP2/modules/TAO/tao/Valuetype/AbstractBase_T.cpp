// $Id$

#ifndef TAO_ABSTRACT_BASE_T_CPP
#define TAO_ABSTRACT_BASE_T_CPP

#include "tao/Valuetype/AbstractBase_T.h"
#include "tao/Valuetype/AbstractBase.h"
#include "tao/Stub.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T> T *
  AbstractBase_Narrow_Utils<T>::narrow (
      CORBA::AbstractBase_ptr obj,
      const char *repo_id,
      Proxy_Broker_Factory pbf
      ACE_ENV_ARG_DECL)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    CORBA::Boolean const is_it =
      obj->_is_a (
          repo_id
          ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK_RETURN (T::_nil ());

    if (is_it == false)
      {
        return T::_nil ();
      }

    return
      AbstractBase_Narrow_Utils<T>::unchecked_narrow (obj,
                                                      repo_id,
                                                      pbf
                                                      ACE_ENV_ARG_PARAMETER);
  }

  template<typename T>  T *
  AbstractBase_Narrow_Utils<T>::unchecked_narrow (
      CORBA::AbstractBase_ptr obj,
      Proxy_Broker_Factory pbf)
  {
    T *proxy = 0;

    ACE_DECLARE_NEW_CORBA_ENV;

    ACE_TRY
      {
        proxy =
          AbstractBase_Narrow_Utils<T>::unchecked_narrow (
              obj,
              0,
              pbf
              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

      }
    ACE_CATCHANY
      {
        // Consume and return proxy
        return proxy;
      }
    ACE_ENDTRY;
    ACE_CHECK_RETURN (proxy);

    return proxy;
  }

  template<typename T>  T *
  AbstractBase_Narrow_Utils<T>::unchecked_narrow (
      CORBA::AbstractBase_ptr obj,
      const char *,
      Proxy_Broker_Factory pbf
      ACE_ENV_ARG_DECL)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    T_ptr proxy = T::_nil ();

    if (obj->_is_objref ())
      {
        TAO_Stub* stub = obj->_stubobj ();

        bool const collocated =
          !CORBA::is_nil (stub->servant_orb_var ().in ())
          && stub->optimize_collocation_objects ()
          && obj->_is_collocated ()
          && pbf != 0;

        ACE_NEW_THROW_EX (proxy,
                          T (obj->_stubobj (),
                             collocated ? 1 : 0,
                             obj->_servant ()),
                          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (proxy);
      }
    else
      {
        proxy = dynamic_cast<T *> (obj);
        proxy->_add_ref ();
      }

    return proxy;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_ABSTRACT_BASE_T_CPP */
