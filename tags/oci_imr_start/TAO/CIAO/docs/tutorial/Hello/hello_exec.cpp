// $Id$

#include "hello_exec.h"

HelloWorld_Impl::HelloWorld_Impl ()
{
}

HelloWorld_Impl::~HelloWorld_Impl
 ()
{
}

char *
HelloWorld_Impl::sayhello (const char *username
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  static const char *fmt = "Hello World message for ";

  CORBA::String_var str =
    CORBA::string_alloc (ACE_OS::strlen (fmt) +
                         ACE_OS::strlen (username) +
                         1);

  ACE_OS::strcpy (str.inout (), fmt);
  ACE_OS::strcat (str.inout (), username);

  return str._retn ();
}

HelloHome_Impl::HelloHome_Impl ()
{
}

HelloHome_Impl::~HelloHome_Impl ()
{
}

Components::EnterpriseComponent_ptr
HelloHome_Impl::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((Components::CCMException,
                   CORBA::SystemException))
{
  return new HelloWorld_Impl ();
}

extern "C" HELLO_EXEC_Export ::Components::HomeExecutorBase_ptr
createHelloHome_Impl (void)
{
  return new HelloHome_Impl ();
}
