// $Id$

#include "IORInterceptor_Adapter_Impl.h"
#include "tao/debug.h"

ACE_RCSID (IORInterceptor,
           IORInterceptor_Adapter_Impl,
           "$Id$")


TAO_IORInterceptor_Adapter_Impl::~TAO_IORInterceptor_Adapter_Impl (void)
{
}

void
TAO_IORInterceptor_Adapter_Impl::add_interceptor (
    PortableInterceptor::IORInterceptor_ptr i
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->ior_interceptor_list_.add_interceptor (i
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_IORInterceptor_Adapter_Impl::destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC (())
{
  TAO_IORInterceptor_List::TYPE & i =
    this->ior_interceptor_list_.interceptors ();

  const size_t len = i.size ();
  size_t ilen = len;

  ACE_TRY
    {
      for (size_t k = 0; k < len; ++k)
        {
          // Destroy the interceptors in reverse order in case the
          // array list is only partially destroyed and another
          // invocation occurs afterwards.
          --ilen;

          i[k]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Since Interceptor::destroy() can throw an exception,
          // decrease the size of the interceptor array incrementally
          // since some interceptors may not have been destroyed yet.
          // Note that this size reduction is fast since no memory is
          // actually deallocated.
          i.size (ilen);
        }
    }
  ACE_CATCHALL
    {
      // Exceptions should not be propagated beyond this call.
      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Exception in ")
                      ACE_TEXT ("IORInterceptor_Adapter_Impl")
                      ACE_TEXT ("::destroy_interceptors () \n")));
        }
    }
  ACE_ENDTRY;
  ACE_CHECK;

  delete this;
}

TAO_IORInterceptor_List *
TAO_IORInterceptor_Adapter_Impl::interceptor_list (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->ior_interceptor_list_;
}
