// $Id$

#include "tao/PI/PI_includeC.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"

#include "ace/os_include/os_stddef.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename InterceptorType, typename DetailsType>
  Interceptor_List<InterceptorType,DetailsType>::Interceptor_List (void)
  {
  }

  template <typename InterceptorType, typename DetailsType>
  typename Interceptor_List<InterceptorType,DetailsType>::RegisteredInterceptor&
  Interceptor_List<InterceptorType,DetailsType>::registered_interceptor (
    size_t index)
  {
    return this->interceptors_[index];
  }

  template <typename InterceptorType, typename DetailsType>
  typename Interceptor_List<InterceptorType,DetailsType>::InterceptorType_ptr_type
  Interceptor_List<InterceptorType,DetailsType>::interceptor (size_t index)
  {
    return this->interceptors_[index].interceptor_.in ();
  }

  template <typename InterceptorType, typename DetailsType>
  size_t
  Interceptor_List<InterceptorType,DetailsType>::size (void) const
  {
    return this->interceptors_.size ();
  }

  template <typename InterceptorType, typename DetailsType>
  void
  Interceptor_List<InterceptorType,DetailsType>::add_interceptor (
    InterceptorType_ptr_type interceptor)
  {
    if (!CORBA::is_nil (interceptor))
      {
        size_t const old_len = this->interceptors_.size ();

        // Don't bother checking the name for duplicates if no
        // interceptors have been registered.  This saves an
        // allocation.
        if (old_len > 0)
          {
            /// If the Interceptor is not anonymous, make sure an
            /// Interceptor with the same isn't already registered.
            CORBA::String_var name = interceptor->name ();

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

                    if (ACE_OS::strcmp (existing_name.in (), name.in ()) == 0)
                      {
                        throw PortableInterceptor::ORBInitInfo::DuplicateName ();
                      }
                  }
              }
          }

        /// Increase the length of the Interceptor sequence by one.
        size_t const new_len = old_len + 1;
        this->interceptors_.size (new_len);

        // Add the interceptor
        this->interceptors_[old_len].interceptor_ =
          InterceptorType::_duplicate (interceptor);
      }
    else
      {
        throw
            CORBA::INV_OBJREF (
                CORBA::SystemException::_tao_minor_code (
                    0,
                    EINVAL
                  ),
                CORBA::COMPLETED_NO);
      }
  }

  template <typename InterceptorType, typename DetailsType>
  void
  Interceptor_List<InterceptorType,DetailsType>::add_interceptor (
    InterceptorType_ptr_type interceptor,
    const CORBA::PolicyList& policies
    )
  {
    if (!CORBA::is_nil (interceptor))
      {
        size_t const old_len = this->interceptors_.size ();

        // Don't bother checking the name for duplicates if no
        // interceptors have been registered.  This saves an
        // allocation.
        if (old_len > 0)
          {
            /// If the Interceptor is not anonymous, make sure an
            /// Interceptor with the same isn't already registered.
            CORBA::String_var name =
              interceptor->name ();

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
                        throw PortableInterceptor::ORBInitInfo::DuplicateName ();
                      }
                  }
              }
          }

        // Create a DetailsType object, and attempt to apply the policies.
        DetailsType details;
        details.apply_policies(policies);

        /// Increase the length of the Interceptor sequence by one.
        size_t const new_len = old_len + 1;
        this->interceptors_.size (new_len);

        // Add the interceptor
        this->interceptors_[old_len].interceptor_ =
          InterceptorType::_duplicate (interceptor);

        // Set the details
        this->interceptors_[old_len].details_ = details;
      }
    else
      {
        throw
            CORBA::INV_OBJREF (
                CORBA::SystemException::_tao_minor_code (
                    0,
                    EINVAL
                  ),
                CORBA::COMPLETED_NO
            );
      }
  }

  template <typename InterceptorType, typename DetailsType>
  void
  Interceptor_List<InterceptorType,DetailsType>::destroy_interceptors (
    void)
  {
    size_t const len = this->interceptors_.size ();
    size_t ilen = len;

    try
      {
        for (size_t k = 0; k < len; ++k)
          {
            // Destroy the interceptors in reverse order in case the
            // array list is only partially destroyed and another
            // invocation occurs afterwards.
            --ilen;

            this->interceptor (k)->destroy ();

            // Since Interceptor::destroy() can throw an exception,
            // decrease the size of the interceptor array incrementally
            // since some interceptors may not have been destroyed yet.
            // Note that this size reduction is fast since no memory is
            // actually deallocated.
            this->interceptors_.size (ilen);
          }
      }
    catch (...)
      {
        // Exceptions should not be propagated beyond this call.
        if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Exception in ")
                        ACE_TEXT ("Interceptor_List")
                        ACE_TEXT ("::destroy_interceptors ()\n")));
          }
      }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
