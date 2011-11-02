// -*- C++ -*-
// $Id$

#include "Sender_exec.h"

namespace CIAO_Sender_Impl
{
  /**
   * Facet Executor Implementation Class: hello_exec_i
   */
  hello_exec_i::hello_exec_i (::CCM_Sender_Context_ptr ctx)
    : ciao_context_ (::CCM_Sender_Context::_duplicate (ctx))
  {
  }

  hello_exec_i::~hello_exec_i (void)
  {
  }

  // Operations from ::Hello
  void
  hello_exec_i::set_point (const pointer_id &p, const char * strat)
   {
     CORBA::Boolean coll = false;

#if !defined (CCM_DIRECTCOLL)
     ACE_DEBUG ((LM_DEBUG,
                  "SENDER: hello_exec_i::set_point pointer"
                  "Strategy desired <%C>, but no collocation "
                  "because CIAO not build with  "
                  "ccm_diect_collocation = 1.\n",
                  start, (void *)&p));
#else
     if ((ACE_OS::strcmp("direct", strat) == 0) ||
         (ACE_OS::strcmp("best", strat) == 0))
       coll = true;
     else if (ACE_OS::strcmp("no_collocation", strat) == 0)
       coll = false;
     else
       ACE_ERROR ((LM_DEBUG, "Error: hello_exec_i::set_point - "
                             "Unexpected strategy <%C> \n",
                              strat));
#endif
     ACE_DEBUG ((LM_DEBUG,
                  "SENDER: hello_exec_i::set_point pointer <%@>\n",
                  (void *)&p));

     CORBA::Long ptr_nmb = (long)&p;
     if (coll)
       {

         // In case of collocation pointer p is still the same pointer
         // as at the point where poiner p was generated.
         // Direct or thru_poa collocation, but thru_poa collocation is disabled,
         // so remains direct .
         if (ptr_nmb == p.point)
           {
             ACE_DEBUG ((LM_DEBUG,
                 "hello_exec_i::set_point - current pointer  <%u> "
                 " is same as starting pointer <%u>,"
                 " direct collocation! \n",
                  ptr_nmb, p.point));
           }
         else
           {
             ACE_ERROR ((LM_ERROR,
                 "Error: hello_exec_i::set_point - current pointer  <%u> "
                 " is not the same as starting pointer <%u>"
                 ", No Collocation\n",
                 ptr_nmb, p.point));
           }
       }
     else
       {
         if (ptr_nmb == p.point)
           {
             ACE_ERROR ((
                 LM_DEBUG,
                 "Error: hello_exec_i::set_point - current pointer  <%u> "
                 " is the same as starting pointer <%u>"
                 ", Unexpected collocation\n",
                 ptr_nmb, p.point));
           }
         else
           {
             ACE_DEBUG ((LM_DEBUG,
                 "hello_exec_i::set_point - current pointer  <%u>"
                 "  is not the same as starting pointer <%u>,"
                 " No collocation, as expected ! \n",
                 ptr_nmb, p.point));
           }
       }
   }

  /**
   * Component Executor Implementation Class: Sender_exec_i
   */
  Sender_exec_i::Sender_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::Sender_exec_i \n "));
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }


  // Component attributes and port operations.
  ::CCM_Hello_ptr
  Sender_exec_i::get_hello (void)
  {
    if ( ::CORBA::is_nil (this->ciao_hello_.in ()))
      {
        hello_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          hello_exec_i (
            this->ciao_context_.in ()),
            ::CCM_Hello::_nil ());

          this->ciao_hello_ = tmp;
      }

    return
      ::CCM_Hello::_duplicate (
        this->ciao_hello_.in ());
  }


  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::set_session_context \n "));

    this->ciao_context_ =
      ::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::configuration_complete \n "));

  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_activate \n "));
    /// Start the timer .
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_remove \n "));
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
