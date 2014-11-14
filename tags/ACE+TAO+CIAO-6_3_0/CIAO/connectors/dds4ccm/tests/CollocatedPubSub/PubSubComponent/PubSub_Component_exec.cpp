// -*- C++ -*-
// $Id$

#include "PubSub_Component_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_CoPubSub_PubSub_comp_Impl
{


  /**
   * WriteHandler
   */
  WriteHandler::WriteHandler (PubSub_comp_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteHandler::handle_timeout (const ACE_Time_Value &, const void *)
  {
    return this->callback_.tick ();
  }

  /**
   * Facet Executor Implementation Class: TestSub_data_listener_exec_i
   */

  TestSub_data_listener_exec_i::TestSub_data_listener_exec_i (
        ::CoPubSub::CCM_PubSub_comp_Context_ptr ctx)
    : ciao_context_ (
        ::CoPubSub::CCM_PubSub_comp_Context::_duplicate (ctx))
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
        ::CoPubSub::CCM_PubSub_comp_Context_ptr ctx)
    : ciao_context_ (
        ::CoPubSub::CCM_PubSub_comp_Context::_duplicate (ctx))
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
   * Component Executor Implementation Class: PubSub_comp_exec_i
   */

  PubSub_comp_exec_i::PubSub_comp_exec_i (void)
    : count_(-3)
  {
    ticker_ = new WriteHandler (*this);
  }

  PubSub_comp_exec_i::~PubSub_comp_exec_i (void)
  {
    delete this->ticker_;
  }

  // Supported operations and attributes.
  ACE_Reactor*
  PubSub_comp_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->ciao_context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  int
  PubSub_comp_exec_i::tick (void)
  {
    CoPubSub::Test_msg sample;
    sample.hello = CORBA::string_dup("CoPubSub Test Message");
    sample.msgId = ++this->count_;

    // Reference to the writer interface for the DDS4CCM connector for msg type
    ::CoPubSub::Test_conn::Writer_var writer;

    writer = ciao_context_->get_connection_TestPub_data();

    if (CORBA::is_nil(writer.in()))
      {
        ACE_ERROR ((LM_ERROR, "PubSub_comp_exec_i::tick - Writer seems nil.\n"));
        return -1; // Don't reschedule timer
      }

    try
      {
          writer->write_one(sample, DDS::HANDLE_NIL);
          ACE_DEBUG ((LM_DEBUG, "PubSub_comp_exec_i::tick - "
                    "Sample has been written: <%C> - <%d>\n",
                    sample.hello.in (), sample.msgId));
      }
    catch (const CCM_DDS::InternalError &) // catch "normal" exception
      {
        ACE_ERROR ((LM_ERROR, "PubSub_comp_exec_i::tick - "
                  "ERROR: caught InternalError.\n"));

        return -1; // Don't reschedule timer
      }
    catch (const CORBA::Exception & ex)
      {
        ex._tao_print_exception("PubSub_comp_exec_i::tick - ERROR: ");
        return -1; // Don't reschedule timer
      }
    catch (...) // catch everything else. If this is called something is wrong
      {
        ACE_ERROR ((LM_ERROR, "PubSub_comp_exec_i::tick - "
                  "ERROR: caught unknown exception.\n"));
        return -1; // Don't reschedule timer
      }
    return 0;
  }

  // Component attributes and port operations.

  ::CoPubSub::Test_conn::CCM_Listener_ptr
  PubSub_comp_exec_i::get_TestSub_data_listener (void)
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
  PubSub_comp_exec_i::get_TestSub_status (void)
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
  PubSub_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::CoPubSub::CCM_PubSub_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  PubSub_comp_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  PubSub_comp_exec_i::ccm_activate (void)
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
              ex._tao_print_exception("PubSub_comp_exec_i::ccm_activate - ERROR: ");
            }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "PubSub_comp_exec_i::ccm_activate - "
                  "ERROR: Data listener control seems nil.\n"));
      }

    ACE_Time_Value delay(3, 0);  // timer = 3 seconds + 0 usec = 3 sec
    ACE_Time_Value repeat(3, 0); // timer = 3 seconds + 0 usec = 3 sec

    if (this->reactor()->schedule_timer(this->ticker_,
                                            0,
                                            delay,
                                            repeat) == -1)
      {
        ACE_ERROR ((LM_ERROR, "PubSub_comp_exec_i::ccm_activate - "
                  "ERROR: Unable to schedule timer.\n"));
      }
    else
      ACE_DEBUG ((LM_DEBUG, "PubSub_comp_exec_i::ccm_activate - "
                "Timer is up and running.\n"));
  }

  void
  PubSub_comp_exec_i::ccm_passivate (void)
  {
    this->reactor()->cancel_timer(this->ticker_);
  }

  void
  PubSub_comp_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" PUBSUB_COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoPubSub_PubSub_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      PubSub_comp_exec_i);

    return retval;
  }
}
