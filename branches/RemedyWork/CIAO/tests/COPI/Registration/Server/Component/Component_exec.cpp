// -*- C++ -*-
// $Id$

#include "Component_exec.h"

#define LOW_PRIORITY     25
#define DEFAULT_PRIORITY 50
#define HIGH_PRIORITY    100

const char * low_priority_name = "LowPriorityServerCOPI";
const char * def_priority_name = "DefaultPriorityServerCOPI";
const char * high_priority_name = "HighPriorityServerCOPI";

//============================================================
// ServerContainerInterceptor
//============================================================
ServerContainerInterceptor::ServerContainerInterceptor (CORBA::UShort priority,
                                                        const char * name)
  : priority_(priority),
    name_ (name)
{
}

ServerContainerInterceptor::~ServerContainerInterceptor ()
{
}

void
ServerContainerInterceptor::receive_request_service_contexts (
  ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr )
{
}

void
ServerContainerInterceptor::receive_request (
  ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr )
{
}

void
ServerContainerInterceptor::send_reply (
  ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr )
{
}

void
ServerContainerInterceptor::send_exception (
  ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr )
{
}

void
ServerContainerInterceptor::send_other (
  ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr )
{
}

char *
ServerContainerInterceptor::name ()
{
  return const_cast <char *> (this->name_);
}

::CORBA::UShort
ServerContainerInterceptor::priority (void)
{
  return this->priority_;
}

void
ServerContainerInterceptor::priority (::CORBA::UShort priority)
{
  this->priority_ = priority;
}

void
ServerContainerInterceptor::destroy ()
{
}

void
ServerContainerInterceptor::set_slot_id (
  ::PortableInterceptor::SlotId /*slot_id*/)
{
}

namespace CIAO_Registration_Server_Registration_Impl
{
  //============================================================
  // Component Executor Implementation Class: Server_Registration_exec_i
  //============================================================
  Server_Registration_exec_i::Server_Registration_exec_i (void)
  {
  }

  Server_Registration_exec_i::~Server_Registration_exec_i (void)
  {
  }

  // Supported operations and attributes.
  //============================================================
  // test_registration
  //============================================================
  int
  Server_Registration_exec_i::test_registration (
    ::CIAO::ServerContainerInterceptorRegistration_Impl * pi_cr)
  {
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_registration\n"));
    ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var ci;
    ACE_NEW_THROW_EX (ci,
                      ServerContainerInterceptor (DEFAULT_PRIORITY,
                                                  def_priority_name),
                      ::CORBA::NO_MEMORY ());
    if (::CORBA::is_nil (ci.in ()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_registration - "
                            "Error: Unable to create Server Interceptor\n"),
                            1);
      }
    ::Components::Cookie * ck = pi_cr->register_server_interceptor(ci.in ());
    if (!ck)
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_registration - "
                            "Error: Unable to register Server Interceptor\n"),
                            1);
      }
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_registration - "
                          "Test unregister_server_interceptor.\n"));
    ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var unregistered_ci;
    try
      {
        unregistered_ci = pi_cr->unregister_server_interceptor(ck);
        if (::CORBA::is_nil (unregistered_ci))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                              "Server_Registration_exec_i::test_registration - "
                              "Error: unregister_server_interceptor return a NIL object\n"),
                              1);
          }
      }
    catch (const ::Components::CCMException &)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Server_Registration_exec_i::test_registration - "
                          "Error: unexpected CCMException caught\n"),
                          1);
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Server_Registration_exec_i::test_registration");
        return 1;
      }
    catch (...)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Server_Registration_exec_i::test_registration - "
                          "Error: unknown exception caught\n"),
                          1);
      }
    if (unregistered_ci->priority() != ci->priority())
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Server_Registration_exec_i::test_registration - "
                          "Error: Unregistered Server Interceptor seems unequal to "
                          "registered Server Interceptor\n"),
                          1);
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_registration - "
                              "Unregister succeeded.\n"));
      }
    return 0;
  }

  //============================================================
  // test_exception
  //============================================================
  int
  Server_Registration_exec_i::test_exception(
    ::CIAO::ServerContainerInterceptorRegistration_Impl * pi_cr)
  {
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_exception\n"));

    int ret = 0;
    ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var
      ci1, ci2;
    ACE_NEW_THROW_EX (ci1,
                      ServerContainerInterceptor (DEFAULT_PRIORITY,
                                                  def_priority_name),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (ci2,
                      ServerContainerInterceptor (HIGH_PRIORITY,
                                                  high_priority_name),
                      ::CORBA::NO_MEMORY ());

     if (::CORBA::is_nil (ci1.in ()) || ::CORBA::is_nil (ci2.in ()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_exception - "
                            "Error: Unable to create Server Interceptors\n"),
                            1);
      }
    ::Components::Cookie * ck1 = pi_cr->register_server_interceptor(ci1.in ());
    ::Components::Cookie * ck2 = pi_cr->register_server_interceptor(ci2.in ());
    if (!ck1 || !ck2)
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_exception - "
                            "Error: Unable to register Server Interceptors\n"),
                            1);
      }

    ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var unregistered_ci;
    try
      {
        ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_exception - "
                              "Unregister cookie 1 (%@).\n",
                              ck1));
        unregistered_ci = pi_cr->unregister_server_interceptor(ck1);
        if (::CORBA::is_nil (unregistered_ci))
          {
            ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_exception - "
                                  "Error: unregister_server_interceptor return a NIL object\n"));
            ++ret;
          }
        //unregister ck1 again.
        ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_exception - "
                              "Unregister cookie 1 (%@) again.\n",
                              ck1));
        unregistered_ci = pi_cr->unregister_server_interceptor(ck1);
        ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_exception - "
                              "Error: No exception thrown while unregistering ck1 "
                              "a the second time\n"));
        ++ret;
      }
    catch (const ::Components::ContainerPortableInterceptor::InvalidRegistration &)
      {
        ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_exception - "
                              "Correct exception caught\n"));
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Server_Registration_exec_i::test_exception");
        ++ret;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_exception - "
                              "Error: unknown exception caught\n"));
        ++ret;
      }
    //unregister ck2
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_exception - "
                          "Unregister cookie 2 (%@).\n",
                          ck2));
    unregistered_ci = pi_cr->unregister_server_interceptor(ck2);
    if (::CORBA::is_nil (unregistered_ci))
      {
        ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_exception - "
                              "Error: unregister_server_interceptor return a NIL object\n"));
        ++ret;
      }
    return ret;
  }

  //============================================================
  // test_priority
  //============================================================
  int
  Server_Registration_exec_i::test_priority(
    ::CIAO::ServerContainerInterceptorRegistration_Impl * pi_cr)
  {
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_priority\n"));

    int ret = 0;
    ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var
      ci_low, ci_def, ci_high;
    ACE_NEW_THROW_EX (ci_low,
                      ServerContainerInterceptor (LOW_PRIORITY,
                                                  low_priority_name),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (ci_def,
                      ServerContainerInterceptor (DEFAULT_PRIORITY,
                                                  def_priority_name),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (ci_high,
                      ServerContainerInterceptor (HIGH_PRIORITY,
                                                  high_priority_name),
                      ::CORBA::NO_MEMORY ());

     if (::CORBA::is_nil (ci_low.in ()) ||
         ::CORBA::is_nil (ci_def.in ()) ||
         ::CORBA::is_nil (ci_high.in ()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_priority - "
                            "Error: Unable to create Server Interceptors\n"),
                            1);
      }
    ::Components::Cookie * ck_def = pi_cr->register_server_interceptor(ci_def.in ());
    ::Components::Cookie * ck_low = pi_cr->register_server_interceptor(ci_low.in ());
    ::Components::Cookie * ck_high = pi_cr->register_server_interceptor(ci_high.in ());
    if (!ck_low || !ck_def || ! ck_high)
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_priority - "
                            "Error: Unable to register Server Interceptors\n"),
                            1);
      }
    //now test the priority
    try
      {
        if (pi_cr->size () != 3)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                            "Server_Registration_exec_i::test_priority - "
                            "Error: Unexpected size of registered Server "
                            "interceptors. expected <3> - actual size <%d>\n",
                            pi_cr->size ()),
                            1);
          }
        for (size_t i = 0; i < pi_cr->size (); ++i)
          {
            ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var
              ict = pi_cr->interceptor(i);
            ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_priority - "
                                  "Found registered COPI : "
                                  "name <%s> - priority <%d>\n",
                                  ict->name (), ict->priority ()));
          }
        size_t it = 0;
        ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var
          ict = pi_cr->interceptor(it);
        if (! ::CORBA::is_nil (ict.in ()))
          {
            if (ict->priority() != HIGH_PRIORITY)
              {
                ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_priority - "
                                      "Error: Unexpected priority received. "
                                      "expected <%d> - received <%d>\n",
                                      HIGH_PRIORITY,
                                      ict->priority()));
                ++ret;
              }
          }
        it = 1;
        ict = pi_cr->interceptor(it);
        if (! ::CORBA::is_nil (ict.in ()))
          {
            if (ict->priority() != DEFAULT_PRIORITY)
              {
                ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_priority - "
                                      "Error: Unexpected priority received. "
                                      "expected <%d> - received <%d>\n",
                                      DEFAULT_PRIORITY,
                                      ict->priority()));
                ++ret;
              }
          }
        it = 2;
        ict = pi_cr->interceptor(it);
        if (! ::CORBA::is_nil (ict.in ()))
          {
            if (ict->priority() != LOW_PRIORITY)
              {
                ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_priority - "
                                      "Error: Unexpected priority received. "
                                      "expected <%d> - received <%d>\n",
                                      LOW_PRIORITY,
                                      ict->priority()));
                ++ret;
              }
          }
       }
     catch (...)
      {
      }

    try
      {
        ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::test_priority - "
                              "Start unregistring\n"));
        ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_var
          unregistered_low, unregistered_def, unregistered_high;
        unregistered_low = pi_cr->unregister_server_interceptor(ck_low);
        unregistered_def = pi_cr->unregister_server_interceptor(ck_def);
        unregistered_high = pi_cr->unregister_server_interceptor(ck_high);
        if (::CORBA::is_nil (unregistered_low.in ()) ||
            ::CORBA::is_nil (unregistered_def.in ()) ||
            ::CORBA::is_nil (unregistered_high.in ()) )
          {
            ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_priority - "
                                  "Error: unregister_server_interceptor return "
                                  "a NIL object\n"));
            ++ret;
          }
      }
    catch (const ::Components::ContainerPortableInterceptor::InvalidRegistration &)
      {
        ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_priority - "
                              "Error: caught InvalidRegistration exception\n"));
        ++ret;
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Server_Registration_exec_i::test_priority");
        ++ret;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::test_priority - "
                              "Error: unknown exception caught\n"));
        ++ret;
      }
    return ret;
  }

  int
  Server_Registration_exec_i::start_test (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::start_test\n"));
    int ret = 0;

    try
      {
        ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_var
          pi_cr = this->ciao_context_->get_server_interceptor_registration ();

        if (::CORBA::is_nil (pi_cr.in ()))
          {
              ACE_ERROR_RETURN ((LM_ERROR,
                                "Server_Registration_exec_i::start_test - "
                                "Error: Server COPI is nil\n"),
                                1);
          }
        ::CIAO::ServerContainerInterceptorRegistration_Impl * impl =
          dynamic_cast < ::CIAO::ServerContainerInterceptorRegistration_Impl * >
            (pi_cr.in ());
        ret += this->test_registration (impl);
        ret += this->test_exception (impl);
        ret += this->test_priority (impl);
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Server_Registration_exec_i::start_test");
        return 1;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Server_Registration_exec_i::start_test - "
                              "Error: Unknown exception caught\n"));
        return 1;
      }
    return ret;
  }

  // Operations from Components::ExtensionComponent.
  void
  Server_Registration_exec_i::set_extension_context (
    ::Components::ExtensionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::set_extension_context\n"));
    this->ciao_context_ =
      ::Registration::CCM_Server_Registration_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    int errors = start_test ();
    if (errors != 0)
      {
        ACE_ERROR ((LM_ERROR, "Server Interceptor Registration test - "
                              "Error: %d error(s) found during test.\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Server Interceptor Registration test - "
                              "No problems found during test.\n"));
      }
  }

  void
  Server_Registration_exec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Server_Registration_exec_i::ccm_remove - GOING DOWN\n"));
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Registration_Server_Registration_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Server_Registration_exec_i);

    return retval;
  }
}
