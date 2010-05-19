// $Id$

#include "DataReader.h"
#include "DataReaderListener.h"
#include "StatusCondition.h"
#include "ReadCondition.h"
#include "QueryCondition.h"
#include "Subscriber.h"
#include "TopicDescription.h"
#include "Topic.h"
#include "ContentFilteredTopic.h"

#include "ndds/SampleLostStatus.h"
#include "ndds/SubscriptionMatchedStatus.h"
#include "ndds/RequestedDeadlineMissedStatus.h"
#include "ndds/SampleRejectedStatus.h"
#include "ndds/LivelinessChangedStatus.h"
#include "ndds/PublicationBuiltinTopicData.h"
#include "ndds/RequestedIncompatibleQosStatus.h"
#include "ndds/Duration_t.h"
#include "ndds/InstanceHandle_t.h"
#include "ndds/InstanceHandleSeq.h"
#include "ndds/DataReaderQos.h"
#include "ndds/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_DataReader_i::CCM_DDS_DataReader_i (DDSDataReader * dr)
      : impl_ (dr),
        lst_mask_ (0)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReader_i::CCM_DDS_DataReader_i");
    }

    CCM_DDS_DataReader_i::~CCM_DDS_DataReader_i (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReader_i::~CCM_DDS_DataReader_i");
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::enable (void)
    {
      return this->impl ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    CCM_DDS_DataReader_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::StatusCondition_var sc = this->impl ()->get_statuscondition ();
      if (! ::CORBA::is_nil (sc.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::StatusMask
    CCM_DDS_DataReader_i::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    DDS_INSTANCE_HANDLE_T_RETN
    CCM_DDS_DataReader_i::get_instance_handle (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t const rtihandle =
        this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
#else
      return this->impl ()->get_instance_handle ();
#endif
    }

    ::DDS::ReadCondition_ptr
    CCM_DDS_DataReader_i::create_readcondition (
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      ::DDS::ReadCondition_var retval = ::DDS::ReadCondition::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSReadCondition* rc =
        this->impl ()->create_readcondition (sample_states, view_states, instance_states);
      if (rc)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_ReadCondition_i (rc),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::ReadCondition_var rc =
        this->impl ()->create_readcondition (sample_states, view_states, instance_states);
      if (! ::CORBA::is_nil (rc.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::QueryCondition_ptr
    CCM_DDS_DataReader_i::create_querycondition (
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states,
      const char * query_expression,
      const ::DDS::StringSeq & query_parameters)
    {
      ::DDS::QueryCondition_var retval = ::DDS::QueryCondition::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      parameters <<= query_parameters;
      DDSQueryCondition* qc = this->impl ()->create_querycondition (
                                     sample_states,
                                     view_states,
                                     instance_states,
                                     query_expression,
                                     parameters);

      if (qc)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_QueryCondition_i (qc),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::QueryCondition_var qc = this->impl ()->create_querycondition (
                                     sample_states,
                                     view_states,
                                     instance_states,
                                     query_expression,
                                     query_parameters);

      if (! ::CORBA::is_nil (qc.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_QueryCondition_i (qc.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::delete_readcondition (
      ::DDS::ReadCondition_ptr a_condition)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_ReadCondition_i *rc = dynamic_cast< CCM_DDS_ReadCondition_i *> (a_condition);
      if (!rc)
        {
          return ::DDS::RETCODE_BAD_PARAMETER;
        }
      return this->impl ()->delete_readcondition (rc->get_impl ());
#else
      return this->impl ()->delete_readcondition (a_condition);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::set_qos (
      const ::DDS::DataReaderQos &qos)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataReaderQos ddsqos;
      ddsqos <<= qos;
      return this->impl ()->set_qos (ddsqos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_qos (
      ::DDS::DataReaderQos &qos)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataReaderQos ddsqos;
      ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
      qos <<= ddsqos;
      return retval;
#else
      return this->impl ()->get_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::set_listener (
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReader_i::set_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_DataReaderListener_i *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            CCM_DDS_DataReaderListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      this->lst_mask_ = mask;
      return this->impl ()->set_listener (ccm_dds_drl, mask);
#else
      return this->impl ()->set_listener (a_listener, mask);
#endif
    }

    ::DDS::DataReaderListener_ptr
    CCM_DDS_DataReader_i::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReader_i::get_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataReaderListener *drl = this->impl ()->get_listener ();
      CCM_DDS_DataReaderListener_i *ccm_dds_drl =
        dynamic_cast <CCM_DDS_DataReaderListener_i *> (drl);
      if (!ccm_dds_drl)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_DataReader_i::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::DataReaderListener::_nil ();
        }
      return ccm_dds_drl->get_datareaderlistener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    ::DDS::TopicDescription_ptr
    CCM_DDS_DataReader_i::get_topicdescription (void)
    {
      ::DDS::TopicDescription_var dds_td = ::DDS::TopicDescription::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDSTopicDescription* td = this->impl ()->get_topicdescription ();
      ::DDSTopic * tp = ::DDSTopic::narrow (td);
      if (tp)
        {
          ACE_NEW_THROW_EX (dds_td,
                            CCM_DDS_Topic_i (tp),
                            CORBA::NO_MEMORY ());
        }
      else
        {
          ::DDSContentFilteredTopic * cft =
            DDSContentFilteredTopic::narrow (td);
          if (cft)
            {
              ACE_NEW_THROW_EX (dds_td,
                                CCM_DDS_ContentFilteredTopic_i (cft),
                                CORBA::NO_MEMORY ());
            }
        }
#else
      ::DDS::TopicDescription_var td = this->impl ()->get_topicdescription ();
      ACE_NEW_THROW_EX (dds_td,
                        CCM_DDS_TopicDescription_i (td.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return dds_td._retn ();
    }

    ::DDS::Subscriber_ptr
    CCM_DDS_DataReader_i::get_subscriber (void)
    {
      ::DDS::Subscriber_var dds_td = ::DDS::Subscriber::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDSSubscriber* subscriber = this->impl ()->get_subscriber ();
      ACE_NEW_THROW_EX (dds_td,
                        CCM_DDS_Subscriber_i (subscriber),
                        CORBA::NO_MEMORY ());
#else
      ::DDSSubscriber_var subscriber = this->impl ()->get_subscriber ();
      ACE_NEW_THROW_EX (dds_td,
                        CCM_DDS_Subscriber_i (subscriber.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return dds_td._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_sample_rejected_status (
      ::DDS::SampleRejectedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_SampleRejectedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_sample_rejected_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_sample_rejected_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_liveliness_changed_status (
      ::DDS::LivelinessChangedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_LivelinessChangedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_changed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_liveliness_changed_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_requested_deadline_missed_status (
      ::DDS::RequestedDeadlineMissedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_RequestedDeadlineMissedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_requested_deadline_missed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_requested_deadline_missed_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_requested_incompatible_qos_status (
      ::DDS::RequestedIncompatibleQosStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_RequestedIncompatibleQosStatus ddsstatus;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_requested_incompatible_qos_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_requested_incompatible_qos_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_subscription_matched_status (
      ::DDS::SubscriptionMatchedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriptionMatchedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_subscription_matched_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_subscription_matched_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_sample_lost_status (
      ::DDS::SampleLostStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_SampleLostStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_sample_lost_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_sample_lost_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::wait_for_historical_data (
      const ::DDS::Duration_t & max_wait)
    {
#if (CIAO_DDS4CCM_NDDS==1)
     ::DDS_Duration_t rtiduration;
     rtiduration <<= max_wait;
     return this->impl ()->wait_for_historical_data (rtiduration);
#else
     return this->impl ()->wait_for_historical_data (max_wait);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_matched_publications (
      ::DDS::InstanceHandleSeq & publication_handles)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReader_i::get_matched_publications");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_publications (rtiseq);
      publication_handles <<= rtiseq;
      return retval;
#else
      return this->impl ()->get_matched_publications (publication_handles);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataReader_i::get_matched_publication_data (
      ::DDS::PublicationBuiltinTopicData & publication_data,
      DDS_INSTANCE_HANDLE_T_IN publication_handle)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublicationBuiltinTopicData ccm_dds_pub_data;
      ::DDS_InstanceHandle_t ccm_dds_pub_handle;

      ccm_dds_pub_data <<= publication_data;
      ccm_dds_pub_handle <<= publication_handle;
      ::DDS::ReturnCode_t const retcode = this->impl()->
                    get_matched_publication_data (ccm_dds_pub_data,
                                                  ccm_dds_pub_handle);
      publication_data <<= ccm_dds_pub_data;
      return retcode;
#else
      return this->impl ()->get_matched_publication_data (publication_data, publication_handle);
#endif
    }

    DDSDataReader *
    CCM_DDS_DataReader_i::get_impl (void)
    {
      return this->impl_;
    }

    ::DDS::StatusMask
    CCM_DDS_DataReader_i::get_mask (void)
    {
      return this->lst_mask_;
    }

    void
    CCM_DDS_DataReader_i::set_impl (DDSDataReader * dr)
    {
      this->impl_ = dr;
    }

    DDSDataReader *
    CCM_DDS_DataReader_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
