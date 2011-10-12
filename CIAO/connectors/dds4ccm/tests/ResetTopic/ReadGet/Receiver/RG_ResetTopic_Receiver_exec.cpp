// -*- C++ -*-
// $Id$

#include "RG_ResetTopic_Receiver_exec.h"
#include "RG_ResetTopic_Receiver_impl.h"

namespace CIAO_RG_ResetTopic_Receiver_Impl
{

  /**
   * Facet Executor Implementation Class: info_get_status_exec_i
   */

  info_get_status_exec_i::info_get_status_exec_i (
        ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx)
    : ciao_context_ (
        ::RG_ResetTopic::CCM_Receiver_Context::_duplicate (ctx))
  {
  }

  info_get_status_exec_i::~info_get_status_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  info_get_status_exec_i::on_requested_deadline_missed (::DDS::DataReader_ptr /* the_reader */,
  const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  info_get_status_exec_i::on_sample_lost (::DDS::DataReader_ptr /* the_reader */,
  const ::DDS::SampleLostStatus & /* status */)
  {
    /* Your code here. */
  }

  /**
   * Facet Executor Implementation Class: info_read_status_exec_i
   */

  info_read_status_exec_i::info_read_status_exec_i (
        ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx)
    : ciao_context_ (
        ::RG_ResetTopic::CCM_Receiver_Context::_duplicate (ctx))
  {
  }

  info_read_status_exec_i::~info_read_status_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  info_read_status_exec_i::on_requested_deadline_missed (::DDS::DataReader_ptr /* the_reader */,
  const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  info_read_status_exec_i::on_sample_lost (::DDS::DataReader_ptr /* the_reader */,
  const ::DDS::SampleLostStatus & /* status */)
  {
    /* Your code here. */
  }

  /**
   * Facet Executor Implementation Class: start_reading_exec_i
   */

  start_reading_exec_i::start_reading_exec_i (
        ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx,
        Receiver_exec_i &callback)
    : ciao_context_ (
        ::RG_ResetTopic::CCM_Receiver_Context::_duplicate (ctx))
    , callback_ (callback)
  {
  }

  start_reading_exec_i::~start_reading_exec_i (void)
  {
  }

  // Operations from ::ReaderStarter

  void
  start_reading_exec_i::start_read (const char * topic_name)
  {
    this->callback_.start_read (topic_name);
  }

  void
  start_reading_exec_i::set_reader_properties (::CORBA::UShort nr_keys,
  ::CORBA::UShort nr_iterations)
  {
    this->callback_.keys (nr_keys);
    this->callback_.iterations (nr_iterations);
  }

  /**
   * Component Executor Implementation Class: Receiver_exec_i
   */

  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10)
    , keys_ (5)
    , impl_(0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
    delete this->impl_;
  }

  // Supported operations and attributes.
  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (
    const ::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
    if (this->impl_)
      {
        this->impl_->iterations (iterations);
      }
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (
    const ::CORBA::UShort keys)
  {
    this->keys_ = keys;
    if (this->impl_)
      {
        this->impl_->keys (keys);
      }
  }

  void
  Receiver_exec_i::start_read (const char * topic_name)
  {
    if (!this->impl_)
      ACE_NEW_THROW_EX (this->impl_,
                        RG_ResetTopic_Receiver_impl (
                          this->ciao_context_.in (),
                          this->iterations_,
                          this->keys_),
                        ::CORBA::INTERNAL ());
    this->impl_->start (topic_name);
  }

  // Component attributes and port operations.

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_info_get_status_.in ()))
      {
        info_get_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_get_status_exec_i (
            this->ciao_context_.in ()),
            ::CCM_DDS::CCM_PortStatusListener::_nil ());

          this->ciao_info_get_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_info_get_status_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_read_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_info_read_status_.in ()))
      {
        info_read_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_read_status_exec_i (
            this->ciao_context_.in ()),
            ::CCM_DDS::CCM_PortStatusListener::_nil ());

          this->ciao_info_read_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_info_read_status_.in ());
  }

  ::CCM_ReaderStarter_ptr
  Receiver_exec_i::get_start_reading (void)
  {
    if ( ::CORBA::is_nil (this->ciao_start_reading_.in ()))
      {
        start_reading_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          start_reading_exec_i (
            this->ciao_context_.in (),
            *this),
            ::CCM_ReaderStarter::_nil ());

          this->ciao_start_reading_ = tmp;
      }

    return
      ::CCM_ReaderStarter::_duplicate (
        this->ciao_start_reading_.in ());
  }

  // Operations from Components::SessionComponent.

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::RG_ResetTopic::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    /* Your code here. */
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_RG_ResetTopic_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
