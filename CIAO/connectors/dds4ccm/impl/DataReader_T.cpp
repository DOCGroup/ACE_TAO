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

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::DataReader_T (DDSDataReader * dr)
  : impl_ (0),
    rd_condition_ (0),
    ws_ (0),
    qc_reader_ (0),
    qc_getter_ (0),
    qc_listener_ (0),
    lst_mask_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::DataReader_T");
  if (dr)
    {
      this->impl_ = DDS_TYPE::data_reader::narrow (dr);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::~DataReader_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::~DataReader_T");
  delete this->ws_;
  this->ws_ = 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::read_wo_instance (
  typename DDS_TYPE::dds_seq_type & data,
  ::DDS_SampleInfoSeq & sample_info)
{
  ::DDS_ReturnCode_t retval = DDS_RETCODE_ERROR;
  if (this->qc_reader_)
    {
      retval = this->impl ()->read_w_condition (data,
                                                sample_info,
                                                DDS_LENGTH_UNLIMITED,
                                                this->qc_reader_);
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
      ::DDS_ReturnCode_t const retval = this->return_loan (data, sample_info);
      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
            "CIAO::DDS4CCM::DataReader_T::read_without_instance - "
            "Error returning loan to DDS - <%C>\n",
            translate_retcode (retval)));
        }
      DDS4CCM_ERROR (1, (LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_without_instance - ")
                            ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::read_w_instance (
  typename DDS_TYPE::dds_seq_type & data,
  const ::DDS_InstanceHandle_t & lookup_hnd,
  DDS_SampleInfoSeq & sample_info)
{
  DDS4CCM_DEBUG (6, (LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_with_instance - ")
                           ACE_TEXT ("Reading with instance.\n")));
  ::DDS_ReturnCode_t const retval = this->impl ()->read_instance (
                                        data,
                                        sample_info,
                                        DDS_LENGTH_UNLIMITED,
                                        lookup_hnd,
                                        DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                                        DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                        DDS_ALIVE_INSTANCE_STATE);
  if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
    {
      ::DDS_ReturnCode_t const retval = this->return_loan (data, sample_info);
      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
            "CIAO::DDS4CCM::DataReader_T::read_with_instance - "
            "Error returning loan to DDS - <%C>\n",
            translate_retcode (retval)));
        }
      DDS4CCM_ERROR (1, (LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::DataReader_T::read_with_instance - ")
                            ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::read_w_condition (
  typename DDS_TYPE::dds_seq_type & data,
  ::DDS_SampleInfoSeq & sample_info,
  ::DDS_Long max_samples)
{
  if (this->qc_getter_)
    {
      return this->impl ()->read_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              this->qc_getter_);
   }
  else
    {
      return this->impl ()->read_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              this->rd_condition_);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::take (
  typename DDS_TYPE::dds_seq_type & data,
  ::DDS_SampleInfoSeq & sample_info,
  ::DDS_Long max_samples)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::take");

  if (this->qc_listener_)
    {
      return this->impl ()->take_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              this->qc_listener_);
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS_InstanceHandle_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::lookup_instance (
  const typename DDS_TYPE::value_type& an_instance)
{
  return this->impl ()->lookup_instance (an_instance);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::return_loan (
  typename DDS_TYPE::dds_seq_type & data,
  ::DDS_SampleInfoSeq & sample_info)
{
  return this->impl ()->return_loan (data, sample_info);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::QueryFilter *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::query (void)
{
  if (!this->qc_reader_)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::query - "
                    "Error: No QueryCondition set yet. First set a filter.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  ::CCM_DDS::QueryFilter_var filter;
  ACE_NEW_THROW_EX (filter,
                    ::CCM_DDS::QueryFilter(),
                    CORBA::NO_MEMORY ());
  filter->expression= this->qc_reader_->get_query_expression ();
  ::DDS_StringSeq dds_qp;
  this->qc_reader_->get_query_parameters (dds_qp);
  filter->parameters <<= dds_qp;
  return filter._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::query (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::query");

  if (!this->qc_reader_)
    {
      // filter not set. Create query conditions for both the
      // getter and reader
      ::DDS_StringSeq dds_qp;
      dds_qp <<= filter.parameters;
      if (this->rd_condition_)
        { // Getter functionality
          // First remove the existing conditions from the waitset
          // Than create a new condition and attach it to the waitset.
          this->remove_conditions ();
          this->qc_getter_ = this->impl ()->create_querycondition (
                                  DDS_NOT_READ_SAMPLE_STATE,
                                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                  DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE,
                                  filter.expression,
                                  dds_qp);
          this->attach_querycondition ();
        }
      this->qc_reader_ = this->impl ()->create_querycondition (
                                  DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                  DDS_ALIVE_INSTANCE_STATE,
                                  filter.expression,
                                  dds_qp);
      if (!this->qc_reader_)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                                    "Error creating query condition for the reader.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
        }
      this->qc_listener_ = this->impl ()->create_querycondition (
                                  DDS_NOT_READ_SAMPLE_STATE,
                                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                  DDS_ANY_INSTANCE_STATE,
                                  filter.expression,
                                  dds_qp);
      if (!this->qc_listener_)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                                    "Error creating query condition for the listeners.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
        }
    }
  else
    {
      this->set_filter (filter, this->qc_reader_);
      this->set_filter (filter, this->qc_getter_);
      this->set_filter (filter, this->qc_listener_);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::set_filter (
  const ::CCM_DDS::QueryFilter & filter,
  DDSQueryCondition * qc)
{
  DDS_StringSeq parameters;
  parameters <<= filter.parameters;
  ::DDS::ReturnCode_t const retval = qc->set_query_parameters (parameters);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                                "Error setting expression_parameters. "
                                "Retval is %C\n",
                                translate_retcode(retval)));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDSReadCondition *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_readcondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_readcondition");
  if (!this->rd_condition_)
    {
      this->rd_condition_ = this->impl ()->create_readcondition (
                                              DDS_NOT_READ_SAMPLE_STATE,
                                              DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                              DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE);
    }
  return this->rd_condition_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_readcondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_readcondition");
  if (!this->ws_)
    {
      ACE_NEW_THROW_EX (this->ws_,
                        DDSWaitSet (),
                        CORBA::NO_MEMORY ());
      DDS_ReturnCode_t const retcode = this->ws_->attach_condition (this->get_readcondition ());
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_readcondition - "
                                              "GETTER: Unable to attach read condition to waitset.\n"));
          throw CCM_DDS::InternalError (retcode, 1);
        }
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CIAO::DDS4CCM::DataReader_T::create_readcondition - "
                                          "Read condition created and attached to Waitset.\n"));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDSQueryCondition *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_querycondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_querycondition");
  return this->qc_getter_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::attach_querycondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::attach_querycondition");

  if (!this->ws_)
    {
      ACE_NEW_THROW_EX (this->ws_,
                        DDSWaitSet (),
                        CORBA::NO_MEMORY ());
    }
  DDS_ReturnCode_t const retcode =
    this->ws_->attach_condition (this->get_querycondition ());
  if (retcode != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_querycondition - "
                                          "GETTER: Unable to attach query condition to waitset.\n"));
      throw CCM_DDS::InternalError (retcode, 1);
    }
  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CIAO::DDS4CCM::DataReader_T::create_querycondition - "
                                      "Query condition created and attached to Waitset.\n"));
}

template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::wait (
  DDSConditionSeq & active_conditions,
  DDS_Duration_t & time_out)
{
   DDS_ReturnCode_t const retcode =
     this->ws_->wait (active_conditions, time_out);
   if (retcode == ::DDS::RETCODE_TIMEOUT)
     {
       DDS4CCM_DEBUG (6,
                      (LM_DEBUG,
                      ACE_TEXT ("Getter: No data available after timeout.\n")));
       return false;
     }
   return true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::remove_condition (
  DDSQueryCondition * qc,
  const char * type)
{
  if (qc)
    {
      if (this->impl ()->delete_readcondition (qc) != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DataReader_T::remove_condition - "
                          "Unable to delete query condition %C from DDSDataReader.\n",
                          type));
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DataReader_T::remove_condition - "
                          "Query condition %C successfully deleted from DDSDataReader.\n",
                          type));
        }
      qc = 0;
    }
}


template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::remove_conditions ()
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::remove_conditions");

  DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
  this->remove_condition (this->qc_reader_, "reader");
  this->remove_condition (this->qc_listener_, "listener");
  if (this->qc_getter_)
    {
      if (this->ws_->detach_condition (this->qc_getter_) == DDS_RETCODE_OK)
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DataReader_T::remove_conditions - "
                          "Query condition successfully detached from waitset.\n"));
          this->remove_condition (this->qc_getter_, "getter");
        }
      else
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DataReader_T::remove_conditions - "
                          "Unable to detach query condition from waitset.\n"));
        }
    }
  else
    {
      if (this->ws_)
        {
          retcode = this->ws_->detach_condition (this->rd_condition_);
          if (retcode != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DataReader_T::remove_conditions - "
                              "Unable to detach read condition from waitset.\n"));
            }
          else
            {
              DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DataReader_T::remove_conditions - "
                              "Read condition successfully detached from waitset.\n"));
            }
        }
    }
  retcode = this->impl ()->delete_readcondition (this->rd_condition_);
  if (retcode != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DataReader_T::remove_conditions - "
                      "Unable to delete read condition from DDSDataReader.\n"));
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DataReader_T::remove_conditions - "
                      "Read condition successfully deleted from DDSDataReader.\n"));
    }
  this->rd_condition_ = 0;
  delete this->ws_;
  this->ws_ = 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  this->remove_conditions ();
  this->set_listener (::DDS::DataReaderListener::_nil (), 0);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_datareader (
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_datareader (
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::delete_datareader (
  ::DDS::Subscriber_ptr subscriber)
{
  ::DDS::ReturnCode_t const retval = subscriber->delete_datareader (this);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
        "DataReader_T::delete_datareader - "
        "Unable to delete DataReader: <%C>\n",
        ::CIAO::DDS4CCM::translate_retcode (retval)));
      throw CORBA::INTERNAL ();
    }
  this->impl_ = 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::impl (void)
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::enable (void)
{
  return this->impl ()->enable ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusCondition_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_statuscondition (void)
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_status_changes (void)
{
  return this->impl ()->get_status_changes ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_INSTANCE_HANDLE_T_RETN
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_instance_handle (void)
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReadCondition_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_readcondition (
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
                        ReadCondition_type (rc),
                        CORBA::NO_MEMORY ());
    }
#else
  ::DDS::ReadCondition_var rc =
    this->impl ()->create_readcondition (sample_states, view_states, instance_states);
  if (! ::CORBA::is_nil (rc.in ()))
    {
      retval = new CCM_DDS_StatusCondition_i<DDS_TYPE, CCM_TYPE> (sc.in ());
      ACE_NEW_THROW_EX (retval,
                        ReadCondition_type (sc.in ()),
                        CORBA::NO_MEMORY ());
    }
#endif
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::QueryCondition_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_querycondition (
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
                        QueryCondition_type (qc),
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
                        QueryCondition_type (qc.in ()),
                        CORBA::NO_MEMORY ());
    }
#endif
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::delete_readcondition (
  ::DDS::ReadCondition_ptr a_condition)
{
#if (CIAO_DDS4CCM_NDDS==1)
  CCM_DDS_ReadCondition_T<DDS_TYPE, CCM_TYPE> *rc =
    dynamic_cast< CCM_DDS_ReadCondition_T<DDS_TYPE, CCM_TYPE> *> (a_condition);
  if (!rc)
    {
      return ::DDS::RETCODE_BAD_PARAMETER;
    }
  return this->impl ()->delete_readcondition (rc->get_impl ());
#else
  return this->impl ()->delete_readcondition (a_condition);
#endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::delete_contained_entities (void)
{
  return this->impl ()->delete_contained_entities ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::set_qos (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_qos (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::set_listener (
  ::DDS::DataReaderListener_ptr a_listener,
  ::DDS::StatusMask mask)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::set_listener");

#if (CIAO_DDS4CCM_NDDS==1)
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
#else
  return this->impl ()->set_listener (a_listener, mask);
#endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::DataReaderListener_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_listener (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_listener");

#if (CIAO_DDS4CCM_NDDS==1)
  DDSDataReaderListener *drl = this->impl ()->get_listener ();
  CCM_DDS_DataReaderListener_T<DDS_TYPE, CCM_TYPE> *ccm_dds_drl =
    dynamic_cast <CCM_DDS_DataReaderListener_T<DDS_TYPE, CCM_TYPE> *> (drl);
  if (!ccm_dds_drl)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, "CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_listener - "
                                "DDS returned a NIL listener.\n"));
      return ::DDS::DataReaderListener::_nil ();
    }
  return ccm_dds_drl->get_datareaderlistener ();
#else
  return this->impl ()->get_listener ();
#endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::TopicDescription_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_topicdescription (void)
{
  ::DDS::TopicDescription_var dds_td = ::DDS::TopicDescription::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
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
#else
  ::DDS::TopicDescription_var td = this->impl ()->get_topicdescription ();
  ACE_NEW_THROW_EX (dds_td,
                    CCM_DDS_TopicDescription_T (td.in ()),
                    CORBA::NO_MEMORY ());
#endif
  return dds_td._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::Subscriber_ptr
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_subscriber (void)
{
  ::DDS::Subscriber_var dds_td = ::DDS::Subscriber::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
  ::DDSSubscriber* subscriber = this->impl ()->get_subscriber ();
  ACE_NEW_THROW_EX (dds_td,
                    Subscriber_type (subscriber),
                    CORBA::NO_MEMORY ());
#else
  ::DDSSubscriber_var subscriber = this->impl ()->get_subscriber ();
  dds_td = new CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE> (subscriber.in ());
  ACE_NEW_THROW_EX (dds_td,
                    Subscriber_type (subscriber.in ()),
                    CORBA::NO_MEMORY ());
#endif
  return dds_td._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_sample_rejected_status (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_liveliness_changed_status (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_requested_deadline_missed_status (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_requested_incompatible_qos_status (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_subscription_matched_status (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_sample_lost_status (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::wait_for_historical_data (
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

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_matched_publications (
  ::DDS::InstanceHandleSeq & publication_handles)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_matched_publications");
#if (CIAO_DDS4CCM_NDDS==1)
  ::DDS_InstanceHandleSeq rtiseq;
  ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_publications (rtiseq);
  publication_handles <<= rtiseq;
  return retval;
#else
  return this->impl ()->get_matched_publications (publication_handles);
#endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_matched_publication_data (
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

template <typename DDS_TYPE, typename CCM_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_impl (void)
{
  return this->impl_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return this->lst_mask_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::set_impl (
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
