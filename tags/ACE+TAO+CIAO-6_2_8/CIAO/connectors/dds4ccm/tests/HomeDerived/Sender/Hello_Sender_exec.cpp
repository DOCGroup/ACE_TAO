// -*- C++ -*-
// $Id$

#include "Hello_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"

namespace CIAO_Hello_Base_comp_Impl
{

  fooSub_data_listener_exec_i::fooSub_data_listener_exec_i (
        ::Hello::CCM_Base_comp_Context_ptr ctx)
    : ciao_context_ (
        ::Hello::CCM_Base_comp_Context::_duplicate (ctx))
  {
  }

  fooSub_data_listener_exec_i::~fooSub_data_listener_exec_i (void)
  {
  }

  // Operations from Listener
  void
  fooSub_data_listener_exec_i::on_one_data (
    const ::DDSHello & /* datum */,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
  }

  void
  fooSub_data_listener_exec_i::on_many_data (
    const ::DDSHelloSeq & /* data */,
    const ::CCM_DDS::ReadInfoSeq & /* infos */)
  {
    /* Your code here. */
  }

  fooSub_status_exec_i::fooSub_status_exec_i (
        ::Hello::CCM_Base_comp_Context_ptr ctx)
    : ciao_context_ (
        ::Hello::CCM_Base_comp_Context::_duplicate (ctx))
  {
  }

  fooSub_status_exec_i::~fooSub_status_exec_i (void)
  {
    //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~fooSub_status_exec_i\n")));
  }

  void
  fooSub_status_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  fooSub_status_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
     //ACE_DEBUG ((LM_ERROR, ACE_TEXT("port status listener::on_sample_lost\n")));
  }

  //============================================================
  // Component Executor Implementation Class: Base_comp_exec_i
  //============================================================

  Base_comp_exec_i::Base_comp_exec_i (void)
    : baseAttr_ (1)
  {
    ACE_DEBUG ((LM_DEBUG, "Base_comp: -2- Base_comp_exec_i::"
                                  "Base_comp_exec_i \n"));
  }

  Base_comp_exec_i::~Base_comp_exec_i (void)
  {
     ACE_DEBUG ((LM_DEBUG, "Base_comp: -10- Base_comp_exec_i::"
                                      "~Base_comp_exec_i \n"));
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  ::CORBA::Short
   Base_comp_exec_i::baseAttr (void)
  {
    return this->baseAttr_;
  }

  void
  Base_comp_exec_i::baseAttr (
    ::CORBA::Short baseAttr)
  {
    this->baseAttr_ = baseAttr;
  }

  ::Foo_conn::CCM_Listener_ptr
   Base_comp_exec_i::get_fooSub_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_fooSub_data_listener_.in ()))
      {
        fooSub_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          fooSub_data_listener_exec_i (
            this->ciao_context_.in ()),
          ::Foo_conn::CCM_Listener::_nil ());

        this->ciao_fooSub_data_listener_ = tmp;
      }

    return
      ::Foo_conn::CCM_Listener::_duplicate (
        this->ciao_fooSub_data_listener_.in ());
  }



  ::CCM_DDS::CCM_PortStatusListener_ptr
   Base_comp_exec_i::get_fooSub_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_fooSub_status_.in ()))
      {
        fooSub_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          fooSub_status_exec_i (
            this->ciao_context_.in ()),
           ::CCM_DDS::CCM_PortStatusListener::_nil ());

        this->ciao_fooSub_status_ = tmp;
      }

    return
       ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_fooSub_status_.in ());
  }

  ::Hello::CCM_Base_obj_ptr
   Base_comp_exec_i::get_baseFacet (void)
  {
    return 0;
  }

  // Operations from Components::SessionComponent.
  void
  Base_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Hello::CCM_Base_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Base_comp_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Base_comp: -3- Base_comp_exec_i::"
                                      "configuration_complete \n"));
  }

  void
  Base_comp_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Base_comp: -4- Base_comp_exec_i::"
                                      "ccm_activate \n"));
  }

  void
  Base_comp_exec_i::ccm_passivate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Base_comp: -8- Base_comp_exec_i::"
                                      "ccm_passivate \n"));
  }

  void
  Base_comp_exec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Base_comp: -9- Base_comp_exec_i::"
                                      "ccm_remove \n"));
  }


  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Base_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Base_comp_exec_i);

    return retval;
  }

}
namespace CIAO_Hello_Base_comp_Impl
{
  //============================================================
  // Home Executor Implementation Class: SenderHome_exec_i
  //============================================================

  Base_compHome_exec_i::Base_compHome_exec_i ()
  {
    ACE_DEBUG ((LM_DEBUG, " Base_compHome: -2- Base_compHome_exec_i::"
                              "Base_compHome_exec_i \n"));
  }

  Base_compHome_exec_i::~Base_compHome_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, " Base_compHome: -11- Base_compHome_exec_i::"
                              "~Base_compHome_exec_i \n"));
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
   Base_compHome_exec_i::create (void)
  {
    ACE_DEBUG ((LM_DEBUG, " Base_compHome: -3- Base_compHome_exec_i::create\n"));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating Base_compHome\n"));
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();


    ACE_NEW_THROW_EX (
      retval,
      CIAO_Hello_Base_comp_Impl::Base_comp_exec_i (),
      ::CORBA::NO_MEMORY ());
    return retval;
  }

  extern "C" HELLO_SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_SenderHome_Impl (void)
  {
     ACE_DEBUG ((LM_DEBUG, " Base_compHome: -1- create_Hello_Base_compHome_Impl \n"));

    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Base_compHome_exec_i ());

    return retval;
  }
}

//============================================================
  // Component Executor Implementation Class: Derived_comp_exec_i
  //============================================================
namespace CIAO_Hello_Derived_comp_Impl
{
  barSub_data_listener_exec_i::barSub_data_listener_exec_i (
         ::Hello::CCM_Derived_comp_Context_ptr ctx)
     : ciao_context_ (
         ::Hello::CCM_Derived_comp_Context::_duplicate (ctx))
  {
  }

  barSub_data_listener_exec_i::~barSub_data_listener_exec_i (void)
  {
  }

  // Operations from Listener
  void
  barSub_data_listener_exec_i::on_one_data (
     const ::DDSHello & /* datum */,
     const ::CCM_DDS::ReadInfo & /* info */)
  {
  }

  void
  barSub_data_listener_exec_i::on_many_data (
    const ::DDSHelloSeq & /* data */,
    const ::CCM_DDS::ReadInfoSeq & /* infos */)
  {
     /* Your code here. */
  }


  barSub_status_exec_i::barSub_status_exec_i (
           ::Hello::CCM_Base_comp_Context_ptr ctx)
       : ciao_context_ (
           ::Hello::CCM_Base_comp_Context::_duplicate (ctx))
  {
  }

  barSub_status_exec_i::~barSub_status_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  barSub_status_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  barSub_status_exec_i::on_sample_lost (
     ::DDS::DataReader_ptr /* the_reader */,
     const ::DDS::SampleLostStatus & /* status */)
  {
  }

  Derived_comp_exec_i::Derived_comp_exec_i (void)
    : baseAttr_ (10),
      derivedAttr_ (100)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -2- Derived_comp_exec_i::"
                                  "Derived_comp_exec_i \n"));
  }

  Derived_comp_exec_i::~Derived_comp_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -10- Derived_comp_exec_i::"
                                      "~Derived_comp_exec_i \n"));
  }

  // Supported operations and attributes.
  ::CORBA::LongLong
  Derived_comp_exec_i::derivedAttr (void)
  {
    return this->derivedAttr_;
  }

  void
  Derived_comp_exec_i::derivedAttr (
  ::CORBA::LongLong derivedAttr)
  {
    this->derivedAttr_ = derivedAttr;
  }


  ::CORBA::Short
  Derived_comp_exec_i::baseAttr (void)
  {
    return this->baseAttr_;
  }

  void
  Derived_comp_exec_i::baseAttr (
     ::CORBA::Short baseAttr)
  {
    this->baseAttr_ = baseAttr;
  }

  // Component attributes and port operations.

  ::Foo_conn::CCM_Listener_ptr
  Derived_comp_exec_i::get_fooSub_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_fooSub_data_listener_.in ()))
      {
      CIAO_Hello_Base_comp_Impl::fooSub_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          CIAO_Hello_Base_comp_Impl::fooSub_data_listener_exec_i (
            this->ciao_context_.in ()),
          ::Foo_conn::CCM_Listener::_nil ());

        this->ciao_fooSub_data_listener_ = tmp;
      }

    return
      ::Foo_conn::CCM_Listener::_duplicate (
        this->ciao_fooSub_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Derived_comp_exec_i::get_fooSub_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_fooSub_status_.in ()))
      {
      CIAO_Hello_Base_comp_Impl::fooSub_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          CIAO_Hello_Base_comp_Impl::fooSub_status_exec_i (
            this->ciao_context_.in ()),
           ::CCM_DDS::CCM_PortStatusListener::_nil ());

        this->ciao_fooSub_status_ = tmp;
      }

    return
       ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_fooSub_status_.in ());
  }

  ::Bar_conn::CCM_Listener_ptr
  Derived_comp_exec_i::get_barSub_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_barSub_data_listener_.in ()))
      {
      barSub_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          barSub_data_listener_exec_i (
            this->ciao_context_.in ()),
          ::Bar_conn::CCM_Listener::_nil ());

        this->ciao_barSub_data_listener_ = tmp;
      }

    return
      ::Bar_conn::CCM_Listener::_duplicate (
        this->ciao_barSub_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Derived_comp_exec_i::get_barSub_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_barSub_status_.in ()))
      {
        barSub_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          barSub_status_exec_i (
            this->ciao_context_.in ()),
           ::CCM_DDS::CCM_PortStatusListener::_nil ());

        this->ciao_barSub_status_ = tmp;
      }

    return
       ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_barSub_status_.in ());
  }

  ::Hello::CCM_Base_obj_ptr
  Derived_comp_exec_i::get_baseFacet (void)
  {
    return 0;
  }

  ::Hello::CCM_Derived_obj_ptr
  Derived_comp_exec_i::get_derivedFacet (void)
  {
    return 0;
  }

  // Operations from Components::SessionComponent.
  void
  Derived_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -3- Derived_comp_exec_i::"
                                          ":set_session_contexti \n"));
    this->ciao_context_ =
      ::Hello::CCM_Derived_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Derived_comp_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -4- Derived_comp_exec_i::"
                                      "configuration_complete \n"));
  }

  void
  Derived_comp_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -5- Derived_comp_exec_i::"
                                      "ccm_activate \n"));
  }

  void
  Derived_comp_exec_i::ccm_passivate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -8- Derived_comp_exec_i::"
                                      "ccm_passivate \n"));
  }

  void
  Derived_comp_exec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_comp: -9- Derived_comp_exec_i::"
                                      "ccm_remove \n"));
  }


  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Derived_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Derived_comp_exec_i);

    return retval;
  }

}
namespace  CIAO_Hello_Derived_comp_Impl
{
  //============================================================
  // Home Executor Implementation Class: Derived_compHome_exec_i
  //============================================================

  Derived_compHome_exec_i::Derived_compHome_exec_i ()
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_compHome: -2- Derived_compHome_exec_i::"
                              "Derived_compHome_exec_i \n"));

    //compare current and original thread
   }

  Derived_compHome_exec_i::~Derived_compHome_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Derived_compHome: -11- Derived_compHome_exec_i::"
                          "~Derived_compHome_exec_i \n"));
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
   Derived_compHome_exec_i::create (void)
  {
     ACE_DEBUG ((LM_DEBUG, "Derived_compHome: -3- Derived_compHome_exec_i::create\n"));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating Derived_compHome\n"));
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();


    ACE_NEW_THROW_EX (
      retval,
      CIAO_Hello_Derived_comp_Impl::Derived_comp_exec_i (),
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" HELLO_SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_Derived_compHome_Impl (void)
  {

     ACE_DEBUG ((LM_DEBUG, "Derived_compHome: -1- create_Hello_Derived_compHome_Impl \n"));

    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();


    ACE_NEW_NORETURN (
      retval,
      Derived_compHome_exec_i ());

   return retval;
  }
}
//============================================================
  // Component Executor Implementation Class: DerivedEmpty_comp_exec_i
  //============================================================
namespace CIAO_Hello_DerivedEmpty_comp_Impl
{
  DerivedEmpty_comp_exec_i::DerivedEmpty_comp_exec_i (void)
    : baseAttr_ (1)
  {
    ACE_DEBUG ((LM_DEBUG, "DerivedEmpty_comp: -2- DerivedEmpty_comp_exec_i::"
                                  "DerivedEmpty_comp_exec_i \n"));
  }

  DerivedEmpty_comp_exec_i::~DerivedEmpty_comp_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "DerivedEmpty_comp: -10- DerivedEmpty_comp_exec_i::"
                                      "~DerivedEmpty_comp_exec_i \n"));
  }

  // Supported operations and attributes.

  ::CORBA::Short
  DerivedEmpty_comp_exec_i::baseAttr (void)
  {
    return this->baseAttr_;
  }

  void
  DerivedEmpty_comp_exec_i::baseAttr (
     ::CORBA::Short baseAttr)
  {
    this->baseAttr_ = baseAttr;
  }

  // Component attributes and port operations.


  ::Foo_conn::CCM_Listener_ptr
  DerivedEmpty_comp_exec_i::get_fooSub_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_fooSub_data_listener_.in ()))
      {
      CIAO_Hello_Base_comp_Impl::fooSub_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          CIAO_Hello_Base_comp_Impl::fooSub_data_listener_exec_i (
            this->ciao_context_.in ()),
          ::Foo_conn::CCM_Listener::_nil ());

        this->ciao_fooSub_data_listener_ = tmp;
      }

    return
      ::Foo_conn::CCM_Listener::_duplicate (
        this->ciao_fooSub_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  DerivedEmpty_comp_exec_i::get_fooSub_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_fooSub_status_.in ()))
       {
      CIAO_Hello_Base_comp_Impl::fooSub_status_exec_i *tmp = 0;
         ACE_NEW_RETURN (
           tmp,
           CIAO_Hello_Base_comp_Impl::fooSub_status_exec_i (
             this->ciao_context_.in ()),
            ::CCM_DDS::CCM_PortStatusListener::_nil ());

         this->ciao_fooSub_status_ = tmp;
       }

    return
        ::CCM_DDS::CCM_PortStatusListener::_duplicate (
         this->ciao_fooSub_status_.in ());
  }


  ::Hello::CCM_Base_obj_ptr
   DerivedEmpty_comp_exec_i::get_baseFacet (void)
   {
     return 0;
   }
  // Operations from Components::SessionComponent.

  void
  DerivedEmpty_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Hello::CCM_DerivedEmpty_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  DerivedEmpty_comp_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, "DerivedEmpty_comp: -3- DerivedEmpty_comp_exec_i::"
                                      "configuration_completei \n"));
    /* Your code here. */
  }

  void
  DerivedEmpty_comp_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "DerivedEmpty_comp: -4- DerivedEmpty_comp_exec_i::"
                                      "ccm_activate \n"));
  }

  void
  DerivedEmpty_comp_exec_i::ccm_passivate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "DerivedEmpty_comp: -8- DerivedEmpty_comp_exec_i::"
                                      "ccm_passivate \n"));
  }

  void
  DerivedEmpty_comp_exec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_DEBUG, "DerivedEmpty_comp: -9- DerivedEmpty_comp_exec_i::"
                                      "ccm_remove \n"));
  }


  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_DerivedEmpty_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      DerivedEmpty_comp_exec_i);

    return retval;
  }

}
