// -*- C++ -*-
// $Id$

#include "Component_exec.h"

#define LOW_PRIORITY     25
#define DEFAULT_PRIORITY 50
#define HIGH_PRIORITY    100

const char * low_priority_name = "LowPriorityServantCOPI";
const char * def_priority_name = "DefaultPriorityServantCOPI";
const char * high_priority_name = "HighPriorityServantCOPI";

//============================================================
// ServantContainerInterceptor
//============================================================
ServantContainerInterceptor::ServantContainerInterceptor (CORBA::UShort priority,
                                                        const char * name)
  : priority_(priority),
    name_ (name)
{
}

ServantContainerInterceptor::~ServantContainerInterceptor ()
{
}

void
ServantContainerInterceptor::servant_receive_request (
  ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr ,
  ::CORBA::Boolean_out )
{
}

void
ServantContainerInterceptor::servant_send_reply (
  ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr ,
  ::CORBA::Boolean_out )
{
}

void
ServantContainerInterceptor::servant_send_exception (
  ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr ,
  ::CORBA::Boolean_out )
{
}

void
ServantContainerInterceptor::servant_send_other (
  ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr )
{
}

char *
ServantContainerInterceptor::name ()
{
  return const_cast <char *> (this->name_);
}

::CORBA::UShort
ServantContainerInterceptor::priority (void)
{
  return this->priority_;
}

void
ServantContainerInterceptor::priority (::CORBA::UShort priority)
{
  this->priority_ = priority;
}

void
ServantContainerInterceptor::destroy ()
{
}

void
ServantContainerInterceptor::set_slot_id (
  ::PortableInterceptor::SlotId /*slot_id*/)
{
}

namespace CIAO_Registration_Servant_Registration_Impl
{
  //============================================================
  // Component Executor Implementation Class: Servant_Registration_exec_i
  //============================================================
  Servant_Registration_exec_i::Servant_Registration_exec_i (void)
  {
  }

  Servant_Registration_exec_i::~Servant_Registration_exec_i (void)
  {
  }

  // Supported operations and attributes.
  //============================================================
  // test_registration
  //============================================================
  int
  Servant_Registration_exec_i::test_registration (
    ::CIAO::ServantContainerInterceptorRegistration_Impl * pi_cr)
  {
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_registration\n"));
    ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var ci;
    ACE_NEW_THROW_EX (ci,
                      ServantContainerInterceptor (DEFAULT_PRIORITY,
                                                  def_priority_name),
                      ::CORBA::NO_MEMORY ());
    if (::CORBA::is_nil (ci.in ()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_registration - "
                            "Error: Unable to create Servant Interceptor\n"),
                            1);
      }
    ::Components::Cookie * ck = pi_cr->register_servant_interceptor(ci.in ());
    if (!ck)
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_registration - "
                            "Error: Unable to register Servant Interceptor\n"),
                            1);
      }
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_registration - "
                          "Test unregister_servant_interceptor.\n"));
    ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var unregistered_ci;
    try
      {
        unregistered_ci = pi_cr->unregister_servant_interceptor(ck);
        if (::CORBA::is_nil (unregistered_ci))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                              "Servant_Registration_exec_i::test_registration - "
                              "Error: unregister_servant_interceptor return a NIL object\n"),
                              1);
          }
      }
    catch (const ::Components::CCMException &)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Servant_Registration_exec_i::test_registration - "
                          "Error: unexpected CCMException caught\n"),
                          1);
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Servant_Registration_exec_i::test_registration");
        return 1;
      }
    catch (...)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Servant_Registration_exec_i::test_registration - "
                          "Error: unknown exception caught\n"),
                          1);
      }
    if (unregistered_ci->priority() != ci->priority())
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Servant_Registration_exec_i::test_registration - "
                          "Error: Unregistered Servant Interceptor seems unequal to "
                          "registered Servant Interceptor\n"),
                          1);
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_registration - "
                              "Unregister succeeded.\n"));
      }
    return 0;
  }

  //============================================================
  // test_exception
  //============================================================
  int
  Servant_Registration_exec_i::test_exception(
    ::CIAO::ServantContainerInterceptorRegistration_Impl * pi_cr)
  {
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_exception\n"));

    int ret = 0;
    ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var
      ci1, ci2;
    ACE_NEW_THROW_EX (ci1,
                      ServantContainerInterceptor (DEFAULT_PRIORITY,
                                                  def_priority_name),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (ci2,
                      ServantContainerInterceptor (HIGH_PRIORITY,
                                                  high_priority_name),
                      ::CORBA::NO_MEMORY ());

     if (::CORBA::is_nil (ci1.in ()) || ::CORBA::is_nil (ci2.in ()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_exception - "
                            "Error: Unable to create Servant Interceptors\n"),
                            1);
      }
    ::Components::Cookie * ck1 = pi_cr->register_servant_interceptor(ci1.in ());
    ::Components::Cookie * ck2 = pi_cr->register_servant_interceptor(ci2.in ());
    if (!ck1 || !ck2)
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_exception - "
                            "Error: Unable to register Servant Interceptors\n"),
                            1);
      }

    ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var unregistered_ci;
    try
      {
        ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_exception - "
                              "Unregister cookie 1 (%@).\n",
                              ck1));
        unregistered_ci = pi_cr->unregister_servant_interceptor(ck1);
        if (::CORBA::is_nil (unregistered_ci))
          {
            ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_exception - "
                                  "Error: unregister_servant_interceptor return a NIL object\n"));
            ++ret;
          }
        //unregister ck1 again.
        ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_exception - "
                              "Unregister cookie 1 (%@) again.\n",
                              ck1));
        unregistered_ci = pi_cr->unregister_servant_interceptor(ck1);
        ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_exception - "
                              "Error: No exception thrown while unregistering ck1 "
                              "a the second time\n"));
        ++ret;
      }
    catch (const ::Components::ContainerPortableInterceptor::InvalidRegistration &)
      {
        ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_exception - "
                              "Correct exception caught\n"));
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Servant_Registration_exec_i::test_exception");
        ++ret;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_exception - "
                              "Error: unknown exception caught\n"));
        ++ret;
      }
    //unregister ck2
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_exception - "
                          "Unregister cookie 2 (%@).\n",
                          ck2));
    unregistered_ci = pi_cr->unregister_servant_interceptor(ck2);
    if (::CORBA::is_nil (unregistered_ci))
      {
        ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_exception - "
                              "Error: unregister_servant_interceptor return a NIL object\n"));
        ++ret;
      }
    return ret;
  }

  //============================================================
  // test_priority
  //============================================================
  int
  Servant_Registration_exec_i::test_priority(
    ::CIAO::ServantContainerInterceptorRegistration_Impl * pi_cr)
  {
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_priority\n"));

    int ret = 0;
    ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var
      ci_low, ci_def, ci_high;
    ACE_NEW_THROW_EX (ci_low,
                      ServantContainerInterceptor (LOW_PRIORITY,
                                                  low_priority_name),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (ci_def,
                      ServantContainerInterceptor (DEFAULT_PRIORITY,
                                                  def_priority_name),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (ci_high,
                      ServantContainerInterceptor (HIGH_PRIORITY,
                                                  high_priority_name),
                      ::CORBA::NO_MEMORY ());

     if (::CORBA::is_nil (ci_low.in ()) ||
         ::CORBA::is_nil (ci_def.in ()) ||
         ::CORBA::is_nil (ci_high.in ()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_priority - "
                            "Error: Unable to create Servant Interceptors\n"),
                            1);
      }
    ::Components::Cookie * ck_def = pi_cr->register_servant_interceptor(ci_def.in ());
    ::Components::Cookie * ck_low = pi_cr->register_servant_interceptor(ci_low.in ());
    ::Components::Cookie * ck_high = pi_cr->register_servant_interceptor(ci_high.in ());
    if (!ck_low || !ck_def || ! ck_high)
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_priority - "
                            "Error: Unable to register Servant Interceptors\n"),
                            1);
      }
    //now test the priority
    try
      {
        if (pi_cr->size () != 3)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                            "Servant_Registration_exec_i::test_priority - "
                            "Error: Unexpected size of registered Servant "
                            "interceptors. expected <3> - actual size <%d>\n",
                            pi_cr->size ()),
                            1);
          }
        for (size_t i = 0; i < pi_cr->size (); ++i)
          {
            ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var
              ict = pi_cr->interceptor(i);
            ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_priority - "
                                  "Found registered COPI : "
                                  "name <%s> - priority <%d>\n",
                                  ict->name (), ict->priority ()));
          }
        size_t it = 0;
        ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var
          ict = pi_cr->interceptor(it);
        if (! ::CORBA::is_nil (ict.in ()))
          {
            if (ict->priority() != HIGH_PRIORITY)
              {
                ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_priority - "
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
                ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_priority - "
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
                ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_priority - "
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
        ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::test_priority - "
                              "Start unregistring\n"));
        ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_var
          unregistered_low, unregistered_def, unregistered_high;
        unregistered_low = pi_cr->unregister_servant_interceptor(ck_low);
        unregistered_def = pi_cr->unregister_servant_interceptor(ck_def);
        unregistered_high = pi_cr->unregister_servant_interceptor(ck_high);
        if (::CORBA::is_nil (unregistered_low.in ()) ||
            ::CORBA::is_nil (unregistered_def.in ()) ||
            ::CORBA::is_nil (unregistered_high.in ()) )
          {
            ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_priority - "
                                  "Error: unregister_servant_interceptor return "
                                  "a NIL object\n"));
            ++ret;
          }
      }
    catch (const ::Components::ContainerPortableInterceptor::InvalidRegistration &)
      {
        ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_priority - "
                              "Error: caught InvalidRegistration exception\n"));
        ++ret;
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Servant_Registration_exec_i::test_priority");
        ++ret;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::test_priority - "
                              "Error: unknown exception caught\n"));
        ++ret;
      }
    return ret;
  }

  int
  Servant_Registration_exec_i::start_test (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::start_test\n"));
    int ret = 0;

    try
      {
        ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_var
          pi_cr = this->ciao_context_->get_servant_interceptor_registration ();

        if (::CORBA::is_nil (pi_cr.in ()))
          {
              ACE_ERROR_RETURN ((LM_ERROR,
                                "Servant_Registration_exec_i::start_test - "
                                "Error: Servant COPI is nil\n"),
                                1);
          }
        ::CIAO::ServantContainerInterceptorRegistration_Impl * impl =
          dynamic_cast < ::CIAO::ServantContainerInterceptorRegistration_Impl * >
            (pi_cr.in ());
        ret += this->test_registration (impl);
        ret += this->test_exception (impl);
        ret += this->test_priority (impl);
      }
    catch (const ::CORBA::Exception &e)
      {
        e._tao_print_exception("Servant_Registration_exec_i::start_test");
        return 1;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Servant_Registration_exec_i::start_test - "
                              "Error: Unknown exception caught\n"));
        return 1;
      }
    return ret;
  }

  // Operations from Components::ExtensionComponent.
  void
  Servant_Registration_exec_i::set_extension_context (
    ::Components::ExtensionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::set_extension_context\n"));
    this->ciao_context_ =
      ::Registration::CCM_Servant_Registration_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    int errors = start_test ();
    if (errors != 0)
      {
        ACE_ERROR ((LM_ERROR, "Servant Interceptor Registration test - "
                              "Error: %d error(s) found during test.\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Servant Interceptor Registration test - "
                              "No problems found during test.\n"));
      }
  }

  void
  Servant_Registration_exec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Servant_Registration_exec_i::ccm_remove - GOING DOWN\n"));
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Registration_Servant_Registration_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Servant_Registration_exec_i);

    return retval;
  }
}
