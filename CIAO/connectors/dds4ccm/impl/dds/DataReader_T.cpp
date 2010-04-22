// $Id$

#include "dds4ccm/impl/dds/DataReader.h"
#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/QueryCondition.h"

#include "dds4ccm/impl/dds/ndds/SampleInfo.h"
#include "dds4ccm/impl/dds/ndds/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::DataReader_T (void)
  : CCM_DDS_DataReader_i (0),
    impl_ (0),
    rd_condition_ (0),
    ws_ (0),
    #if (DDS4CCM_USES_QUERY_CONDITION==1)
      qc_reader_ (0),
      qc_getter_ (0),
      qc_listener_ (0)
    #else
      cft_ (0),
      library_name_ (""),
      profile_name_ ("")
    #endif
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::DataReader_T");
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
#if (DDS4CCM_USES_QUERY_CONDITION==1)
  if (this->qc_reader_)
    {
      retval = this->impl ()->read_w_condition (data,
                                       sample_info,
                                       DDS_LENGTH_UNLIMITED,
                                       this->qc_reader_);
    }
  else
    {
#endif
      retval = this->impl ()->read (data,
                                    sample_info,
                                    DDS_LENGTH_UNLIMITED,
                                    DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                    DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                    DDS_ALIVE_INSTANCE_STATE);
#if (DDS4CCM_USES_QUERY_CONDITION==1)
    }
#endif
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
#if (DDS4CCM_USES_QUERY_CONDITION==1)
  if (this->qc_getter_)
    {
      return this->impl ()->read_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              this->qc_getter_);
   }
  else
    {
#endif
      return this->impl ()->read_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              this->rd_condition_);
#if (DDS4CCM_USES_QUERY_CONDITION==1)
    }
#endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_ReturnCode_t
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::take (
  typename DDS_TYPE::dds_seq_type & data,
  ::DDS_SampleInfoSeq & sample_info,
  ::DDS_Long max_samples)
{
#if (DDS4CCM_USES_QUERY_CONDITION==1)
  if (this->qc_listener_)
    {
      return this->impl ()->take_w_condition (data,
                                              sample_info,
                                              max_samples,
                                              this->qc_listener_);
    }
  else
    {
#endif
      return this->impl ()->take (data,
                                  sample_info,
                                  max_samples,
                                  DDS_NOT_READ_SAMPLE_STATE,
                                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                  DDS_ANY_INSTANCE_STATE);
#if (DDS4CCM_USES_QUERY_CONDITION==1)
    }
#endif
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
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_contentfilteredtopic (
  const ::CCM_DDS::QueryFilter & filter,
  ::DDSSubscriber * sub)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_contentfilteredtopic");

  // To create a ContentFilteredTopic we need a DDSTopic.
  // Since a ContentFilteredTopic is created on the DomainParticipant,
  // we need to obtain the DomainParticipant through the
  // Subscriber.
  ::DDSDomainParticipant * dp = sub->get_participant ();
  if (!dp)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_contentfilteredtopic - "
                        "Unable to get the Participant from the DDS Subscriber\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
    }
  // Now, get the topic.
  ::DDSTopicDescription * td = this->impl ()->get_topicdescription ();
  if (!td)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_contentfilteredtopic - "
                        "Unable to get the TopicDescription from the DDS DomainParticipant\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 2);
    }
  ::DDSTopic * tp = ::DDSTopic::narrow (td);
  if (!tp)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_contentfilteredtopic - "
                        "Unable to narrow the DDS TopicDescription to a DDS Topic\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 3);
    }
  // Now create the ContentFilteredTopic
  DDS_StringSeq params;
  params <<= filter.query_parameters;
  this->cft_ = dp->create_contentfilteredtopic (
                        "DDS4CCMContentFilteredTopic",
                        tp,
                        filter.query,
                        params);
  if (!this->cft_)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_contentfilteredtopic - "
                    "Error: Unable to create ContentFilteredTopic.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 4);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::delete_datareader (
  ::DDSSubscriber * sub)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::delete_datareader");
  DDS_ReturnCode_t const retval = sub->delete_datareader (this->impl ());
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::delete_datareader - "
                                "Unable to delete original DataReader. "
                                "Retval is %C\n",
                                translate_retcode(retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_filter");

  // we need to use the DDS entities direct since we're not allowed
  // to change the CORBA interfaces.  These are known to component
  // the end user has created.

  // To set a ContentFilteredTopic on a DataReader, the DataReader
  // should be recreated. Since the Getter uses the same DataReader,
  // the original DataReader should not be destroyed.
  ::DDSSubscriber * sub = this->impl ()->get_subscriber ();
  if (!sub)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_filter - "
                        "Unable to get the Subscriber from the type specific DDS DataReader\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  this->create_contentfilteredtopic (filter, sub);

  // Now recreate the DataReader, using the ContentFilteredTopic.
  // After recreation, connect the original DataReaderListener to it.
  ::DDSDataReaderListener *drl = this->impl ()->get_listener ();

  ::DDSDataReader * dr = 0;
  if (this->library_name_.length () > 0 &&
      this->profile_name_.length () > 0)
    {
      dr = sub->create_datareader_with_profile (this->cft_,
                                                this->library_name_.c_str (),
                                                this->profile_name_.c_str (),
                                                drl,
                                                ::DDS::DATA_AVAILABLE_STATUS |
                                                ::DDS::REQUESTED_DEADLINE_MISSED_STATUS |
                                                ::DDS::SAMPLE_LOST_STATUS);
    }
  else
    {
      DDS_DataReaderQos const qos = DDS_DATAREADER_QOS_DEFAULT;
      dr = sub->create_datareader (this->cft_,
                                   qos,
                                   drl,
                                   ::DDS::DATA_AVAILABLE_STATUS |
                                   ::DDS::REQUESTED_DEADLINE_MISSED_STATUS |
                                   ::DDS::SAMPLE_LOST_STATUS);
    }
  if (!dr)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_filter - "
                    "Error: Unable to create a new DataReader.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
    }
  this->remove_conditions ();
  this->delete_datareader (sub);
  // Now we need to set the new created DataReader in our proxy classes.
  this->set_impl (dr);
  this->impl_ = DDS_TYPE::data_reader::narrow (dr);
  this->create_readcondition ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::QueryFilter *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::filter (void)
{
  #if (DDS4CCM_USES_QUERY_CONDITION==1)
    if (!this->qc_reader_)
      {
        DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                      "Error: No QueryCondition set yet. First set a filter.\n"));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
      }
    ::CCM_DDS::QueryFilter_var filter = 0;
    ACE_NEW_THROW_EX (filter,
                      ::CCM_DDS::QueryFilter(),
                      CORBA::NO_MEMORY ());
    filter->query = this->qc_reader_->get_query_expression ();
    ::DDS_StringSeq dds_qp;
    this->qc_reader_->get_query_parameters (dds_qp);
    filter->query_parameters <<= dds_qp;
    return filter._retn ();
  #else
    if (!this->cft_)
      {
        DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                      "Error: No ContentFilter set yet. First set a filter.\n"));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
      }
    ::CCM_DDS::QueryFilter_var filter = 0;
    ACE_NEW_THROW_EX (filter,
                      ::CCM_DDS::QueryFilter(),
                      CORBA::NO_MEMORY ());
    filter->query = this->cft_->get_filter_expression ();
    DDS_StringSeq params;
    ::DDS::ReturnCode_t const retval = this->cft_->get_expression_parameters (
                                          params);
    filter->query_parameters <<= params;
    if (retval != DDS::RETCODE_OK)
      {
        DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                                  "Error getting expression_parameters. "
                                  "Retval is %C\n",
                                  translate_retcode(retval)));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
      }
    return filter._retn ();
  #endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::filter");

  #if (DDS4CCM_USES_QUERY_CONDITION==1)
    if (!this->qc_reader_)
      {
        // filter not set. Create query conditions for both the
        // getter and reader
        ::DDS_StringSeq dds_qp;
        dds_qp <<= filter.query_parameters;
        if (this->rd_condition_)
          { // Getter functionality
            // First remove the existing conditions from the waitset
            // Than create a new condition and attach it to the waitset.
            this->remove_conditions ();
            this->qc_getter_ = this->impl ()->create_querycondition (
                                    DDS_NOT_READ_SAMPLE_STATE,
                                    DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                    DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE,
                                    filter.query,
                                    dds_qp);
            this->attach_querycondition ();
          }
        this->qc_reader_ = this->impl ()->create_querycondition (
                                    DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                    DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                    DDS_ALIVE_INSTANCE_STATE,
                                    filter.query,
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
                                    filter.query,
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
      }
  #else
    if (!this->cft_)
      {
        this->create_filter (filter);
      }
    else
      {
        DDS_StringSeq params;
        params <<= filter.query_parameters;
        ::DDS::ReturnCode_t retval = this->cft_->set_expression_parameters (
          params);
        if (retval != ::DDS::RETCODE_OK)
          {
            DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::filter - "
                                      "Error setting expression_parameters. "
                                      "Retval is %C\n",
                                      translate_retcode(retval)));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
          }
      }
  #endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::set_filter (
  const ::CCM_DDS::QueryFilter & filter,
  ::DDSQueryCondition * qc)
{
  ::DDS_StringSeq dds_qp;
  dds_qp <<= filter.query_parameters;
  ::DDS::ReturnCode_t retval = qc->set_query_parameters (
                                            dds_qp);
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
      DDS_ReturnCode_t retcode = this->ws_->attach_condition (this->get_readcondition ());
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DataReader_T::create_readcondition - "
                                              "GETTER: Unable to attach read condition to waitset.\n"));
          throw CCM_DDS::InternalError (retcode, 1);
        }
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CIAO::DDS4CCM::DataReader_T::create_readcondition - "
                                          "Read condition created and attched to Waitset.\n"));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDSReadCondition *
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::get_querycondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get_querycondition");
  #if (DDS4CCM_USES_QUERY_CONDITION==1)
    return this->qc_getter_;
  #else
    return 0;
  #endif
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
  DDS_ReturnCode_t retcode = this->ws_->attach_condition (this->get_querycondition ());
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
   if (retcode == DDS_RETCODE_TIMEOUT)
     {
       DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("Getter: No data available after timeout.\n")));
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
#if (DDS4CCM_USES_QUERY_CONDITION==1)
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
#endif
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
#if (DDS4CCM_USES_QUERY_CONDITION==1)
    }
#endif
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
  this->set_listener (::DDS::DataReaderListener::_nil (),
                      0);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::create_datareader (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char * library_name,
  const char * profile_name)
{
  ::DDS::DataReader_var reader;
  if (library_name && profile_name)
    {
      #if (DDS4CCM_USES_QUERY_CONDITION==0)
        this->profile_name_ = profile_name;
        this->library_name_ = library_name ;
      #endif

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
  ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *rd =
    dynamic_cast < ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *> (reader.in ());

  //pass this proxy on to the listener
  DDS_DataReaderQos qos;
  rd->get_impl ()->get_qos (qos);
  char * value = 0;
  ACE_NEW_THROW_EX (value,
                    char[15],
                    CORBA::NO_MEMORY ());
  ACE_OS::sprintf (value ,
                   "%ld",
                   reinterpret_cast <unsigned long> (this));

  DDSPropertyQosPolicyHelper::add_property (qos.property,
                                            "CCM_DataReaderProxy",
                                            value,
                                            DDS_BOOLEAN_FALSE);
  rd->get_impl ()->set_qos (qos);
  delete value;

  this->set_impl (rd->get_impl ());
  this->impl_ = DDS_TYPE::data_reader::narrow (rd->get_impl ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE>::delete_datareader (
  ::DDS::Subscriber_ptr subscriber)
{
  ::DDS::ReturnCode_t retval = subscriber->delete_datareader (this);
  if (retval != DDS::RETCODE_OK)
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
