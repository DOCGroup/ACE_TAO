// -*- C++ -*-
//
// $Id$

#include "tao/Interceptor_List.h"

#include "tao/CORBA_String.h"

#if !defined (__ACE_INLINE__)
# include "tao/Interceptor_List.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Interceptor_List, "$Id$")

// ****************************************************************

TAO_Interceptor_List::TAO_Interceptor_List (void)
{
}

TAO_Interceptor_List::~TAO_Interceptor_List (void)
{
}

size_t
TAO_Interceptor_List::add_interceptor_i (
    PortableInterceptor::Interceptor_ptr interceptor
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
  if (!CORBA::is_nil (interceptor))
    {
      /// If the Interceptor is not anonymous, make sure an
      /// Interceptor with the same isn't already registered.
      CORBA::String_var name = interceptor->name (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      size_t old_len = this->length ();

      if (ACE_OS_String::strlen (name.in ()) != 0)
        {
          // @@ This simple search algorithm isn't the greatest thing
          //    in the world, but since we only register interceptors
          //    when bootstrapping an ORB, there will be no run-time
          //    penalty.
          //
          //    Another source of inefficiency is that Interceptors
          //    duplicate their name each time the name() accessor is
          //    called!  This can slow down bootstrap time noticeably
          //    when registering a huge number of interceptors.  We
          //    could cache the names somewhere, but since this is
          //    only a bootstrapping issue there's no rush to
          //    implement such a scheme.

          /// Prevent interceptors with the same name from being
          /// registered.  Anonymous interceptors are okay.
          for (size_t i = 0; i < old_len; ++i)
            {
              CORBA::String_var existing_name =
                this->interceptor (i)->name ();
              if (ACE_OS_String::strcmp (existing_name.in (),
                                         name.in ()) == 0)
                ACE_THROW_RETURN
                  (PortableInterceptor::ORBInitInfo::DuplicateName (),
                   0);
            }
        }

      /// Increase the length of the Interceptor sequence by one.
      size_t new_len = old_len + 1;
      this->length (new_len);
          return old_len;
    }
  else
    ACE_THROW_RETURN (CORBA::INV_OBJREF (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EINVAL),
                        CORBA::COMPLETED_NO),
                      0);
}


// -------------------------------------------------------------------

#if TAO_HAS_INTERCEPTORS == 1

TAO_ClientRequestInterceptor_List::TAO_ClientRequestInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_ClientRequestInterceptor_List::~TAO_ClientRequestInterceptor_List (void)
{
  size_t len = this->interceptors_.size ();

  for (size_t i = 0; i < len; ++i)
    CORBA::release (this->interceptors_[i]);
}

size_t
TAO_ClientRequestInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_ClientRequestInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}


PortableInterceptor::Interceptor_ptr
TAO_ClientRequestInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index];
}

void
TAO_ClientRequestInterceptor_List::add_interceptor (
  PortableInterceptor::ClientRequestInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  size_t index = this->add_interceptor_i (interceptor
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::ClientRequestInterceptor::_duplicate (interceptor);
}

// -------------------------------------------------------------------

TAO_ServerRequestInterceptor_List::TAO_ServerRequestInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_ServerRequestInterceptor_List::~TAO_ServerRequestInterceptor_List (void)
{
  size_t len = this->interceptors_.size ();

  for (size_t i = 0; i < len; ++i)
    CORBA::release (this->interceptors_[i]);
}

size_t
TAO_ServerRequestInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_ServerRequestInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}

PortableInterceptor::Interceptor_ptr
TAO_ServerRequestInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index];
}

void
TAO_ServerRequestInterceptor_List::add_interceptor (
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  size_t index = this->add_interceptor_i (interceptor
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::ServerRequestInterceptor::_duplicate (interceptor);
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

// -------------------------------------------------------------------

TAO_IORInterceptor_List::TAO_IORInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_IORInterceptor_List::~TAO_IORInterceptor_List (void)
{
  size_t len = this->interceptors_.size ();

  for (size_t i = 0; i < len; ++i)
    CORBA::release (this->interceptors_[i]);
}

size_t
TAO_IORInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_IORInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}

PortableInterceptor::Interceptor_ptr
TAO_IORInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index];
}

void
TAO_IORInterceptor_List::add_interceptor (
  PortableInterceptor::IORInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  size_t index = this->add_interceptor_i (interceptor
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::IORInterceptor::_duplicate (interceptor);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

# if TAO_HAS_INTERCEPTORS == 1

template class ACE_Array_Base<PortableInterceptor::ClientRequestInterceptor_ptr>;
template class ACE_Array_Base<PortableInterceptor::ServerRequestInterceptor_ptr>;

# endif  /* TAO_HAS_INTERCEPTORS == 1 */

template class ACE_Array_Base<PortableInterceptor::IORInterceptor_ptr>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# if TAO_HAS_INTERCEPTORS == 1

#pragma instantiate ACE_Array_Base<PortableInterceptor::ClientRequestInterceptor_ptr>
#pragma instantiate ACE_Array_Base<PortableInterceptor::ServerRequestInterceptor_ptr>

# endif  /* TAO_HAS_INTERCEPTORS == 1 */

#pragma instantiate ACE_Array_Base<PortableInterceptor::IORInterceptor_ptr>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

