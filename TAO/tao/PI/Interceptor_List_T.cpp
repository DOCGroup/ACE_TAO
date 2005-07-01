// $Id$

#include "InterceptorC.h"
#include "ORBInitInfoC.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"

#include "ace/os_include/os_stddef.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

namespace TAO
{
  template <typename InterceptorType>
  Interceptor_List<InterceptorType>::Interceptor_List (void)
  {
  }

  template <typename InterceptorType>
  Interceptor_List<typename InterceptorType>::InterceptorType_ptr_type
  Interceptor_List<InterceptorType>::interceptor (size_t index)
  {
    return this->interceptors_[index].in ();
  }

  template <typename InterceptorType>
  size_t
  Interceptor_List<InterceptorType>::size (void)
  {
    return this->interceptors_.size ();
  }

  template <typename InterceptorType>
  void
  Interceptor_List<InterceptorType>::add_interceptor (
    InterceptorType_ptr_type interceptor
    ACE_ENV_ARG_DECL)
  {
    if (!CORBA::is_nil (interceptor))
      {
        const size_t old_len = this->interceptors_.size ();

        // Don't bother checking the name for duplicates if no
        // interceptors have been registered.  This saves an
        // allocation.
        if (old_len > 0)
          {
            /// If the Interceptor is not anonymous, make sure an
            /// Interceptor with the same isn't already registered.
            CORBA::String_var name =
              interceptor->name (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK;

            if (ACE_OS::strlen (name.in ()) != 0)
              {
                // @@ This simple search algorithm isn't the greatest
                //    thing in the world, but since we only register
                //    interceptors when bootstrapping an ORB, there will
                //    be no runtime penalty.
                //
                //    Another source of inefficiency is that
                //    Interceptors duplicate their name each time the
                //    name() accessor is called!  This can slow down
                //    bootstrap time noticeably when registering a huge
                //    number of interceptors.  We could cache the names
                //    somewhere, but since this is only a bootstrapping
                //    issue there's no rush to implement such a scheme.

                // Prevent interceptors with the same name from being
                // registered.  Anonymous interceptors are okay.
                for (size_t i = 0; i < old_len; ++i)
                  {
                    CORBA::String_var existing_name =
                      this->interceptor (i)->name ();

                    if (ACE_OS::strcmp (existing_name.in (),
                                        name.in ()) == 0)
                      {
                        ACE_THROW (PortableInterceptor::ORBInitInfo::DuplicateName ());
                      }
                  }
              }
          }

        /// Increase the length of the Interceptor sequence by one.
        const size_t new_len = old_len + 1;
        this->interceptors_.size (new_len);

        // Add the interceptor
        this->interceptors_[old_len] = InterceptorType::_duplicate (interceptor);
      }
    else
      {
        ACE_THROW (
            CORBA::INV_OBJREF (
                CORBA::SystemException::_tao_minor_code (
                    0,
                    EINVAL
                  ),
                CORBA::COMPLETED_NO
              )
            );
      }
  }

  template <typename InterceptorType>
  void
  Interceptor_List<InterceptorType>::destroy_interceptors (
    ACE_ENV_SINGLE_ARG_DECL)
  {
    const size_t len = this->interceptors_.size ();
    size_t ilen = len;

    ACE_TRY
      {
        for (size_t k = 0; k < len; ++k)
          {
            // Destroy the interceptors in reverse order in case the
            // array list is only partially destroyed and another
            // invocation occurs afterwards.
            --ilen;

            this->interceptor (k)->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            // Since Interceptor::destroy() can throw an exception,
            // decrease the size of the interceptor array incrementally
            // since some interceptors may not have been destroyed yet.
            // Note that this size reduction is fast since no memory is
            // actually deallocated.
            this->interceptors_.size (ilen);
          }
      }
    ACE_CATCHALL
      {
        // Exceptions should not be propagated beyond this call.
        if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Exception in ")
                        ACE_TEXT ("Interceptor_List")
                        ACE_TEXT ("::destroy_interceptors () \n")));
          }
      }
    ACE_ENDTRY;
    ACE_CHECK;
  }
}
