// $Id$

#include "dds4ccm/impl/dds/DataReader.h"
#include "dds4ccm/impl/dds/DataReaderListener_T.h"
#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/Subscriber.h"
#include "dds4ccm/impl/dds/QueryCondition.h"

#include "dds4ccm/impl/dds/ndds/SampleInfo.h"
#include "dds4ccm/impl/dds/ndds/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::Reader_T (void)
  : topic_ (0),
    library_name_ (""),
    profile_name_ (""),
    impl_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::Reader_T");
  #if defined (DDS4CCM_USES_QUERY_CONDITION) && (DDS4CCM_USES_QUERY_CONDITION==1)
    this->qc_ = 0;
  #else
    this->cft_ = ::DDS::ContentFilteredTopic::_nil ();
  #endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::~Reader_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::~Reader_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::impl (void)
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

// For the requirement : 'samples ordered by instances' the following settings are necessary:
// ordered_access -> true   and     DDS_INSTANCE_PRESENTATION_QOS (default) .
template <typename DDS_TYPE, typename CCM_TYPE>
CORBA::ULong
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::get_nr_valid_samples (
  const DDS_SampleInfoSeq& sample_info,
  bool determine_last)
{
  CORBA::ULong nr_of_samples = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if (determine_last)
        {
          if (sample_info[i].sample_rank == 0 && sample_info[i].valid_data)
            {
              ++nr_of_samples;
            }
        }
      else if (sample_info[i].valid_data)
        {
          ++nr_of_samples;
        }
    }
  return nr_of_samples;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::read_without_instance (
  typename DDS_TYPE::dds_seq_type & data,
  DDS_SampleInfoSeq & sample_info)
{
  DDS_ReturnCode_t retval = DDS_RETCODE_ERROR;
#if defined (DDS4CCM_USES_QUERY_CONDITION) && (DDS4CCM_USES_QUERY_CONDITION==1)
  if (this->qc_)
    {
      retval = this->impl ()->read_w_condition (data,
                                       sample_info,
                                       DDS_LENGTH_UNLIMITED,
                                       this->qc_);
    }
  else
    throw ::CCM_DDS::InternalError (retval, 0);
#else
  retval = this->impl ()->read (data,
                                sample_info,
                                DDS_LENGTH_UNLIMITED,
                                DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                DDS_ALIVE_INSTANCE_STATE);
#endif
  if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
    {
      this->impl ()->return_loan(data, sample_info);
      DDS4CCM_ERROR (1, (LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_without_instance - ")
                            ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}


template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::read_last (
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  // This function has to return the last sample of all instances
  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->read_without_instance (data, sample_info);

  typename CCM_TYPE::seq_type * inst_seq = 0;
  ACE_NEW_THROW_EX (inst_seq,
                    typename CCM_TYPE::seq_type,
                    CORBA::NO_MEMORY ());
  ::CCM_DDS::ReadInfoSeq * infoseq = 0;
  ACE_NEW_THROW_EX (infoseq,
                    ::CCM_DDS::ReadInfoSeq,
                    CORBA::NO_MEMORY ());

  CORBA::ULong const nr_of_last_samples = this->get_nr_valid_samples (sample_info, true);

  DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_last - ")
                            ACE_TEXT ("total number of samples <%u> - ")
                            ACE_TEXT ("last number of samples <%u>\n"),
                            data.length(),
                            nr_of_last_samples));
  CORBA::ULong ix = 0;

  infoseq->length (nr_of_last_samples);
  inst_seq->length (nr_of_last_samples);

  // We need only the last sample of each instance
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
        {
          (*infoseq)[ix] <<= sample_info[i];
          (*inst_seq)[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  this->impl ()->return_loan(data,sample_info);
  infos = infoseq;
  instances = inst_seq;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  // This function has to return all samples of all instances
  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->read_without_instance (data, sample_info);

  CORBA::ULong const nr_of_valid_samples = this->get_nr_valid_samples (sample_info);
  DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_all - ")
                            ACE_TEXT ("total number of samples <%u> - ")
                            ACE_TEXT ("valid number of samples <%u>\n"),
                            data.length (),
                            nr_of_valid_samples));

  typename CCM_TYPE::seq_type * inst_seq = 0;
  ACE_NEW_THROW_EX (inst_seq,
                    typename CCM_TYPE::seq_type,
                    CORBA::NO_MEMORY ());
  ::CCM_DDS::ReadInfoSeq * infoseq = 0;
  ACE_NEW_THROW_EX (infoseq,
                    ::CCM_DDS::ReadInfoSeq,
                    CORBA::NO_MEMORY ());

  infoseq->length (nr_of_valid_samples);
  inst_seq->length (nr_of_valid_samples);

  CORBA::ULong ix = 0;

  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
        if(sample_info[i].valid_data)
          {
            (*infoseq)[ix] <<= sample_info[i];
            (*inst_seq)[ix] = data[i];
            ++ix;
          }
    }

  // Return the loan
  this->impl ()->return_loan(data,sample_info);

  infos = infoseq;
  instances = inst_seq;
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_InstanceHandle_t
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::check_handle (
  const typename DDS_TYPE::value_type& an_instance,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
  hnd <<= instance_handle;

  DDS_InstanceHandle_t const lookup_hnd =
      this->impl ()->lookup_instance (an_instance);
  if (!DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL) &&
      !DDS_InstanceHandle_equals (&hnd, &lookup_hnd))
    {
      throw ::CCM_DDS::InternalError (0, 0);
    }
  if (DDS_InstanceHandle_equals (&lookup_hnd, &::DDS_HANDLE_NIL))
    {
      throw ::CCM_DDS::NonExistent ();
    }
  return lookup_hnd;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::read_with_instance (
  typename DDS_TYPE::dds_seq_type & data,
  const ::DDS_InstanceHandle_t & lookup_hnd,
  DDS_SampleInfoSeq & sample_info)
{
  DDS4CCM_DEBUG (6, (LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_with_instance - ")
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
      this->impl ()->return_loan(data, sample_info);
      DDS4CCM_ERROR (1, (LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_with_instance - ")
                            ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::read_one_last (
  typename DDS_TYPE::value_type& an_instance,
  ::CCM_DDS::ReadInfo_out info,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t const lookup_hnd = this->check_handle (an_instance, instance_handle);

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  // For now, only read with instance...
  this->read_with_instance (data, lookup_hnd, sample_info);

  ::DDS_Long sample = data.length();
  DDS4CCM_DEBUG (6, (LM_INFO,
              ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_one_last - ")
              ACE_TEXT ("total number of samples <%u>\n"),
              sample));
  while (sample >= 0 && !sample_info[sample-1].valid_data)
    {
      --sample;
    }
  if (sample >= 0)
    {
      if(sample_info[sample-1].valid_data)
        {
          an_instance = data[sample-1];
          info <<= sample_info[sample-1];
        }
    }
  // Return the loan
  this->impl ()->return_loan(data, sample_info);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::read_one_all (
  const typename DDS_TYPE::value_type& an_instance,
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t const lookup_hnd =
    this->check_handle (an_instance, instance_handle);

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->read_with_instance (data, lookup_hnd, sample_info);

  // Count the number of valid samples
  CORBA::ULong const nr_of_valid_samples = this->get_nr_valid_samples (sample_info);
  DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_all - ")
                            ACE_TEXT ("total number of samples <%u> - ")
                            ACE_TEXT ("valid number of samples <%u>\n"),
                            data.length (),
                            nr_of_valid_samples));

  typename CCM_TYPE::seq_type * inst_seq = 0;
  ACE_NEW_THROW_EX (inst_seq,
                    typename CCM_TYPE::seq_type (nr_of_valid_samples),
                    CORBA::NO_MEMORY ());
  ::CCM_DDS::ReadInfoSeq * infoseq = 0;
  ACE_NEW_THROW_EX (infoseq,
                    ::CCM_DDS::ReadInfoSeq (nr_of_valid_samples),
                    CORBA::NO_MEMORY ());

  infoseq->length (nr_of_valid_samples);
  inst_seq->length (nr_of_valid_samples);

  // Copy the valid samples
  CORBA::ULong ix = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if(sample_info[i].valid_data)
        {
          (*infoseq)[ix] <<= sample_info[i];
          (*inst_seq)[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  this->impl ()->return_loan(data, sample_info);

  infos = infoseq;
  instances = inst_seq;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::create_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::create_filter");
  ::DDS::Subscriber_var sub = this->reader_->get_subscriber ();
  if (CORBA::is_nil (sub.in ()))
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::create_filter - "
                    "Error: Unable to get Subscriber.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  ::DDS::DomainParticipant_var dp = sub->get_participant ();
  if (CORBA::is_nil (dp.in ()))
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::create_filter - "
                    "Error: Unable to get Participant.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
    }

  this->cft_ = dp->create_contentfilteredtopic (
                        "DDS4CCMContentFilteredTopic",
                        this->topic_.in (),
                        filter.query,
                        filter.query_parameters);
  if (CORBA::is_nil (this->cft_))
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::create_filter - "
                    "Error: Unable to create ContentFilteredTopic.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
    }
  ::DDS::DataReaderListener_var listener = this->reader_->get_listener ();
  CCM_DDS::PortStatusListener_ptr psl = CCM_DDS::PortStatusListener::_nil ();
  if (!CORBA::is_nil (listener))
    {
      psl = dynamic_cast <CCM_DDS::PortStatusListener_ptr> (listener.in ());
      this->reader_->set_listener (::DDS::DataReaderListener::_nil (), 0);
    }
  ::DDS::ReturnCode_t const retval = sub->delete_datareader (this->reader_);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::create_filter - "
                    "Error: Unable to delete DataReader.\n"));
    }
  this->reader_ = ::DDS::CCM_DataReader::_nil ();

  ::DDS::DataReader_var reader = ::DDS::DataReader::_nil ();
  if (this->library_name_.length () > 0 &&
      this->profile_name_.length () > 0)
    {
      reader = sub->create_datareader_with_profile (
                        this->cft_,
                        this->library_name_.c_str (),
                        this->profile_name_.c_str (),
                        listener,
                        ::CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
                          psl));
    }
  else
    {
      ::DDS::DataReaderQos drqos;
      reader = sub->create_datareader (
                        this->cft_,
                        drqos,
                        listener,
                        ::CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
                          psl));
    }
  if (CORBA::is_nil(reader))
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::create_filter - "
                    "Error: Unable to create a new DataReader.\n"));
    }
  this->reader_ = ::DDS::CCM_DataReader::_narrow (reader);
  this->set_impl (reader);
}

template <typename DDS_TYPE, typename CCM_TYPE>
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::filter (void)
{
  #if defined (DDS4CCM_USES_QUERY_CONDITION) && (DDS4CCM_USES_QUERY_CONDITION==1)
    if (!this->qc_)
      {
        DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::filter - "
                      "Error: No QueryCondition set yet. First set a filter.\n"));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
      }
    ::CCM_DDS::QueryFilter_var filter = 0;
    ACE_NEW_THROW_EX (filter,
                      ::CCM_DDS::QueryFilter(),
                      CORBA::NO_MEMORY ());
    filter->query = this->qc_->get_query_expression ();
    ::DDS_StringSeq dds_qp;
    this->qc_->get_query_parameters (dds_qp);
    filter->query_parameters <<= dds_qp;
    return filter._retn ();
  #else
    if (CORBA::is_nil (this->cft_))
      {
        DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::filter - "
                      "Error: No ContentFilter set yet. First set a filter.\n"));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
      }
    ::CCM_DDS::QueryFilter_var filter = 0;
    ACE_NEW_THROW_EX (filter,
                      ::CCM_DDS::QueryFilter(),
                      CORBA::NO_MEMORY ());
    filter->query = this->cft_->get_filter_expression ();
    ::DDS::ReturnCode_t const retval = this->cft_->get_expression_parameters (
                                          filter->query_parameters);
    if (retval != DDS::RETCODE_OK)
      {
        DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::filter - "
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
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::filter");
  #if defined (DDS4CCM_USES_QUERY_CONDITION) && (DDS4CCM_USES_QUERY_CONDITION==1)
    if (!this->qc_)
      {
        ::DDS_StringSeq dds_qp;
        dds_qp <<= filter.query_parameters;
        this->qc_ = this->impl ()->create_querycondition (
                                DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                DDS_ALIVE_INSTANCE_STATE,
                                filter.query,
                                dds_qp);
        if (!this->qc_)
          {
            DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::filter - "
                                      "Error creating query condition."));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
          }
      }
    else
      {
        ::DDS_StringSeq dds_qp;
        dds_qp <<= filter.query_parameters;
        ::DDS::ReturnCode_t retval = this->qc_->set_query_parameters (
                                                  dds_qp);
        if (retval != ::DDS::RETCODE_OK)
          {
            DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::filter - "
                                      "Error setting expression_parameters. "
                                      "Retval is %C\n",
                                      translate_retcode(retval)));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
          }
      }
  #else
    if (CORBA::is_nil (this->cft_))
      {
        this->create_filter (filter);
      }
    else
      {
        ::DDS::ReturnCode_t retval = this->cft_->set_expression_parameters (
          filter.query_parameters);
        if (retval != ::DDS::RETCODE_OK)
          {
            DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::filter - "
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
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::set_qos (
  ::DDS::Topic_ptr topic,
  const char * library_name,
  const char * profile_name)
{
  this->library_name_ = library_name;
  this->profile_name_ = profile_name;
  this->topic_ = topic;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE>::set_impl (
  ::DDS::DataReader_ptr reader)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::set_impl");

  if (::CORBA::is_nil (reader))
    {
      this->impl_ = 0;
      #if defined (DDS4CCM_USES_QUERY_CONDITION) && (DDS4CCM_USES_QUERY_CONDITION==1)
        if (this->qc_)
          {
            this->impl ()->delete_readcondition (this->qc_);
          }
      #endif
    }
  else
    {
      this->reader_ = reader;
      CCM_DDS_DataReader_i *rdr = dynamic_cast <CCM_DDS_DataReader_i *> (reader);

      if (!rdr)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::data_reader - "
                       "Unable to cast provided DataReader to servant\n"));
          throw ::CORBA::INTERNAL ();
        }

      this->impl_ =  DDS_TYPE::data_reader::narrow (rdr->get_impl ());

      if (!this->impl ())
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Reader_T::data_reader - "
                       "Unable to narrow the provided reader entity to the specific "
                       "type necessary to publish messages\n"));
          throw ::CORBA::INTERNAL ();
        }
    }
}

