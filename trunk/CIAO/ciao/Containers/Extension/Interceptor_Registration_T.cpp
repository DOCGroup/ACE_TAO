// $Id$

#include "ciao/Valuetype_Factories/Cookies.h"
namespace CIAO
{
  template <typename BASE, typename COPITYPE>
  Interceptor_Registration_T<BASE, COPITYPE>::Interceptor_Registration_T (void)
  {
  }

  template <typename BASE, typename COPITYPE>
  ::Components::Cookie *
  Interceptor_Registration_T<BASE, COPITYPE>::register_interceptor (
    InterceptorType_ptr_type ict)
  {
    // Create cookie
    Components::Cookie *key_cookie = 0;
    ACE_NEW_THROW_EX (key_cookie,
                      Cookie_Impl (reinterpret_cast<ptrdiff_t> (ict)),
                      CORBA::NO_MEMORY ());
    this->interceptors_[key_cookie] = ict;
    return key_cookie;
  }

  template <typename BASE, typename COPITYPE>
  typename Interceptor_Registration_T<BASE, COPITYPE>::InterceptorType_ptr_type
  Interceptor_Registration_T<BASE, COPITYPE>::unregister_interceptor (
    ::Components::Cookie *ck)
  {
    typename RegisteredArray::iterator it = this->interceptors_.find (ck);

    if (it != this->interceptors_.end ())
      {
        InterceptorType_ptr_type interceptor = it->second;
        this->interceptors_.erase (it);
        return interceptor;
      }
    throw ::Components::ContainerPortableInterceptor::InvalidRegistration ();
  }
}
