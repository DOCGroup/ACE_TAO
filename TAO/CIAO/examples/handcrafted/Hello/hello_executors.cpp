// $Id$

#include "hello_executors.h"

HelloWorld_Impl::HelloWorld ()
{
}

HelloWorld_Impl::~HelloWorld ()
{
}

char *
HelloWorld_Impl::sayHello (const char *username
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  static const char *fmt = "Hello World message for \n";

  CORBA::String_var str =
    CORBA::string_alloc (ACE_OS::strlen (fmt) +
                         ACE_OS::strlen (username));

  ACE_OS::strcpy (str.inout (), fmt);
  ACE_OS::strcat (str.inout (), username);

  return str._retn ();
}

HelloHome_Impl::HelloHome_impl ()
{
}

HelloHome_Impl::~HelloHome_impl ()
{
}

Components::EnterpriseComponent_ptr
HelloHome_Impl::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  return new HelloWorld_Impl ();
}
