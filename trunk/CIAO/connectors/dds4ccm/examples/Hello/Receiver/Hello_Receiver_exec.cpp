// -*- C++ -*-
//
// $Id$

#include "Hello_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Date_Time.h"

namespace CIAO_Hello_Receiver_Impl
{
  //============================================================
  // Facet Executor Implementation Class: DDSHello_RawListener_exec_i
  //============================================================

  DDSHello_RawListener_exec_i::DDSHello_RawListener_exec_i (Atomic_ULong &received,
                                                            const ACE_CString &name)
      : received_ (received),
        name_ (name)
  {
  }

  DDSHello_RawListener_exec_i::~DDSHello_RawListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::DDSHello_RawListener

  void
  DDSHello_RawListener_exec_i::on_data (
    const DDSHello & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {

    ++this->received_;
    ACE_CString rec (an_instance.hello.in ());
    ACE_Date_Time now;
    int sec_rec = ACE_OS::atoi (rec.substr (0, 2).c_str() );
    if (sec_rec > 0)
      {
        int usec_rec = ACE_OS::atoi (rec.substr (3, 6).c_str ());
        if (sec_rec != now.second ())
          usec_rec += 10000000;
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("<%s> received <%s> - <%d>. difference <%d>\n"),
                    this->name_.c_str (),
                    an_instance.hello.in (),
                    an_instance.iterator,
                    now.microsec () - usec_rec));
      }
    else
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT("<%s> received <%s> - <%d>.\n"),
      this->name_.c_str (),
      an_instance.hello.in (),
      an_instance.iterator));
    }
/*
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DDSHello_RawListener: ")
            ACE_TEXT ("received hello for <%s> - iterator <%d>\n"),
            an_instance.hello.in (),
            an_instance.iterator));
*/
  }
  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================

  PortStatusListener_exec_i::PortStatusListener_exec_i (Atomic_ULong &lost)
    : lost_ (lost)
  {
  }

  PortStatusListener_exec_i::~PortStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  PortStatusListener_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
    ++this->lost_;
    CIAO_DEBUG ((LM_ERROR, ACE_TEXT("port status listener::on_sample_lost\n")));
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iDDSHello_RawListener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : expected_ (100),
      received_ (0),
      lost_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::CORBA::ULong
  Receiver_exec_i::expected_samples (void)
  {
    return this->expected_;
  }

  void
  Receiver_exec_i::expected_samples (::CORBA::ULong expected_samples)
  {
    this->expected_ = expected_samples;
  }

  char *
  Receiver_exec_i::name (void)
  {
    return CORBA::string_dup (this->name_.c_str ());
  }

  void
  Receiver_exec_i::name (const char *name)
  {
    this->name_ = name;
  }

  // Port operations.
  ::CCM_DDS::CCM_DDSHello_RawListener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new DDSHello RAW listener\n")));
    return new DDSHello_RawListener_exec_i (this->received_, this->name_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i (this->lost_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::ListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->enabled (true);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    CIAO_DEBUG ((LM_INFO, "Receiver_exec_i received %u messages and lost %u messages\n",
                this->received_.value (),
                this->lost_.value ()));

    if (this->received_ != this->expected_)
      {
        CIAO_ERROR ((LM_EMERGENCY, "Receiver_exec_i: Warning: Expected to receive %u messages, actually got %u\n",
        this->expected_, this->received_.value ()));
      }
  }

  extern "C" HELLO_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}




