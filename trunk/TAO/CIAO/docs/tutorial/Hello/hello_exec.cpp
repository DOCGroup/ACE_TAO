// $Id$

#include "hello_exec.h"

#include "ace/OS_NS_string.h"

namespace CIDL_hello_example
{
  HelloWorld_Impl::HelloWorld_Impl ()
  {
  }

  HelloWorld_Impl::~HelloWorld_Impl ()
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


  void
  HelloWorld_Impl::set_session_context (
      Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "HelloWorld_Impl::set_session_context\n"));

    this->context_ =
      HelloWorld_Exec_Context::_narrow (ctx
                                                      ACE_ENV_ARG_PARAMETER);

    ACE_CHECK;

    if (CORBA::is_nil (this->context_.in ()))
      ACE_THROW (CORBA::INTERNAL ());
    // Urm, we actually discard exceptions thown from this operation.
  }

  void
  HelloWorld_Impl::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
  }

  void
  HelloWorld_Impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
  }

  void
  HelloWorld_Impl::ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
  }

  void
  HelloWorld_Impl::ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
  }

  void
  HelloWorld_Impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
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
}

