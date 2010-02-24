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

#include "dds4ccm/impl/logger/Log_Macros.h"

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
      RTI_DataWriter_i::set_qos (const ::DDS::DataWriterQos & qos)
      {
        DDS4CCM_TRACE ("RTI_DataWriter_i::set_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ACE_UNUSED_ARG (qos);
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
#else
        return this->impl ()->set_qos (qos);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_qos (::DDS::DataWriterQos & qos)
      {
        DDS4CCM_TRACE ("RTI_DataWriter_i::get_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ACE_UNUSED_ARG (qos);
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
#else
        return this->impl ()->get_qos (qos);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::set_listener (::DDS::DataWriterListener_ptr a_listener,
                                      ::DDS::StatusMask mask)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        RTI_DataWriterListener_i* rti_impl_list = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_impl_list,
                              RTI_DataWriterListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        return this->impl ()->set_listener (rti_impl_list, mask);
#else
        return this->impl ()->set_listener (a_listener, mask);
#endif
      }

      ::DDS::DataWriterListener_ptr
      RTI_DataWriter_i::get_listener (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        DDSDataWriterListener *wr = this->impl ()->get_listener ();
        RTI_DataWriterListener_i *list_proxy = dynamic_cast <RTI_DataWriterListener_i *> (wr);
        if (!list_proxy)
          {
            DDS4CCM_DEBUG (6, (LM_DEBUG, "RTI_DataWriter_i::get_listener - "
                                      "DDS returned a NIL listener.\n"));
            return ::DDS::DataWriterListener::_nil ();
          }
        return list_proxy->get_datawriterlistener ();
#else
        return this->impl ()->get_listener ();
#endif
      }

      ::DDS::Topic_ptr
      RTI_DataWriter_i::get_topic (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_Topic_i (),
                          CORBA::NO_MEMORY ());

        DDSTopic* t = this->impl ()->get_topic ();
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (retval.in ());
        tp->set_impl (t);
        return retval._retn ();
#else
        return this->impl ()->get_topic ();
#endif
      }

      ::DDS::Publisher_ptr
      RTI_DataWriter_i::get_publisher (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_Publisher_i (),
                          CORBA::NO_MEMORY ());

        DDSPublisher* p = this->impl ()->get_publisher ();
        RTI_Publisher_i *rti_p = dynamic_cast < RTI_Publisher_i *> (retval.in ());
        rti_p->set_impl (p);
        return retval._retn ();
#else
        return this->impl ()->get_publisher ();
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->impl ()->wait_for_acknowledgments (rtiduration);
#else
       return this->impl ()->wait_for_acknowledgments (max_wait);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS_LivelinessLostStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_lost_status (rtistatus);
        rtistatus >>= status;
        return retval;
#else
        return this->impl ()->get_liveliness_lost_status (status);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS_OfferedDeadlineMissedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_offered_deadline_missed_status (rtistatus);
        rtistatus >>= status;
        return retval;
#else
        return this->impl ()->get_offered_deadline_missed_status (status);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS_OfferedIncompatibleQosStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_offered_incompatible_qos_status (rtistatus);
        rtistatus >>= status;
        return retval;
#else
        return this->impl ()->get_offered_incompatible_qos_status (status);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_publication_matched_status (::DDS::PublicationMatchedStatus & status)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS_PublicationMatchedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_publication_matched_status (rtistatus);
        rtistatus >>= status;
        return retval;
#else
        return this->impl ()->get_publication_matched_status (status);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::assert_liveliness (void)
      {
        return this->impl ()->assert_liveliness ();
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_subscriptions (rtiseq);
        rtiseq >>= subscription_handles;
        return retval;
#else
        return this->impl ()->get_matched_subscriptions (subscription_handles);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & subscription_data,
                                                       DDS_INSTANCE_HANDLE_T_IN subscription_handle)
      {
        DDS4CCM_TRACE ("RTI_DataWriter_i::get_matched_subscription_data");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ACE_UNUSED_ARG (subscription_data);
        ACE_UNUSED_ARG (subscription_handle);
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
#else
        return this->impl ()->get_matched_subscription_data (subscription_data, subscription_handle);
#endif
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataWriter_i::get_statuscondition (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_StatusCondition_i (),
                          CORBA::NO_MEMORY ());

        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
#else
        return this->impl ()->get_statuscondition ();
#endif
      }

      ::DDS::StatusMask
      RTI_DataWriter_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      DDS_INSTANCE_HANDLE_T_RETN
      RTI_DataWriter_i::get_instance_handle (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
#else
        return this->impl ()->get_instance_handle ();
#endif
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
