// $Id$

#include "DataReader.h"
#include "DataReaderListener.h"
#include "StatusCondition.h"
#include "ReadCondition.h"
#include "QueryCondition.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"
#include "Duration_t.h"
#include "InstanceHandle_t.h"
#include "InstanceHandleSeq.h"
#include "Subscriber.h"
#include "TopicDescription.h"
#include "DataReaderQos.h"
#include "StringSeq.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_DataReader_i::RTI_DataReader_i (void)
        : impl_ (0)
      {
      }

      RTI_DataReader_i::~RTI_DataReader_i (void)
      {
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataReader_i::get_statuscondition (void)
      {
        ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_StatusCondition_i (),
                          CORBA::NO_MEMORY ());

        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DataReader_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DataReader_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      ::DDS::ReadCondition_ptr
      RTI_DataReader_i::create_readcondition (
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states)
      {
        ::DDS::ReadCondition_var retval = ::DDS::ReadCondition::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_ReadCondition_i (),
                          CORBA::NO_MEMORY ());

        DDSReadCondition* rc = this->impl ()->create_readcondition (sample_states, view_states, instance_states);
        RTI_ReadCondition_i *rti_rc = dynamic_cast < RTI_ReadCondition_i *>(retval.in ());
        rti_rc->set_impl (rc);
        return retval._retn ();
      }

      ::DDS::QueryCondition_ptr
      RTI_DataReader_i::create_querycondition (
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states,
        const char * query_expression,
        const ::DDS::StringSeq & query_parameters)
      {
        DDS_StringSeq parameters;
        parameters <<= query_parameters;

        ::DDS::QueryCondition_var retval = ::DDS::QueryCondition::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_QueryCondition_i (),
                          CORBA::NO_MEMORY ());

        DDSQueryCondition* qc = this->impl ()->create_querycondition (
                                       sample_states,
                                       view_states,
                                       instance_states,
                                       query_expression,
                                       parameters);

        RTI_QueryCondition_i *rti_qc = dynamic_cast < RTI_QueryCondition_i *> (retval.in ());
        rti_qc->set_impl (qc);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::delete_readcondition (
        ::DDS::ReadCondition_ptr a_condition)
      {
        RTI_ReadCondition_i *rc = dynamic_cast< RTI_ReadCondition_i *> (a_condition);
        if (!rc)
          {
            return ::DDS::RETCODE_BAD_PARAMETER;
          }
        return this->impl ()->delete_readcondition (rc->get_impl ());
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::delete_contained_entities (void)
      {
        return this->impl ()->delete_contained_entities ();
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::set_qos (
        const ::DDS::DataReaderQos &qos)
      {
        ::DDS_DataReaderQos ddsqos;
        ddsqos <<= qos;
        return this->impl ()->set_qos (ddsqos);
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_qos (
        ::DDS::DataReaderQos &qos)
      {
        ::DDS_DataReaderQos ddsqos;
        ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
        qos <<= ddsqos;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::set_listener (
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_DataReader_i::set_listener");

        RTI_DataReaderListener_i *rti_drl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_drl,
                              RTI_DataReaderListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        return this->impl ()->set_listener (rti_drl, mask);
      }

      ::DDS::DataReaderListener_ptr
      RTI_DataReader_i::get_listener (void)
      {
        CIAO_TRACE ("RTI_DataReader_i::get_listener");

        DDSDataReaderListener *drl = this->impl ()->get_listener ();
        RTI_DataReaderListener_i *rti_drl = dynamic_cast <RTI_DataReaderListener_i *> (drl);
        if (!rti_drl)
          {
            CIAO_DEBUG (6, (LM_DEBUG, "RTI_DataReader_i::get_listener - "
                                      "DDS returned a NIL listener.\n"));
            return ::DDS::DataReaderListener::_nil ();
          }
        return rti_drl->get_datareaderlistener ();
      }

      ::DDS::TopicDescription_ptr
      RTI_DataReader_i::get_topicdescription (void)
      {
        ::DDS::TopicDescription_var dds_td = ::DDS::TopicDescription::_nil ();
        ACE_NEW_THROW_EX (dds_td,
                          RTI_TopicDescription_i (),
                          CORBA::NO_MEMORY ());

        ::DDSTopicDescription* td = this->impl ()->get_topicdescription ();
        RTI_TopicDescription_i *rti_td = dynamic_cast < RTI_TopicDescription_i *>(dds_td.in ());
        rti_td->set_impl (td);
        return dds_td._retn ();
      }

      ::DDS::Subscriber_ptr
      RTI_DataReader_i::get_subscriber (void)
      {
        ::DDS::Subscriber_var dds_td = ::DDS::Subscriber::_nil ();
        ACE_NEW_THROW_EX (dds_td,
                          RTI_Subscriber_i (),
                          CORBA::NO_MEMORY ());

        ::DDSSubscriber* subscriber = this->impl ()->get_subscriber ();
        RTI_Subscriber_i *sub = dynamic_cast < RTI_Subscriber_i *>(dds_td.in ());
        sub->set_impl (subscriber);
        return dds_td._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_sample_rejected_status (
        ::DDS::SampleRejectedStatus & status)
      {
        DDS_SampleRejectedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_sample_rejected_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_liveliness_changed_status (
        ::DDS::LivelinessChangedStatus & status)
      {
        DDS_LivelinessChangedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_changed_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_requested_deadline_missed_status (
        ::DDS::RequestedDeadlineMissedStatus & status)
      {
        DDS_RequestedDeadlineMissedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_requested_deadline_missed_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_requested_incompatible_qos_status (
        ::DDS::RequestedIncompatibleQosStatus & status)
      {
        DDS_RequestedIncompatibleQosStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_requested_incompatible_qos_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_subscription_matched_status (
        ::DDS::SubscriptionMatchedStatus & status)
      {
        ::DDS_SubscriptionMatchedStatus ddsstatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_subscription_matched_status (ddsstatus);
        ddsstatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_sample_lost_status (
        ::DDS::SampleLostStatus & status)
      {
        DDS_SampleLostStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_sample_lost_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::wait_for_historical_data (
        const ::DDS::Duration_t & max_wait)
      {
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->impl ()->wait_for_historical_data (rtiduration);
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_matched_publications (
        ::DDS::InstanceHandleSeq & publication_handles)
      {
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_publications (rtiseq);
        rtiseq >>= publication_handles;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_matched_publication_data (
        ::DDS::PublicationBuiltinTopicData & /*publication_data*/,
        const ::DDS::InstanceHandle_t & /*publication_handle*/)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSDataReader *
      RTI_DataReader_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_DataReader_i::set_impl (DDSDataReader * dr)
      {
        this->impl_ = dr;
      }

      DDSDataReader *
      RTI_DataReader_i::impl (void)
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
