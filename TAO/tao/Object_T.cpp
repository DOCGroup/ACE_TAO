// $Id$

#ifndef TAO_OBJECT_T_CPP
#define TAO_OBJECT_T_CPP

#include "tao/Object_T.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T>
  T *
  Narrow_Utils<T>::narrow (CORBA::Object_ptr obj,
                           const char *repo_id)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    if (!obj->_is_a (repo_id))
      {
        return T::_nil ();
      }

    return TAO::Narrow_Utils<T>::unchecked_narrow (obj);
  }

  template<typename T> T *
  Narrow_Utils<T>::unchecked_narrow (CORBA::Object_ptr obj)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    if (obj->_is_local ())
      {
        return T::_duplicate (dynamic_cast<T *> (obj));
      }

    T_ptr proxy = T::_nil ();
    try
      {
        proxy = Narrow_Utils<T>::lazy_evaluation (obj);

        if (CORBA::is_nil (proxy))
          {
            TAO_Stub* stub = obj->_stubobj ();

            if (stub != 0)
              {
                stub->_incr_refcnt ();

                bool const collocated =
                  !CORBA::is_nil (stub->servant_orb_var ().in ())
                  && stub->optimize_collocation_objects ()
                  && obj->_is_collocated ();

                ACE_NEW_RETURN (proxy,
                                T (stub,
                                   collocated,
                                   obj->_servant ()),
                                T::_nil ());
              }
          }
      }
    catch (const CORBA::Exception&)
      {
        // Swallow the exception
        return T::_nil ();
      }

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
