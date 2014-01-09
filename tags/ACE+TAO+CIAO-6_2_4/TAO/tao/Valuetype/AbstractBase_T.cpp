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

    return AbstractBase_Narrow_Utils<T>::unchecked_narrow (obj);
  }

  template<typename T>  T *
  AbstractBase_Narrow_Utils<T>::unchecked_narrow (CORBA::AbstractBase_ptr obj)
  {
    if (CORBA::is_nil (obj))
      {
        return T::_nil ();
      }

    T_ptr proxy = T::_nil ();

    try
      {
        if (obj->_is_objref ())
          {
            TAO_Stub* stub = obj->_stubobj ();

            bool const collocated =
              !CORBA::is_nil (stub->servant_orb_var ().in ())
              && stub->optimize_collocation_objects ()
              && obj->_is_collocated ();

            ACE_NEW_RETURN (proxy,
                            T (obj->_stubobj (),
                               collocated,
                               obj->_servant ()),
                            T::_nil ());
          }
        else
          {
            proxy = dynamic_cast<T *> (obj);
            if (proxy)
              proxy->_add_ref ();
          }
      }
    catch (const ::CORBA::Exception&)
      {
      }

    return proxy;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_ABSTRACT_BASE_T_CPP */
