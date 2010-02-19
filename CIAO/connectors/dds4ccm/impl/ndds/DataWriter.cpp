// $Id$

#include "DataWriter.h"
#include "StatusCondition.h"
#include "Publisher.h"
#include "DataWriterListener.h"
#include "Topic.h"
#include "Duration_t.h"
#include "InstanceHandle_t.h"
#include "PublicationMatchedStatus.h"
#include "LivelinessLostStatus.h"
#include "OfferedIncompatibleQosStatus.h"
#include "OfferedDeadlineMissedStatus.h"
#include "InstanceHandleSeq.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_DataWriter_i::RTI_DataWriter_i (void)
        : impl_ (0)
      {
      }

      RTI_DataWriter_i::~RTI_DataWriter_i (void)
      {
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::set_qos (const ::DDS::DataWriterQos & /*qos*/)
      {
        CIAO_TRACE ("RTI_DataWriter_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_qos (::DDS::DataWriterQos & /*qos*/)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::set_listener (::DDS::DataWriterListener_ptr a_listener,
                                      ::DDS::StatusMask mask)
      {
        RTI_DataWriterListener_i* rti_impl_list = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_impl_list,
                              RTI_DataWriterListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        return this->impl ()->set_listener (rti_impl_list, mask);
      }

      ::DDS::DataWriterListener_ptr
      RTI_DataWriter_i::get_listener (void)
      {
        DDSDataWriterListener *wr = this->impl ()->get_listener ();
        RTI_DataWriterListener_i *list_proxy = dynamic_cast <RTI_DataWriterListener_i *> (wr);
        if (!list_proxy)
          {
            CIAO_DEBUG (6, (LM_DEBUG, "RTI_DataWriter_i::get_listener - "
                                      "DDS returned a NIL listener.\n"));
            return ::DDS::DataWriterListener::_nil ();
          }
        return list_proxy->get_datawriterlistener ();
      }

      ::DDS::Topic_ptr
      RTI_DataWriter_i::get_topic (void)
      {
        DDSTopic* t = this->impl ()->get_topic ();
        ::DDS::Topic_var retval = new RTI_Topic_i ();
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (retval.in ());
        tp->set_impl (t);
        return retval._retn ();
      }

      ::DDS::Publisher_ptr
      RTI_DataWriter_i::get_publisher (void)
      {
        DDSPublisher* p = this->impl ()->get_publisher ();
        ::DDS::Publisher_var retval = new RTI_Publisher_i ();
        RTI_Publisher_i *rti_p = dynamic_cast < RTI_Publisher_i *> (retval.in ());
        rti_p->set_impl (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->impl ()->wait_for_acknowledgments (rtiduration);
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
      {
        ::DDS_LivelinessLostStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_lost_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status)
      {
        ::DDS_OfferedDeadlineMissedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_offered_deadline_missed_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status)
      {
        ::DDS_OfferedIncompatibleQosStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_offered_incompatible_qos_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_publication_matched_status (::DDS::PublicationMatchedStatus & status)
      {
        ::DDS_PublicationMatchedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_publication_matched_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::assert_liveliness (void)
      {
        return this->impl ()->assert_liveliness ();
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles)
      {
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_subscriptions (rtiseq);
        rtiseq >>= subscription_handles;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & /*subscription_data*/,
                                                       const ::DDS::InstanceHandle_t & /*subscription_handle*/)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_matched_subscription_data");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataWriter_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DataWriter_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DataWriter_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      DDSDataWriter *
      RTI_DataWriter_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_DataWriter_i::set_impl (DDSDataWriter * dw)
      {
        this->impl_ = dw;
      }

      DDSDataWriter *
      RTI_DataWriter_i::impl (void)
      {
        if (!this->impl_)
          {
            throw ::CORBA::BAD_INV_ORDER ();
          }
        return this->impl_;
      }

    }
  }
}
