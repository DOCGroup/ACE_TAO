// $Id$

#include "DDSDataReaderListener_T.h"
#include "StatusCondition.h"
#include "ReadCondition_T.h"
#include "QueryCondition_T.h"
#include "Subscriber_T.h"
#include "TopicDescription_T.h"
#include "Topic_T.h"
#include "ContentFilteredTopic_T.h"

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

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DataReader_T (DDSDataReader * dr)
  : impl_ (0),
    lst_mask_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::DataReader_T");
  if (dr)
    {
      this->impl_ = DDS_TYPE::data_reader::narrow (dr);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DataReader_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::~DataReader_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::log_query_condition (
  DDSQueryCondition * qc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::log_query_condition");

  if (CIAO_debug_level >= 10)
    {
      DDS4CCM_DEBUG (10, (LM_DEBUG, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::")
                    ACE_TEXT ("read_wo_instance - read_w_condition:\n")));
      DDS4CCM_DEBUG (10, (LM_DEBUG, CLINFO
                    ACE_TEXT ("\t\texpression <%C>\n"),
                    qc->get_query_expression ()));
      ::DDS_StringSeq dds_qp;
      qc->get_query_parameters (dds_qp);
      for (DDS_Long i = 0; i < dds_qp.length (); ++i)
        {
          DDS4CCM_DEBUG (10, (LM_DEBUG, CLINFO
                        ACE_TEXT ("\t\tparameter %d <%C>\n"),
                        i + 1,
                        dds_qp[i]));

        }
      DDS4CCM_DEBUG (10, (LM_DEBUG, CLINFO "\t\tinstance state mask <%d>\n",
                          qc->get_instance_state_mask ()));
      DDS4CCM_DEBUG (10, (LM_DEBUG, CLINFO "\t\tsample state mask <%d>\n",
                          qc->get_sample_state_mask ()));
      DDS4CCM_DEBUG (10, (LM_DEBUG, CLINFO "\t\tview state mask <%d>\n",
                          qc->get_view_state_mask ()));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::read_wo_instance (
  typename DDS_TYPE::dds_seq_type & data,
  DDS_SampleInfoSeq & sample_info,
  DDSQueryCondition * qc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::read_wo_instance");
  DDS_ReturnCode_t retval = DDS_RETCODE_ERROR;
  if (qc)
    {
      this->log_query_condition (qc);
      retval = this->impl ()->read_w_condition (data,
                                                sample_info,
                                                DDS_LENGTH_UNLIMITED,
                                                qc);
    }
  else
    {
      retval = this->impl ()->read (data,
                                    sample_info,
                                    DDS_LENGTH_UNLIMITED,
                                    DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                    DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                    DDS_ALIVE_INSTANCE_STATE);
    }
  if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
    {
      DDS_ReturnCode_t const retval = this->return_loan (data, sample_info);
      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_wo_instance - ")
                        ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                        translate_retcode (retval)));
        }
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_wo_instance - ")
                            ACE_TEXT ("retval is %C\n"),
                            translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::read_w_instance (
  typename DDS_TYPE::dds_seq_type & data,
  const ::DDS_InstanceHandle_t & lookup_hnd,
  DDS_SampleInfoSeq & sample_info)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::read_w_instance");

  DDS4CCM_DEBUG (6, (LM_INFO, CLINFO
                ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_w_instance - ")
                ACE_TEXT ("Reading with instance.\n")));
  DDS_ReturnCode_t const retval = this->impl ()->read_instance (
                                        data,
                                        sample_info,
                                        DDS_LENGTH_UNLIMITED,
                                        lookup_hnd,
                                        DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                                        DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                        DDS_ALIVE_INSTANCE_STATE);
  if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
    {
      DDS_ReturnCode_t const retval = this->return_loan (data, sample_info);
      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_w_instance - ")
                        ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                        translate_retcode (retval)));
        }
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_w_instance - ")
                    ACE_TEXT ("retval is %C\n"),
                    translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get (
  typename DDS_TYPE::dds_seq_type & data,
  DDS_SampleInfoSeq & sample_info,
  const DDS_Long & max_samples,
  DDSQueryCondition * qc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get");

  if (qc)
    {
      this->log_query_condition (qc);
      return this->impl ()->read_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              qc);
   }
  return DDS_RETCODE_ERROR;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get (
  typename DDS_TYPE::dds_seq_type & data,
  DDS_SampleInfoSeq & sample_info,
  const DDS_Long & max_samples,
  DDSReadCondition * rd)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get");

  if (rd)
    {
      return this->impl ()->read_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              rd);
    }
   return DDS_RETCODE_ERROR;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::take (
  typename DDS_TYPE::dds_seq_type & data,
  DDS_SampleInfoSeq & sample_info,
  const DDS_Long & max_samples,
  DDSQueryCondition * qc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::take");

  if (qc)
    {
      this->log_query_condition (qc);
      return this->impl ()->take_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              qc);
    }
  else
    {
      return this->impl ()->take (data,
                                  sample_info,
                                  max_samples,
                                  DDS_NOT_READ_SAMPLE_STATE,
                                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                  DDS_ANY_INSTANCE_STATE);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS_InstanceHandle_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::lookup_instance (
  const typename DDS_TYPE::value_type& an_instance)
{
  return this->impl ()->lookup_instance (an_instance);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::return_loan (
  typename DDS_TYPE::dds_seq_type & data,
  ::DDS_SampleInfoSeq & sample_info)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::return_loan");

  return this->impl ()->return_loan (data, sample_info);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::passivate");

  this->set_listener (::DDS::DataReaderListener::_nil (), 0);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader (
  ::DDS::ContentFilteredTopic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char * library_name,
  const char * profile_name)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_datareader");

  ::DDS::DataReader_var reader;
  if (library_name && profile_name)
    {
      reader = subscriber->create_datareader_with_profile (
          topic,
          library_name,
          profile_name,
          ::DDS::DataReaderListener::_nil (),
          0);
    }
  else
    {
      ::DDS::DataReaderQos drqos;
      reader = subscriber->create_datareader (
            topic,
            drqos,
            ::DDS::DataReaderListener::_nil (),
            0);
    }
  this->set_impl(reader.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char * library_name,
  const char * profile_name)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_datareader");

  ::DDS::DataReader_var reader;

  if (library_name && profile_name)
    {
      reader = subscriber->create_datareader_with_profile (
          topic,
          library_name,
          profile_name,
          ::DDS::DataReaderListener::_nil (),
          0);
    }
  else
    {
      ::DDS::DataReaderQos drqos;
      reader = subscriber->create_datareader (
            topic,
            drqos,
            ::DDS::DataReaderListener::_nil (),
            0);
    }

  this->set_impl (reader.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_datareader (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::delete_datareader");

  ::DDS::ReturnCode_t const retval = subscriber->delete_datareader (this);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    ACE_TEXT ("DataReader_T::delete_datareader - ")
                    ACE_TEXT ("Unable to delete DataReader: <%C>\n"),
                    translate_retcode (retval)));
      throw CORBA::INTERNAL ();
    }
  this->impl_ = 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
{
  if (this->impl_)
    {
      return this->impl_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::enable (void)
{
  return this->impl ()->enable ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusCondition_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_statuscondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_statuscondition");

  ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
  DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
  if (sc)
    {
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_StatusCondition_i (sc),
                        CORBA::NO_MEMORY ());
    }
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_status_changes (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_status_changes");

  return this->impl ()->get_status_changes ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_INSTANCE_HANDLE_T_RETN
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_instance_handle (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_instance_handle");

  ::DDS_InstanceHandle_t const rtihandle =
    this->impl ()->get_instance_handle ();
  ::DDS::InstanceHandle_t handle;
  handle <<= rtihandle;
  return handle;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReadCondition_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_readcondition (
  ::DDS::SampleStateMask sample_states,
  ::DDS::ViewStateMask view_states,
  ::DDS::InstanceStateMask instance_states)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_readcondition");

  ::DDS::ReadCondition_var retval = ::DDS::ReadCondition::_nil ();
  DDSReadCondition* rc =
    this->impl ()->create_readcondition (sample_states,
                                         view_states,
                                         instance_states);
  if (rc)
    {
      ACE_NEW_THROW_EX (retval,
                        ReadCondition_type (rc),
                        CORBA::NO_MEMORY ());
    }
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::QueryCondition_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_querycondition (
  ::DDS::SampleStateMask sample_states,
  ::DDS::ViewStateMask view_states,
  ::DDS::InstanceStateMask instance_states,
  const char * query_expression,
  const ::DDS::StringSeq & query_parameters)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_querycondition");

  ::DDS::QueryCondition_var retval = ::DDS::QueryCondition::_nil ();
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
                        QueryCondition_type (qc),
                        CORBA::NO_MEMORY ());
    }
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_readcondition (
  ::DDS::ReadCondition_ptr a_condition)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::delete_readcondition");

  DDSReadCondition * dds_rc = 0;
  ReadCondition_type * rc = dynamic_cast< ReadCondition_type *> (a_condition);
  QueryCondition_type * qc = dynamic_cast< QueryCondition_type *> (a_condition);

  if (!rc)
    {
      if (!qc)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::delete_readcondition - ")
                        ACE_TEXT ("Unable to cast provided condition to a typed ")
                        ACE_TEXT ("read condition nor a typed query condition\n")));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }
      dds_rc = dynamic_cast < DDSQueryCondition *> (qc->get_impl ());
      if (!dds_rc)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::delete_readcondition - ")
                        ACE_TEXT ("Unable to cast DDSQueryCondition to a ")
                        ACE_TEXT ("DDSReadCondition\n")));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }
    }
  else
    {
      dds_rc = rc->get_impl ();
    }
  ::DDS::ReturnCode_t retcode = this->impl ()->delete_readcondition (dds_rc);
  if (retcode == ::DDS::RETCODE_OK)
    {
      if (rc)
        {
          rc->set_impl (0);
        }
      if (qc)
        {
          qc->set_impl (0);
        }
    }
  return retcode;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_contained_entities (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::delete_contained_entities");

  return this->impl ()->delete_contained_entities ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos (
  const ::DDS::DataReaderQos &qos)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::set_qos");

  ::DDS_DataReaderQos ddsqos;
  ddsqos <<= qos;
  return this->impl ()->set_qos (ddsqos);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_qos (
  ::DDS::DataReaderQos &qos)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_qos");

  ::DDS_DataReaderQos ddsqos;
  ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
  qos <<= ddsqos;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_listener (
  ::DDS::DataReaderListener_ptr a_listener,
  ::DDS::StatusMask mask)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_listener");

  DataReaderListener_type * ccm_dds_drl = 0;
  if (! ::CORBA::is_nil (a_listener))
    {
      ACE_NEW_THROW_EX (ccm_dds_drl,
                        DataReaderListener_type (
                          a_listener,
                          this),
                        CORBA::NO_MEMORY ());
    }
  this->lst_mask_ = mask;
  return this->impl ()->set_listener (ccm_dds_drl, mask);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::DataReaderListener_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener");

  DDSDataReaderListener *drl = this->impl ()->get_listener ();
  CCM_DDS_DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *ccm_dds_drl =
    dynamic_cast <CCM_DDS_DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *> (drl);
  if (!ccm_dds_drl)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, "CIAO::DDS4CCM::DataReader_T"
                                "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener - "
                                "DDS returned a NIL listener.\n"));
      return ::DDS::DataReaderListener::_nil ();
    }
  return ccm_dds_drl->get_datareaderlistener ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::TopicDescription_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_topicdescription (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_topicdescription");

  ::DDS::TopicDescription_var dds_td = ::DDS::TopicDescription::_nil ();
  ::DDSTopicDescription* td = this->impl ()->get_topicdescription ();
  ::DDSTopic * tp = ::DDSTopic::narrow (td);
  if (tp)
    {
      ACE_NEW_THROW_EX (dds_td,
                        Topic_type (tp),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      ::DDSContentFilteredTopic * cft =
        DDSContentFilteredTopic::narrow (td);
      if (cft)
        {
          ACE_NEW_THROW_EX (dds_td,
                            ContentFilteredTopic_type (cft),
                            CORBA::NO_MEMORY ());
        }
    }
  return dds_td._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::Subscriber_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_subscriber (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_subscriber");

  ::DDS::Subscriber_var dds_td = ::DDS::Subscriber::_nil ();
  ::DDSSubscriber* subscriber = this->impl ()->get_subscriber ();
  ACE_NEW_THROW_EX (dds_td,
                    Subscriber_type (subscriber),
                    CORBA::NO_MEMORY ());
  return dds_td._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_sample_rejected_status (
  ::DDS::SampleRejectedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_sample_rejected_status");

  DDS_SampleRejectedStatus ddsstatus;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_sample_rejected_status (ddsstatus);
  status <<= ddsstatus;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_liveliness_changed_status (
  ::DDS::LivelinessChangedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_liveliness_changed_status");

  DDS_LivelinessChangedStatus ddsstatus;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_changed_status (ddsstatus);
  status <<= ddsstatus;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_requested_deadline_missed_status (
  ::DDS::RequestedDeadlineMissedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_requested_deadline_missed_status");

  DDS_RequestedDeadlineMissedStatus ddsstatus;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_requested_deadline_missed_status (ddsstatus);
  status <<= ddsstatus;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_requested_incompatible_qos_status (
  ::DDS::RequestedIncompatibleQosStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_requested_incompatible_qos_status");

  DDS_RequestedIncompatibleQosStatus ddsstatus;
  ::DDS::ReturnCode_t const retval =
    this->impl ()->get_requested_incompatible_qos_status (ddsstatus);
  status <<= ddsstatus;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_subscription_matched_status (
  ::DDS::SubscriptionMatchedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_subscription_matched_status");

  ::DDS_SubscriptionMatchedStatus ddsstatus;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_subscription_matched_status (ddsstatus);
  status <<= ddsstatus;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_sample_lost_status (
  ::DDS::SampleLostStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_sample_lost_status");

  DDS_SampleLostStatus ddsstatus;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_sample_lost_status (ddsstatus);
  status <<= ddsstatus;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::wait_for_historical_data (
  const ::DDS::Duration_t & max_wait)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::wait_for_historical_data");

  ::DDS_Duration_t rtiduration;
  rtiduration <<= max_wait;
  return this->impl ()->wait_for_historical_data (rtiduration);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_matched_publications (
  ::DDS::InstanceHandleSeq & publication_handles)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_matched_publications");

  ::DDS_InstanceHandleSeq rtiseq;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_publications (rtiseq);
  publication_handles <<= rtiseq;
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_matched_publication_data (
  ::DDS::PublicationBuiltinTopicData & publication_data,
  DDS_INSTANCE_HANDLE_T_IN publication_handle)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_matched_publication_data");

  ::DDS_PublicationBuiltinTopicData ccm_dds_pub_data;
  ::DDS_InstanceHandle_t ccm_dds_pub_handle;

  ccm_dds_pub_data <<= publication_data;
  ccm_dds_pub_handle <<= publication_handle;
  ::DDS::ReturnCode_t const retcode = this->impl()->
                get_matched_publication_data (ccm_dds_pub_data,
                                              ccm_dds_pub_handle);
  publication_data <<= ccm_dds_pub_data;
  return retcode;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_impl (void)
{
  return this->impl_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_mask (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_mask");

  return this->lst_mask_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (
  ::DDS::DataReader_ptr dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::set_impl");

  DataReader_type * dds_dr = dynamic_cast < DataReader_type * > (dr);
  if (dds_dr)
    {
      this->impl_ = dds_dr->get_impl ();
      if (!this->impl_)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, "CIAO::DDS4CCM::DataReader_T::create_datareader - "
                      "Failed to cast DDS::DataReader to DataReader_T\n"));
        }
    }
  else
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "CIAO::DDS4CCM::DataReader_T::create_datareader - "
                    "Failed to cast DDS::DataReader to DataReader_T\n"));
      throw CCM_DDS::InternalError ();
    }
}
