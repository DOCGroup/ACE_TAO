// -*- C++ -*-
// $Id$

#include "Sub_Component_exec.h"

namespace CIAO_CoPubSub_Sub_comp_Impl
{

  /**
   * Facet Executor Implementation Class: TestSub_data_listener_exec_i
   */

  TestSub_data_listener_exec_i::TestSub_data_listener_exec_i (
        ::CoPubSub::CCM_Sub_comp_Context_ptr ctx)
    : ciao_context_ (
        ::CoPubSub::CCM_Sub_comp_Context::_duplicate (ctx))
  {
  }

  TestSub_data_listener_exec_i::~TestSub_data_listener_exec_i (void)
  {
  }

  // Operations from ::CoPubSub::Test_conn::Listener

  void
  TestSub_data_listener_exec_i::on_one_data (
    const ::CoPubSub::Test_msg & datum,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ACE_DEBUG ((LM_DEBUG, "TestSub_data_listener_exec_i::on_one_data - "
              "New data received: <%C> - <%d>\n",
              datum.hello.in (), datum.msgId));
  }

  void
  TestSub_data_listener_exec_i::on_many_data (
    const ::CoPubSub::Test_msgSeq & /* data */,
    const ::CCM_DDS::ReadInfoSeq & /* infos */)
  {
    /* Your code here. */
  }

  /**
   * Facet Executor Implementation Class: TestSub_status_exec_i
   */

  TestSub_status_exec_i::TestSub_status_exec_i (
        ::CoPubSub::CCM_Sub_comp_Context_ptr ctx)
    : ciao_context_ (
        ::CoPubSub::CCM_Sub_comp_Context::_duplicate (ctx))
  {
  }

  TestSub_status_exec_i::~TestSub_status_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  TestSub_status_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  TestSub_status_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
    /* Your code here. */
  }

  /**
   * Component Executor Implementation Class: Sub_comp_exec_i
   */

  Sub_comp_exec_i::Sub_comp_exec_i (void){
  }

  Sub_comp_exec_i::~Sub_comp_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  ::CoPubSub::Test_conn::CCM_Listener_ptr
  Sub_comp_exec_i::get_TestSub_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_TestSub_data_listener_.in ()))
      {
        TestSub_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          TestSub_data_listener_exec_i (
            this->ciao_context_.in ()),
            ::CoPubSub::Test_conn::CCM_Listener::_nil ());

          this->ciao_TestSub_data_listener_ = tmp;
      }

    return
      ::CoPubSub::Test_conn::CCM_Listener::_duplicate (
        this->ciao_TestSub_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Sub_comp_exec_i::get_TestSub_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_TestSub_status_.in ()))
      {
        TestSub_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          TestSub_status_exec_i (
            this->ciao_context_.in ()),
            ::CCM_DDS::CCM_PortStatusListener::_nil ());

          this->ciao_TestSub_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_TestSub_status_.in ());
  }

  // Operations from Components::SessionComponent.

  void
  Sub_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::CoPubSub::CCM_Sub_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sub_comp_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Sub_comp_exec_i::ccm_activate (void)
  {
    CCM_DDS::DataListenerControl_var control =
      ciao_context_->get_connection_TestSub_data_control();

    if (! CORBA::is_nil(control.in()))
      {
         try
          {
            control->mode(CCM_DDS::ONE_BY_ONE);
          }
         catch (const CORBA::Exception & ex)
            {
              ex._tao_print_exception("Sub_comp_exec_i::ccm_activate - ERROR: ");
            }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Sub_comp_exec_i::ccm_activate - "
                  "ERROR: Listener control is nil.\n"));
      }
  }

  void
  Sub_comp_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Sub_comp_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" PUBSUB_COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoPubSub_Sub_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sub_comp_exec_i);

    return retval;
  }
}
