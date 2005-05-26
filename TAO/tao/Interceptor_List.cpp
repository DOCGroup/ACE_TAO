// $Id$

#include "tao/Interceptor_List.h"

ACE_RCSID (tao,
           Interceptor_List,
           "$Id$")

#include "tao/InterceptorC.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "ace/os_include/os_stddef.h"
#include "ace/OS_NS_string.h"

namespace TAO
{
  template <typename InterceptorType>
  Interceptor_List<InterceptorType>::Interceptor_List (void)
  {
  }

  template <typename InterceptorType>
  size_t
  Interceptor_List<InterceptorType>::add_interceptor_i (
      PortableInterceptor::Interceptor_ptr interceptor
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::DuplicateName))
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
            ACE_CHECK_RETURN (0);

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
                        ACE_THROW_RETURN
                          (PortableInterceptor::ORBInitInfo::DuplicateName (),
                           0);
                      }
                  }
              }
          }

        /// Increase the length of the Interceptor sequence by one.
        const size_t new_len = old_len + 1;
        this->interceptors_.size (new_len);
        return old_len;
      }
    else
      {
        ACE_THROW_RETURN (
            CORBA::INV_OBJREF (
                CORBA::SystemException::_tao_minor_code (
                    0,
                    EINVAL
                  ),
                CORBA::COMPLETED_NO
              ),
            0
          );
      }
  }

  template <typename InterceptorType>
  PortableInterceptor::Interceptor_ptr
  Interceptor_List<InterceptorType>::interceptor (size_t index)
  {
    return this->interceptors_[index].in ();
  }

  template <typename InterceptorType>
  void
  Interceptor_List<InterceptorType>::add_interceptor (
    InterceptorType_ptr_type interceptor
    ACE_ENV_ARG_DECL)
  {
    const size_t index = this->add_interceptor_i (interceptor
                                                  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->interceptors_[index] = InterceptorType::_duplicate (interceptor);
  }

  template <typename InterceptorType>
  typename Interceptor_List<InterceptorType>::TYPE &
  Interceptor_List<InterceptorType>::interceptors (void)
  {
    return this->interceptors_;
  }
}
