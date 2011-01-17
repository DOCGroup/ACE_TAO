// $Id$

#include "ciao/Valuetype_Factories/Cookies.h"

#include <deque>
#include <queue>
#include <utility>
#include <functional>

namespace CIAO
{
  template <typename BASE, typename COPITYPE>
  Interceptor_Registration_T<BASE, COPITYPE>::Interceptor_Registration_T (void)
  {
  }

  template <typename BASE, typename COPITYPE>
  Interceptor_Registration_T<BASE, COPITYPE>::~Interceptor_Registration_T (void)
  {
  }

  template <typename BASE, typename COPITYPE>
  size_t
  Interceptor_Registration_T<BASE, COPITYPE>::size (void) const
  {
    return this->interceptors_.size ();
  }

  template <typename BASE, typename COPITYPE>
  typename Interceptor_Registration_T<BASE, COPITYPE>::InterceptorType_ptr_type
  Interceptor_Registration_T<BASE, COPITYPE>::interceptor (size_t index)
  {
    if (index > this->interceptors_.size ())
      {
        return COPITYPE::_nil ();
      }
    typedef std::pair< ::CORBA::UShort, InterceptorCookie > data_t;

    typedef std::vector<data_t> DataInVector;
    DataInVector vec (this->interceptors_.begin(), this->interceptors_.end());
    return COPITYPE::_duplicate (vec.at (index).second.second);
  }

  template <typename BASE, typename COPITYPE>
  typename Interceptor_Registration_T<BASE, COPITYPE>::InterceptorType_ptr_type
  Interceptor_Registration_T<BASE, COPITYPE>::interceptor (::Components::Cookie * ck)
  {
    typename RegisteredInterceptors::iterator it;
    for (it =  this->interceptors_.begin ();
         it != this->interceptors_.end ();
         ++it)
      {
        if (it->second.first.in () == ck)
          {
            return COPITYPE::_duplicate (it->second.second);
          }
      }
    return COPITYPE::_nil ();
  }

  template <typename BASE, typename COPITYPE>
  ::Components::Cookie *
  Interceptor_Registration_T<BASE, COPITYPE>::register_interceptor (
    InterceptorType_ptr_type ict)
  {
     ::Components::Cookie_var key_cookie;
    ACE_NEW_THROW_EX (key_cookie,
                      Cookie_Impl (reinterpret_cast<ptrdiff_t> (ict)),
                      CORBA::NO_MEMORY ());

    this->interceptors_.insert (
      std::pair < ::CORBA::UShort, InterceptorCookie > (
        ict->priority (),
        InterceptorCookie (key_cookie.in (), ict)));
     return key_cookie._retn ();
  }

  template <typename BASE, typename COPITYPE>
  typename Interceptor_Registration_T<BASE, COPITYPE>::InterceptorType_ptr_type
  Interceptor_Registration_T<BASE, COPITYPE>::unregister_interceptor (
    ::Components::Cookie *ck)
  {
    typename RegisteredInterceptors::iterator it;
    for (it =  this->interceptors_.begin ();
         it != this->interceptors_.end ();
         ++it)
      {
        if (it->second.first.in () == ck)
          {
            InterceptorType_ptr_type interceptor = it->second.second;
            this->interceptors_.erase (it);
            return COPITYPE::_duplicate (interceptor);
          }
      }
    throw ::Components::ContainerPortableInterceptor::InvalidRegistration ();
  }
}
